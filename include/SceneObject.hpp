#pragma once
#include <tiny_obj_loader.h>
#include <vector>
#include <string>
#include <glad/glad.h>
class SceneObject
{
private:
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  void initBuffers();
  void ComputeNormals();

public:
    SceneObject(const char* filename);
    GLuint vertex_array_object_id;
    std::string name;
    size_t first_index;
    size_t num_indices;
    GLenum rendering_mode;
    void render();
};
