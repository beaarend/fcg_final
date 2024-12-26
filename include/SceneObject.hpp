#pragma once
#include <tiny_obj_loader.h>
#include <vector>
#include <string>
#include <cstring>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include<matrices.h>
#include <gpuProgramController.h>
class SceneObject
{
private:
  // tinyobj::attrib_t attrib;
  // std::vector<tinyobj::shape_t> shapes;
  // std::vector<tinyobj::material_t> materials;
  void initBuffers();
  void ComputeNormals();
  void calculateHitbox();
  glm::vec3 object_color=glm::vec3(0.5f,0.5f,0.5f);

  glm::vec3 hitboxMin;
  glm::vec3 hitboxMax;


public:
    SceneObject(const char* filename, const char *flag);

    SceneObject(const tinyobj::attrib_t &attrib, 
                         const tinyobj::shape_t &shape, 
                         const std::vector<tinyobj::material_t> &materials);

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials; 

    int object_id;
    GLuint vertex_array_object_id;
    glm::mat4 model_matrix=Matrices::Identity();
    std::string name;
    size_t first_index;
    size_t num_indices;
    GLenum rendering_mode;
    void render(GpuProgramController& gpuProgramController);
    bool checkCollision(const SceneObject& other);
    glm::vec3 getHitboxMin();
    glm::vec3 getHitboxMax();
    void scale(const glm::vec3& scale);
    void translate(float x, float y, float z); 
    void rotateX(float angle);
    void rotateY(float angle);
    void rotateZ(float angle);
    void setObjectID(int object_id);
    void setModelMatrix(glm::mat4 model_matrix);
    void setObjectColor(glm::vec3 object_color);

    bool getPlaneInfo(glm::vec3& planePoint, glm::vec3& planeNormal);
};
