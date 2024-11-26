#ifndef _GPUPROGRAMCONTROLLER_H
#define _GPUPROGRAMCONTROLLER_H

#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

class GpuProgramController
{
private:
    GLint view_uniform;
    GLint projection_uniform;

public:
    GpuProgramController(GLuint gpu_program_id);

    void SendPlayerCameraMatrices(glm::mat4 view, glm::mat4 projection);
};

#endif