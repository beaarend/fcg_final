#ifndef _FREECAMERA_H
#define _FREECAMERA_H

#include "gpuProgramController.h"

// Matrices
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>
// OpenGl
#include <glad/glad.h>

class FreeCamera
{
private:
    GpuProgramController *gpu_controller;
    const float MIN_FIELD_OF_VIEW = 3.141592 / 3.0f; // 60º
    const float MAX_FIELD_OF_VIEW = 3.141592 / 2.5f; // 72º
    float field_of_view;

public:
    float view_angle_phi;   // angle between ZX plane and Z axis
    float view_angle_theta; // angle with respect to the Y axis
    float screen_ratio;     // user window width by height ratio
    glm::vec4 up_vector;
    glm::vec4 view_vector;

    glm::mat4 view;
    glm::mat4 projection;

    FreeCamera(float screen_ratio, GpuProgramController *gpu_controller);
    void Update(glm::vec4 player_position, float delta_run_velocity/*, glm::vec4 head_movement*/);
};

#endif 

// _FREECAMERA_H