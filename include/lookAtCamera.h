#ifndef _LOOKATCAMERA_H
#define _LOOKATCAMERA_H

#include "gpuProgramController.h"

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

class LookAtCamera
{
private:
    GpuProgramController *gpu_controller;

public:
    float view_angle_phi;   // angle between zx plane and z axis
    float view_angle_theta; // angle to the Y axis
    float distance;         // distance to lookAt point
    float screen_ratio;     

    glm::mat4 view;
    glm::mat4 projection;

    LookAtCamera(float screen_ratio, GpuProgramController *gpu_controller);
    void Update(glm::vec4 player_position);
};

#endif 