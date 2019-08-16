#include "stdafx.h"
#include "ResourceManager.h"
#include <io.h>
#include <cstdlib>
#pragma warning(disable:26495 26451)
#include "rapidjson/document.h"
#pragma warning(default:26495 26451) 
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

void ResourceManager::Load(const char* pfilename)
{
	// Load from file
	if (!pfilename || _access(pfilename, 0) == -1) return;
	m_Filename = pfilename;

	// Open as json
	std::ifstream jsonfile(pfilename);
	std::vector<char> jsonbuffer((std::istreambuf_iterator<char>(jsonfile)), std::istreambuf_iterator<char>());
	jsonbuffer.push_back('\0');

	// Attempt to parse json
	rapidjson::Document jsondoc;
	jsondoc.Parse(&jsonbuffer[0]);
	if (!jsondoc.IsObject()) return;

	// Load any required shaders here
	if (jsondoc.HasMember("shaders"))
	{
		const rapidjson::Value& shaders = jsondoc["shaders"];
		for (rapidjson::SizeType i = 0; shaders.IsArray() && i < shaders.Size(); i++)
		{
			const rapidjson::Value& shader = shaders[i];

			// Shader properties
			std::string vertex, fragment, name;

			// Parse shader properties
			if (shader.HasMember("vertex"))
				vertex = shader["vertex"].GetString();
			if (shader.HasMember("fragment"))
				fragment = shader["fragment"].GetString();
			if (shader.HasMember("name"))
				name = shader["name"].GetString();

			// Load shader
			if (!vertex.empty() && !fragment.empty() && !name.empty())
				LoadShader(vertex.c_str(), fragment.c_str(), name.c_str());
		}
	}

	// Load any lights here
	if (jsondoc.HasMember("lights"))
	{
		const rapidjson::Value& lights = jsondoc["lights"];
		for (rapidjson::SizeType i = 0; lights.IsArray() && i < lights.Size(); i++)
		{
			const rapidjson::Value& light = lights[i];

			// Light properties
			std::string type;
			bool enabled = false;
			glm::vec3 position = glm::vec3(0.0f,0.0f,0.0f);
			glm::vec3 ambient = glm::vec3(0.0f, 0.0f, 0.0f);
			glm::vec3 diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
			glm::vec3 specular = glm::vec3(0.0f, 0.0f, 0.0f);
			glm::vec3 direction = glm::vec3(0.0f, 0.0f, 0.0f);
			float quadratic = 0.0f, spotCutoff = 0.0f, exponent = 0.0f;

			// Parse light properties
			if (light.HasMember("type"))
				type = light["type"].GetString();
			if (light.HasMember("enabled"))
				enabled = light["enabled"].GetBool();
			if (light.HasMember("position") &&
				light["position"].IsArray() &&
				light["position"].Size() >= 3)
			{
				position = glm::vec3(
					light["position"][0].GetDouble(),
					light["position"][1].GetDouble(),
					light["position"][2].GetDouble());
			}
			if (light.HasMember("ambient") &&
				light["ambient"].IsArray() &&
				light["ambient"].Size() >= 3)
			{
				ambient = glm::vec3(
					light["ambient"][0].GetDouble(),
					light["ambient"][1].GetDouble(),
					light["ambient"][2].GetDouble());
			}
			if (light.HasMember("diffuse") &&
				light["diffuse"].IsArray() &&
				light["diffuse"].Size() >= 3)
			{
				diffuse = glm::vec3(
					light["diffuse"][0].GetDouble(),
					light["diffuse"][1].GetDouble(),
					light["diffuse"][2].GetDouble());
			}
			if (light.HasMember("specular") &&
				light["specular"].IsArray() &&
				light["specular"].Size() >= 3)
			{
				specular = glm::vec3(
					light["specular"][0].GetDouble(),
					light["specular"][1].GetDouble(),
					light["specular"][2].GetDouble());
			}
			if (light.HasMember("direction") &&
				light["direction"].IsArray() &&
				light["direction"].Size() >= 3)
			{
				direction = glm::vec3(
					light["direction"][0].GetDouble(),
					light["direction"][1].GetDouble(),
					light["direction"][2].GetDouble());
			}
			if (light.HasMember("quadratic"))
				quadratic = light["quadratic"].GetDouble();
			if (light.HasMember("spotCutoff"))
				spotCutoff = light["spotCutoff"].GetDouble();
			if (light.HasMember("exponent"))
				exponent = light["exponent"].GetDouble();

			// Do some processing on the type
			std::transform(type.begin(), type.end(), type.begin(),
				[](unsigned char c) { return std::tolower(c); });

			// Load light
			if (type == "directional")
			{
				DirectionalLight* pLight = new DirectionalLight();
				pLight->SetEnabled(enabled);
				pLight->SetDirection(direction);
				pLight->SetAmbient(ambient);
				pLight->SetDiffuse(diffuse);
				pLight->SetSpecular(specular);
				AddLight(pLight);
			}
			if (type == "point")
			{
				PointLight* pLight = new PointLight();
				pLight->SetEnabled(enabled);
				pLight->SetPosition(position);
				pLight->SetAmbient(ambient);
				pLight->SetDiffuse(diffuse);
				pLight->SetSpecular(specular);
				pLight->SetQuadratic(quadratic);
				AddLight(pLight);
			}
			if (type == "spot")
			{
				SpotLight* pLight = new SpotLight();
				pLight->SetEnabled(enabled);
				pLight->SetPosition(position);
				pLight->SetAmbient(ambient);
				pLight->SetDiffuse(diffuse);
				pLight->SetSpecular(specular);
				pLight->SetQuadratic(quadratic);
				pLight->SetDirection(direction);
				pLight->SetSpotCutoff(spotCutoff / 180.0f * glm::pi<float>()); // Degrees
				pLight->SetSpotExponent(exponent);
				AddLight(pLight);
			}
		}
	}

	// Load any cameras here
	if (jsondoc.HasMember("cameras"))
	{
		const rapidjson::Value& cameras = jsondoc["cameras"];
		for (rapidjson::SizeType i = 0; cameras.IsArray() && i < cameras.Size(); i++)
		{
			const rapidjson::Value& camera = cameras[i];

			// Camera properties
			std::string type;
			glm::vec3 position = glm::vec3(0.0f, 0.0f, 1.0f);
			glm::vec3 lookAt = glm::vec3(0.0f, 0.0f, 0.0f);
			glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
			
			// Parse camera properties
			if (camera.HasMember("type"))
				type = camera["type"].GetString();
			if (camera.HasMember("position") &&
				camera["position"].IsArray() &&
				camera["position"].Size() >= 3)
			{
				position = glm::vec3(
					camera["position"][0].GetDouble(),
					camera["position"][1].GetDouble(),
					camera["position"][2].GetDouble());
			}
			if (camera.HasMember("lookAt") &&
				camera["lookAt"].IsArray() &&
				camera["lookAt"].Size() >= 3)
			{
				lookAt = glm::vec3(
					camera["lookAt"][0].GetDouble(),
					camera["lookAt"][1].GetDouble(),
					camera["lookAt"][2].GetDouble());
			}
			if (camera.HasMember("up") &&
				camera["up"].IsArray() &&
				camera["up"].Size() >= 3)
			{
				up = glm::vec3(
					camera["up"][0].GetDouble(),
					camera["up"][1].GetDouble(),
					camera["up"][2].GetDouble());
			}

			// Do some processing on the type
			std::transform(type.begin(), type.end(), type.begin(),
				[](unsigned char c) { return std::tolower(c); });

			// Load camera
			if (type == "first")
			{
				FirstPersonCamera* pCam = new FirstPersonCamera(position);
				glm::vec3 direction = glm::vec3(glm::rotate(glm::mat4(1.0f), (30.0f / 180.0f * glm::pi<float>()), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
				pCam->LockFieldOfView(direction, 180.0f);
				AddCamera(pCam);
			}
			else if (type == "third")
			{
				ThirdPersonCamera* pCam = new ThirdPersonCamera(position);
				AddCamera(pCam);
			}
			else
			{
				Camera* pCam = new Camera(position, lookAt, up);
				AddCamera(pCam);
			}
		}
	}

	// Set current camera index if applicable
	if (jsondoc.HasMember("cameraIndex"))
	{
		int index = jsondoc["cameraIndex"].GetInt();
		SetCurrentCamera(std::max(0, std::min(index, GetNumCameras()-1)));
	}
	else
	{
		SetCurrentCamera(0);
	}

	// Load any objects here
	if (jsondoc.HasMember("objects"))
	{
		const rapidjson::Value& objects = jsondoc["objects"];
		for (rapidjson::SizeType i = 0; objects.IsArray() && i < objects.Size(); i++)
		{
			const rapidjson::Value& object = objects[i];

			// Object properties
			std::string name, obj;

			// Parse object properties
			if (object.HasMember("name"))
				name = object["name"].GetString();
			if (object.HasMember("obj"))
				obj = object["obj"].GetString();

			// Add object
			Obj_Object* pObj = new Obj_Object(obj.c_str());
			AddLoadedObject(name.c_str(), pObj);
		}
	}

	// Load any materials here
	if (jsondoc.HasMember("materials"))
	{
		const rapidjson::Value& materials = jsondoc["materials"];
		for (rapidjson::SizeType i = 0; materials.IsArray() && i < materials.Size(); i++)
		{
			const rapidjson::Value& material = materials[i];

			// Material properties
			std::string name;
			glm::vec3 ambient = glm::vec3(0.0f);
			glm::vec3 diffuse = glm::vec3(0.0f);
			glm::vec3 specular = glm::vec3(0.0f);
			float shininess = 0.0f;

			// Parse material properties
			if (material.HasMember("name"))
				name = material["name"].GetString();
			if (material.HasMember("ambient") &&
				material["ambient"].IsArray() &&
				material["ambient"].Size() >= 3)
			{
				ambient = glm::vec3(
					material["ambient"][0].GetDouble(),
					material["ambient"][1].GetDouble(),
					material["ambient"][2].GetDouble());
			}
			if (material.HasMember("diffuse") &&
				material["diffuse"].IsArray() &&
				material["diffuse"].Size() >= 3)
			{
				diffuse = glm::vec3(
					material["diffuse"][0].GetDouble(),
					material["diffuse"][1].GetDouble(),
					material["diffuse"][2].GetDouble());
			}
			if (material.HasMember("specular") &&
				material["specular"].IsArray() &&
				material["specular"].Size() >= 3)
			{
				specular = glm::vec3(
					material["specular"][0].GetDouble(),
					material["specular"][1].GetDouble(),
					material["specular"][2].GetDouble());
			}
			if (material.HasMember("shininess"))
				shininess = material["shininess"].GetDouble();

			// Add material
			Material* pMat = new Material();
			pMat->SetAmbient(ambient);
			pMat->SetDiffuse(diffuse);
			pMat->SetSpecular(specular);
			pMat->SetShininess(shininess);
			AddMaterial(name.c_str(), pMat);
		}
	}

	// Load any instances here
	if (jsondoc.HasMember("instances"))
	{
		const rapidjson::Value& instances = jsondoc["instances"];
		for (rapidjson::SizeType i = 0; instances.IsArray() && i < instances.Size(); i++)
		{
			const rapidjson::Value& instance = instances[i];
			
			// instance properties
			std::string name, material;
			glm::vec3 translate = glm::vec3(0.0f);
			glm::vec3 rotate = glm::vec3(0.0f);
			glm::vec3 scale = glm::vec3(1.0f);

			// Parse instance properties
			if (instance.HasMember("name"))
				name = instance["name"].GetString();
			if (instance.HasMember("material"))
				material = instance["material"].GetString();
			if (instance.HasMember("translate") &&
				instance["translate"].IsArray() &&
				instance["translate"].Size() >= 3)
			{
				translate = glm::vec3(
					instance["translate"][0].GetDouble(),
					instance["translate"][1].GetDouble(),
					instance["translate"][2].GetDouble());
			}
			if (instance.HasMember("rotate") &&
				instance["rotate"].IsArray() &&
				instance["rotate"].Size() >= 3)
			{
				rotate = glm::vec3(
					instance["rotate"][0].GetDouble(),
					instance["rotate"][1].GetDouble(),
					instance["rotate"][2].GetDouble());
			}
			if (instance.HasMember("scale") &&
				instance["scale"].IsArray() &&
				instance["scale"].Size() >= 3)
			{
				scale = glm::vec3(
					instance["scale"][0].GetDouble(),
					instance["scale"][1].GetDouble(),
					instance["scale"][2].GetDouble());
			}

			// Add instance
			Object *pObj = GetLoadedObject(name.c_str());
			Material* pMat = GetMaterial(material.c_str());
			if (pObj)
			{
				Instance_Object* pObjInstance = new Instance_Object(*pObj);
				pObjInstance->Translate(translate);
				pObjInstance->Rotate(rotate.x, glm::vec3(1.0f, 0.0f, 0.0f));
				pObjInstance->Rotate(rotate.y, glm::vec3(0.0f, 1.0f, 0.0f));
				pObjInstance->Rotate(rotate.z, glm::vec3(0.0f, 0.0f, 1.0f));
				pObjInstance->Scale(scale);
				if (pMat)
					pObjInstance->SetMaterial(*pMat);
				AddObject(pObjInstance);
			}
		}
	}

	/*
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
	*/
}

void ResourceManager::Save()
{
}

Shader* ResourceManager::LoadShader(const char* vertex_file_path, const char* fragment_file_path, const char* pName)
{
	m_Shaders[pName] = new Shader(vertex_file_path, fragment_file_path);
	return m_Shaders[pName];
}
