#include "stdafx.h"
#include "ResourceManager.h"

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{

}

void ResourceManager::Load(const char * pFile)
{
	// Attempt to load from file here.
	if (pFile)
	{

		return;
	}

	// Load any required shaders here.
	LoadShader("shaders/object.vert", "shaders/object.frag", "object");
	LoadShader("shaders/skybox.vert", "shaders/skybox.frag", "skybox");

	// Directional Light
	DirectionalLight *pDirectional = new DirectionalLight();
	pDirectional->SetEnabled(true);
	pDirectional->SetDirection(glm::vec3(-0.2f, -1.0f, -0.3f));
	pDirectional->SetAmbient(glm::vec3(1.0f, 1.0f, 1.0f));
	pDirectional->SetDiffuse(glm::vec3(1.0f, 1.0f, 1.0f));
	pDirectional->SetSpecular(glm::vec3(1.0f, 1.0f, 1.0f));

	// PointLight
	PointLight *pPoint = new PointLight();
	pPoint->SetEnabled(true);
	pPoint->SetPosition(glm::vec3(10.0f, 10.0f, 10.0f));
	pPoint->SetAmbient(glm::vec3(0.2f, 0.2f, 0.2f));
	pPoint->SetDiffuse(glm::vec3(0.5f, 0.5f, 0.5f));
	pPoint->SetSpecular(glm::vec3(0.8f, 0.8f, 0.8f));
	pPoint->SetQuadratic(0.032f);

	// SpotLight
	SpotLight *pSpot = new SpotLight();
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
	m_Lights.push_back(pDirectional);
	m_Lights.push_back(pPoint);
	m_Lights.push_back(pSpot);

	// Initialize the camera
	glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 20.0f);
	glm::vec3 camera_look_at = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
	Camera *pDefaultCamera = new Camera(camera_pos, camera_look_at, camera_up);
	Camera *pThirdCamera = new ThirdPersonCamera(glm::vec3(0.0f));
	Camera *pFirstCamera = new FirstPersonCamera(glm::vec3(0.0f));
	
	// Add to cameras.
	m_Cameras.push_back(pDefaultCamera);
	m_Cameras.push_back(pThirdCamera);
	m_Cameras.push_back(pFirstCamera);
	SetCurrentCamera(1); // Set the current one that we use.

	// Load all the skybox faces.
	std::vector<const char *> faces;
	faces.push_back("assets/skybox/right.ppm");
	faces.push_back("assets/skybox/left.ppm");
	faces.push_back("assets/skybox/top.ppm");
	faces.push_back("assets/skybox/bottom.ppm");
	faces.push_back("assets/skybox/back.ppm");
	faces.push_back("assets/skybox/front.ppm");
	Skybox *pSky = new Skybox(faces);
	AddObject(pSky);

	// Load materials and objects
	Material *mat = new Material();
	mat->SetAmbient(glm::vec3(0.24725f, 0.2245f, 0.0645f));
	mat->SetDiffuse(glm::vec3(0.34615f, 0.3143f, 0.0903f));
	mat->SetSpecular(glm::vec3(0.797357f, 0.723991f, 0.208006f));
	mat->SetShininess(83.2f);
	AddMaterial("default", mat);
	
	Obj_Object *Obj = new Obj_Object("./assets/obj/pod.obj");
	Geo_Object *Geo = new Geo_Object();
	Geo->LoadSphereIntoBuffer(1.0f, 20, 20);

	AddLoadedObject("obj", Obj);
	AddLoadedObject("geo", Geo);

	// Create instances
	int num_instances = 10;
	float num_instancesf = float(num_instances);
	for (int i = 0; i <= num_instances; i++)
	{
		Instance_Object *pInstance = new Instance_Object(*Geo);
		pInstance->SetMaterial(*mat);
		float radius = num_instancesf / 2.0f;
		float x = glm::cos(360.0f / num_instancesf * (float)i) * radius;
		float y = 0.0f;
		float z = glm::sin(360.0f / num_instancesf * (float)i) * radius;
		pInstance->Translate(glm::vec3(x, y, z));
		AddObject(pInstance);
	}
}

void ResourceManager::Save()
{
}

Shader* ResourceManager::LoadShader(const char * vertex_file_path, const char * fragment_file_path, const char * pName)
{
	m_Shaders[pName] = new Shader(vertex_file_path, fragment_file_path);
	return m_Shaders[pName];
}
