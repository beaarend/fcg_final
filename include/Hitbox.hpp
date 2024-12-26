#pragma once
#include <glm/glm.hpp>
#include <tiny_obj_loader.h>
#include <gpuProgramController.h>
class Hitbox{
private:
  glm::vec3 hitboxMin;
  glm::vec3 hitboxMax;
  glm::vec3 vertices[8];

public:
  Hitbox(tinyobj::attrib_t& attrib);
  void calculateHitbox(tinyobj::attrib_t& attrib);
  void UpdateHitbox(glm::mat4 model_matrix);
  glm::vec3 getHitboxMin();
  glm::vec3 getHitboxMax();
  glm::vec3* getVertices();
  void resetVertices();
  void draw(GpuProgramController& gpuProgramController);
};
