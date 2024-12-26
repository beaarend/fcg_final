#include "glm/ext/vector_float4.hpp"
#include<Hitbox.hpp>
#include <matrices.h>
#include <gpuProgramController.h>
#include <glm/glm.hpp>
#include <tiny_obj_loader.h>


Hitbox::Hitbox(tinyobj::attrib_t& attrib){
  calculateHitbox(attrib);

  glm::vec3 vertices[8] = {
      hitboxMin,
      glm::vec3(hitboxMax.x, hitboxMin.y, hitboxMin.z),
      glm::vec3(hitboxMax.x, hitboxMax.y, hitboxMin.z),
      glm::vec3(hitboxMin.x, hitboxMax.y, hitboxMin.z),
      glm::vec3(hitboxMin.x, hitboxMin.y, hitboxMax.z),
      glm::vec3(hitboxMax.x, hitboxMin.y, hitboxMax.z),
      hitboxMax,
      glm::vec3(hitboxMin.x, hitboxMax.y, hitboxMax.z)
  };
  for(int i=0;i<8;i++){
    this->vertices[i]=vertices[i];
  }
}

void Hitbox::calculateHitbox(tinyobj::attrib_t& attrib){
  if(attrib.vertices.empty()){
    return;
  }
  hitboxMin = glm::vec3(attrib.vertices[0], attrib.vertices[1], attrib.vertices[2]);
  hitboxMax = hitboxMin;
  for (size_t i = 0; i < attrib.vertices.size(); i += 3) {
    glm::vec3 vertex(attrib.vertices[i], attrib.vertices[i + 1], attrib.vertices[i + 2]);
    hitboxMin = glm::min(hitboxMin, vertex);
    hitboxMax = glm::max(hitboxMax, vertex);
  }
}


// A INTENCAO É APLICAR A MODEL MATRIX PARA CADA VERTICE DO HITBOX
void Hitbox::UpdateHitbox(glm::mat4 model_matrix){
  
  resetVertices();
  if(hitboxMin.x > hitboxMax.x){
    std::swap(hitboxMin.x, hitboxMax.x);
  }
  if(hitboxMin.y > hitboxMax.y){
    std::swap(hitboxMin.y, hitboxMax.y);
  }
  if(hitboxMin.z > hitboxMax.z){
    std::swap(hitboxMin.z, hitboxMax.z);
  }
  for(int i=0;i<8;i++){
    vertices[i]=model_matrix * glm::vec4(vertices[i],1.0f);
  }
}

void Hitbox::resetVertices(){
  glm::vec3 vertices[8] = {
      hitboxMin,
      glm::vec3(hitboxMax.x, hitboxMin.y, hitboxMin.z),
      glm::vec3(hitboxMax.x, hitboxMax.y, hitboxMin.z),
      glm::vec3(hitboxMin.x, hitboxMax.y, hitboxMin.z),
      glm::vec3(hitboxMin.x, hitboxMin.y, hitboxMax.z),
      glm::vec3(hitboxMax.x, hitboxMin.y, hitboxMax.z),
      hitboxMax,
      glm::vec3(hitboxMin.x, hitboxMax.y, hitboxMax.z)
  };
  for(int i=0;i<8;i++){
    this->vertices[i]=vertices[i];
  }
}


void Hitbox::draw(GpuProgramController& gpuProgramController){
    GLuint indices[24] = {
        0, 1, 1, 2, 2, 3, 3, 0,
        4, 5, 5, 6, 6, 7, 7, 4,
        0, 4, 1, 5, 2, 6, 3, 7
    };

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glUniformMatrix4fv(gpuProgramController.GetModelUniform(), 1, GL_FALSE, glm::value_ptr(Matrices::Identity())); //resetando o model matrix para desenhar o hitbox

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glBindVertexArray(VAO);
    glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}
