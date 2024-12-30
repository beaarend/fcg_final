#ifndef _GPUPROGRAMCONTROLLER_H
#define _GPUPROGRAMCONTROLLER_H

#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

class GpuProgramController
{
private:
    GLint view_uniform;
    GLint projection_uniform;
    GLint model_uniform;
    GLint object_id_uniform;
    GLint object_color_uniform;
    GLint hitbox_min_uniform;
    GLint hitbox_max_uniform;

public:
    GpuProgramController(GLuint gpu_program_id);

    void SendPlayerCameraMatrices(glm::mat4 view, glm::mat4 projection);
    void DrawObject(GLuint vertex_array_object_id, glm::mat4 model,int objectID,glm::vec3 objectColor);

    void DrawObjectHitbox(GLuint vertex_array_object_id ,glm::mat4 model,int objectID,glm::vec3 objectColor, glm::vec3 hitboxMin, glm::vec3 hitboxMax);

    GLint GetModelUniform();
};

#endif
