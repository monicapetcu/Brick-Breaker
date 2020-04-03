#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

namespace Object2D_Tema1
{

	// Create square with given bottom left corner, length and color
	Mesh* CreateSquare(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);

	// Create circle with given bottom left corner, length and color
	Mesh* CreateCircle(std::string name, glm::vec3 leftBottomCorner, float radius, glm::vec3 color, bool fill = false);

	// Create brick with given bottom left corner, length and color
	Mesh* CreateBrick(std::string name, glm::vec3 leftBottomCorner, float length, float width, glm::vec3 color, bool fill = false);

}

