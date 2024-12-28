#include "glm/ext/vector_float4.hpp"
#include<Hitbox.hpp>
#include <matrices.h>
#include <iostream>
#include <gpuProgramController.h>
#include <glm/glm.hpp>
#include <tiny_obj_loader.h>


AxisAlignedBoundingBox::AxisAlignedBoundingBox(tinyobj::attrib_t& attrib){
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
    this->vertices.push_back(vertices[i]);
  }
}

void AxisAlignedBoundingBox::calculateHitbox(tinyobj::attrib_t& attrib){
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


void AxisAlignedBoundingBox::UpdateHitbox(glm::mat4 model_matrix){
  
    std::cout<<"vertices antes de atualizar normal"<<std::endl;
    for(int i=0;i<8;i++){
      std::cout<<vertices[i].x<<" "<<vertices[i].y<<" "<<vertices[i].z<<std::endl;
    }
    resetVertices();
    std::cout<<"vertices depois de atualizar normal"<<std::endl;
    for(int i=0; i<8; i++){
        vertices[i] = model_matrix * glm::vec4(vertices[i], 1.0f);
    }
    for(int i=0;i<8;i++){
      std::cout<<vertices[i].x<<" "<<vertices[i].y<<" "<<vertices[i].z<<std::endl;
    }
  /*if(hitboxMin.x > hitboxMax.x){*/
  /*  std::swap(hitboxMin.x, hitboxMax.x);*/
  /*}*/
  /*if(hitboxMin.y > hitboxMax.y){*/
  /*  std::swap(hitboxMin.y, hitboxMax.y);*/
  /*}*/
  /*if(hitboxMin.z > hitboxMax.z){*/
  /*  std::swap(hitboxMin.z, hitboxMax.z);*/
  /*}*/
}

void AxisAlignedBoundingBox::resetVertices(){
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


void AxisAlignedBoundingBox::draw(GpuProgramController& gpuProgramController){
    GLuint indices[24] = {
        0, 1, 1, 2, 2, 3, 3, 0,
        4, 5, 5, 6, 6, 7, 7, 4,
        0, 4, 1, 5, 2, 6, 3, 7
    };
  //💀
  glm::vec3 vertices[8] = {
    this->vertices[0],
    this->vertices[1],
    this->vertices[2],
    this->vertices[3],
    this->vertices[4],
    this->vertices[5],
    this->vertices[6],
    this->vertices[7]
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

glm::vec3 Hitbox::getHitboxMin(){
  return vertices[0];
}

glm::vec3 Hitbox::getHitboxMax(){
  return vertices[6];
}
glm::vec3* Hitbox::getVertices(){
    //convertendo o vetor de vertices para um array de vertices
    int size = vertices.size();
    glm::vec3* vertices = new glm::vec3[size];
    for(int i=0;i<size;i++){
        vertices[i] = this->vertices[i];
    }
    return vertices;
  }

OrientedBoundingBox::OrientedBoundingBox(tinyobj::attrib_t& attrib){
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
    this->vertices.push_back(vertices[i]);
  }
}

void OrientedBoundingBox::calculateHitbox(tinyobj::attrib_t& attrib){
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
  center = (hitboxMin + hitboxMax) / 2.0f;
  halfSize = (hitboxMax - hitboxMin) / 2.0f;
  axis[0] = glm::vec3(1.0f, 0.0f, 0.0f);
  axis[1] = glm::vec3(0.0f, 1.0f, 0.0f);
  axis[2] = glm::vec3(0.0f, 0.0f, 1.0f);
}

void OrientedBoundingBox::UpdateHitbox(glm::mat4 model_matrix){
    //parte para atualizar os vertices (para desenhar a hitbox)
    /*std::cout<<"vertices antes de atualizar"<<std::endl;*/
    /*for(int i=0;i<8;i++){*/
    /*  std::cout<<vertices[i].x<<" "<<vertices[i].y<<" "<<vertices[i].z<<std::endl;*/
    /*}*/
    resetVertices();
    for(int i=0; i<8; i++){
        vertices[i] = model_matrix * glm::vec4(vertices[i], 1.0f);
    }
   /*std::cout<<"vertices depois de atualizar"<<std::endl;*/
    /*for(int i=0;i<8;i++){*/
    /*  std::cout<<vertices[i].x<<" "<<vertices[i].y<<" "<<vertices[i].z<<std::endl;*/
    /*}*/

    //parte para atualizar os dados logicos da hitbox
    center = glm::vec3(model_matrix * glm::vec4(center, 1.0f)); 
    axis[0] = glm::normalize(glm::vec3(model_matrix * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f)));
    axis[1] = glm::normalize(glm::vec3(model_matrix * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f)));
    axis[2] = glm::normalize(glm::vec3(model_matrix * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f)));
}

void OrientedBoundingBox::resetVertices(){
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
  this->vertices.clear();
  for(int i=0;i<8;i++){
    this->vertices.push_back(vertices[i]);
  }
}
void OrientedBoundingBox::draw(GpuProgramController& gpuProgramController){
    GLuint indices[24] = {
        0, 1, 1, 2, 2, 3, 3, 0,
        4, 5, 5, 6, 6, 7, 7, 4,
        0, 4, 1, 5, 2, 6, 3, 7
    };
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
  //💀
  glm::vec3 vertices[8] = {
    this->vertices[0],
    this->vertices[1],
    this->vertices[2],
    this->vertices[3],
    this->vertices[4],
    this->vertices[5],
    this->vertices[6],
    this->vertices[7]
  };

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


