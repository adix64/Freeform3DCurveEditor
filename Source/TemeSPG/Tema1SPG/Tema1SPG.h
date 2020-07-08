#pragma once
#include <Component/SimpleScene.h>
#include <Component/Transform/Transform.h>
#include <Core/GPU/Mesh.h>
#include <Core/GPU/CubeMapFBO.h>
#include <Core/GPU/Texture2D.h>
#include <Core/GPU/ShadowCubeMapFBO.h>
#include "Gizmo.hpp"
#include "Camera.hpp"
#include "Grid.hpp"
#include "FullscreenQuad.hpp"
#include <Core/GPU/Framebuffer.hpp>
#include "ColorGenerator.hpp"
#include <Core\GPU\Sprite.hpp>
#include "MinCoinsAlgo.h"

typedef std::vector<VertexFormat> TVertexList;
typedef std::vector<uint32_t> TIndexList;
typedef std::vector<glm::vec3> CurvePointList;

inline uint64_t calculateColorHash(glm::uvec3 v)
{
	uint64_t out = 0x0000000000000000;
	out = out | v.x;
	out = out | v.y << 8;
	out = out | v.z << 16;
	return out;
}

using namespace std;
class CCurve
{
public:
	unsigned numInstances = 1;
	glm::vec3 color;
	enum CCurveType { BEZIER, SPLINE, BSPLINE, NURBS, HERMITE };
	CCurveType type;
	CurvePointList pointsList;
	std::vector<glm::uvec3> pointsColors;
	std::map<uint64_t, GLuint> colorHashedIndices;
	std::vector<int> curveOrders;
	ColorGenerator colorGen;
	std::vector<int> availableOrders; //gradul polinomului
	glm::vec3 position;
	int generatesSurface = 0; // 0 - none, 1 - translation, 2 - rotation
	CCurve()
	{
		std::vector<glm::uvec3> reservedColors = {
			glm::uvec3(255, 0, 0), glm::uvec3(0, 255, 0), glm::uvec3(0, 0, 255),
			glm::uvec3(255, 255, 0), glm::uvec3(255, 0, 255), glm::uvec3(0, 127, 127),
			glm::uvec3(127, 0, 0), glm::uvec3(0, 127, 0), glm::uvec3(0, 0, 127),
			glm::uvec3(127, 127, 0), glm::uvec3(127, 0, 127), glm::uvec3(0, 127, 127),
			glm::uvec3(0, 128, 128), glm::uvec3(128, 0, 0), glm::uvec3(0, 128, 0),
			glm::uvec3(0, 0, 128), glm::uvec3(128, 128, 0), glm::uvec3(128, 0, 128),
			glm::uvec3(0, 128, 128) };
		colorGen.SetReservedColors(reservedColors);
	}

	void UpdatePos()
	{
		glm::vec3 sum = glm::vec3(0, 0, 0);
		int k = 0;
		for (auto point : pointsList)
		{
			sum += point;
			k++;
		}
		this->position = sum * (1.0f / k);
	}

	CCurve(CCurveType newType, glm::vec3 col)
	{
		this->type = newType;

		switch(type)
		{
		case BEZIER:
			availableOrders = { 3,2,1 };
			break;
		case SPLINE:
			break;
		case BSPLINE:
			break;
		case NURBS:
			break;
		case HERMITE:
			break;
		}
		color = col;
		std::vector<glm::uvec3> reservedColors = {
			glm::uvec3(255, 0, 0), glm::uvec3(0, 255, 0), glm::uvec3(0, 0, 255),
			glm::uvec3(255, 255, 0), glm::uvec3(255, 0, 255), glm::uvec3(0, 127, 127),
			glm::uvec3(127, 0, 0), glm::uvec3(0, 127, 0), glm::uvec3(0, 0, 127),
			glm::uvec3(127, 127, 0), glm::uvec3(127, 0, 127), glm::uvec3(0, 127, 127) };
		colorGen.SetReservedColors(reservedColors);
	}
	void AddPoint(glm::vec3 &newPoint)
	{
		pointsList.push_back(glm::vec3(newPoint));
		glm::uvec3 nextCol = colorGen.getNextColor();
		pointsColors.push_back(nextCol);
		colorHashedIndices[calculateColorHash(nextCol)] = pointsColors.size() - 1;
		if (type == BEZIER)
		{
			curveOrders = minCoins(availableOrders, std::max((unsigned)pointsList.size() - 1, (unsigned)0));
		}
	}
	void DeletePoint(int index)
	{
		if (index >= 0 && index < pointsList.size())
		{
			uint64_t h = calculateColorHash(pointsColors[index]);
			colorHashedIndices.erase(h);
			for (int i = index; i < pointsColors.size(); i++)
			{
				uint64_t h = calculateColorHash(pointsColors[i]);
				colorHashedIndices[h] = colorHashedIndices[h] - 1;
			}
			pointsList.erase(pointsList.begin() + index);
			pointsColors.erase(pointsColors.begin() + index);
			if (type == BEZIER)
			{
				curveOrders = minCoins(availableOrders, std::max((unsigned)pointsList.size() - 1, (unsigned)0));
			}
		}
	}
};
class Tema1SPG : public SimpleScene
{
	enum ActiveToolType { SELECT_TOOL, MOVE_TOOL, CREATE_CURVE };
	public:
		Tema1SPG();
		~Tema1SPG();

		void Init() override;
	private:
		void LoadMaterials();
		void LoadMeshes();
		void LoadShaders();

		void addPoint(glm::vec2 screenSpacePos);
		
		void DrawHermites(bool onScreen);
		void DrawBeziers(bool onScreen);
		void DrawNURBS(bool onScreen);
		void DrawSplines(bool onScreen);
		void DrawBSplines(bool onScreen);

		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void ForceRedraw();
		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;

		void RenderButtons();
private:

	Gizmo *gizmo;
	Camera camera;

	glm::mat4 model_matrix, view_matrix, projection_matrix;
	BaseMesh *lineMesh;
	BaseMesh *gizmoLine, *gizmoCone;
	
	Grid *grid;
	BaseMesh *pointMesh;
	Sprite *fsQuad;

	std::map<uint64_t, CCurve> theCurves;
	CCurve *activeCurve = NULL;
	int selectedIndex = -1;

	bool keyStates[256];
	bool m_LMB = false, m_RMB = false, m_MMB = false;
	bool m_altDown = false;

	float m_deltaTime;
	int m_width = 1280, m_height = 720;

	std::vector<int> bezierTypes;

	lab::Framebuffer colorPickingFB;
	GLubyte *readPixels;
	unsigned int quadTexture;
	glm::vec3 gizmoPos;
	glm::ivec2 prev_mousePos;
	glm::vec2 prev_ssdir = glm::vec2(0, 0);
	ColorGenerator colorGen;
	Texture2D *selectToolPic, *moveToolPic, *createCurveToolPic, *buttonUp, *buttonDown, *buttonDisabled;
	Texture2D *hermiteBtnPic, *bezierBtnPic, *nurbsBtnPic, *splineBtnPic, *bsplineBtnPic;
	Sprite *m_sprite;
	bool showColorPickingFB = false;
	int fraction = 16;
	ActiveToolType toolType = SELECT_TOOL;
	CCurve::CCurveType newCurveType = CCurve::BEZIER;
};