#include "gpuProgramController.h"

GpuProgramController::GpuProgramController(GLuint gpu_program_id)
{
    this->view_uniform = glGetUniformLocation(gpu_program_id, "view");
    this->projection_uniform = glGetUniformLocation(gpu_program_id, "projection");
}

void GpuProgramController::SendPlayerCameraMatrices(glm::mat4 view, glm::mat4 projection)
{
    glUniformMatrix4fv(view_uniform, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projection_uniform, 1, GL_FALSE, glm::value_ptr(projection));
}