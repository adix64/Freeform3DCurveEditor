#include "Tema2SPG.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

struct CameraDirection
{
	GLenum CubemapFace;
	glm::vec3 Target;
	glm::vec3 Up;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CameraDirection gCameraDirections[] =
{
	{ GL_TEXTURE_CUBE_MAP_POSITIVE_X, glm::vec3(1.0f, 0.0f, 0.0f),  glm::vec3(0.0f, -1.0f, 0.0f) },
	{ GL_TEXTURE_CUBE_MAP_NEGATIVE_X, glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f) },
	{ GL_TEXTURE_CUBE_MAP_POSITIVE_Y, glm::vec3(0.0f, 1.0f, 0.0f),  glm::vec3(0.0f, 0.0f, 1.0f) },
	{ GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f) },
	{ GL_TEXTURE_CUBE_MAP_POSITIVE_Z, glm::vec3(0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, -1.0f, 0.0f) },
	{ GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f) }
};

using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Tema2SPG::Tema2SPG()
{
	
	// Create a shader program for drawing face polygon with the color of the normal
	{
		Shader *shader = new Shader("DefaultShader");
		shader->AddShader("Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	// Create a shader program for drawing face polygon with the color of the normal
	{
		Shader *shader = new Shader("ReflectionsShader");
		shader->AddShader("Shaders/CubeMapVertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Shaders/CubeMapFragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	// Create a shader program for drawing face polygon with the color of the normal
	{
		Shader *shader = new Shader("SkyShader");
		shader->AddShader("Shaders/SkyVertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Shaders/DiffuseOnlyFragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	// Create a shader program for drawing face polygon with the color of the normal
	{
		Shader *shader = new Shader("ShadowMapShader");
		shader->AddShader("Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Shaders/ShadowMapFragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Tema2SPG::~Tema2SPG()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Tema2SPG::LoadMaterials()
{
	const string textureLoc = "Assets/Textures/";

	{//Bamboo
		Texture2D* diffuse = new Texture2D();
		diffuse->Load2D((textureLoc + "bambooDiffuse.png").c_str(), GL_REPEAT);
		Texture2D* specular = new Texture2D();
		specular->Load2D((textureLoc + "bambooSpecular.png").c_str(), GL_REPEAT);
		//Texture2D* normal = new Texture2D();
		//normal->Load2D((textureLoc + "bambooNormal.png").c_str(), GL_REPEAT);

		Material *mat = new Material();
		mat->diffuseMap = diffuse;
		mat->specularMap = specular;
		//mat->normalMap = normal;
		mat->ambientReflectivity = 0.35f;
		mat->fresnelCos = 0.35f;
		materials["bamboo"] = mat;
	}

	{//Terrain
		Texture2D* diffuse = new Texture2D();
		diffuse->Load2D((textureLoc + "terrainDiffuse.png").c_str(), GL_REPEAT);
		Texture2D* specular = new Texture2D();
		specular->Load2D((textureLoc + "terrainSpecular.png").c_str(), GL_REPEAT);
		//Texture2D* normal = new Texture2D();
		//normal->Load2D((textureLoc + "terrainNormal.png").c_str(), GL_REPEAT);

		Material *mat = new Material();
		mat->diffuseMap = diffuse;
		mat->specularMap = specular;
	//	mat->normalMap = normal;
		mat->ambientReflectivity = 0.35f;
		mat->fresnelCos = 0.35f;
		materials["terrainTile"] = mat;
	}

	{//Car
		Texture2D* diffuse = new Texture2D();
		diffuse->Load2D((textureLoc + "carDiffuse.png").c_str(), GL_REPEAT);
		Texture2D* specular = new Texture2D();
		specular->Load2D((textureLoc + "carSpecular.png").c_str(), GL_REPEAT);
		//Texture2D* normal = new Texture2D();
		//normal->Load2D((textureLoc + "carNormal.png").c_str(), GL_REPEAT);
	
		CubeMapFBO *reflectionMap = new CubeMapFBO();
		reflectionMap->Init(1024, 1024);

		Material *mat = new Material();
		mat->diffuseMap = diffuse;
		mat->specularMap = specular;
	//	mat->normalMap = normal;
		mat->ambientReflectivity = 0.1;
		mat->reflectionCubeMap = reflectionMap;
		mat->fresnelCos = 0.25f;
		materials["car"] = mat;
		carPos = glm::vec3(0, 2, 0);
		reflectionCubeMaps["car"] = std::pair<CubeMapFBO*, glm::vec3*>(reflectionMap, &carPos);
	}

	{
		skysphereTex = new Texture2D();
		skysphereTex->Load2D((textureLoc + "sky.png").c_str(), GL_REPEAT);
	}

	{
		cloudsTex= new Texture2D();
		cloudsTex->Load2D((textureLoc + "clouds.png").c_str(), GL_REPEAT);
	}

	{
		mountainsTex = new Texture2D();
		mountainsTex->Load2D((textureLoc + "mountains.png").c_str(), GL_REPEAT);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Tema2SPG::LoadMeshes()
{
	{// Car
		Mesh* mesh = new Mesh("car");
		mesh->LoadMesh(RESOURCE_PATH::MODELS, "car.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{// Bamboo
		Mesh* mesh = new Mesh("bamboo");
		mesh->LoadMesh(RESOURCE_PATH::MODELS, "bamboo.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{// SKY
		skysphereMesh = new Mesh("skySphere");
		std::vector<VertexFormat> verts;
		std::vector<unsigned short> indices;

		float phi = 0.0f, theta = 0.0f;

		glm::vec3 baseVert = glm::vec3(0, 1, 0);

		const float step = 10.f;
		unsigned numVSlices = 0, numHSlices = 0;


		for (phi = 0.0f; phi <= 180.f; phi += step)
		{
			numVSlices++;
			numHSlices = 0;
			for (theta = 0.0f; theta <= 360.f; theta += step)
			{
				const glm::mat4 rotPhi = glm::rotate(glm::mat4(1), glm::radians(phi), glm::vec3(1, 0, 0));
				const glm::mat4 rotTheta = glm::rotate(glm::mat4(1), glm::radians(theta), glm::vec3(0, 1, 0));
				glm::vec3 pointOnSphere = glm::vec3(rotTheta * rotPhi * glm::vec4(baseVert, 1));
				verts.push_back(VertexFormat(pointOnSphere, glm::vec3(1), glm::vec3(1), glm::vec2(theta / 360, phi / 180)));
				numHSlices++;
			}

		}

		for (int v = 0; v < numVSlices; v++)
		{
			for (int h = 0; h < numHSlices; h++)
			{
				indices.push_back(v * numHSlices + h);
				indices.push_back(v * numHSlices + h + 1);
				indices.push_back((v + 1) * numHSlices + h);

				indices.push_back(v * numHSlices + h + 1);
				indices.push_back((v + 1)* numHSlices + h + 1);
				indices.push_back((v + 1) * numHSlices + h);
			}
		}
		skysphereMesh->CreateMesh(verts, indices);

		skyModelMatrix = glm::scale(glm::mat4(1), glm::vec3(12500, 12500, 12500));
		cloudsModelMatrix = glm::rotate(glm::mat4(1), 3.141926f / 2.f, glm::vec3(1,0,0)) * glm::scale(glm::mat4(1), glm::vec3(8500, 8500, 8500));
		cloudsModelMatrix2 = glm::rotate(glm::mat4(1), -3.141926f / 2.f, glm::vec3(1, 0, 0)) * glm::scale(glm::mat4(1), glm::vec3(-5000, -5000, 5000));
		mountainsModelMatrix = glm::scale(glm::mat4(1), glm::vec3(5000, 3000, 5000));
	}

	
	{//Terrain Tile
		vector<glm::vec3> vertices
		{
			glm::vec3(-0.5f,   0.0f, -0.5f),	// Top Right
			glm::vec3(-0.5f, 0.0f, 0.5f),	// Bottom Right
			glm::vec3(0.5f, 0.0f, 0.5f),	// Bottom Left
			glm::vec3(0.5f,  0.0f, -0.5f),	// Top Left
		};

		vector<glm::vec3> normals
		{
			glm::vec3(0, 1, 0),
			glm::vec3(0, 1, 0),
			glm::vec3(0, 1, 0),
			glm::vec3(0, 1, 0)
		};

		vector<glm::vec2> textureCoords
		{
			glm::vec2(0.0f, 0.0f),
			glm::vec2(0.0f, 1.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(1.0f, 0.0f)
		};

		vector<unsigned short> indices =
		{
			0, 1, 3,
			1, 2, 3
		};

		Mesh* mesh = new Mesh("terrainTile");
		mesh->InitFromData(vertices, normals, textureCoords, indices);
		meshes[mesh->GetMeshID()] = mesh;
	}

	
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Tema2SPG::Init()
{

	LoadMaterials();
	LoadMeshes();

	{//Terrain Tiles' Transforms
		const float limit = 20.f;
		const glm::mat4 terrainScale = glm::scale(glm::mat4(1), glm::vec3(10, 10, 10));
		for (float offsetZ = -limit; offsetZ <= limit; offsetZ += 1.0f)
		{
			for (float offsetX = -limit; offsetX <= limit; offsetX += 1.0f)
			{
				glm::mat4 modelMatrix = glm::translate(glm::mat4(1), glm::vec3(offsetX, 0, offsetZ));
				meshes["terrainTile"]->instanceModelMat.push_back(terrainScale * modelMatrix);
			}
		}
	}

	{//Bamboo Trees' Transforms
		for(int i = -5; i <= 5; i++)
		{
			for (int j = -5; j <= 5; j++)
			{
				if (!i && !j) continue;
				float randScale = (float)rand() / (float)RAND_MAX * 0.25 - 0.125;
				glm::mat4 scaleM = glm::scale(glm::mat4(1), glm::vec3(0.25 + randScale, 0.25 + randScale, 0.35 + randScale));
				glm::mat4 rotM = glm::rotate(glm::mat4(1), randScale * 5, glm::vec3(0, 1, 0));
				glm::mat4 modelMatrix = glm::translate(glm::mat4(1), glm::vec3(5 * i + randScale * 16, 0.00, 5 * j + randScale * 16)) * rotM * scaleM;
				meshes["bamboo"]->instanceModelMat.push_back(modelMatrix);
			}
		}
	}
	meshes["car"]->instanceModelMat.push_back(glm::scale(glm::mat4(1), glm::vec3(0.025,0.025,0.025)));

	glEnable(GL_BLEND);
	//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(1, 0, 0, 1);
	shadowCubeMap.resize(2, ShadowCubeMapFBO());
	for (int i = 0; i < 2; i++)
		shadowCubeMap[i].Init(1024, 1024);
	lightPos.resize(2);
	lightPos[0] = glm::vec3(5, 5, 5);
	lightPos[1] = glm::vec3(-5, 5, -5);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Tema2SPG::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Tema2SPG::ShadowMapPass()
{
//	return;
	glUseProgram(shaders["ShadowMapShader"]->program);
	int modelMatrixLoc, viewMatrixLoc, projectionMatrixLoc, eyePosLoc;
	int diffuseMapLoc, useDiffuseMapLoc;
	glClearColor(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
	glDisable(GL_BLEND);
	CheckOpenGLError();
	modelMatrixLoc = glGetUniformLocation(shaders["ShadowMapShader"]->program, "Model");
	viewMatrixLoc = glGetUniformLocation(shaders["ShadowMapShader"]->program, "View");
	projectionMatrixLoc = glGetUniformLocation(shaders["ShadowMapShader"]->program, "Projection");
	eyePosLoc = glGetUniformLocation(shaders["ShadowMapShader"]->program, "eye_position");
	useDiffuseMapLoc = glGetUniformLocation(shaders["ShadowMapShader"]->program, "useDiffuseMap");
	diffuseMapLoc = glGetUniformLocation(shaders["ShadowMapShader"]->program, "diffuseMap");
	{
		glUniformMatrix4fv(projectionMatrixLoc, 1, false, glm::value_ptr(glm::perspective(glm::radians(90.f), 1.f, 1.f, 1000.f)));
		
		for (int lID = 0; lID < shadowCubeMap.size(); lID++)
		{
			glUniform3f(eyePosLoc, lightPos[lID].x, lightPos[lID].y, lightPos[lID].z);

			for (int i = 0; i < 6; i++)
			{
				glm::mat4 localViewMat = glm::lookAt(lightPos[lID], lightPos[lID] + gCameraDirections[i].Target, gCameraDirections[i].Up);
				glUniformMatrix4fv(viewMatrixLoc, 1, false, glm::value_ptr(localViewMat));

				shadowCubeMap[lID].BindForWriting(gCameraDirections[i].CubemapFace);
				glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
				glViewport(0, 0, 1024, 1024);

				for (auto obj : meshes)
				{
					const std::string &objName = obj.first;

					Material *mat = materials[objName];

					if (mat->diffuseMap)
					{
						glUniform1i(useDiffuseMapLoc, 1);
						glActiveTexture(GL_TEXTURE0 + 1);
						glBindTexture(GL_TEXTURE_2D, mat->diffuseMap->GetTextureID());
						glUniform1i(diffuseMapLoc, 1);
					}
					else
					{
						glUniform1i(useDiffuseMapLoc, 0);
					}
					glBindVertexArray(obj.second->GetBuffers()->VAO);
					for (glm::mat4 &modelMat : obj.second->instanceModelMat)
					{
						glUniformMatrix4fv(modelMatrixLoc, 1, false, glm::value_ptr(modelMat));
						glDrawElements(obj.second->GetDrawMode(), static_cast<int>(obj.second->indices.size()), GL_UNSIGNED_SHORT, 0);
					}
				}
			}
		}
	}
	glEnable(GL_BLEND);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Tema2SPG::ReflectionPass()
{
	glUseProgram(shaders["ReflectionsShader"]->program);
	glClearColor(1, 0, 0, 1);
	int diffuseMapLoc, useDiffuseMapLoc, diffuseColorLoc;
	int specularMapLoc, useSpecularMapLoc, specularPowerLoc;
	int normalMapLoc, useNormalMapLoc;
	//int reflectionCubeMapLoc, useReflectionsLoc;
	int fresnelCosLoc, ambientReflectivityLoc;
	int modelMatrixLoc, viewMatrixLoc, projectionMatrixLoc;
	int eyePosLoc;
	int light1PosLoc, light2PosLoc;
	useDiffuseMapLoc = glGetUniformLocation(shaders["ReflectionsShader"]->program, "useDiffuseMap");
	diffuseMapLoc = glGetUniformLocation(shaders["ReflectionsShader"]->program, "diffuseMap");
	diffuseColorLoc = glGetUniformLocation(shaders["ReflectionsShader"]->program, "diffuseColor");

	useSpecularMapLoc = glGetUniformLocation(shaders["ReflectionsShader"]->program, "useSpecularMap");
	specularMapLoc = glGetUniformLocation(shaders["ReflectionsShader"]->program, "specularMap");
	specularPowerLoc = glGetUniformLocation(shaders["ReflectionsShader"]->program, "specularPower");

	normalMapLoc = glGetUniformLocation(shaders["ReflectionsShader"]->program, "normalMap");
	useNormalMapLoc = glGetUniformLocation(shaders["ReflectionsShader"]->program, "useNormalMap");
	//reflectionCubeMapLoc = glGetUniformLocation(shaders["ReflectionsShader"]->program, "reflectionCubeMap");
	//useReflectionsLoc = glGetUniformLocation(shaders["ReflectionsShader"]->program, "useReflections");
	fresnelCosLoc = glGetUniformLocation(shaders["ReflectionsShader"]->program, "fresnelCos");
	ambientReflectivityLoc= glGetUniformLocation(shaders["ReflectionsShader"]->program, "ambientReflectivity");

	modelMatrixLoc = glGetUniformLocation(shaders["ReflectionsShader"]->program, "Model");
	viewMatrixLoc = glGetUniformLocation(shaders["ReflectionsShader"]->program, "View");
	projectionMatrixLoc = glGetUniformLocation(shaders["ReflectionsShader"]->program, "Projection");
	
	light1PosLoc = glGetUniformLocation(shaders["ReflectionsShader"]->program, "lightPos1");
	light2PosLoc = glGetUniformLocation(shaders["ReflectionsShader"]->program, "lightPos2");

	int skyModelMatLoc = glGetUniformLocation(shaders["SkyShader"]->program, "Model");
	int skyViewMatLoc = glGetUniformLocation(shaders["SkyShader"]->program, "View");
	int skyProjectionMatLoc = glGetUniformLocation(shaders["SkyShader"]->program, "Projection");
	int skyDiffuseMapLoc = glGetUniformLocation(shaders["SkyShader"]->program, "diffuseMap");
	int skyForceZLoc = glGetUniformLocation(shaders["SkyShader"]->program, "forceZ");
	int skyAlphaOverriderLoc = glGetUniformLocation(shaders["SkyShader"]->program, "alphaOverrider");

	eyePosLoc = glGetUniformLocation(shaders["ReflectionsShader"]->program, "eye_position");
	{
		glUniformMatrix4fv(projectionMatrixLoc, 1, false, glm::value_ptr(glm::perspective(glm::radians(90.f), 1.f, 1.f, 1000.f)));
		glUniform3f(light1PosLoc, lightPos[0].x, lightPos[0].y, lightPos[0].z);
		glUniform3f(light2PosLoc, lightPos[1].x, lightPos[1].y, lightPos[1].z);
		for (auto obj : reflectionCubeMaps)
		{
			const std::string &objName = obj.first;
			const glm::vec3 &objPos = *obj.second.second;
			glUniform3f(eyePosLoc, objPos.x, objPos.y, objPos.z);

			CubeMapFBO &crtRefCubeMap = *obj.second.first;
			for (int i = 0; i < 6; i++)
			{
				glm::mat4 localViewMat = glm::lookAt(objPos, objPos + gCameraDirections[i].Target, gCameraDirections[i].Up);
				glUniformMatrix4fv(viewMatrixLoc, 1, false, glm::value_ptr(localViewMat));
				crtRefCubeMap.BindForWriting(gCameraDirections[i].CubemapFace, i);
				glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
				glViewport(0, 0, 1024, 1024);
				
				glUseProgram(shaders["SkyShader"]->program);
				glUniformMatrix4fv(skyProjectionMatLoc, 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetProjectionMatrix()));
				glUniformMatrix4fv(skyViewMatLoc, 1, GL_FALSE, glm::value_ptr(localViewMat));
				glBindVertexArray(skysphereMesh->GetBuffers()->VAO);

				glUniform1f(skyAlphaOverriderLoc, 1.0f);
				glUniform1f(skyForceZLoc, 0.999999f);
				glUniformMatrix4fv(skyModelMatLoc, 1, GL_FALSE, glm::value_ptr(skyModelMatrix));
				glActiveTexture(GL_TEXTURE0 + 1);
				glBindTexture(GL_TEXTURE_2D, skysphereTex->GetTextureID());
				glUniform1i(skyDiffuseMapLoc, 1);
				glDrawElements(skysphereMesh->GetDrawMode(), static_cast<int>(skysphereMesh->indices.size()), GL_UNSIGNED_SHORT, 0);

				glUniform1f(skyAlphaOverriderLoc, 0.25f);
				glUniform1f(skyForceZLoc, 0.99998f);
				//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glUniformMatrix4fv(skyModelMatLoc, 1, GL_FALSE, glm::value_ptr(cloudsModelMatrix));
				glActiveTexture(GL_TEXTURE0 + 1);
				glBindTexture(GL_TEXTURE_2D, cloudsTex->GetTextureID());
				glUniform1i(skyDiffuseMapLoc, 1);
				glDrawElements(skysphereMesh->GetDrawMode(), static_cast<int>(skysphereMesh->indices.size()), GL_UNSIGNED_SHORT, 0);
				//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

				glUniform1f(skyForceZLoc, 0.99996f);
				//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glUniformMatrix4fv(skyModelMatLoc, 1, GL_FALSE, glm::value_ptr(cloudsModelMatrix2));
				glActiveTexture(GL_TEXTURE0 + 1);
				glBindTexture(GL_TEXTURE_2D, cloudsTex->GetTextureID());
				glUniform1i(skyDiffuseMapLoc, 1);
				glDrawElements(skysphereMesh->GetDrawMode(), static_cast<int>(skysphereMesh->indices.size()), GL_UNSIGNED_SHORT, 0);
				//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

				glUniform1f(skyAlphaOverriderLoc, 1.0f);
				glUniform1f(skyForceZLoc, 0.99994f);
				//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glUniformMatrix4fv(skyModelMatLoc, 1, GL_FALSE, glm::value_ptr(mountainsModelMatrix));
				glActiveTexture(GL_TEXTURE0 + 1);
				glBindTexture(GL_TEXTURE_2D, mountainsTex->GetTextureID());
				glUniform1i(skyDiffuseMapLoc, 1);
				glDrawElements(skysphereMesh->GetDrawMode(), static_cast<int>(skysphereMesh->indices.size()), GL_UNSIGNED_SHORT, 0);
				//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glUseProgram(shaders["ReflectionsShader"]->program);

				for (auto reflectedObj : meshes)
				{
					const std::string &reflectedObjName = reflectedObj.first;
					if (reflectedObjName == objName)
					{
						continue;
					}
				
					Material *mat = materials[reflectedObjName];
				
					if (mat->diffuseMap)
					{
						glUniform1i(useDiffuseMapLoc, 1);
						glActiveTexture(GL_TEXTURE0 + 1);
						glBindTexture(GL_TEXTURE_2D, mat->diffuseMap->GetTextureID());
						glUniform1i(diffuseMapLoc, 1);
					}
					else
					{
						glUniform1i(useDiffuseMapLoc, 0);
						glUniform3f(diffuseColorLoc, mat->diffuseColor.x, mat->diffuseColor.y, mat->diffuseColor.z);
					}
					if (mat->specularMap)
					{
						glUniform1i(useSpecularMapLoc, 1);
						glActiveTexture(GL_TEXTURE0 + 2);
						glBindTexture(GL_TEXTURE_2D, mat->specularMap->GetTextureID());
						glUniform1i(specularMapLoc, 2);
					}
					else
					{
						glUniform1i(useSpecularMapLoc, 0);
						glUniform1f(specularPowerLoc, mat->specularPower);
					}
					if (mat->normalMap)
					{
						glUniform1i(useNormalMapLoc, 1);
						glActiveTexture(GL_TEXTURE0 + 3);
						glBindTexture(GL_TEXTURE_2D, mat->normalMap->GetTextureID());
						glUniform1i(normalMapLoc, 3);
					}
					else
					{
						glUniform1i(useNormalMapLoc, 0);
					}
					glUniform1f(fresnelCosLoc, mat->fresnelCos);
					glUniform1f(ambientReflectivityLoc, mat->ambientReflectivity);
				
				
					glBindVertexArray(reflectedObj.second->GetBuffers()->VAO);
					for (glm::mat4 &modelMat: reflectedObj.second->instanceModelMat)
					{
						glUniformMatrix4fv(modelMatrixLoc, 1, false, glm::value_ptr(modelMat));
						glDrawElements(reflectedObj.second->GetDrawMode(), static_cast<int>(reflectedObj.second->indices.size()), GL_UNSIGNED_SHORT, 0);
					}
				}
				
			}
		}
	}
}

void Tema2SPG::ForwardRenderPass()
{
	glUseProgram(shaders["DefaultShader"]->program);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(1, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::ivec2 resolution = window->GetResolution();
	glViewport(0, 0, resolution.x, resolution.y);

	int diffuseMapLoc, useDiffuseMapLoc, diffuseColorLoc;
	int specularMapLoc, useSpecularMapLoc, specularPowerLoc;
	int normalMapLoc, useNormalMapLoc;
	int reflectionCubeMapLoc, useReflectionsLoc;
	int fresnelCosLoc, ambientReflectivityLoc;
	int modelMatrixLoc, viewMatrixLoc, projectionMatrixLoc;
	int eyePosLoc;
	int cubeShadowMapLoc1, cubeShadowMapLoc2;
	int light1PosLoc, light2PosLoc;

	useDiffuseMapLoc = glGetUniformLocation(shaders["DefaultShader"]->program, "useDiffuseMap");
	diffuseMapLoc = glGetUniformLocation(shaders["DefaultShader"]->program, "diffuseMap");
	diffuseColorLoc = glGetUniformLocation(shaders["DefaultShader"]->program, "diffuseColor");

	useSpecularMapLoc = glGetUniformLocation(shaders["DefaultShader"]->program, "useSpecularMap");
	specularMapLoc = glGetUniformLocation(shaders["DefaultShader"]->program, "specularMap");
	specularPowerLoc = glGetUniformLocation(shaders["DefaultShader"]->program, "specularPower");

	normalMapLoc = glGetUniformLocation(shaders["DefaultShader"]->program, "normalMap");
	useNormalMapLoc = glGetUniformLocation(shaders["DefaultShader"]->program, "useNormalMap");
	reflectionCubeMapLoc = glGetUniformLocation(shaders["DefaultShader"]->program, "reflectionCubeMap");
	useReflectionsLoc = glGetUniformLocation(shaders["DefaultShader"]->program, "useReflections");
	fresnelCosLoc = glGetUniformLocation(shaders["DefaultShader"]->program, "fresnelCos");
	ambientReflectivityLoc = glGetUniformLocation(shaders["DefaultShader"]->program, "ambientReflectivity");

	modelMatrixLoc = glGetUniformLocation(shaders["DefaultShader"]->program, "Model");
	viewMatrixLoc = glGetUniformLocation(shaders["DefaultShader"]->program, "View");
	projectionMatrixLoc = glGetUniformLocation(shaders["DefaultShader"]->program, "Projection");

	eyePosLoc = glGetUniformLocation(shaders["DefaultShader"]->program, "eye_position");
	
	cubeShadowMapLoc1 = glGetUniformLocation(shaders["DefaultShader"]->program, "cubeShadowMap1");
	cubeShadowMapLoc2 = glGetUniformLocation(shaders["DefaultShader"]->program, "cubeShadowMap2");

	light1PosLoc = glGetUniformLocation(shaders["DefaultShader"]->program, "lightPos1");
	light2PosLoc = glGetUniformLocation(shaders["DefaultShader"]->program, "lightPos2");

	int skyModelMatLoc = glGetUniformLocation(shaders["SkyShader"]->program, "Model");
	int skyViewMatLoc = glGetUniformLocation(shaders["SkyShader"]->program, "View");
	int skyProjectionMatLoc = glGetUniformLocation(shaders["SkyShader"]->program, "Projection");
	int skyDiffuseMapLoc = glGetUniformLocation(shaders["SkyShader"]->program, "diffuseMap");
	int skyForceZLoc = glGetUniformLocation(shaders["SkyShader"]->program, "forceZ");
	int skyAlphaOverriderLoc = glGetUniformLocation(shaders["SkyShader"]->program, "alphaOverrider");

	glm::vec3 eyePos = glm::vec3(glm::inverse(GetSceneCamera()->GetViewMatrix())[3]);
	//printf("%f %f %f\n", eyePos.x, eyePos.y, eyePos.z);
	glUniform3f(eyePosLoc, eyePos.x, eyePos.y, eyePos.z);
	glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
	glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	glUniformMatrix4fv(projectionMatrixLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	
	glUseProgram(shaders["SkyShader"]->program);
	glUniformMatrix4fv(skyProjectionMatLoc, 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetProjectionMatrix()));
	glUniformMatrix4fv(skyViewMatLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glBindVertexArray(skysphereMesh->GetBuffers()->VAO);

	glUniform1f(skyAlphaOverriderLoc, 1.0f);
	glUniform1f(skyForceZLoc, 0.999999f);
	glUniformMatrix4fv(skyModelMatLoc, 1, GL_FALSE, glm::value_ptr(skyModelMatrix));
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, skysphereTex->GetTextureID());
	glUniform1i(skyDiffuseMapLoc, 1);	
	glDrawElements(skysphereMesh->GetDrawMode(), static_cast<int>(skysphereMesh->indices.size()), GL_UNSIGNED_SHORT, 0);

	glUniform1f(skyAlphaOverriderLoc, 0.25f);
	glUniform1f(skyForceZLoc, 0.99998f);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glUniformMatrix4fv(skyModelMatLoc, 1, GL_FALSE, glm::value_ptr(cloudsModelMatrix));
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, cloudsTex->GetTextureID());
	glUniform1i(skyDiffuseMapLoc, 1);
	glDrawElements(skysphereMesh->GetDrawMode(), static_cast<int>(skysphereMesh->indices.size()), GL_UNSIGNED_SHORT, 0);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glUniform1f(skyForceZLoc, 0.99996f);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glUniformMatrix4fv(skyModelMatLoc, 1, GL_FALSE, glm::value_ptr(cloudsModelMatrix2));
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, cloudsTex->GetTextureID());
	glUniform1i(skyDiffuseMapLoc, 1);
	glDrawElements(skysphereMesh->GetDrawMode(), static_cast<int>(skysphereMesh->indices.size()), GL_UNSIGNED_SHORT, 0);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glUniform1f(skyAlphaOverriderLoc, 1.0f);
	glUniform1f(skyForceZLoc, 0.99994f);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glUniformMatrix4fv(skyModelMatLoc, 1, GL_FALSE, glm::value_ptr(mountainsModelMatrix));
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, mountainsTex->GetTextureID());
	glUniform1i(skyDiffuseMapLoc, 1);
	glDrawElements(skysphereMesh->GetDrawMode(), static_cast<int>(skysphereMesh->indices.size()), GL_UNSIGNED_SHORT, 0);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glUseProgram(shaders["DefaultShader"]->program);
	glUniform3f(light1PosLoc, lightPos[0].x, lightPos[0].y, lightPos[0].z);
	glUniform3f(light2PosLoc, lightPos[1].x, lightPos[1].y, lightPos[1].z);
	for (auto mesh : meshes)
	{
		const std::string &meshName = mesh.first;

		Material *mat = materials[meshName];

		if (mat->diffuseMap)
		{
			glUniform1i(useDiffuseMapLoc, 1);
			glActiveTexture(GL_TEXTURE0 + 1);
			glBindTexture(GL_TEXTURE_2D, mat->diffuseMap->GetTextureID());
			glUniform1i(diffuseMapLoc, 1);
		}
		else
		{
			glUniform1i(useDiffuseMapLoc, 0);
			glUniform3f(diffuseColorLoc, mat->diffuseColor.x, mat->diffuseColor.y, mat->diffuseColor.z);
		}
		if (mat->specularMap)
		{
			glUniform1i(useSpecularMapLoc, 1);
			glActiveTexture(GL_TEXTURE0 + 2);
			glBindTexture(GL_TEXTURE_2D, mat->specularMap->GetTextureID());
			glUniform1i(specularMapLoc, 2);
		}
		else
		{
			glUniform1i(useSpecularMapLoc, 0);
			glUniform1f(specularPowerLoc, mat->specularPower);
		}
		if (mat->normalMap)
		{
			glUniform1i(useNormalMapLoc, 1);
			glActiveTexture(GL_TEXTURE0 + 3);
			glBindTexture(GL_TEXTURE_2D, mat->normalMap->GetTextureID());
			glUniform1i(normalMapLoc, 3);
		}
		else
		{
			glUniform1i(useNormalMapLoc, 0);
		}
		auto got = reflectionCubeMaps.find(meshName);

		if (got != reflectionCubeMaps.end())
		{
			glUniform1i(useReflectionsLoc, 1);
			reflectionCubeMaps[meshName].first->BindForReading(GL_TEXTURE0 + 4, reflectionCubeMapLoc);
			glUniform1i(reflectionCubeMapLoc, 4);
		}
		else
		{
			glUniform1i(useReflectionsLoc, 0);
		}
		glUniform1f(fresnelCosLoc, mat->fresnelCos);
		glUniform1f(ambientReflectivityLoc, mat->ambientReflectivity);

		shadowCubeMap[0].BindForReading(GL_TEXTURE0 + 5, cubeShadowMapLoc1);
		shadowCubeMap[1].BindForReading(GL_TEXTURE0 + 6, cubeShadowMapLoc2);
		
		glBindVertexArray(mesh.second->GetBuffers()->VAO);//???
		for (glm::mat4 &modelMat : mesh.second->instanceModelMat)
		{
			glUniformMatrix4fv(modelMatrixLoc, 1, false, glm::value_ptr(modelMat));
			//mesh.second->Render();
			glDrawElements(mesh.second->GetDrawMode(), static_cast<int>(mesh.second->indices.size()), GL_UNSIGNED_SHORT, 0);
		}
	}
	
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Tema2SPG::Update(float deltaTimeSeconds)
{
	cloudsModelMatrix = glm::rotate(glm::mat4(1), deltaTimeSeconds * 0.03f, glm::vec3(0, 0, 1)) * cloudsModelMatrix;
	cloudsModelMatrix2 = glm::rotate(glm::mat4(1), -deltaTimeSeconds * 0.05f, glm::vec3(0, 0, 1)) * cloudsModelMatrix2;
	ShadowMapPass();
	ReflectionPass();
	ForwardRenderPass();
}

void Tema2SPG::FrameEnd()
{
	//DrawCoordinatSystem();
}

void Tema2SPG::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, Material *mat)
{
#ifdef DEBUG_NULL_MESH_SHADER
	if (!mesh || !shader || !shader->GetProgramID())
		return;
#endif
	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

// Documentation for the input functions can be found in: "/Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Tema2SPG::OnInputUpdate(float deltaTime, int mods)
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

void Tema2SPG::OnKeyPress(int key, int mods)
{
	//if (key == GLFW_KEY_Z) { deplasament += 0.5f; }
	//if (key == GLFW_KEY_X) { deplasament -= 0.5f; }
	// add key press event
}

void Tema2SPG::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Tema2SPG::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Tema2SPG::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Tema2SPG::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema2SPG::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema2SPG::OnWindowResize(int width, int height)
{
}
