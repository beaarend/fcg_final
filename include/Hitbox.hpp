#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <tiny_obj_loader.h>
#include <gpuProgramController.h>

enum class HitboxType{
  AABB,
  OBB,
  SPHERE
};

//classe base para as hitboxes
class Hitbox{
protected:
  //servem para desenhar a hitbox
  HitboxType hitboxType;
  std::vector<glm::vec3> vertices;

public:
  virtual ~Hitbox() = default;
  virtual void calculateHitbox(tinyobj::attrib_t& attrib) = 0;
  virtual void UpdateHitbox(glm::mat4 model_matrix) = 0;
  virtual void draw(GpuProgramController& gpuProgramController) = 0;
  glm::vec3* getVertices();
  HitboxType getHitboxType(); 
  virtual bool checkCollision(Hitbox* hitbox) = 0;
  virtual void translate(float x, float y, float z) = 0;
  virtual void scale(const glm::vec3& scale) = 0;
  virtual void rotateX(float angle) = 0;
  virtual void rotateY(float angle) = 0;
  virtual void rotateZ(float angle) = 0;
  void printVertices();
};

class AxisAlignedBoundingBox: public Hitbox{
private:
  glm::vec3 min;
  glm::vec3 max;
public:
  AxisAlignedBoundingBox(tinyobj::attrib_t& attrib);
  void calculateHitbox(tinyobj::attrib_t& attrib) override;
  void UpdateHitbox(glm::mat4 model_matrix) override;
  void translate(float x, float y, float z) override;
  void scale(const glm::vec3& scale) override;
  void rotateX(float angle) override;
  void rotateY(float angle) override;
  void rotateZ(float angle) override;
  void resetVertices() ;
  void draw(GpuProgramController& gpuProgramController) override;
  bool checkCollision(Hitbox* hitbox) override;
  glm::vec3 getMin();
  glm::vec3 getMax();
};

class OrientedBoundingBox: public Hitbox{
private:
  glm::vec3 center;//ponto central do OOBB
  glm::vec3 halfSize;//metade do tamanho do OOBB
  glm::vec3 axis[3];//eixos do OOBB
  glm::vec3 min;
  glm::vec3 max;
public:
  OrientedBoundingBox(tinyobj::attrib_t& attrib);
  void calculateHitbox(tinyobj::attrib_t& attrib) override;
  void UpdateHitbox(glm::mat4 model_matrix) override;
  void draw(GpuProgramController& gpuProgramController) override;
  void resetVertices() ;
  void translate(float x, float y, float z) override;
  void scale(const glm::vec3& scale) override;
  void rotateX(float angle) override;
  void rotateY(float angle) override;
  void rotateZ(float angle) override;
  bool checkCollision(Hitbox* hitbox) override;
  glm::vec3 getCenter();
  glm::vec3 getHalfSize();
  glm::vec3* getAxis();
  glm::vec3 getMin();
  glm::vec3 getMax();

};


class SphereHitbox: public Hitbox{
private:
  float radius;
  glm::vec3 center;
public:
  SphereHitbox(tinyobj::attrib_t& attrib);
  void calculateHitbox(tinyobj::attrib_t& attrib) override;
  void calculateVertices();
  void UpdateHitbox(glm::mat4 model_matrix) override;
  void draw(GpuProgramController& gpuProgramController) override;
  bool checkCollision(Hitbox* hitbox) override;
  void translate(float x, float y, float z) override;
  void scale(const glm::vec3& scale) override;
  void rotateX(float angle) override;
  void rotateY(float angle) override;
  void rotateZ(float angle) override;
  float getRadius();
  glm::vec3 getCenter();
};
