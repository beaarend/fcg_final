#include "gpuProgramController.h"

GpuProgramController::GpuProgramController(GLuint gpu_program_id)
{
    this->view_uniform = glGetUniformLocation(gpu_program_id, "view");
    this->projection_uniform = glGetUniformLocation(gpu_program_id, "projection");
    this->model_uniform = glGetUniformLocation(gpu_program_id, "model");
    this->object_id_uniform  = glGetUniformLocation(gpu_program_id, "object_id"); // Variável "object_id" em shader_fragment.glsl
    this->object_color_uniform = glGetUniformLocation(gpu_program_id, "objectColor");
}

void GpuProgramController::SendPlayerCameraMatrices(glm::mat4 view, glm::mat4 projection)
{
    glUniformMatrix4fv(view_uniform, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projection_uniform, 1, GL_FALSE, glm::value_ptr(projection));
}

void GpuProgramController::DrawObject(GLuint vertex_array_object_id ,glm::mat4 model,int objectID,glm::vec3 objectColor)
{
    glBindVertexArray(vertex_array_object_id);
    glUniformMatrix4fv(model_uniform, 1, GL_FALSE, glm::value_ptr(model));
    glUniform1i(object_id_uniform, objectID);
    glUniform3f(object_color_uniform, objectColor.x, objectColor.y, objectColor.z); // Red color

}


GLint GpuProgramController::GetModelUniform(){
    return model_uniform;
}

