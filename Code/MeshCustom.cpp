#include "Application.h"
#include "MeshCustom.h"
#include "DebugCube.h"

MeshCustom::MeshCustom()
{}

MeshCustom::MeshCustom(const aiScene* scene, int num) : Mesh()
{
	// Vertices
	verticesArray = new GLfloat[scene->mMeshes[num]->mNumVertices * 3];
	int auxCounterVertex = 0;
	for (int i = 0; i < scene->mMeshes[num]->mNumVertices; i++)
	{
		verticesArray[auxCounterVertex] = scene->mMeshes[num]->mVertices[i].x;
		auxCounterVertex++;
		verticesArray[auxCounterVertex] = scene->mMeshes[num]->mVertices[i].y;
		auxCounterVertex++;
		verticesArray[auxCounterVertex] = scene->mMeshes[num]->mVertices[i].z;
		auxCounterVertex++;
	}
	
	verticesSize = scene->mMeshes[num]->mNumVertices;

	glGenBuffers(1, (GLuint*) &(vertices));
	glBindBuffer(GL_ARRAY_BUFFER, vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * verticesSize * 3, verticesArray, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Indices
	indicesArray = new uint[scene->mMeshes[num]->mNumFaces * 3];
	int auxCounterIndex = 0;
	for (int i = 0; i < scene->mMeshes[num]->mNumFaces; i++)
	{
		indicesArray[auxCounterIndex] = scene->mMeshes[num]->mFaces[i].mIndices[0];
		auxCounterIndex++;
		indicesArray[auxCounterIndex] = scene->mMeshes[num]->mFaces[i].mIndices[1];
		auxCounterIndex++;
		indicesArray[auxCounterIndex] = scene->mMeshes[num]->mFaces[i].mIndices[2];
		auxCounterIndex++;
	}

	indicesSize = scene->mMeshes[num]->mNumFaces * 3;
	
	glGenBuffers(1, (GLuint*) &(indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * indicesSize , indicesArray, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Texture
	if (scene->mMeshes[num]->HasTextureCoords(0))
	{
		textureCoords = new GLfloat[scene->mMeshes[num]->mNumVertices * 2];
		for (int i = 0; i < scene->mMeshes[num]->mNumVertices; i++)
		{
			textureCoords[i * 2] = scene->mMeshes[num]->mTextureCoords[0][i].x;
			textureCoords[(i * 2) + 1] = scene->mMeshes[num]->mTextureCoords[0][i].y;
		}

		glGenBuffers(1, (GLuint*)&textureIndex);
		glBindBuffer(GL_ARRAY_BUFFER, textureIndex);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat*) * scene->mMeshes[num]->mNumVertices * 2, textureCoords, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	// Normals TODO: MAKE IT WORK
	if (scene->mMeshes[num]->HasNormals())
	{
		normals = new DebugArrow[scene->mMeshes[num]->mNumVertices];
		for (int i = 0; i < scene->mMeshes[num]->mNumVertices; i++)
		{
			normals[i].SetDebugArrow(math::float3(scene->mMeshes[num]->mVertices[i].x, scene->mMeshes[num]->mVertices[i].y, scene->mMeshes[num]->mVertices[i].z), math::float3(scene->mMeshes[num]->mVertices[i].x + scene->mMeshes[num]->mNormals[i].x, scene->mMeshes[num]->mVertices[i].y + scene->mMeshes[num]->mNormals[i].y, scene->mMeshes[num]->mVertices[i].z + scene->mMeshes[num]->mNormals[i].z));
		}
	}

	// Bounding Box
	boundingBox.SetNegativeInfinity();
	boundingBox.Enclose((const math::float3*)verticesArray, verticesSize);
	boundingBoxCube = new DebugCube(boundingBox.CenterPoint(), boundingBox.Size());
}

void MeshCustom::Render(Texture* texture)
{
	glPushMatrix();

	// Draw Geometry
	if (texture != nullptr)
	{
		glBindTexture(GL_TEXTURE_2D, texture->GetBufferPos());
	}
	glEnableClientState(GL_VERTEX_ARRAY);

	glTranslatef(position.x, position.y, position.z);
	glScalef(scale.x, scale.y, scale.z);

	glBindBuffer(GL_ARRAY_BUFFER, vertices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	if (textureCoords != nullptr)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, textureIndex);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
	glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	// Draw Normals
	for (int i = 0; i < verticesSize; i++)
	{
		if (normals != nullptr)
			normals[i].Render();
	}
	boundingBoxCube->Render();

	glPopMatrix();
}