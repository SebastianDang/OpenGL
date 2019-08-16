#include "stdafx.h"
#include "ResourceManager.h"
#include "Obj_Object.h"
#include "Geo_Object.h"
#include "Instance_Object.h"
#include "Skybox.h"
#include "Terrain.h"

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
	// Clear shaders
	for (auto shader : m_Shaders)
		delete shader.second;
	m_Shaders.clear();

	// Clear lights
	for (auto light : m_Lights)
		delete light;
	m_Lights.clear();

	// Clear cameras
	for (auto camera : m_Cameras)
		delete camera;
	m_Cameras.clear();

	// Clear materials
	for (auto material : m_Materials)
		delete material.second;
	m_Materials.clear();

	// Clear objects
	for (auto object : m_LoadedObjects)
		delete object.second;
	m_LoadedObjects.clear();

	// Clear instanced objects
	m_Objects.clear();
}

void ResourceManager::Load(const char* pFile)
{
	// Attempt to load from file here.
	if (pFile)
	{
		// Store filename.
		m_Filename = pFile;

		// Open the file for reading.
		std::FILE* objFile = fopen(pFile, "r");
		if (objFile)
		{
			// Read the file until the end. "# are commments to be ignored".
			while (true)
			{
				char buf[BUFSIZ];
				buf[BUFSIZ - 1] = '\0'; // Null terminated.
				int check = fscanf(objFile, "%s", buf);
				if (check == EOF) break;

				// Shaders
				if (strcmp(buf, "shader") == 0)
				{

				}
			}
		}
		return;
	}

	// Load any required shaders here.
	LoadShader("shaders/object.vert", "shaders/object.frag", "object");
	LoadShader("shaders/skybox.vert", "shaders/skybox.frag", "skybox");

	// Directional Light
	DirectionalLight* pDirectional = new DirectionalLight();
	pDirectional->SetEnabled(true);
	pDirectional->SetDirection(glm::vec3(-0.2f, -1.0f, -0.3f));
	pDirectional->SetAmbient(glm::vec3(1.0f, 1.0f, 1.0f));
	pDirectional->SetDiffuse(glm::vec3(1.0f, 1.0f, 1.0f));
	pDirectional->SetSpecular(glm::vec3(1.0f, 1.0f, 1.0f));

	// PointLight
	PointLight* pPoint = new PointLight();
	pPoint->SetEnabled(true);
	pPoint->SetPosition(glm::vec3(10.0f, 10.0f, 10.0f));
	pPoint->SetAmbient(glm::vec3(0.2f, 0.2f, 0.2f));
	pPoint->SetDiffuse(glm::vec3(0.5f, 0.5f, 0.5f));
	pPoint->SetSpecular(glm::vec3(0.8f, 0.8f, 0.8f));
	pPoint->SetQuadratic(0.032f);

	// SpotLight
	SpotLight* pSpot = new SpotLight();
	pSpot->SetEnabled(true);
	pSpot->SetPosition(glm::vec3(10.0f, 10.0f, 10.0f));
	pSpot->SetAmbient(glm::vec3(0.2f, 0.2f, 0.2f));
	pSpot->SetDiffuse(glm::vec3(0.5f, 0.5f, 0.5f));
	pSpot->SetSpecular(glm::vec3(0.8f, 0.8f, 0.8f));
	pSpot->SetQuadratic(0.032f); // attenuation
	pSpot->SetDirection(-pSpot->GetPosition());//This ensures that the spotlight is always pointing at the origin (position away from the origin).
	pSpot->SetSpotCutoff((30.0f / 180.0f * glm::pi<float>()));
	pSpot->SetSpotExponent(1.0f);

	// Add to lights
	AddLight(pDirectional);
	AddLight(pPoint);
	AddLight(pSpot);

	// Initialize the camera
	glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 20.0f);
	glm::vec3 camera_look_at = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
	Camera* pDefaultCamera = new Camera(camera_pos, camera_look_at, camera_up);
	ThirdPersonCamera* pThirdCamera = new ThirdPersonCamera(glm::vec3(0.0f));
	FirstPersonCamera* pFirstCamera = new FirstPersonCamera(glm::vec3(0.0f));

	// Set default direction.
	glm::vec3 direction = glm::vec3(glm::rotate(glm::mat4(1.0f), (30.0f / 180.0f * glm::pi<float>()), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
	pFirstCamera->LockFieldOfView(direction, 180.0f);

	// Add to cameras.
	AddCamera(pDefaultCamera);
	AddCamera(pThirdCamera);
	AddCamera(pFirstCamera);
	SetCurrentCamera(1); // Set the current one that we use.

	// Load materials and objects.
	Material* pMat = new Material();
	pMat->SetAmbient(glm::vec3(0.24725f, 0.2245f, 0.0645f));
	pMat->SetDiffuse(glm::vec3(0.34615f, 0.3143f, 0.0903f));
	pMat->SetSpecular(glm::vec3(0.797357f, 0.723991f, 0.208006f));
	pMat->SetShininess(83.2f);
	AddMaterial("default", pMat);

	// Load all the skybox faces.
	std::vector<const char*> faces;
	faces.push_back("assets/skybox/right.ppm");
	faces.push_back("assets/skybox/left.ppm");
	faces.push_back("assets/skybox/top.ppm");
	faces.push_back("assets/skybox/bottom.ppm");
	faces.push_back("assets/skybox/back.ppm");
	faces.push_back("assets/skybox/front.ppm");

	// Create skybox.
	Skybox* pSky = new Skybox(faces);
	AddLoadedObject("skybox", pSky);
	AddObject(pSky); // Only one

	// Create obj file obj.
	Obj_Object* pObj = new Obj_Object("./assets/obj/pod.obj");
	AddLoadedObject("obj", pObj);

	// Create geo obj.
	Geo_Object* pGeo = new Geo_Object(1.0f, 20, 20);
	AddLoadedObject("geo", pGeo);

	// Create obj instance.
	Instance_Object* pObjInstance = new Instance_Object(*pObj);
	pObjInstance->SetMaterial(*pMat);
	AddObject(pObjInstance);

	// Create geometry instances
	int num_instances = 10;
	float num_instancesf = float(num_instances);
	for (int i = 0; i <= num_instances; i++)
	{
		Instance_Object* pGeoInstance = new Instance_Object(*pGeo);
		pGeoInstance->SetMaterial(*pMat);
		float radius = num_instancesf / 2.0f;
		float x = glm::cos(360.0f / num_instancesf * (float)i) * radius;
		float y = 0.0f;
		float z = glm::sin(360.0f / num_instancesf * (float)i) * radius;
		pGeoInstance->Translate(glm::vec3(x, y, z));
		AddObject(pGeoInstance);
	}

	// Add Terrain
	Terrain* pTerrain = new Terrain();
	pTerrain->LoadDataFromSimpleGrid(10.0f, 10.0f, 0.0f, 100);
	pTerrain->SmoothUsingDiamondSquare(0, pTerrain->GetNumVertices() - 1, 0, pTerrain->GetNumVertices() - 1, (int)glm::pow(2, 1), 5.0f, 0.01f);
	pTerrain->LoadDataIntoBuffers();
	AddLoadedObject("terrain", pTerrain);
	AddObject(pTerrain);
}

void ResourceManager::Save()
{
}

Shader* ResourceManager::LoadShader(const char* vertex_file_path, const char* fragment_file_path, const char* pName)
{
	m_Shaders[pName] = new Shader(vertex_file_path, fragment_file_path);
	return m_Shaders[pName];
}
