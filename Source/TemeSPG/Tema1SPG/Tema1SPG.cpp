#include "Tema1SPG.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>
#include <algorithm>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Tema1SPG::LoadShaders()
{
	{// DULL COLOR
		Shader *shader = new Shader("DullColorShader");
		shader->AddShader("Shaders/Tema1SPG/pointVertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Shaders/Tema1SPG/fragment.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
	{// FULL-SCREEN SHADER
		Shader *shader = new Shader("FullScreenShader");
		shader->AddShader("Shaders/Tema1SPG/fullscreenVertex.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Shaders/Tema1SPG/fullscreenFragment.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
	{//HERMITE
		Shader *shader = new Shader("HermiteShader");
		shader->AddShader("Shaders/Tema1SPG/vertex.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Shaders/Tema1SPG/hermite_curveGeometry.glsl", GL_GEOMETRY_SHADER);
		shader->AddShader("Shaders/Tema1SPG/fragment.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
	{// BEZIER
		Shader *shader = new Shader("BezierShader");
		shader->AddShader("Shaders/Tema1SPG/vertex.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Shaders/Tema1SPG/bezier_curveGeometry.glsl", GL_GEOMETRY_SHADER);
		shader->AddShader("Shaders/Tema1SPG/fragment.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
	{// B-SPLINE
		Shader *shader = new Shader("BSplineShader");
		shader->AddShader("Shaders/Tema1SPG/vertex.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Shaders/Tema1SPG/bspline_curveGeometry.glsl", GL_GEOMETRY_SHADER);
		shader->AddShader("Shaders/Tema1SPG/fragment.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	{// SPLINE
		Shader *shader = new Shader("SplineShader");
		shader->AddShader("Shaders/Tema1SPG/vertex.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Shaders/Tema1SPG/spline_curveGeometry.glsl", GL_GEOMETRY_SHADER);
		shader->AddShader("Shaders/Tema1SPG/fragment.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
	{// NURBS
		Shader *shader = new Shader("NURBSShader");
		shader->AddShader("Shaders/Tema1SPG/vertex.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Shaders/Tema1SPG/nurbs_curveGeometry.glsl", GL_GEOMETRY_SHADER);
		shader->AddShader("Shaders/Tema1SPG/fragment.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
/////SURFACE SHADERS
	{//SURFACE HERMITE
		Shader *shader = new Shader("HermiteSurfaceShader");
		shader->AddShader("Shaders/Tema1SPG/vertex.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Shaders/Tema1SPG/hermite_surfaceGeometry.glsl", GL_GEOMETRY_SHADER);
		shader->AddShader("Shaders/Tema1SPG/diffuse_fragment.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
	{//SURFACE  BEZIER
		Shader *shader = new Shader("BezierSurfaceShader");
		shader->AddShader("Shaders/Tema1SPG/vertex.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Shaders/Tema1SPG/bezier_surfaceGeometry.glsl", GL_GEOMETRY_SHADER);
		shader->AddShader("Shaders/Tema1SPG/diffuse_fragment.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
	{//SURFACE  B-SPLINE
		Shader *shader = new Shader("BSplineSurfaceShader");
		shader->AddShader("Shaders/Tema1SPG/vertex.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Shaders/Tema1SPG/bspline_surfaceGeometry.glsl", GL_GEOMETRY_SHADER);
		shader->AddShader("Shaders/Tema1SPG/diffuse_fragment.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	{//SURFACE  SPLINE
		Shader *shader = new Shader("SplineSurfaceShader");
		shader->AddShader("Shaders/Tema1SPG/vertex.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Shaders/Tema1SPG/spline_surfaceGeometry.glsl", GL_GEOMETRY_SHADER);
		shader->AddShader("Shaders/Tema1SPG/diffuse_fragment.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
	{//SURFACE  NURBS
		Shader *shader = new Shader("NURBSSurfaceShader");
		shader->AddShader("Shaders/Tema1SPG/vertex.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Shaders/Tema1SPG/nurbs_surfaceGeometry.glsl", GL_GEOMETRY_SHADER);
		shader->AddShader("Shaders/Tema1SPG/diffuse_fragment.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
}

Tema1SPG::Tema1SPG()
{
	m_width = 1280; m_height = 720;
	glClearColor(0.25, 0.25, 0.25, 1);
	glClearDepth(1);
	glEnable(GL_DEPTH_TEST);
	LoadShaders();

	model_matrix = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	view_matrix = glm::lookAt(glm::vec3(-5, 10, 75), glm::vec3(5, 10, 0), glm::vec3(0, 1, 0));

	//wireframe draw mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	camera = Camera(glm::vec3(-5, 10, 75), glm::vec3(5, 10, 0), glm::vec3(0, 1, 0));
	memset(keyStates, 0, 256);

	bezierTypes = { 3, 2, 1 };

	gizmo = new Gizmo();
	gizmo->Init(shaders["DullColorShader"], &view_matrix, &projection_matrix);

	grid = new Grid();
	grid->Init(shaders["DullColorShader"], &view_matrix, &projection_matrix);

	colorPickingFB.generate(1280, 720);

	readPixels = (GLubyte*)malloc(3 * 1280 * 720);
	
	fsQuad = new Sprite(shaders["FullScreenShader"], &m_width, &m_height, glm::vec3(-1,-1,0), glm::vec3(1,1,0));
	
	std::vector<glm::uvec3> reservedColors = {
		glm::uvec3(255, 0, 0), glm::uvec3(0, 255, 0), glm::uvec3(0, 0, 255),
		glm::uvec3(255, 255, 0), glm::uvec3(255, 0, 255), glm::uvec3(0, 127, 127),
		glm::uvec3(127, 0, 0), glm::uvec3(0, 127, 0), glm::uvec3(0, 0, 127),
		glm::uvec3(127, 127, 0), glm::uvec3(127, 0, 127), glm::uvec3(0, 127, 127),
		glm::uvec3(0, 128, 128), glm::uvec3(128, 0, 0), glm::uvec3(0, 128, 0),
		glm::uvec3(0, 0, 128), glm::uvec3(128, 128, 0), glm::uvec3(128, 0, 128),
		glm::uvec3(0, 128, 128) };
	colorGen.SetReservedColors(reservedColors);
	//quadTexture = lab::loadTextureBMP("resurse/bamboo.bmp");

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Tema1SPG::~Tema1SPG()
{
	//distruge shader
	//distruge mesh incarcat
	delete lineMesh;
	delete pointMesh;
}

void Tema1SPG::addPoint(glm::vec2 screenSpacePos)
{
	float leftLimit = (float)m_width / (float)fraction;
	if (screenSpacePos.x < leftLimit)
		return;
	screenSpacePos.x -= leftLimit;
	screenSpacePos.x *= 1 + leftLimit / (float)m_width;

	screenSpacePos.x = screenSpacePos.x / (float)m_width * 2.f - 1.f;
	screenSpacePos.y = (m_height - screenSpacePos.y) / (float)m_height * 2.f - 1.f;
	
	glm::vec4 worldSpacePos = glm::inverse(projection_matrix * view_matrix) * glm::vec4(screenSpacePos.x, screenSpacePos.y, 0.95, 1);

	worldSpacePos.w = 1.0 / worldSpacePos.w;
	worldSpacePos.x *= worldSpacePos.w;
	worldSpacePos.y *= worldSpacePos.w;
	worldSpacePos.z *= worldSpacePos.w;
#pragma message "HANDLE DIVISION BY ZER(0add)!!!"
	const glm::vec3 camPos = camera.GetPosition();
	glm::vec3 dirVec = glm::normalize(glm::vec3(worldSpacePos) - camPos);
	worldSpacePos = glm::vec4(camPos - dirVec * camPos.z / dirVec.z, 1); //TODO
	activeCurve->AddPoint(glm::vec3(worldSpacePos));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Tema1SPG::LoadMaterials()
{ 
	selectToolPic = new Texture2D();
	selectToolPic->Load2D("Assets/selectTool.png", GL_REPEAT);

	moveToolPic = new Texture2D();
	moveToolPic->Load2D("Assets/moveTool.png", GL_REPEAT);

	buttonUp= new Texture2D();
	buttonUp->Load2D("Assets/buttonUp.png", GL_REPEAT);

	buttonDown = new Texture2D();
	buttonDown->Load2D("Assets/buttonDown.png", GL_REPEAT);
	
	buttonDisabled = new Texture2D();
	buttonDisabled->Load2D("Assets/buttonDisabled.png", GL_REPEAT);

	createCurveToolPic = new Texture2D();
	createCurveToolPic->Load2D("Assets/createCurveTool.png", GL_REPEAT);

	hermiteBtnPic = new Texture2D();
	hermiteBtnPic->Load2D("Assets/Hermite.png", GL_REPEAT);

	bezierBtnPic = new Texture2D();
	bezierBtnPic->Load2D("Assets/Bezier.png", GL_REPEAT);

	nurbsBtnPic = new Texture2D();
	nurbsBtnPic->Load2D("Assets/NURBS.png", GL_REPEAT);

	splineBtnPic = new Texture2D();
	splineBtnPic->Load2D("Assets/Spline.png", GL_REPEAT);

	bsplineBtnPic = new Texture2D();
	bsplineBtnPic->Load2D("Assets/B-Spline.png", GL_REPEAT);

	m_sprite = new Sprite(shaders["FullScreenShader"], &m_width, &m_height);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Tema1SPG::LoadMeshes()
{
	lineMesh = generateLineMesh();
	pointMesh = generatePointMesh();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Tema1SPG::Init()
{
	LoadMaterials();
	LoadMeshes();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Tema1SPG::FrameStart()
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Tema1SPG::DrawHermites(bool onScreen)
{
	glLineWidth(3);
	glUseProgram(shaders["HermiteShader"]->GetProgramID());
	model_matrix = glm::mat4(1);

	glUniformMatrix4fv(shaders["HermiteShader"]->GetUniformLocation(std::string("model_matrix")), 1, false, glm::value_ptr(model_matrix));
	glUniformMatrix4fv(shaders["HermiteShader"]->GetUniformLocation(std::string("view_matrix")), 1, false, glm::value_ptr(view_matrix));
	glUniformMatrix4fv(shaders["HermiteShader"]->GetUniformLocation(std::string("projection_matrix")), 1, false, glm::value_ptr(projection_matrix));
	for (auto curveIt : theCurves)
	{
		CCurve *curve = &(curveIt.second);
		if (curve->type != CCurve::HERMITE)
		{
			continue;
		}
		if (onScreen)
		{
			glUniform3f(shaders["HermiteShader"]->GetUniformLocation(std::string("color")), 0.0, 0.1, 0.1);
			if (activeCurve != NULL && activeCurve->color == curve->color)
			{
				glUniform3f(shaders["HermiteShader"]->GetUniformLocation(std::string("color")), 0.2, 1, 1);
			}
		}
		else
		{
			glLineWidth(7);
			if (activeCurve != NULL && activeCurve->color == curve->color)
			{
				continue;
			}
			glUniform3f(shaders["HermiteShader"]->GetUniformLocation(std::string("color")), curve->color.x, curve->color.y, curve->color.z);
		}
		if (curve->pointsList.size() > 0)
		{
			glUniform3fv(shaders["HermiteShader"]->GetUniformLocation(std::string("control_points")), curve->pointsList.size(), &curve->pointsList[0][0]);
		}
		int numDrawnPoints = curve->pointsList.size();
		int remainder = (numDrawnPoints - 1) % 3;
		if (remainder == 1)
			numDrawnPoints -= 2;
		else if (remainder == 2)
			numDrawnPoints -= 3;
		glUniform1i(shaders["HermiteShader"]->GetUniformLocation(std::string("num_control_points")), numDrawnPoints);
		glUniform1f(shaders["HermiteShader"]->GetUniformLocation(std::string("step")), 0.05f);

		
		for (float t = 0.0; t < 1.0f; t += 0.2f)
		{
			glUniform1f(shaders["HermiteShader"]->GetUniformLocation(std::string("minT")), t);
			glUniform1f(shaders["HermiteShader"]->GetUniformLocation(std::string("maxT")), t + 0.2f);
			lineMesh->drawInstanced(5);
		}
		
	}

//////// SURFACES
	glUseProgram(shaders["HermiteSurfaceShader"]->GetProgramID());
	model_matrix = glm::mat4(1);
	//trimite variabile uniforme la shader

	glUniformMatrix4fv(shaders["HermiteSurfaceShader"]->GetUniformLocation(std::string("model_matrix")), 1, false, glm::value_ptr(model_matrix));
	glUniformMatrix4fv(shaders["HermiteSurfaceShader"]->GetUniformLocation(std::string("view_matrix")), 1, false, glm::value_ptr(view_matrix));
	glUniformMatrix4fv(shaders["HermiteSurfaceShader"]->GetUniformLocation(std::string("projection_matrix")), 1, false, glm::value_ptr(projection_matrix));
	for (auto curveIt : theCurves)
	{
		CCurve *curve = &(curveIt.second);
		if (curve->type != CCurve::HERMITE || !curve->generatesSurface)
		{
			continue;
		}
		if (onScreen)
		{
			glUniform3f(shaders["HermiteSurfaceShader"]->GetUniformLocation(std::string("color")), 0.0, 0.1, 0.1);
			if (activeCurve != NULL && activeCurve->color == curve->color)
			{
				glUniform3f(shaders["HermiteSurfaceShader"]->GetUniformLocation(std::string("color")), 0.2, 1, 1);
			}
		}
		else
		{
			glLineWidth(7);
			if (activeCurve != NULL && activeCurve->color == curve->color)
			{
				continue;
			}
			glUniform3f(shaders["HermiteSurfaceShader"]->GetUniformLocation(std::string("color")), curve->color.x, curve->color.y, curve->color.z);
		}
		if (curve->pointsList.size() > 0)
		{
			glUniform3fv(shaders["HermiteSurfaceShader"]->GetUniformLocation(std::string("control_points")), curve->pointsList.size(), &curve->pointsList[0][0]);
		}
		int numDrawnPoints = curve->pointsList.size();
		int remainder = (numDrawnPoints - 1) % 3;
		if (remainder == 1)
			numDrawnPoints -= 2;
		else if (remainder == 2)
			numDrawnPoints -= 3;
		glUniform1i(shaders["HermiteSurfaceShader"]->GetUniformLocation(std::string("num_control_points")), numDrawnPoints);
		glUniform1i(shaders["HermiteSurfaceShader"]->GetUniformLocation(std::string("surface_type")), curve->generatesSurface);
		glUniform1f(shaders["HermiteSurfaceShader"]->GetUniformLocation(std::string("step")), 0.05f);
		if (onScreen) {
			for (int i = 0; i < curve->numInstances; i++)
			{
				glUniform1f(shaders["HermiteSurfaceShader"]->GetUniformLocation(std::string("offset")), i);
				for (float t = 0.0; t < 1.0f; t += 0.2f)
				{
					glUniform1f(shaders["HermiteSurfaceShader"]->GetUniformLocation(std::string("minT")), t);
					glUniform1f(shaders["HermiteSurfaceShader"]->GetUniformLocation(std::string("maxT")), t + 0.2f);
					lineMesh->drawInstanced(5);
				}
			}
			glUniform3f(shaders["HermiteSurfaceShader"]->GetUniformLocation(std::string("color")), 0.5, 0.5, 0.5);
		}
		else{
			glUniform3f(shaders["HermiteSurfaceShader"]->GetUniformLocation(std::string("color")), curve->color.x, curve->color.y, curve->color.z);
		}
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		
		for (int i = 0; i < curve->numInstances; i++)
		{
			glUniform1f(shaders["HermiteSurfaceShader"]->GetUniformLocation(std::string("offset")), i);
			for (float t = 0.0; t < 1.0f; t += 0.2f)
			{
				glUniform1f(shaders["HermiteSurfaceShader"]->GetUniformLocation(std::string("minT")), t);
				glUniform1f(shaders["HermiteSurfaceShader"]->GetUniformLocation(std::string("maxT")), t + 0.2f);
				lineMesh->drawInstanced(5);
			}
		}

	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Tema1SPG::DrawBeziers(bool onScreen)
{
	glLineWidth(3);
	glUseProgram(shaders["BezierShader"]->GetProgramID());
	model_matrix = glm::mat4(1);

	glUniformMatrix4fv(shaders["BezierShader"]->GetUniformLocation(std::string("model_matrix")), 1, false, glm::value_ptr(model_matrix));
	glUniformMatrix4fv(shaders["BezierShader"]->GetUniformLocation(std::string("view_matrix")), 1, false, glm::value_ptr(view_matrix));
	glUniformMatrix4fv(shaders["BezierShader"]->GetUniformLocation(std::string("projection_matrix")), 1, false, glm::value_ptr(projection_matrix));
	for (auto curveIt : theCurves)
	{
		CCurve *curve = &(curveIt.second);
		if (curve->type != CCurve::BEZIER)
		{
			continue;
		}
		if (onScreen)
		{
			glUniform3f(shaders["BezierShader"]->GetUniformLocation(std::string("color")), 0.0, 0.1, 0.1);
			if (activeCurve != NULL && activeCurve->color == curve->color)
			{
				glUniform3f(shaders["BezierShader"]->GetUniformLocation(std::string("color")), 0.2, 1, 1);
			}
		}
		else
		{
			glLineWidth(7);
			if (activeCurve != NULL && activeCurve->color == curve->color)
			{
				continue;
			}
			glUniform3f(shaders["BezierShader"]->GetUniformLocation(std::string("color")), curve->color.x, curve->color.y, curve->color.z);
		}
		if (curve->pointsList.size() > 0)
		{
			glUniform3fv(shaders["BezierShader"]->GetUniformLocation(std::string("control_points")), curve->pointsList.size(), &curve->pointsList[0][0]);
		}
		int numDrawnPoints = curve->pointsList.size();
	
		glUniform1i(shaders["BezierShader"]->GetUniformLocation(std::string("num_control_points")), numDrawnPoints);
		glUniform1i(shaders["BezierShader"]->GetUniformLocation(std::string("num_curves")), curve->curveOrders.size());
		if (curve->curveOrders.size() > 0)
		{
			glUniform1iv(shaders["BezierShader"]->GetUniformLocation(std::string("curve_orders")), curve->curveOrders.size(), &curve->curveOrders[0]);
		}
		glUniform1f(shaders["BezierShader"]->GetUniformLocation(std::string("step")), 0.05f);


		for (float t = 0.0; t < 1.0f; t += 0.2f)
		{
			glUniform1f(shaders["BezierShader"]->GetUniformLocation(std::string("minT")), t);
			glUniform1f(shaders["BezierShader"]->GetUniformLocation(std::string("maxT")), t + 0.2f);
			lineMesh->drawInstanced(5);
		}

	}

	//////// SURFACES
	glUseProgram(shaders["BezierSurfaceShader"]->GetProgramID());
	model_matrix = glm::mat4(1);
	//trimite variabile uniforme la shader

	glUniformMatrix4fv(shaders["BezierSurfaceShader"]->GetUniformLocation(std::string("model_matrix")), 1, false, glm::value_ptr(model_matrix));
	glUniformMatrix4fv(shaders["BezierSurfaceShader"]->GetUniformLocation(std::string("view_matrix")), 1, false, glm::value_ptr(view_matrix));
	glUniformMatrix4fv(shaders["BezierSurfaceShader"]->GetUniformLocation(std::string("projection_matrix")), 1, false, glm::value_ptr(projection_matrix));
	for (auto curveIt : theCurves)
	{
		CCurve *curve = &(curveIt.second);
		if (curve->type != CCurve::BEZIER || !curve->generatesSurface)
		{
			continue;
		}
		if (onScreen)
		{
			glUniform3f(shaders["BezierSurfaceShader"]->GetUniformLocation(std::string("color")), 0.0, 0.1, 0.1);
			if (activeCurve != NULL && activeCurve->color == curve->color)
			{
				glUniform3f(shaders["BezierSurfaceShader"]->GetUniformLocation(std::string("color")), 0.2, 1, 1);
			}
		}
		else
		{
			glLineWidth(7);
			if (activeCurve != NULL && activeCurve->color == curve->color)
			{
				continue;
			}
			glUniform3f(shaders["BezierSurfaceShader"]->GetUniformLocation(std::string("color")), curve->color.x, curve->color.y, curve->color.z);
		}
		if (curve->pointsList.size() > 0)
		{
			glUniform3fv(shaders["BezierSurfaceShader"]->GetUniformLocation(std::string("control_points")), curve->pointsList.size(), &curve->pointsList[0][0]);
		}
		int numDrawnPoints = curve->pointsList.size();

		glUniform1i(shaders["BezierSurfaceShader"]->GetUniformLocation(std::string("num_control_points")), numDrawnPoints);
		glUniform1i(shaders["BezierSurfaceShader"]->GetUniformLocation(std::string("num_curves")), curve->curveOrders.size());
		if (curve->curveOrders.size() > 0)
		{
			glUniform1iv(shaders["BezierSurfaceShader"]->GetUniformLocation(std::string("curve_orders")), curve->curveOrders.size(), &curve->curveOrders[0]);
		}
		glUniform1i(shaders["BezierSurfaceShader"]->GetUniformLocation(std::string("surface_type")), curve->generatesSurface);
		glUniform1f(shaders["BezierSurfaceShader"]->GetUniformLocation(std::string("step")), 0.05f);
		if (onScreen){
			for (int i = 0; i < curve->numInstances; i++)
			{
				glUniform1f(shaders["BezierSurfaceShader"]->GetUniformLocation(std::string("offset")), i);
				for (float t = 0.0; t < 1.0f; t += 0.2f)
				{
					glUniform1f(shaders["BezierSurfaceShader"]->GetUniformLocation(std::string("minT")), t);
					glUniform1f(shaders["BezierSurfaceShader"]->GetUniformLocation(std::string("maxT")), t + 0.2f);
					lineMesh->drawInstanced(5);
				}
			}
			glUniform3f(shaders["BezierSurfaceShader"]->GetUniformLocation(std::string("color")), 0.5, 0.5, 0.5);
		}
		else
		{
			glUniform3f(shaders["BezierSurfaceShader"]->GetUniformLocation(std::string("color")), curve->color.x, curve->color.y, curve->color.z);
		}
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		for (int i = 0; i < curve->numInstances; i++)
		{
			glUniform1f(shaders["BezierSurfaceShader"]->GetUniformLocation(std::string("offset")), i);
			for (float t = 0.0; t < 1.0f; t += 0.2f)
			{
				glUniform1f(shaders["BezierSurfaceShader"]->GetUniformLocation(std::string("minT")), t);
				glUniform1f(shaders["BezierSurfaceShader"]->GetUniformLocation(std::string("maxT")), t + 0.2f);
				lineMesh->drawInstanced(5);
			}
		}

	}

}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Tema1SPG::DrawNURBS(bool onScreen)
{
	glLineWidth(3);
	glUseProgram(shaders["NURBSShader"]->GetProgramID());
	model_matrix = glm::mat4(1);

	glUniformMatrix4fv(shaders["NURBSShader"]->GetUniformLocation(std::string("model_matrix")), 1, false, glm::value_ptr(model_matrix));
	glUniformMatrix4fv(shaders["NURBSShader"]->GetUniformLocation(std::string("view_matrix")), 1, false, glm::value_ptr(view_matrix));
	glUniformMatrix4fv(shaders["NURBSShader"]->GetUniformLocation(std::string("projection_matrix")), 1, false, glm::value_ptr(projection_matrix));
	for (auto curveIt : theCurves)
	{
		CCurve *curve = &(curveIt.second);
		if (curve->type != CCurve::NURBS)
		{
			continue;
		}
		if (onScreen)
		{
			glUniform3f(shaders["NURBSShader"]->GetUniformLocation(std::string("color")), 0.0, 0.1, 0.1);
			if (activeCurve != NULL && activeCurve->color == curve->color)
			{
				glUniform3f(shaders["NURBSShader"]->GetUniformLocation(std::string("color")), 0.2, 1, 1);
			}
		}
		else
		{
			glLineWidth(7);
			if (activeCurve != NULL && activeCurve->color == curve->color)
			{
				continue;
			}
			glUniform3f(shaders["NURBSShader"]->GetUniformLocation(std::string("color")), curve->color.x, curve->color.y, curve->color.z);
		}
		if (curve->pointsList.size() > 0)
		{
			glUniform3fv(shaders["NURBSShader"]->GetUniformLocation(std::string("control_points")), curve->pointsList.size(), &curve->pointsList[0][0]);
		}
		int numDrawnPoints = curve->pointsList.size();

		glUniform1i(shaders["NURBSShader"]->GetUniformLocation(std::string("num_control_points")), numDrawnPoints);
		glUniform1f(shaders["NURBSShader"]->GetUniformLocation(std::string("step")), 0.05f);


		for (float t = 0.0; t < 1.0f; t += 0.2f)
		{
			glUniform1f(shaders["NURBSShader"]->GetUniformLocation(std::string("minT")), t);
			glUniform1f(shaders["NURBSShader"]->GetUniformLocation(std::string("maxT")), t + 0.2f);
			lineMesh->drawInstanced(5);
		}

	}

	//////// SURFACES
	glUseProgram(shaders["NURBSSurfaceShader"]->GetProgramID());
	model_matrix = glm::mat4(1);
	//trimite variabile uniforme la shader

	glUniformMatrix4fv(shaders["NURBSSurfaceShader"]->GetUniformLocation(std::string("model_matrix")), 1, false, glm::value_ptr(model_matrix));
	glUniformMatrix4fv(shaders["NURBSSurfaceShader"]->GetUniformLocation(std::string("view_matrix")), 1, false, glm::value_ptr(view_matrix));
	glUniformMatrix4fv(shaders["NURBSSurfaceShader"]->GetUniformLocation(std::string("projection_matrix")), 1, false, glm::value_ptr(projection_matrix));
	for (auto curveIt : theCurves)
	{
		CCurve *curve = &(curveIt.second);
		if (curve->type != CCurve::NURBS || !curve->generatesSurface)
		{
			continue;
		}
		if (onScreen)
		{
			glUniform3f(shaders["NURBSSurfaceShader"]->GetUniformLocation(std::string("color")), 0.0, 0.1, 0.1);
			if (activeCurve != NULL && activeCurve->color == curve->color)
			{
				glUniform3f(shaders["NURBSSurfaceShader"]->GetUniformLocation(std::string("color")), 0.2, 1, 1);
			}
		}
		else
		{
			glLineWidth(7);
			if (activeCurve != NULL && activeCurve->color == curve->color)
			{
				continue;
			}
			glUniform3f(shaders["NURBSSurfaceShader"]->GetUniformLocation(std::string("color")), curve->color.x, curve->color.y, curve->color.z);
		}
		if (curve->pointsList.size() > 0)
		{
			glUniform3fv(shaders["NURBSSurfaceShader"]->GetUniformLocation(std::string("control_points")), curve->pointsList.size(), &curve->pointsList[0][0]);
		}
		int numDrawnPoints = curve->pointsList.size();

		glUniform1i(shaders["NURBSSurfaceShader"]->GetUniformLocation(std::string("num_control_points")), numDrawnPoints);
		glUniform1i(shaders["NURBSSurfaceShader"]->GetUniformLocation(std::string("surface_type")), curve->generatesSurface);
		glUniform1f(shaders["NURBSSurfaceShader"]->GetUniformLocation(std::string("step")), 0.05f);
		if (onScreen){
			for (int i = 0; i < curve->numInstances; i++)
			{
				glUniform1f(shaders["NURBSSurfaceShader"]->GetUniformLocation(std::string("offset")), i);
				for (float t = 0.0; t < 1.0f; t += 0.2f)
				{
					glUniform1f(shaders["NURBSSurfaceShader"]->GetUniformLocation(std::string("minT")), t);
					glUniform1f(shaders["NURBSSurfaceShader"]->GetUniformLocation(std::string("maxT")), t + 0.2f);
					lineMesh->drawInstanced(5);
				}
			}
			glUniform3f(shaders["NURBSSurfaceShader"]->GetUniformLocation(std::string("color")), 0.5, 0.5, 0.5);
		}
		else
		{
			glUniform3f(shaders["NURBSSurfaceShader"]->GetUniformLocation(std::string("color")), curve->color.x, curve->color.y, curve->color.z);
		}
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		
		for (int i = 0; i < curve->numInstances; i++)
		{
			glUniform1f(shaders["NURBSSurfaceShader"]->GetUniformLocation(std::string("offset")), i);
			for (float t = 0.0; t < 1.0f; t += 0.2f)
			{
				glUniform1f(shaders["NURBSSurfaceShader"]->GetUniformLocation(std::string("minT")), t);
				glUniform1f(shaders["NURBSSurfaceShader"]->GetUniformLocation(std::string("maxT")), t + 0.2f);
				lineMesh->drawInstanced(5);
			}
		}

	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Tema1SPG::DrawSplines(bool onScreen)
{
	glLineWidth(3);
	glUseProgram(shaders["SplineShader"]->GetProgramID());
	model_matrix = glm::mat4(1);

	glUniformMatrix4fv(shaders["SplineShader"]->GetUniformLocation(std::string("model_matrix")), 1, false, glm::value_ptr(model_matrix));
	glUniformMatrix4fv(shaders["SplineShader"]->GetUniformLocation(std::string("view_matrix")), 1, false, glm::value_ptr(view_matrix));
	glUniformMatrix4fv(shaders["SplineShader"]->GetUniformLocation(std::string("projection_matrix")), 1, false, glm::value_ptr(projection_matrix));
	for (auto curveIt : theCurves)
	{
		CCurve *curve = &(curveIt.second);
		if (curve->type != CCurve::SPLINE)
		{
			continue;
		}
		if (onScreen)
		{
			glUniform3f(shaders["SplineShader"]->GetUniformLocation(std::string("color")), 0.0, 0.1, 0.1);
			if (activeCurve != NULL && activeCurve->color == curve->color)
			{
				glUniform3f(shaders["SplineShader"]->GetUniformLocation(std::string("color")), 0.2, 1, 1);
			}
		}
		else
		{
			glLineWidth(7);
			if (activeCurve != NULL && activeCurve->color == curve->color)
			{
				continue;
			}
			glUniform3f(shaders["SplineShader"]->GetUniformLocation(std::string("color")), curve->color.x, curve->color.y, curve->color.z);
		}
		if (curve->pointsList.size() > 0)
		{
			glUniform3fv(shaders["SplineShader"]->GetUniformLocation(std::string("control_points")), curve->pointsList.size(), &curve->pointsList[0][0]);
		}
		int numDrawnPoints = curve->pointsList.size();

		glUniform1i(shaders["SplineShader"]->GetUniformLocation(std::string("num_control_points")), numDrawnPoints);
		glUniform1f(shaders["SplineShader"]->GetUniformLocation(std::string("step")), 0.05f);


		for (float t = 0.0; t < 1.0f; t += 0.2f)
		{
			glUniform1f(shaders["SplineShader"]->GetUniformLocation(std::string("minT")), t);
			glUniform1f(shaders["SplineShader"]->GetUniformLocation(std::string("maxT")), t + 0.2f);
			lineMesh->drawInstanced(5);
		}

	}

	//////// SURFACES
	glUseProgram(shaders["SplineSurfaceShader"]->GetProgramID());
	model_matrix = glm::mat4(1);
	//trimite variabile uniforme la shader

	glUniformMatrix4fv(shaders["SplineSurfaceShader"]->GetUniformLocation(std::string("model_matrix")), 1, false, glm::value_ptr(model_matrix));
	glUniformMatrix4fv(shaders["SplineSurfaceShader"]->GetUniformLocation(std::string("view_matrix")), 1, false, glm::value_ptr(view_matrix));
	glUniformMatrix4fv(shaders["SplineSurfaceShader"]->GetUniformLocation(std::string("projection_matrix")), 1, false, glm::value_ptr(projection_matrix));
	for (auto curveIt : theCurves)
	{
		CCurve *curve = &(curveIt.second);
		if (curve->type != CCurve::SPLINE || !curve->generatesSurface)
		{
			continue;
		}
		if (onScreen)
		{
			glUniform3f(shaders["SplineSurfaceShader"]->GetUniformLocation(std::string("color")), 0.0, 0.1, 0.1);
			if (activeCurve != NULL && activeCurve->color == curve->color)
			{
				glUniform3f(shaders["SplineSurfaceShader"]->GetUniformLocation(std::string("color")), 0.2, 1, 1);
			}
		}
		else
		{
			glLineWidth(7);
			if (activeCurve != NULL && activeCurve->color == curve->color)
			{
				continue;
			}
			glUniform3f(shaders["SplineSurfaceShader"]->GetUniformLocation(std::string("color")), curve->color.x, curve->color.y, curve->color.z);
		}
		if (curve->pointsList.size() > 0)
		{
			glUniform3fv(shaders["SplineSurfaceShader"]->GetUniformLocation(std::string("control_points")), curve->pointsList.size(), &curve->pointsList[0][0]);
		}
		int numDrawnPoints = curve->pointsList.size();

		glUniform1i(shaders["SplineSurfaceShader"]->GetUniformLocation(std::string("num_control_points")), numDrawnPoints);
		glUniform1i(shaders["SplineSurfaceShader"]->GetUniformLocation(std::string("surface_type")), curve->generatesSurface);
		glUniform1f(shaders["SplineSurfaceShader"]->GetUniformLocation(std::string("step")), 0.05f);
		if (onScreen){
			for (int i = 0; i < curve->numInstances; i++)
			{
				glUniform1f(shaders["SplineSurfaceShader"]->GetUniformLocation(std::string("offset")), i);
				for (float t = 0.0; t < 1.0f; t += 0.2f)
				{
					glUniform1f(shaders["SplineSurfaceShader"]->GetUniformLocation(std::string("minT")), t);
					glUniform1f(shaders["SplineSurfaceShader"]->GetUniformLocation(std::string("maxT")), t + 0.2f);
					lineMesh->drawInstanced(5);
				}
			}
			glUniform3f(shaders["SplineSurfaceShader"]->GetUniformLocation(std::string("color")), 0.5, 0.5, 0.5);
		}
		else
		{
			glUniform3f(shaders["SplineSurfaceShader"]->GetUniformLocation(std::string("color")), curve->color.x, curve->color.y, curve->color.z);
		}
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
		for (int i = 0; i < curve->numInstances; i++)
		{
			glUniform1f(shaders["SplineSurfaceShader"]->GetUniformLocation(std::string("offset")), i);
			for (float t = 0.0; t < 1.0f; t += 0.2f)
			{
				glUniform1f(shaders["SplineSurfaceShader"]->GetUniformLocation(std::string("minT")), t);
				glUniform1f(shaders["SplineSurfaceShader"]->GetUniformLocation(std::string("maxT")), t + 0.2f);
				lineMesh->drawInstanced(5);
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Tema1SPG::DrawBSplines(bool onScreen)
{
	glLineWidth(3);
	glUseProgram(shaders["BSplineShader"]->GetProgramID());
	model_matrix = glm::mat4(1);

	glUniformMatrix4fv(shaders["BSplineShader"]->GetUniformLocation(std::string("model_matrix")), 1, false, glm::value_ptr(model_matrix));
	glUniformMatrix4fv(shaders["BSplineShader"]->GetUniformLocation(std::string("view_matrix")), 1, false, glm::value_ptr(view_matrix));
	glUniformMatrix4fv(shaders["BSplineShader"]->GetUniformLocation(std::string("projection_matrix")), 1, false, glm::value_ptr(projection_matrix));
	for (auto curveIt : theCurves)
	{
		CCurve *curve = &(curveIt.second);
		if (curve->type != CCurve::BSPLINE)
		{
			continue;
		}
		if (onScreen)
		{
			glUniform3f(shaders["BSplineShader"]->GetUniformLocation(std::string("color")), 0.0, 0.1, 0.1);
			if (activeCurve != NULL && activeCurve->color == curve->color)
			{
				glUniform3f(shaders["BSplineShader"]->GetUniformLocation(std::string("color")), 0.2, 1, 1);
			}
		}
		else
		{
			glLineWidth(7);
			if (activeCurve != NULL && activeCurve->color == curve->color)
			{
				continue;
			}
			glUniform3f(shaders["BSplineShader"]->GetUniformLocation(std::string("color")), curve->color.x, curve->color.y, curve->color.z);
		}
		if (curve->pointsList.size() > 0)
		{
			glUniform3fv(shaders["BSplineShader"]->GetUniformLocation(std::string("control_points")), curve->pointsList.size(), &curve->pointsList[0][0]);
		}
		int numDrawnPoints = curve->pointsList.size();

		glUniform1i(shaders["BSplineShader"]->GetUniformLocation(std::string("num_control_points")), numDrawnPoints);
		glUniform1f(shaders["BSplineShader"]->GetUniformLocation(std::string("step")), 0.05f);


		for (float t = 0.0; t < 1.0f; t += 0.2f)
		{
			glUniform1f(shaders["BSplineShader"]->GetUniformLocation(std::string("minT")), t);
			glUniform1f(shaders["BSplineShader"]->GetUniformLocation(std::string("maxT")), t + 0.2f);
			lineMesh->drawInstanced(5);
		}

	}

	//////// SURFACES
	glUseProgram(shaders["BSplineSurfaceShader"]->GetProgramID());
	model_matrix = glm::mat4(1);
	//trimite variabile uniforme la shader

	glUniformMatrix4fv(shaders["BSplineSurfaceShader"]->GetUniformLocation(std::string("model_matrix")), 1, false, glm::value_ptr(model_matrix));
	glUniformMatrix4fv(shaders["BSplineSurfaceShader"]->GetUniformLocation(std::string("view_matrix")), 1, false, glm::value_ptr(view_matrix));
	glUniformMatrix4fv(shaders["BSplineSurfaceShader"]->GetUniformLocation(std::string("projection_matrix")), 1, false, glm::value_ptr(projection_matrix));
	for (auto curveIt : theCurves)
	{
		CCurve *curve = &(curveIt.second);
		if (curve->type != CCurve::BSPLINE || !curve->generatesSurface)
		{
			continue;
		}
		if (onScreen)
		{
			glUniform3f(shaders["BSplineSurfaceShader"]->GetUniformLocation(std::string("color")), 0.0, 0.1, 0.1);
			if (activeCurve != NULL && activeCurve->color == curve->color)
			{
				glUniform3f(shaders["BSplineSurfaceShader"]->GetUniformLocation(std::string("color")), 0.2, 1, 1);
			}
		}
		else
		{
			glLineWidth(7);
			if (activeCurve != NULL && activeCurve->color == curve->color)
			{
				continue;
			}
			glUniform3f(shaders["BSplineSurfaceShader"]->GetUniformLocation(std::string("color")), curve->color.x, curve->color.y, curve->color.z);
		}
		if (curve->pointsList.size() > 0)
		{
			glUniform3fv(shaders["BSplineSurfaceShader"]->GetUniformLocation(std::string("control_points")), curve->pointsList.size(), &curve->pointsList[0][0]);
		}
		int numDrawnPoints = curve->pointsList.size();

		glUniform1i(shaders["BSplineSurfaceShader"]->GetUniformLocation(std::string("num_control_points")), numDrawnPoints);
		glUniform1i(shaders["BSplineSurfaceShader"]->GetUniformLocation(std::string("surface_type")), curve->generatesSurface);
		glUniform1f(shaders["BSplineSurfaceShader"]->GetUniformLocation(std::string("step")), 0.05f);
		if(onScreen){
			for (int i = 0; i < curve->numInstances; i++)
			{
				glUniform1f(shaders["BSplineSurfaceShader"]->GetUniformLocation(std::string("offset")), i);
				for (float t = 0.0; t < 1.0f; t += 0.2f)
				{
					glUniform1f(shaders["BSplineSurfaceShader"]->GetUniformLocation(std::string("minT")), t);
					glUniform1f(shaders["BSplineSurfaceShader"]->GetUniformLocation(std::string("maxT")), t + 0.2f);
					lineMesh->drawInstanced(5);
				}
			}
			glUniform3f(shaders["BSplineSurfaceShader"]->GetUniformLocation(std::string("color")), 0.5, 0.5, 0.5);
		}
		else
		{
			glUniform3f(shaders["BSplineSurfaceShader"]->GetUniformLocation(std::string("color")), curve->color.x, curve->color.y, curve->color.z);
		}
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		for (int i = 0; i < curve->numInstances; i++)
		{
			glUniform1f(shaders["BSplineSurfaceShader"]->GetUniformLocation(std::string("offset")), i);
			for (float t = 0.0; t < 1.0f; t += 0.2f)
			{
				glUniform1f(shaders["BSplineSurfaceShader"]->GetUniformLocation(std::string("minT")), t);
				glUniform1f(shaders["BSplineSurfaceShader"]->GetUniformLocation(std::string("maxT")), t + 0.2f);
				lineMesh->drawInstanced(5);
			}
		}
	}
}

void Tema1SPG::Update(float deltaTimeSeconds)
{
	
	
	m_deltaTime = deltaTimeSeconds;

	colorPickingFB.unbind();
	glEnable(GL_DEPTH_TEST);
	glLineWidth(3);
	//pe tot ecranul
	glViewport(m_width / fraction , 0, m_width - m_width / fraction, m_height);
	glClearColor(0.25, 0.25, 0.25, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	view_matrix = camera.GetViewMatrix();
	
	glUseProgram(shaders["DullColorShader"]->GetProgramID());

	projection_matrix = glm::perspective(45.f, 1.4f, 1.f, 1000.f);
	glUniformMatrix4fv(shaders["DullColorShader"]->GetUniformLocation(std::string("view_matrix")), 1, false, glm::value_ptr(view_matrix));
	glUniformMatrix4fv(shaders["DullColorShader"]->GetUniformLocation(std::string("projection_matrix")), 1, false, glm::value_ptr(projection_matrix));
	//foloseste shaderul
	
	DrawHermites(true);
	DrawBeziers(true);
	DrawNURBS(true);
	DrawSplines(true);
	DrawBSplines(true);

	grid->DrawGrid();
	
	
///////////DRAW POINTS
	glUseProgram(shaders["DullColorShader"]->GetProgramID());
	glDisable(GL_DEPTH_TEST);
	model_matrix = glm::mat4(1);
	glUniformMatrix4fv(shaders["DullColorShader"]->GetUniformLocation(std::string("model_matrix")), 1, false, glm::value_ptr(model_matrix));
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glUniform3f(shaders["DullColorShader"]->GetUniformLocation(std::string("color")), 1, 1, 1);
	
	glPointSize(14);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (activeCurve != NULL)
	{
		if (selectedIndex == -1)
		{
			if (toolType == MOVE_TOOL)
			{
				gizmo->SetVisible(true);
				gizmoPos = activeCurve->position;
			}
			else
			{
				gizmo->SetVisible(false);
			}
			for (int i = 0; i < activeCurve->pointsList.size(); i++)
			{
				model_matrix = glm::translate(glm::mat4(1), activeCurve->pointsList[i]);
				glUniformMatrix4fv(shaders["DullColorShader"]->GetUniformLocation(std::string("model_matrix")), 1, false, glm::value_ptr(model_matrix));
				pointMesh->draw(GL_POINTS);
			}
			
		}
		else
		{
			if (toolType == MOVE_TOOL)
			{
				gizmo->SetVisible(true);
				gizmoPos = activeCurve->pointsList[selectedIndex];
			}
			else
			{
				gizmo->SetVisible(false);
			}
			//gizmo->Render(gizmoPos);
			glUniform3f(shaders["DullColorShader"]->GetUniformLocation(std::string("color")), 1, 1, 1);
			for (int i = 0; i < selectedIndex; i++)
			{
				model_matrix = glm::translate(glm::mat4(1), activeCurve->pointsList[i]);
				glUniformMatrix4fv(shaders["DullColorShader"]->GetUniformLocation(std::string("model_matrix")), 1, false, glm::value_ptr(model_matrix));
				pointMesh->draw(GL_POINTS);
			}
			
			glUniform3f(shaders["DullColorShader"]->GetUniformLocation(std::string("color")), 0.5, 1, 0);
			model_matrix = glm::translate(glm::mat4(1), activeCurve->pointsList[selectedIndex]);
			glUniformMatrix4fv(shaders["DullColorShader"]->GetUniformLocation(std::string("model_matrix")), 1, false, glm::value_ptr(model_matrix));
			pointMesh->draw(GL_POINTS);

			glUniform3f(shaders["DullColorShader"]->GetUniformLocation(std::string("color")), 1, 1, 1);
			for (int i = selectedIndex + 1; i < activeCurve->pointsList.size(); i++)
			{
				model_matrix = glm::translate(glm::mat4(1), activeCurve->pointsList[i]);
				glUniformMatrix4fv(shaders["DullColorShader"]->GetUniformLocation(std::string("model_matrix")), 1, false, glm::value_ptr(model_matrix));
				pointMesh->draw(GL_POINTS);
			}	
		}
	}
	else
	{
		gizmo->SetVisible(false);
	}
	
	gizmo->Render(camera, gizmoPos);
	
	///////////////////////////////// COLOR PICKING FB /////////////////////////////// 
	colorPickingFB.bind();
	glClearColor(0.0f, 0.0f, 0.0f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(m_width / fraction, 0, m_width - m_width / fraction, m_height);
	glEnable(GL_DEPTH_TEST);
	
	DrawHermites(false);
	DrawBeziers(false);
	DrawNURBS(false);
	DrawSplines(false);
	DrawBSplines(false); 
	
	glUseProgram(shaders["DullColorShader"]->GetProgramID());
	
	if (activeCurve != NULL)
	{
		for (int i = 0; i < activeCurve->pointsColors.size(); i++)
		{
			model_matrix = glm::translate(glm::mat4(1), activeCurve->pointsList[i]);
			glUniform3f(shaders["DullColorShader"]->GetUniformLocation(std::string("color")), (float)activeCurve->pointsColors[i].x / 255.f, (float)activeCurve->pointsColors[i].y / 255.f, (float)activeCurve->pointsColors[i].z / 255.f);
			glUniformMatrix4fv(shaders["DullColorShader"]->GetUniformLocation(std::string("model_matrix")), 1, false, glm::value_ptr(model_matrix));
			pointMesh->draw(GL_POINTS);
		}
		if (selectedIndex != -1)
		{
			glDisable(GL_DEPTH_TEST);
			//gizmo->Render(gizmoPos);
		}
	}
	glLineWidth(8);
	gizmo->Render(camera, gizmoPos);
	
	colorPickingFB.unbind();
	
	RenderButtons();
	
	colorPickingFB.bind();
	
	glReadPixels(0, 0, m_width, m_height, GL_RGB, GL_UNSIGNED_BYTE, readPixels);
	
	glDeleteTextures(1, &quadTexture);
	
	quadTexture = loadTexture(readPixels, m_width, m_height);
	
	colorPickingFB.unbind();
	
}

////////////////////////////////////////////////////////////////////////////////
void Tema1SPG::RenderButtons()
{
	glViewport(0, 0, m_width, m_height);
	
	Texture2D *selectBtnBG = buttonUp, *moveBtnBG = buttonUp, *createCurveBtnBG = buttonUp;
	
	switch(toolType)
	{
	case SELECT_TOOL:
		selectBtnBG = buttonDown;
		break;
	case MOVE_TOOL:
		moveBtnBG = buttonDown;
		break;
	default:
	case CREATE_CURVE:
		createCurveBtnBG = buttonDown;
		break;
	}
	m_sprite->SetCorners(glm::vec3(-1, 0.8, 0), glm::vec3(-0.9, 1, 0));
	m_sprite->Render(selectBtnBG);
	m_sprite->Render(selectToolPic);

	m_sprite->SetCorners(glm::vec3(-1, 0.6, 0), glm::vec3(-0.9, 0.8, 0));
	m_sprite->Render(moveBtnBG);
	m_sprite->Render(moveToolPic);

	m_sprite->SetCorners(glm::vec3(-1, 0.4, 0), glm::vec3(-0.9, 0.6, 0));
	m_sprite->Render(createCurveBtnBG);
	m_sprite->Render(createCurveToolPic);


	if (toolType == CREATE_CURVE)
	{
		Texture2D *hermiteBtn, *bezierBtn, *nurbsBtn, *splineBtn, *bsplineBtn;
		hermiteBtn = bezierBtn = nurbsBtn = splineBtn = bsplineBtn = buttonUp;
		switch (newCurveType)
		{
		case CCurve::HERMITE:
			hermiteBtn = buttonDown;
			break;
		case CCurve::BEZIER:
			bezierBtn = buttonDown;
			break;
		case CCurve::NURBS:
			nurbsBtn = buttonDown;
			break;
		case CCurve::SPLINE:
			splineBtn = buttonDown;
			break;
		case CCurve::BSPLINE:
			bsplineBtn = buttonDown;
			break;
		default:
			break;
		}
		m_sprite->SetCorners(glm::vec3(-0.98, 0.39 - 0.06f * (0 + 1), 0), glm::vec3(-0.9, 0.44 - 0.06f * (0 + 1), 0));
		m_sprite->Render(hermiteBtn);
		m_sprite->Render(hermiteBtnPic);

		m_sprite->SetCorners(glm::vec3(-0.98, 0.39 - 0.06f * (1 + 1), 0), glm::vec3(-0.9, 0.44 - 0.06f * (1 + 1), 0));
		m_sprite->Render(bezierBtn);
		m_sprite->Render(bezierBtnPic);

		m_sprite->SetCorners(glm::vec3(-0.98, 0.39 - 0.06f * (2 + 1), 0), glm::vec3(-0.9, 0.44 - 0.06f * (2 + 1), 0));
		m_sprite->Render(nurbsBtn);
		m_sprite->Render(nurbsBtnPic);

		m_sprite->SetCorners(glm::vec3(-0.98, 0.39 - 0.06f * (3 + 1), 0), glm::vec3(-0.9, 0.44 - 0.06f * (3 + 1), 0));
		m_sprite->Render(splineBtn);
		m_sprite->Render(splineBtnPic);

		m_sprite->SetCorners(glm::vec3(-0.98, 0.39 - 0.06f * (4 + 1), 0), glm::vec3(-0.9, 0.44 - 0.06f * (4 + 1), 0));
		m_sprite->Render(bsplineBtn);
		m_sprite->Render(bsplineBtnPic);
		
	}
	//COLOR PICKING FB
	colorPickingFB.bind();
	m_sprite->SetCorners(glm::vec3(-1, 0.8, 0), glm::vec3(-0.9, 1, 0));
	m_sprite->Render(glm::vec3(1, 1, 0));
	m_sprite->SetCorners(glm::vec3(-1, 0.6, 0), glm::vec3(-0.9, 0.8, 0));
	m_sprite->Render(glm::vec3(1, 0, 1));
	m_sprite->SetCorners(glm::vec3(-1, 0.4, 0), glm::vec3(-0.9, 0.6, 0));
	m_sprite->Render(glm::vec3(0, 1, 1));
	
	if (toolType == CREATE_CURVE) {
		std::vector<glm::vec3> types = { glm::vec3(0.5, 0, 0), glm::vec3(0, 0.5, 0), glm::vec3(0, 0, 0.5), 
										  glm::vec3(0.5, 0.5, 0), glm::vec3(0.5, 0, 0.5) };
		for (int i = 0; i < 5; i++)
		{
			m_sprite->SetCorners(glm::vec3(-0.98, 0.39 - 0.06f * (i+1), 0), glm::vec3(-0.9, 0.44 - 0.06f * (i + 1), 0));
			m_sprite->Render(types[i]);
		}
	}

	colorPickingFB.unbind();
#define	DEBUG_MODE_READPIXELS
#ifdef DEBUG_MODE_READPIXELS
	if (showColorPickingFB)
		fsQuad->Render(quadTexture);
#endif

}
//functie chemata dupa ce am terminat cadrul de desenare (poate fi folosita pt modelare/simulare)
void notifyEndFrame() {}
//functei care e chemata cand se schimba dimensiunea ferestrei initiale
void Tema1SPG::OnWindowResize(int width, int height)
{
	//reshape
	if (height == 0) height = 1;
	glViewport(m_width / fraction, 0, m_width - m_width / fraction, m_height);
	m_width = width; m_height = height;
	float aspect = (float)width / (float)height;
	colorPickingFB.destroy();
	colorPickingFB.generate(width, height);
	free(readPixels);
	readPixels = (GLubyte*)malloc(3 * width * height);
	aspect = (float)(m_width - m_width / fraction) / (float)height;
	projection_matrix = glm::perspective(75.0f, aspect, 0.1f, 10000.0f);
}

void Tema1SPG::FrameEnd()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	//DrawCoordinatSystem();
}

// Documentation for the input functions can be found in: "/Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Tema1SPG::OnInputUpdate(float deltaTime, int mods)
{
	float speed = 2;

	if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		glm::vec3 up = glm::vec3(0, 1, 0);
		glm::vec3 right = GetSceneCamera()->transform->GetLocalOXVector();
		glm::vec3 forward = GetSceneCamera()->transform->GetLocalOZVector();
		forward = glm::normalize(glm::vec3(forward.x, 0, forward.z));
	}
}

void Tema1SPG::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_DELETE)
	{
		if (activeCurve != NULL)
		{
			if (selectedIndex != -1)
			{
				activeCurve->DeletePoint(selectedIndex);
				selectedIndex = -1;
			}
			else
			{
				glm::uvec3 uv = glm::uvec3(activeCurve->color.x*255.f, activeCurve->color.y*255.f, activeCurve->color.z*255.f);
				activeCurve = NULL; 
				theCurves.erase(calculateColorHash(uv));				
			}
		}
	}else
	if (key == GLFW_KEY_F)
	{
		if (activeCurve != NULL)
		{
			activeCurve->generatesSurface = (activeCurve->generatesSurface + 1) % 3;
		}
	}else
	if (key == GLFW_KEY_COMMA)
	{
		if (activeCurve != NULL)
		{
			if (activeCurve != NULL)
			{
				if (activeCurve->numInstances <= 24)
					activeCurve->numInstances += 1;
			}
		}
	}
	if (key == GLFW_KEY_PERIOD)
	{
		if (activeCurve != NULL)
		{
			if(activeCurve->numInstances >= 2)
				activeCurve->numInstances -= 1;
		}
	}


	if (key == GLFW_KEY_LEFT_ALT || key == GLFW_KEY_RIGHT_ALT)
	{
		glfwSetInputMode(window->GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		m_altDown = true;
		return;
	}
	
	keyStates[key] = true;
	if (key == 27)
	{
		exit(0);
	}
	else
	if (key == 32)
	{
		for(auto s : shaders)
		{
			s.second->Reload();
		}			
	}else
	if (key == GLFW_KEY_S)
	{
		showColorPickingFB = !showColorPickingFB;
	}else
	if (key == GLFW_KEY_Z)
	{
		camera = Camera(glm::vec3(-5, 10, 75), glm::vec3(5, 10, 0), glm::vec3(0, 1, 0));
	}
	else if (key == GLFW_KEY_Q)
	{
		toolType = SELECT_TOOL;
	}
	else if (key == GLFW_KEY_W)
	{
		toolType = MOVE_TOOL;
	}
	else if (key == GLFW_KEY_C)
	{
		toolType = CREATE_CURVE;
	}
}

void Tema1SPG::OnKeyRelease(int key, int mods)
{
	if (key == GLFW_KEY_LEFT_ALT || key == GLFW_KEY_RIGHT_ALT)
	{
		glfwSetInputMode(window->GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		m_altDown = false;
		return;
	}
	keyStates[key] = false;
	
}

void Tema1SPG::ForceRedraw()
{
	FrameStart();
	Update(0.02);
	FrameEnd();
	window->SwapBuffers();
}


void Tema1SPG::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	if (m_altDown)
	{
		float dx = (float)deltaX;
		float dy = (float)deltaY;

		if (m_LMB)
		{
			if (activeCurve == NULL){
				camera.RotateAroundOriginX(-dy * 0.0499f * m_deltaTime);
				camera.RotateAroundOriginY(-dx * 0.0499f * m_deltaTime);
			}
			else
			{
				camera.RotateAroundPointX(-dy * 0.0499f * m_deltaTime, gizmoPos);// activeCurve->position);
				camera.RotateAroundPointY(-dx * 0.0499f * m_deltaTime, gizmoPos);// activeCurve->position);

			}
			ForceRedraw();
		}
		else if (m_RMB)
		{
			camera.TranslateAlongZ((-dy + dx) * 0.0999f * m_deltaTime);
			ForceRedraw();
		}
		else if (m_MMB)
		{
			camera.TranslateAlongX(-dx * 0.099f * m_deltaTime);
			camera.TranslateAlongY(dy * 0.099f * m_deltaTime);
			ForceRedraw();
		}
		
		//glfwSetCursorPos(window->GetGLFWWindow(), m_width / 2, m_height / 2);
	}
	else
	{
		float dx = mouseX - prev_mousePos.x;
		float dy = prev_mousePos.y - mouseY;
		
		if (activeCurve != NULL) {
			if (selectedIndex != -1)
			{
				glm::vec2 dir = glm::vec2(mouseX, -mouseY) - glm::vec2(prev_mousePos.x, -prev_mousePos.y);
				if (gizmo->getSelectedX())
				{
					glm::vec2 ssdir = glm::vec2(projection_matrix * view_matrix * glm::translate(glm::mat4(1), gizmoPos) * glm::vec4(-1, 0, 0, 1));
					ssdir -= glm::vec2(projection_matrix * view_matrix * glm::translate(glm::mat4(1), gizmoPos) * glm::vec4(-0.5, 0, 0, 1));
		
					float dirlen = sqrt(glm::length(dir));
					float ssdirlen = glm::length(ssdir);
					if (dirlen > 0.001 && ssdirlen > 0.001) {
						float dotprod = glm::dot(glm::normalize(dir), glm::normalize(ssdir));// - prev_ssdir));

						activeCurve->pointsList[selectedIndex] = glm::vec3(glm::translate(glm::mat4(1), glm::vec3(-dirlen * dotprod, 0, 0)) *
							glm::vec4(activeCurve->pointsList[selectedIndex], 1));
						gizmoPos = activeCurve->pointsList[selectedIndex];
					}
					prev_ssdir = ssdir;
					activeCurve->UpdatePos();
					//gizmoPos = activeCurve->position;

				}
				else if (gizmo->getSelectedY())
				{
					glm::vec2 ssdir = glm::vec2(projection_matrix * view_matrix * glm::translate(glm::mat4(1), gizmoPos) * glm::vec4(0, 1, 0, 1));
					ssdir -= glm::vec2(projection_matrix * view_matrix * glm::translate(glm::mat4(1), gizmoPos) * glm::vec4(0, 0.5, 0, 1));
		
					float dirlen = sqrt(glm::length(dir));
					float ssdirlen = glm::length(ssdir);
					if (dirlen > 0.001 && ssdirlen > 0.001)
					{
						float dotprod = glm::dot(glm::normalize(dir), glm::normalize(ssdir));// - prev_ssdir));
						activeCurve->pointsList[selectedIndex] = glm::vec3(glm::translate(glm::mat4(1), glm::vec3(0, dirlen * dotprod, 0)) *
							glm::vec4(activeCurve->pointsList[selectedIndex], 1));
						gizmoPos = activeCurve->pointsList[selectedIndex];
					}
					prev_ssdir = ssdir;
					activeCurve->UpdatePos();
					//gizmoPos = activeCurve->position;
				}
				else if (gizmo->getSelectedZ())
				{
					glm::vec2 ssdir = glm::vec2(projection_matrix * view_matrix * glm::translate(glm::mat4(1), gizmoPos) * glm::vec4(0, 0, 1, 1));
					ssdir -= glm::vec2(projection_matrix * view_matrix * glm::translate(glm::mat4(1), gizmoPos) * glm::vec4(0, 0, 0.5, 1));
			
					float dirlen = sqrt(glm::length(dir));
					float ssdirlen = glm::length(ssdir);
					if (dirlen > 0.001 && ssdirlen > 0.001) {
						float dotprod = glm::dot(glm::normalize(dir), glm::normalize(ssdir));// - prev_ssdir));

						activeCurve->pointsList[selectedIndex] = glm::vec3(glm::translate(glm::mat4(1), glm::vec3(0, 0, dirlen * dotprod)) *
							glm::vec4(activeCurve->pointsList[selectedIndex], 1));
						gizmoPos = activeCurve->pointsList[selectedIndex];
					}
					prev_ssdir = ssdir;
					activeCurve->UpdatePos();
					//gizmoPos = activeCurve->position;
				}
			}
			else
			{
				glm::vec2 dir = glm::vec2(mouseX, -mouseY) - glm::vec2(prev_mousePos.x, -prev_mousePos.y);
				if (gizmo->getSelectedX())
				{
					glm::vec2 ssdir = glm::vec2(projection_matrix * view_matrix * glm::translate(glm::mat4(1), gizmoPos) * glm::vec4(-1, 0, 0, 1));
					ssdir -= glm::vec2(projection_matrix * view_matrix * glm::translate(glm::mat4(1), gizmoPos) * glm::vec4(-0.5, 0, 0, 1));
					
					float dirlen = sqrt(glm::length(dir));
					float ssdirlen = glm::length(ssdir);
					if (dirlen > 0.001 && ssdirlen > 0.001) {
						float dotprod = glm::dot(glm::normalize(dir), glm::normalize(ssdir));// - prev_ssdir));

						for(int i = 0, sz = activeCurve->pointsList.size(); i < sz; i++)
						{
							activeCurve->pointsList[i] = glm::vec3(glm::translate(glm::mat4(1), glm::vec3(-dirlen * dotprod, 0, 0)) * glm::vec4(activeCurve->pointsList[i], 1));
						}
					}
					prev_ssdir = ssdir;
					activeCurve->UpdatePos();
					gizmoPos = activeCurve->position;
				}
				else if (gizmo->getSelectedY())
				{
					glm::vec2 ssdir = glm::vec2(projection_matrix * view_matrix * glm::translate(glm::mat4(1), gizmoPos) * glm::vec4(0, 1, 0, 1));
					ssdir -= glm::vec2(projection_matrix * view_matrix * glm::translate(glm::mat4(1), gizmoPos) * glm::vec4(0, 0.5, 0, 1));
			
					float dirlen = sqrt(glm::length(dir));
					float ssdirlen = glm::length(ssdir);
					if (dirlen > 0.001 && ssdirlen > 0.001)
					{
						float dotprod = glm::dot(glm::normalize(dir), glm::normalize(ssdir));// - prev_ssdir));
						for (int i = 0, sz = activeCurve->pointsList.size(); i < sz; i++)
						{
							activeCurve->pointsList[i] = glm::vec3(glm::translate(glm::mat4(1), glm::vec3(0, dirlen * dotprod, 0)) *
								glm::vec4(activeCurve->pointsList[i], 1));
						}
					}
					prev_ssdir = ssdir;
					activeCurve->UpdatePos();
					gizmoPos = activeCurve->position;
				}
				else if (gizmo->getSelectedZ())
				{
					glm::vec2 ssdir = glm::vec2(projection_matrix * view_matrix * glm::translate(glm::mat4(1), gizmoPos) * glm::vec4(0, 0, 1, 1));
					ssdir -= glm::vec2(projection_matrix * view_matrix * glm::translate(glm::mat4(1), gizmoPos) * glm::vec4(0, 0, 0.5, 1));
			
					float dirlen = sqrt(glm::length(dir));
					float ssdirlen = glm::length(ssdir);
					if (dirlen > 0.001 && ssdirlen > 0.001) {
						float dotprod = glm::dot(glm::normalize(dir), glm::normalize(ssdir));// - prev_ssdir));

						for (int i = 0, sz = activeCurve->pointsList.size(); i < sz; i++)
						{
							activeCurve->pointsList[i] = glm::vec3(glm::translate(glm::mat4(1), glm::vec3(0, 0, dirlen * dotprod)) * glm::vec4(activeCurve->pointsList[i], 1));
						}
					}
					prev_ssdir = ssdir;
					activeCurve->UpdatePos();
					gizmoPos = activeCurve->position;
				}
			}
		}
	}	
	prev_mousePos = glm::ivec2(mouseX, mouseY);
}

void Tema1SPG::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{	
	if (button == 1)
	{
		m_LMB = true;
		if (!m_altDown)
		{
			glm::uvec3 readPx = glm::uvec3(readPixels[(m_width * (m_height - mouseY) + mouseX) * 3],
				readPixels[(m_width * (m_height - mouseY) + mouseX) * 3 + 1],
				readPixels[(m_width * (m_height - mouseY) + mouseX) * 3 + 2]);
			if (readPx.r == 255 && readPx.g == 0 && readPx.b == 0)
			{
				gizmo->setSelectedX(true);
			}
			else if (readPx.r == 0 && readPx.g == 255 && readPx.b == 0)
			{
				gizmo->setSelectedY(true);
			}
			else if (readPx.r == 0 && readPx.g == 0 && readPx.b == 255)
			{
				gizmo->setSelectedZ(true);
			}
			else if (readPx.r == 255 && readPx.g == 255 && readPx.b == 0)
			{
				toolType = SELECT_TOOL;
				//gizmo->SetVisible(false);
			}
			else if (readPx.r == 255 && readPx.g == 0 && readPx.b == 255)
			{
				toolType = MOVE_TOOL;
			}else if (readPx.r == 0 && readPx.g == 255 && readPx.b == 255)
			{
				toolType = CREATE_CURVE;
				//gizmo->SetVisible(false);
			}
			else
			{

				if (toolType == SELECT_TOOL)
				{
					if (activeCurve != NULL) //SELECT A POINT ON THE ACTIVE CURVE
					{
						std::map<uint64_t, GLuint>::iterator it = activeCurve->colorHashedIndices.find(calculateColorHash(readPx));
						if (it != activeCurve->colorHashedIndices.end())
						{
							selectedIndex = it->second;
							gizmoPos = activeCurve->pointsList[selectedIndex];
							////gizmo->SetVisible(true);
						}
						else
						{
							selectedIndex = -1;
							activeCurve = NULL;
						}
					}else //SELECT A CURVE
					{
						std::map<uint64_t, CCurve>::iterator it = theCurves.find(calculateColorHash(readPx));
						if (it != theCurves.end())
						{
							activeCurve = &(it->second);
							selectedIndex = -1;
							gizmoPos = activeCurve->position;
							////gizmo->SetVisible(true);
						}
						else
						{
							activeCurve = NULL;
							selectedIndex = -1;
						}
					}
					/////////////////////////////////////////////////////////////////
				}
				else if (toolType == MOVE_TOOL)
				{
					if (activeCurve != NULL) //SELECT A POINT ON THE ACTIVE CURVE
					{
						std::map<uint64_t, GLuint>::iterator it = activeCurve->colorHashedIndices.find(calculateColorHash(readPx));
						
						if (it != activeCurve->colorHashedIndices.end())
						{
							selectedIndex = it->second;
							gizmoPos = activeCurve->pointsList[selectedIndex];
						}
						else
						{
							activeCurve = NULL;
							selectedIndex = -1;
						}
					}
					else //SELECT A CURVE
					{
						std::map<uint64_t, CCurve>::iterator it = theCurves.find(calculateColorHash(readPx));
						if (it != theCurves.end())
						{
							activeCurve = &(it->second);
							selectedIndex = -1;
							gizmoPos = activeCurve->position;
						}
						else
						{
							activeCurve = NULL;
							selectedIndex = -1;
						}
					}
				}
				else if(toolType ==  CREATE_CURVE)
				{
					bool choseCol = false;
					if ((readPx.r == 127 || readPx.r == 128) && readPx.g == 0 && readPx.b == 0)
					{
						newCurveType = CCurve::HERMITE;
						activeCurve = NULL;
						choseCol = true;
						printf("Drawing|HERMITE>\n");
					}
					else if (readPx.r == 0 && (readPx.g == 127 || readPx.g == 128) && readPx.b == 0)
					{
						newCurveType = CCurve::BEZIER;
						activeCurve = NULL;
						choseCol = true;

						printf("Drawing|BEZIER>\n");
					}
					else if (readPx.r == 0 && readPx.g == 0 && (readPx.b == 127 || readPx.b == 128))
					{
						newCurveType = CCurve::NURBS;
						activeCurve = NULL;
						choseCol = true;
						printf("Drawing|NURBS>\n");
					}
					else if ((readPx.r == 127 || readPx.r == 128) && (readPx.g == 127 || readPx.g == 128) && readPx.b == 0)
					{
						newCurveType = CCurve::SPLINE;
						activeCurve = NULL;
						choseCol = true;
						printf("Drawing|SPLINE>\n");
					}
					else if ((readPx.r == 127 || readPx.r == 128) && readPx.g == 0 && (readPx.b == 127 || readPx.b == 128))
					{
						newCurveType = CCurve::BSPLINE;
						activeCurve = NULL;
						choseCol = true;
						printf("Drawing|B-SPLINE>\n");
					}
					if(!choseCol){
						if (activeCurve == NULL)
						{
							glm::uvec3 nc = colorGen.getNextColor();
							glm::vec3 nextCurveCol = glm::vec3((float)nc.x / 255.f, (float)nc.y / 255.f, (float)nc.z / 255.f);
							CCurve c(newCurveType, nextCurveCol);
							uint64_t h = calculateColorHash(nc);
							theCurves[h] = c;
							activeCurve = &(theCurves[h]);
						}
						addPoint(glm::vec2(mouseX, mouseY));
						activeCurve->UpdatePos();
						gizmoPos = activeCurve->position;
					}					
				}
			}
		}
		else
		{
			//glfwSetCursorPos(window->GetGLFWWindow(), m_width / 2, m_height / 2);
		}
	}
	else if (button == 2)
	{
		m_RMB = true;
		if (!m_altDown) {
			//????????????????? TODO 
		}
	}
	else
		m_MMB = true;

}

void Tema1SPG::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	gizmo->setSelectedX(false);
	gizmo->setSelectedY(false);
	gizmo->setSelectedZ(false);
	if (button == 1)
		m_LMB = false;
	else if (button == 2)
		m_RMB = false;
	else
		m_MMB = false;
}

void Tema1SPG::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
	////glfwSetCursorPos(window->GetGLFWWindow(), m_width / 2, m_height / 2);
	//float dx = m_width / 2 - mouseX;
	//float dy = m_height / 2 - mouseY;
	camera.TranslateAlongZ(offsetY * 0.1 * m_deltaTime);
	ForceRedraw();
	printf("OFFSETY : %d\n", offsetY);
}