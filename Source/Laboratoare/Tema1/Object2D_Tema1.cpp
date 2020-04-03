#include "Object2D_Tema1.h"

#include <Core/Engine.h>

Mesh* Object2D_Tema1::CreateSquare(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(length, 0, 0), color),
		VertexFormat(corner + glm::vec3(length, length, 0), color),
		VertexFormat(corner + glm::vec3(0, length, 0), color)
	};

	Mesh* square = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };
	
	if (!fill) {
		square->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	square->InitFromData(vertices, indices);
	return square;
}

Mesh* Object2D_Tema1::CreateCircle(std::string name, glm::vec3 leftBottomCorner, float radius, glm::vec3 color, bool fill)
{
	glm::vec3 corner = leftBottomCorner;
	const float PI = 3.1415926f;
	int i;
	int triangleAmount = 50; //# of triangles used to draw circle
	std::vector<VertexFormat> vertices;

	Mesh* circle = new Mesh(name);

	//GLfloat radius = 0.8f; //radius
	GLfloat twicePi = (GLfloat)2.0f * PI;
	
	glBegin(GL_TRIANGLE_FAN);
	vertices.push_back(VertexFormat(glm::vec3(corner.x, corner.y, 0), color)); // center of circle
	for (i = 0; i <= triangleAmount; i++) {
		vertices.push_back(VertexFormat(glm::vec3(
			corner.x + (radius * cos(i *  twicePi / triangleAmount)),
			corner.y + (radius * sin(i * twicePi / triangleAmount)), 0
		), color));
	}
	glEnd();

	std::vector<unsigned short> indices = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
	for (i = 1; i <= triangleAmount - 1; i++) {
		indices.push_back(0);
		indices.push_back(i);
		indices.push_back(i + 1);
	}
	
	indices.push_back(0);
	indices.push_back(triangleAmount);
	indices.push_back(1);

	circle->InitFromData(vertices, indices);
	return circle;
}

Mesh* Object2D_Tema1::CreateBrick(std::string name, glm::vec3 leftBottomCorner, float length, float width, glm::vec3 color, bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(length, 0, 0), color),
		VertexFormat(corner + glm::vec3(length, width, 0), color),
		VertexFormat(corner + glm::vec3(0, width, 0), color)
	};

	Mesh* brick = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };

	if (!fill) {
		brick->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	brick->InitFromData(vertices, indices);
	return brick;
}