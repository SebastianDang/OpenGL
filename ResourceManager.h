#pragma once

#include "Shader.h"
#include "Light.h"
#include "Camera.h"
#include "Obj_Object.h"
#include "Geo_Object.h"
#include "Instance_Object.h"

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	// Load default settings
	// TODO: Load from config file
	void Load(const char *pFile = nullptr);

	// TODO: Save into config file
	void Save();

	// Shader
	Shader *GetShader(const char *pName) { return m_Shaders[pName]; }
	Shader *LoadShader(const char * vertex_file_path, const char *fragment_file_path, const char *pName);

	// Materials
	Material * GetMaterial(const char *pName) { return m_Materials[pName]; }
	void AddMaterial(const char *pName, Material* pMaterial) { m_Materials[pName] = pMaterial; }

	// Loaded Objects (We really only try loading these once).
	Object * GetLoadedObject(const char *pName) { return m_LoadedObjects[pName]; }
	void AddLoadedObject(const char *pName, Object* pObject) { m_LoadedObjects[pName] = pObject; }

	// Light
	Light* GetLight(int index = 0) { if (index < 0 || index >= (int)m_Lights.size()) return nullptr; return m_Lights[index]; }
	void AddLight(Light* pLight) { m_Lights.push_back(pLight); }
	int GetNumLights() { return (int)m_Lights.size(); }

	// Camera
	void AddCamera(Camera *pCamera) { m_Cameras.push_back(pCamera); }
	int GetNumCameras() { return (int)m_Cameras.size(); }
	Camera* GetCamera(int index = 0) { { if (index < 0 || index >= (int)m_Cameras.size()) return nullptr; return m_Cameras[index]; } }
	Camera *GetCurrentCamera() { return GetCamera(m_CurrentCameraIndex); }
	void SetCurrentCamera(int index) { if (index < 0 || index >= (int)m_Cameras.size()) return; m_CurrentCameraIndex = index; }

	// Object
	Object* GetObject(int index = 0) { if (index < 0 || index >= (int)m_Objects.size()) return nullptr; return m_Objects[index]; }
	void AddObject(Object* pObject) { m_Objects.push_back(pObject); }
	int GetNumObjects() { return (int)m_Objects.size(); }

private:
	std::map<std::string, Shader*> m_Shaders;
	std::map<std::string, Material*> m_Materials;
	std::map<std::string, Object*> m_LoadedObjects;

	std::vector<Light*> m_Lights;
	std::vector<Camera*> m_Cameras;
	int m_CurrentCameraIndex = -1;
	std::vector<Object*> m_Objects;
};

