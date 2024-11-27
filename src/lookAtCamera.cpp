#include "lookAtCamera.h"
#include "matrices.h"

LookAtCamera::LookAtCamera(float screen_ratio, GpuProgramController *gpu_controller)
{
    this->gpu_controller = gpu_controller;

    this->view_angle_theta = 0.0f;
    this->view_angle_phi = 0.0f;
    this->distance = 3.5f;
    this->screen_ratio = screen_ratio;
}

void LookAtCamera::Update(glm::vec4 player_position)
{
    float r = distance;
    float y = r * sin(view_angle_phi);
    float z = r * cos(view_angle_phi) * cos(view_angle_theta);
    float x = r * cos(view_angle_phi) * sin(view_angle_theta);

    glm::vec4 camera_view_vector = glm::vec4(-x, -y, -z, 0.0f);
    glm::vec4 camera_position = player_position - camera_view_vector;
    glm::vec4 camera_up_vector = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
    glm::mat4 view = Matrices::CameraView(camera_position, camera_view_vector, camera_up_vector);

    float nearplane = -0.1f;
    float farplane = -20.0f;
    float field_of_view = 3.141592 / 3.0f;
    glm::mat4 projection = Matrices::Perspective(field_of_view, screen_ratio, nearplane, farplane);

    gpu_controller->SendPlayerCameraMatrices(view, projection);
}