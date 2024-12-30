#pragma once
#include <glm/glm.hpp>


class Collisions {
public:
    static bool AABBsTest(const glm::vec3& min1, const glm::vec3& max1, const glm::vec3& min2, const glm::vec3& max2);
    static bool OBBsTest(const glm::vec3& center1, const glm::vec3& halfSize1, const glm::vec3 axis1[3], const glm::vec3& center2, const glm::vec3& halfSize2, const glm::vec3 axis2[3]);
  static bool TestOBBCollisionVertices(glm::vec3* verticesA, glm::vec3* verticesB);
  static bool SpheresTest(const glm::vec3& center1, float radius1, const glm::vec3& center2, float radius2);
};


