
#pragma once
#include <glm/glm.hpp>


bool checkCollision(const glm::vec3& sphereCenter, float sphereRadius, const glm::vec3& planePoint, const glm::vec3& planeNormal, float tolerance = 0.01f); 
