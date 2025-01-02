#include "glm/ext/vector_float4.hpp"
#include<Hitbox.hpp>
#include <matrices.h>
#include <iostream>
#include <gpuProgramController.h>
#include <glm/glm.hpp>
#include <tiny_obj_loader.h>
#include <collisions.hpp>


glm::vec3 Hitbox::getHitboxMin(){
  return vertices[0];
}

glm::vec3 Hitbox::getHitboxMax(){
  return vertices[6];
}

glm::vec3* Hitbox::getVertices(){
    int size = vertices.size();
    glm::vec3* vertices = new glm::vec3[size];
    for(int i=0;i<size;i++){
        vertices[i] = this->vertices[i];
    }
    return vertices;
}

HitboxType Hitbox::getHitboxType(){
  return hitboxType;
}

void Hitbox::printVertices(){
  for(int i=0;i<8;i++){
    std::cout<<vertices[i].x<<" "<<vertices[i].y<<" "<<vertices[i].z<<std::endl;
  }
}

AxisAlignedBoundingBox::AxisAlignedBoundingBox(tinyobj::attrib_t& attrib){
  calculateHitbox(attrib);
  this->hitboxType = HitboxType::AABB;
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

void AxisAlignedBoundingBox::translate(float x, float y, float z){
  glm::vec3 translation(x, y, z);
  hitboxMin += translation;
  hitboxMax += translation;
}

void AxisAlignedBoundingBox::scale(const glm::vec3& scale){
  hitboxMin *= scale;
  hitboxMax *= scale;
}

void AxisAlignedBoundingBox::rotateX(float angle){
  glm::mat4 rotation_matrix = Matrices::RotateX(angle);
  glm::vec4 min(hitboxMin, 1.0f);
  glm::vec4 max(hitboxMax, 1.0f);
  min = rotation_matrix * min;
  max = rotation_matrix * max;
  hitboxMin = glm::vec3(min);
  hitboxMax = glm::vec3(max);
}

void AxisAlignedBoundingBox::rotateY(float angle){
  glm::mat4 rotation_matrix = Matrices::RotateY(angle);
  glm::vec4 min(hitboxMin, 1.0f);
  glm::vec4 max(hitboxMax, 1.0f);
  min = rotation_matrix * min;
  max = rotation_matrix * max;
  hitboxMin = glm::vec3(min);
  hitboxMax = glm::vec3(max);
}

void AxisAlignedBoundingBox::rotateZ(float angle){
  glm::mat4 rotation_matrix = Matrices::RotateZ(angle);
  glm::vec4 min(hitboxMin, 1.0f);
  glm::vec4 max(hitboxMax, 1.0f);
  min = rotation_matrix * min;
  max = rotation_matrix * max;
  hitboxMin = glm::vec3(min);
  hitboxMax = glm::vec3(max);
}


void AxisAlignedBoundingBox::UpdateHitbox(glm::mat4 model_matrix){
  
    // std::cout<<"vertices antes de atualizar normal"<<std::endl;
    // for(int i=0;i<8;i++){
    //   std::cout<<vertices[i].x<<" "<<vertices[i].y<<" "<<vertices[i].z<<std::endl;
    // }

    resetVertices();

    // std::cout<<"vertices depois de atualizar normal"<<std::endl;
    for(int i=0; i<8; i++){
        vertices[i] = model_matrix * glm::vec4(vertices[i], 1.0f);
    }
    // for(int i=0;i<8;i++){
    //   std::cout<<vertices[i].x<<" "<<vertices[i].y<<" "<<vertices[i].z<<std::endl;
    // }
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


bool AxisAlignedBoundingBox::checkCollision(Hitbox* hitbox){
  hitboxType = hitbox->getHitboxType();
  //std::cout<<"testando colisoes"<<std::endl;
  //dependendo do tipo de hitbox, a colisao sera feita de uma forma
  if(hitboxType == HitboxType::AABB){
    return Collisions::AABBsTest(this->getHitboxMin(), this->getHitboxMax(), hitbox->getHitboxMin(), hitbox->getHitboxMax());
  }
  else{
    throw std::invalid_argument("Hitbox type not supported");
  }
};

OrientedBoundingBox::OrientedBoundingBox(tinyobj::attrib_t& attrib){
  calculateHitbox(attrib);
  this->hitboxType = HitboxType::OBB;
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

void OrientedBoundingBox::translate(float x, float y, float z){
  glm::vec3 translation(x, y, z);
  hitboxMin += translation;
  hitboxMax += translation;
  center += translation;
}

void OrientedBoundingBox::scale(const glm::vec3& scale) {
    // Ajusta o tamanho do OBB multiplicando o halfSize pelo fator de escala em cada eixo
    halfSize.x *= scale.x;
    halfSize.y *= scale.y;
    halfSize.z *= scale.z;
    center *= scale;
    
}


void OrientedBoundingBox::rotateX(float angle) {
    // Cria a matriz de rotação em torno do eixo X
    glm::mat4 rotationMatrix = Matrices::RotateX(angle);
    //aplica a matriz no hitbox minimo e maximo
    glm::vec4 min(hitboxMin, 1.0f);
    glm::vec4 max(hitboxMax, 1.0f);
    min = rotationMatrix * min;
    max = rotationMatrix * max;
    hitboxMin = glm::vec3(min);
    hitboxMax = glm::vec3(max);
    center = (hitboxMin + hitboxMax) / 2.0f;

    // Rotaciona cada eixo do OBB
    for (int i = 0; i < 3; ++i) {
        std::cout<<"axis antes de rotacionar"<<std::endl;
        std::cout<<axis[i].x<<" "<<axis[i].y<<" "<<axis[i].z<<std::endl;
        axis[i] = glm::vec3(rotationMatrix * glm::vec4(axis[i], 1.0f));
        std::cout<<"axis depois de rotacionar"<<std::endl;
        std::cout<<axis[i].x<<" "<<axis[i].y<<" "<<axis[i].z<<std::endl;
    }
}

void OrientedBoundingBox::rotateY(float angle){
  glm::mat4 rotation_matrix = Matrices::RotateY(angle);

  for (int i = 0; i < 3; ++i) {
    axis[i] = glm::vec3(rotation_matrix * glm::vec4(axis[i], 0.0f));
  }
}

void OrientedBoundingBox::rotateZ(float angle){
  glm::mat4 rotation_matrix = Matrices::RotateZ(angle);

  for (int i = 0; i < 3; ++i) {
    axis[i] = glm::vec3(rotation_matrix * glm::vec4(axis[i], 0.0f));
  }
}

void OrientedBoundingBox::UpdateHitbox(glm::mat4 model_matrix){
    //parte para atualizar os vertices (para desenhar a hitbox)
    std::cout<<"chamando update hitbox"<<std::endl;
    std::cout<<"Model matrix no Update Hitbox: "<<std::endl;
    for(int i=0;i<4;i++){
      for(int j=0;j<4;j++){
        std::cout<<model_matrix[i][j]<<" ";
      }
      std::cout<<std::endl;
    }
    resetVertices();
    for(int i=0; i<8; i++){
        vertices[i] = model_matrix * glm::vec4(vertices[i], 1.0f);
    }


    std::cout<<"Axis antes de atualizar"<<std::endl;
    for(int i=0;i<3;i++){
      std::cout<<axis[i].x<<" "<<axis[i].y<<" "<<axis[i].z<<std::endl;
    }

    glm::vec3 hitboxMax=this->getHitboxMax();  
    glm::vec3 hitboxMin=this->getHitboxMin(); 
    halfSize = (hitboxMax - hitboxMin) / 2.0f;
    center = (hitboxMin + hitboxMax) / 2.0f;

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
void OrientedBoundingBox::draw(GpuProgramController& gpuProgramController) {
    // Calcula os 8 vértices do OBB com base no centro, eixos e halfSize
    glm::vec3 vertices[8];
    vertices[0] = center + axis[0] * halfSize.x + axis[1] * halfSize.y + axis[2] * halfSize.z;
    vertices[1] = center - axis[0] * halfSize.x + axis[1] * halfSize.y + axis[2] * halfSize.z;
    vertices[2] = center + axis[0] * halfSize.x - axis[1] * halfSize.y + axis[2] * halfSize.z;
    vertices[3] = center - axis[0] * halfSize.x - axis[1] * halfSize.y + axis[2] * halfSize.z;
    vertices[4] = center + axis[0] * halfSize.x + axis[1] * halfSize.y - axis[2] * halfSize.z;
    vertices[5] = center - axis[0] * halfSize.x + axis[1] * halfSize.y - axis[2] * halfSize.z;
    vertices[6] = center + axis[0] * halfSize.x - axis[1] * halfSize.y - axis[2] * halfSize.z;
    vertices[7] = center - axis[0] * halfSize.x - axis[1] * halfSize.y - axis[2] * halfSize.z;

    // Define os índices para as linhas que formam as arestas do OBB
    GLuint indices[] = {
        0, 1, 1, 3, 3, 2, 2, 0, // Top face
        4, 5, 5, 7, 7, 6, 6, 4, // Bottom face
        0, 4, 1, 5, 2, 6, 3, 7  // Connections between top and bottom
    };

    // Buffer para os vértices e índices
    GLuint vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    // Configuração do VAO
    glBindVertexArray(vao);

    // Envia os vértices para a GPU
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Envia os índices para a GPU
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Configura os atributos de vértice (posição)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    // Define a matriz de modelo do OBB (neste caso, identidade pois os vértices já estão transformados)
    glm::mat4 model = glm::mat4(1.0f);
    gpuProgramController.DrawObjectHitbox(vao, model, -1, glm::vec3(1.0f, 0.0f, 0.0f), vertices[0], vertices[7]);

    // Desenha as linhas da caixa
    glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);

    // Limpeza
    glBindVertexArray(0);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &vao);
}

glm::vec3 OrientedBoundingBox::getCenter(){
  return center;
}

glm::vec3 OrientedBoundingBox::getHalfSize(){
  return halfSize;
}

glm::vec3* OrientedBoundingBox::getAxis(){
  return axis;
}

bool OrientedBoundingBox::checkCollision(Hitbox* hitbox){
  HitboxType hitboxType = hitbox->getHitboxType();
  if(hitboxType == HitboxType::OBB){
    // std::cout<<"testando colisao entre OBBs"<<std::endl;
    // this->printVertices();
    // std::cout<<"-------------------"<<std::endl;  
    // hitbox->printVertices();
    /*return Collisions::OBBsTest(center, halfSize, axis, ((OrientedBoundingBox*)hitbox)->getCenter(), ((OrientedBoundingBox*)hitbox)->getHalfSize(), ((OrientedBoundingBox*)hitbox)->getAxis());*/
    return Collisions::OBBsTest(center, halfSize, axis, ((OrientedBoundingBox*)hitbox)->getCenter(), ((OrientedBoundingBox*)hitbox)->getHalfSize(), ((OrientedBoundingBox*)hitbox)->getAxis());
  }
  else {
    throw std::invalid_argument("Hitbox type not supported");
  }

}

// --------------------------------------------

SphereHitbox::SphereHitbox(tinyobj::attrib_t& attrib){
  calculateHitbox(attrib);
  this->hitboxType = HitboxType::SPHERE;
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

void SphereHitbox::calculateHitbox(tinyobj::attrib_t& attrib){
    if(attrib.vertices.empty()){
        std::cout<<"vertices vazios"<<std::endl;
        return;
    }
    glm::dvec3 hitboxMinD(attrib.vertices[0], attrib.vertices[1], attrib.vertices[2]);
    glm::dvec3 hitboxMaxD = hitboxMinD;
    for (size_t i = 0; i < attrib.vertices.size(); i += 3) {
        glm::dvec3 vertex(attrib.vertices[i], attrib.vertices[i + 1], attrib.vertices[i + 2]);
        hitboxMinD = glm::min(hitboxMinD, vertex);
        hitboxMaxD = glm::max(hitboxMaxD, vertex);
    }
    glm::dvec3 centerD = (hitboxMinD + hitboxMaxD) / 2.0;
    // Calcule o raio como a maior distância de qualquer vértice ao centro
    double radiusD = 0.0;
    for (size_t i = 0; i < attrib.vertices.size(); i += 3) {
      glm::dvec3 vertex(attrib.vertices[i], attrib.vertices[i + 1], attrib.vertices[i + 2]);
      double distanceToCenter = glm::distance(centerD, vertex);
      radiusD = glm::max(radiusD, distanceToCenter);
    }

    // Convert back to single precision
    hitboxMin = glm::vec3(hitboxMinD);
    hitboxMax = glm::vec3(hitboxMaxD);
    center = glm::vec3(centerD);
    radius = static_cast<float>(radiusD);
}

void SphereHitbox::translate(float x, float y, float z){
    center += glm::vec3(x, y, z);
}

void SphereHitbox::scale(const glm::vec3& scale){
    radius *= glm::max(scale.x, glm::max(scale.y, scale.z));
}

void SphereHitbox::rotateX(float angle){
    // Não precisa fazer nada
}

void SphereHitbox::rotateY(float angle){
    // Não precisa fazer nada
}

void SphereHitbox::rotateZ(float angle){
    // Não precisa fazer nada
}


void SphereHitbox::UpdateHitbox(glm::mat4 model_matrix){
    std::cout<<"Center antes da mudanca: "<<center.x<<" "<<center.y<<" "<<center.z<<std::endl;
    center = model_matrix * glm::vec4(center, 1.0f);
    std::cout<<"Center depois da mudanca: "<<center.x<<" "<<center.y<<" "<<center.z<<std::endl;
    radius = 0.0f;
    for(int i=0;i<8;i++){
        float distanceToCenter = glm::distance(center, vertices[i]);
        radius = glm::max(radius, distanceToCenter);
    }
    calculateVertices();
}


void SphereHitbox::resetVertices(){
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

void SphereHitbox::calculateVertices(){
  const int stacks = 10;
  const int slices = 10;

  this->vertices.clear();
  for (int i = 0; i <= stacks; i++) {
      float V = i / (float)stacks;
      float phi = V * glm::pi<float>();

      for (int j = 0; j <= slices; j++) {
          float U = j / (float)slices;
          float theta = U * (glm::pi<float>() * 2);

          float X = cos(theta) * sin(phi);
          float Y = cos(phi);
          float Z = sin(theta) * sin(phi);

          glm::vec3 final_vertex=glm::vec3(X, Y, Z) * radius + center;
          vertices.push_back(final_vertex);
      }
  }

}

void SphereHitbox::draw(GpuProgramController& gpuProgramController){
    const int stacks = 10;
    const int slices = 10;

    /*std::vector<glm::vec3> vertices;*/
    std::vector<GLuint> indices;

    /*for (int i = 0; i <= stacks; i++) {*/
    /*    float V = i / (float)stacks;*/
    /*    float phi = V * glm::pi<float>();*/
    /**/
    /*    for (int j = 0; j <= slices; j++) {*/
    /*        float U = j / (float)slices;*/
    /*        float theta = U * (glm::pi<float>() * 2);*/
    /**/
    /*        float X = cos(theta) * sin(phi);*/
    /*        float Y = cos(phi);*/
    /*        float Z = sin(theta) * sin(phi);*/
    /**/
    /*        glm::vec3 final_vertex=glm::vec3(X, Y, Z) * radius + center;*/
    /*        vertices.push_back(final_vertex);*/
    /*    }*/
    /*}*/
    calculateVertices();

    for (int i = 0; i < slices * stacks + slices; i++) {
        indices.push_back(i);
        indices.push_back(i + slices + 1);
        indices.push_back(i + slices);

        indices.push_back(i + slices + 1);
        indices.push_back(i);
        indices.push_back(i + 1);
    }

    GLuint VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glUniformMatrix4fv(gpuProgramController.GetModelUniform(), 1, GL_FALSE, glm::value_ptr(Matrices::Identity())); //resetando o model matrix para desenhar o hitbox

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glBindVertexArray(VAO);
    glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

bool SphereHitbox::checkCollision(Hitbox* hitbox){
  HitboxType hitboxType = hitbox->getHitboxType();
  if(hitboxType == HitboxType::SPHERE){
    return Collisions::SpheresTest(center, radius, ((SphereHitbox*)hitbox)->center, ((SphereHitbox*)hitbox)->radius);
  }
  else {
    throw std::invalid_argument("Hitbox type not supported");
  }
}
