#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

struct SphericalCoordinates {
  float Theta;
  float Phi;
  float Distance; // distância da câmera até a origem ( r nas coordenadas esféricas )
  
  SphericalCoordinates(float theta, float phi, float distance){
    Theta=theta;
    Phi=phi;
    Distance=distance;
  };
};

class Camera {
public:
  Camera(glm::vec4 position_c, glm::vec4 view_vector, glm::vec4 up_vector);
  Camera(glm::vec4 position_c);
  Camera(SphericalCoordinates sphericalCoordinates, glm::vec4 view_vector, glm::vec4 up_vector);
  glm::mat4 camera_view_matrix;
  SphericalCoordinates sphericalCoordinates;
  void update();

  void set_position_c(glm::vec4 position_c);
  void look_at(glm::vec4 target_point);
  void set_up_vector(glm::vec4 up_vector);
  void set_spherical_coordinates(float theta, float phi, float distance);

  glm::mat4 get_matrix_view();
  glm::vec4 get_position_c();
  glm::vec4 get_view_vector();
  glm::vec4 get_up_vector();
  SphericalCoordinates get_spherical_coordinates();
  void set_matrix_view();
private:
  glm::vec4 l_point;
  glm::vec4 position_c;
  glm::vec4 view_vector;
  glm::vec4 up_vector;
  void set_matrix_view(glm::vec4 position_c, glm::vec4 view_vector, glm::vec4 up_vector);
  glm::vec4 convert_spherical_to_cartesian(SphericalCoordinates sphericalCoordinates);
  SphericalCoordinates convert_cartesian_to_spherical(glm::vec4 position_c);
};
