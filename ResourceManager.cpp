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
	LoadShader("shaders/shader.vert", "shaders/shader.frag", "object");
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
	Camera *pCamera = new Camera(camera_pos, camera_look_at, camera_up);
	m_Cameras.push_back(pCamera);

	Camera *pThirdCamera = new ThirdPersonCamera(glm::vec3(0.0f));
	m_Cameras.push_back(pThirdCamera);

	Camera *pFirstCamera = new FirstPersonCamera(glm::vec3(0.0f));
	m_Cameras.push_back(pFirstCamera);
	
	SetCurrentCamera(1);

	// Objects
	Material mat;
	mat.SetAmbient(glm::vec3(0.24725f, 0.2245f, 0.0645f));
	mat.SetDiffuse(glm::vec3(0.34615f, 0.3143f, 0.0903f));
	mat.SetSpecular(glm::vec3(0.797357f, 0.723991f, 0.208006f));
	mat.SetShininess(83.2f);
	
	Obj_Object *pObj = new Obj_Object("./assets/obj/pod.obj");
	pObj->AddMaterial(mat);
	m_Objects.push_back(pObj);

	Geo_Object *pGeo = new Geo_Object();
	pGeo->LoadSphere(1.0f, 20, 20);
	pGeo->Scale(glm::vec3(2.0f));
	pGeo->Translate(glm::vec3(0.0f, -3.0f, 0.0f));
	pGeo->AddMaterial(mat);
	m_Objects.push_back(pGeo);
}

void ResourceManager::Save()
{
}

Shader* ResourceManager::LoadShader(const char * vertex_file_path, const char * fragment_file_path, const char * pName)
{
	m_Shaders[pName] = new Shader(vertex_file_path, fragment_file_path);
	return m_Shaders[pName];
}
