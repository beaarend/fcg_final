#include "gpuProgramController.h"

GpuProgramController::GpuProgramController(GLuint gpu_program_id)
{
    this->view_uniform = glGetUniformLocation(gpu_program_id, "view");
    this->projection_uniform = glGetUniformLocation(gpu_program_id, "projection");
    this->model_uniform = glGetUniformLocation(gpu_program_id, "model");
    this->object_id_uniform  = glGetUniformLocation(gpu_program_id, "object_id"); // Variável "object_id" em shader_fragment.glsl
    this->object_color_uniform = glGetUniformLocation(gpu_program_id, "objectColor");

    /*this->hitbox_min_uniform   = glGetUniformLocation(gpu_program_id, "bbox_min");*/
    /*this->hitbox_max_uniform   = glGetUniformLocation(gpu_program_id, "bbox_max");*/

    this->bbox_min_uniform   = glGetUniformLocation(gpu_program_id,  "bbox_min");
    this->bbox_max_uniform   = glGetUniformLocation(gpu_program_id,  "bbox_max");

    glUseProgram(gpu_program_id);
    glUniform1i(glGetUniformLocation(gpu_program_id, "TextureImage0"), 0); // FAUSTAO FACE
    glUniform1i(glGetUniformLocation(gpu_program_id, "TextureImage1"), 1); // FAUSTAO HAIR
    glUniform1i(glGetUniformLocation(gpu_program_id, "TextureImage2"), 2); // FAUSTAO CLOTHES
    glUniform1i(glGetUniformLocation(gpu_program_id, "TextureImage3"), 3); // RAMPAAAAAAAAA
    glUseProgram(0);
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

void GpuProgramController::DrawObjectHitbox(GLuint vertex_array_object_id ,glm::mat4 model,int objectID,glm::vec3 objectColor, glm::vec3 hitboxMin, glm::vec3 hitboxMax)
{
    glBindVertexArray(vertex_array_object_id);
    glUniformMatrix4fv(model_uniform, 1, GL_FALSE, glm::value_ptr(model));
    glUniform1i(object_id_uniform, objectID);
    glUniform3f(object_color_uniform, objectColor.x, objectColor.y, objectColor.z); // Red color

     /*std::cout << "hitboxMin: " << hitboxMin.x << " " << hitboxMin.y << " " << hitboxMin.z << std::endl;*/
     /*std::cout << "hitboxMax: " << hitboxMax.x << " " << hitboxMax.y << " " << hitboxMax.z << std::endl;*/

    /*glUniform4f(hitbox_min_uniform, hitboxMin.x, hitboxMin.y, hitboxMin.z, 1.0f);*/
    /*glUniform4f(hitbox_max_uniform, hitboxMax.x, hitboxMax.y, hitboxMax.z, 1.0f);*/
}


GLint GpuProgramController::GetModelUniform(){
    return model_uniform;
}

