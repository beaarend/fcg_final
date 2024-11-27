#include "Camera.hpp"
#include<iostream>
#include "glm/ext/matrix_float4x4.hpp"
#include "matrices.h"

Camera::Camera(glm::vec4 position_c, glm::vec4 view_vector, glm::vec4 up_vector):sphericalCoordinates(convert_cartesian_to_spherical(position_c)){
  std::cout<<"Camera created"<<std::endl;
  set_matrix_view(position_c, view_vector, up_vector);

};

Camera::Camera(SphericalCoordinates sphericalCoordinates, glm::vec4 view_vector, glm::vec4 up_vector):sphericalCoordinates(sphericalCoordinates){
  std::cout<<"Camera created with spherical coordinates"<<std::endl;
  this->sphericalCoordinates = sphericalCoordinates;
  this->view_vector = view_vector;
  this->up_vector = up_vector;
  this->position_c = convert_spherical_to_cartesian(sphericalCoordinates);
  set_matrix_view(position_c, view_vector, up_vector);
};

Camera::Camera(glm::vec4 position_c):sphericalCoordinates(convert_cartesian_to_spherical(position_c)){
  std::cout<<"Camera created with position"<<std::endl;
  this->position_c=position_c;
}

void Camera::set_matrix_view(){
  set_matrix_view(position_c, view_vector, up_vector); //seta a matriz de view com os vetores da camera
}

 void Camera:: set_matrix_view(glm::vec4 position_c, glm::vec4 view_vector, glm::vec4 up_vector)
{
    /*std::cout<<"normal"<<std::endl;*/
    /*PrintVector(position_c);*/
    /*PrintVector(view_vector);*/
    /*PrintVector(up_vector);*/

    glm::vec4 w = -view_vector;
    glm::vec4 u = Matrices::CrossProduct(up_vector, w);

    // Normalizamos os vetores u e w
    w = w / Matrices::Norm(w);
    u = u / Matrices::Norm(u);

    /*PrintVector(w);*/
    /*PrintVector(u);*/

    glm::vec4 v = Matrices::CrossProduct(w, u);

    glm::vec4 origin_o = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 vector_c = position_c - origin_o;

    float ux = u.x;
    float uy = u.y;
    float uz = u.z;
    float vx = v.x;
    float vy = v.y;
    float vz = v.z;
    float wx = w.x;
    float wy = w.y;
    float wz = w.z;

    this->camera_view_matrix = Matrices::New(
        ux  , uy  , uz  , Matrices::DotProduct(-u, vector_c),  // LINHA 1
        vx  , vy  , vz  , Matrices::DotProduct(-v, vector_c),  // LINHA 2
        wx  , wy  , wz  , Matrices::DotProduct(-w, vector_c),  // LINHA 3
        0.0f, 0.0f, 0.0f, 1.0f                       // LINHA 4
  );

}


void Camera::set_position_c(glm::vec4 position_c){
  this->position_c = position_c;
  this->sphericalCoordinates = convert_cartesian_to_spherical(position_c);
  /*set_matrix_view(position_c, view_vector, up_vector);*/
};
void Camera::look_at(glm::vec4 target_point){
  view_vector = target_point - position_c;
  this->l_point=target_point;
  /*set_matrix_view(position_c, view_vector, up_vector);*/
};

void Camera::set_up_vector(glm::vec4 up_vector){
  this->up_vector = up_vector;
  /*set_matrix_view(position_c, view_vector, up_vector);*/
};

glm::mat4 Camera::get_matrix_view(){
  return camera_view_matrix;
};

glm::vec4 Camera::get_position_c(){
  return position_c;
};

glm::vec4 Camera::get_view_vector(){
  return view_vector;
};

glm::vec4 Camera::get_up_vector(){
  return up_vector;
};

SphericalCoordinates Camera::get_spherical_coordinates(){
  return this->sphericalCoordinates;
};


void Camera::set_spherical_coordinates(float theta,float phi, float distance){
  this->sphericalCoordinates.Phi=phi;
  this->sphericalCoordinates.Theta=theta;
  this->sphericalCoordinates.Distance=distance;
  /*std::cout<<"spherical coordinates"<<std::endl;*/
  glm::vec4 new_pos=convert_spherical_to_cartesian(this->sphericalCoordinates); //da update na posicao da camera quando muda as coordenadas esfericas
  this->position_c=new_pos;
};

glm::vec4 Camera::convert_spherical_to_cartesian(SphericalCoordinates sphericalCoordinates){
  float x = sphericalCoordinates.Distance * cos(sphericalCoordinates.Phi) * sin(sphericalCoordinates.Theta);
  float y = sphericalCoordinates.Distance * sin(sphericalCoordinates.Phi); 
  float z = sphericalCoordinates.Distance * cos(sphericalCoordinates.Phi)*cos(sphericalCoordinates.Theta);
  glm::vec4 position_c = glm::vec4(x,y,z,1.0f);
  return position_c;
};

SphericalCoordinates Camera::convert_cartesian_to_spherical(glm::vec4 position_c){
  float x = position_c.x;
  float y = position_c.y;
  float z = position_c.z;
  float distance = sqrt(x*x + y*y + z*z);
  float phi = acos(z/distance);
  float theta = atan2(y,x);
  SphericalCoordinates sphericalCoordinates = {theta, phi, distance};
  return sphericalCoordinates;
};

void Camera::update(){
  look_at(this->l_point);
  set_matrix_view();
};
