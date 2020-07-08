#pragma once
#include <Component/SimpleScene.h>
#include <Component/Transform/Transform.h>
#include <Core/GPU/Mesh.h>
#include <Core/GPU/CubeMapFBO.h>
#include <Core/GPU/ShadowCubeMapFBO.h>

struct Material;

class Tema2SPG : public SimpleScene
{
	public:
		Tema2SPG();
		~Tema2SPG();

		void Init() override;

	private:
		void LoadMaterials();
		void LoadMeshes();
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void ShadowMapPass();
		void ReflectionPass();
		void ForwardRenderPass();

		void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, Material *mat);

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;

		std::unordered_map<std::string, Material*> materials;
		std::vector<glm::mat4> terrainOffsets, bambooModelMatrices;
		glm::mat4 skyModelMatrix, cloudsModelMatrix, cloudsModelMatrix2, mountainsModelMatrix;
		GLuint randomTextureID;

		std::unordered_map<std::string, std::pair<CubeMapFBO*, glm::vec3 *> > reflectionCubeMaps;
		Texture2D *skysphereTex, *cloudsTex, *mountainsTex;
		Mesh* skysphereMesh;

		glm::vec3 carPos;
		
		std::vector<ShadowCubeMapFBO> shadowCubeMap;
		std::vector<glm::vec3> lightPos;
};

class Material
{
public:
	Texture2D *diffuseMap;
	Texture2D *specularMap;
	Texture2D *normalMap;
	CubeMapFBO *reflectionCubeMap;
	glm::vec3 diffuseColor;
	float specularPower;
	float fresnelCos;
	float ambientReflectivity;

	Material() :diffuseMap(NULL),
		specularMap(NULL),
		normalMap(NULL),
		reflectionCubeMap(NULL),
		diffuseColor(glm::vec3(0.5, 0.5, 0.5)),
		specularPower(1.f),
		fresnelCos(0.f),
		ambientReflectivity(0.2) {}
	~Material()
	{
		if (diffuseMap) delete diffuseMap;
		if (specularMap) delete specularMap;
		if (normalMap) delete normalMap;
		if (reflectionCubeMap) delete reflectionCubeMap;
	}
	//void SetDiffuseMap(Texture2D *m) { diffuseMap = m; }
	//void SetSpecularMap(Texture2D *m) { specularMap = m; }
	//void SetReflectionMap(CubeMapFBO *m) { reflectionCubeMap = m; }
};