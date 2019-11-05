#include "Globals.h"
#include "Application.h"
#include "SceneImporter.h"

#include "Mesh.h"
#include "C_Mesh.h"
#include "C_Material.h"

#include "physfs/include/physfs.h"

#include "Assimp/include/Importer.hpp"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#include "Assimp/include/cimport.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")
#pragma comment (lib, "Assimp/libx86/zlibstatic.lib")

void myCallback(const char* msg, char* userData)
{
	LOG_CONSOLE("%s", msg);
}

SceneImporter::SceneImporter()
{
	bool ret = true;
	// Assimp CallBack
	struct aiLogStream stream;
	stream.callback = myCallback;
	aiAttachLogStream(&stream);
}

SceneImporter::~SceneImporter()
{
	aiDetachAllLogStreams();
}

void SceneImporter::Import(const char* path, const ImporterSettings* settings)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType | aiProcess_FlipUVs);
	if (!scene)
	{
		LOG_CONSOLE_IDE("Mesh can not be loaded");
		return;
	}

	SceneImporterSettings* sceneSettings = (SceneImporterSettings*)settings;
	IterateSceneLoading(scene, scene->mRootNode, App->scene->root, sceneSettings->originalPath.c_str());

	App->scene->MakeCameraLookThisGOSelected();
}

void SceneImporter::IterateSceneLoading(const aiScene* scene, const aiNode* node, GameObject* parent, const char* originalPath)
{
	// Create GameObject
	GameObject* go = App->scene->CreateGameObject(node->mName.C_Str(), parent);

	// Create & Load Mesh
	if (node->mMeshes != nullptr)
	{
		Mesh* mesh = new Mesh(scene, node);
		App->renderer3D->AddMesh(mesh);
		App->fs->CreateOwnMesh(mesh);
		Component* compMesh = go->CreateComponent(COMPONENT_TYPE::COMPONENT_MESH, "Mesh");
		Component* comTexture = go->CreateComponent(COMPONENT_TYPE::COMPONENT_MATERIAL, "Material");
		compMesh->GetComponentAsMesh()->mesh = mesh;

		aiString str;
		scene->mMaterials[scene->mMeshes[(*node->mMeshes)]->mMaterialIndex]->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &str);
		std::string tmp_texture_path(originalPath);
		if (sizeof(originalPath) <= 4) // Thats to check if originalPath exist
		{
			tmp_texture_path = App->fs->resources_directory;
		}
		tmp_texture_path.append(str.C_Str());
		// Load texture
		App->fs->materialImporter->Import(tmp_texture_path.c_str());
		// Search it
		std::string texture_name_and_extension(tmp_texture_path);
		texture_name_and_extension = texture_name_and_extension.substr(texture_name_and_extension.find_last_of(92) + 1);
		comTexture->GetComponentAsMaterial()->texture = App->textures->SearchTexture(texture_name_and_extension.c_str());
		str.Clear();
	}

	// Iterate Childs
	for (int i = 0; i < node->mNumChildren; i++)
	{
		IterateSceneLoading(scene, node->mChildren[i], go, originalPath);
	}

	// Look if there are more meshes at same Node
	for (int i = 1; i < node->mNumMeshes; i++)
	{
		// Create GameObject
		GameObject* auxGo = App->scene->CreateGameObject(node->mName.C_Str(), go);

		// Create & Load Mesh & Load Texture
		if (node->mMeshes != nullptr)
		{
			Mesh* mesh = new Mesh(scene, node, i);
			App->renderer3D->AddMesh(mesh);
			Component* compMesh = auxGo->CreateComponent(COMPONENT_TYPE::COMPONENT_MESH, "Mesh");
			compMesh->GetComponentAsMesh()->mesh = mesh;
			Component* comTexture = auxGo->CreateComponent(COMPONENT_TYPE::COMPONENT_MATERIAL, "Material");

			aiString str;
			scene->mMaterials[scene->mMeshes[(*node->mMeshes)]->mMaterialIndex]->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &str);
			std::string tmp_texture_path(originalPath);
			tmp_texture_path.append(str.C_Str());
			// Load texture
			App->fs->materialImporter->Import(tmp_texture_path.c_str());
			// Search it
			std::string texture_name_and_extension(tmp_texture_path);
			texture_name_and_extension = texture_name_and_extension.substr(texture_name_and_extension.find_last_of(92) + 1);
			comTexture->GetComponentAsMaterial()->texture = App->textures->SearchTexture(texture_name_and_extension.c_str());
			str.Clear();
			str.Clear();
		}
	}
}

void SceneImporter::Load(const char* exportedFile)
{

}

Mesh* SceneImporter::LoadMesh(const char* exportedFile)
{
	// Process of read it
	FILE* nFile = fopen(exportedFile, "r");
	fseek(nFile, 0L, SEEK_END);
	const unsigned int size = ftell(nFile);
	fseek(nFile, 0L, SEEK_SET);

	char* cursor;
	cursor = new char[size];
	fread(cursor, sizeof(char), size, nFile);

	// Get Header
	unsigned int ranged[2];
	unsigned int bytes = sizeof(ranged);
	memcpy(ranged, cursor, bytes);

	// Get Indices
	cursor += bytes;
	
	return nullptr;
}