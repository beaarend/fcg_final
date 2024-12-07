#include <collisions.hpp>

// COLLISION TEST
// Function to check collision between sphere and plane
bool checkCollision(const glm::vec3& sphereCenter, float sphereRadius, const glm::vec3& planePoint, const glm::vec3& planeNormal, float tolerance ) {
    float distance = glm::dot(sphereCenter - planePoint, planeNormal);
    return std::abs(distance) <= (sphereRadius + tolerance);
}
