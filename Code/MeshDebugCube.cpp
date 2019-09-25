#include "MeshDebugCube.h"

MeshDebugCube::MeshDebugCube(math::float3 center, math::float3 resize) : Mesh()
{
	position = center;
	scale = resize;

	verticesSize = 24;
	verticesArray = new GLfloat[verticesSize]{
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,
		-0.5f, 0.5f, -0.5f,
		-0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f
	};

	vertices = 0u;
	glGenBuffers(1, (GLuint*) &(vertices));
	glBindBuffer(GL_ARRAY_BUFFER, vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * verticesSize, verticesArray, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	indicesSize = 24;
	unsigned int* indicesArray = new unsigned int[indicesSize] {
		0,1,
		1,2,
		2,3,
		3,0,
		4,5,
		5,6,
		6,7,
		7,4,
		0,4,
		3,7,
		2,6,
		1,5
	};

	indices = 0u;
	glGenBuffers(1, (GLuint*) &(indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indicesSize, indicesArray, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

MeshDebugCube::~MeshDebugCube()
{

}

void MeshDebugCube::Render()
{
	glEnableClientState(GL_VERTEX_ARRAY);

	glTranslatef(position.x, position.y, position.z);
	// TODO : si actives l'scale aqui es modifica la malla de la casa (pero no la del guerrer...)
	//glScalef(scale.x, scale.y, scale.z);

	glBindBuffer(GL_ARRAY_BUFFER, vertices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
	glLineWidth(4);
	glColor3f(0,255,0);
	// TODO: AQUI FICAR L'INDICES SIZE NO M'ACABA DE MOLAR, PREGUNTAR SI ESTA BE
	glDrawElements(GL_LINES, indicesSize, GL_UNSIGNED_INT, NULL);
	glColor3f(255, 255, 255);
	glLineWidth(1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
}