#include "glm/ext/vector_float4.hpp"
#include "gpuProgramController.h"
#include <SceneObject.hpp>
#include <iostream>
#include <glm/glm.hpp>
#include <matrices.h>


void SceneObject::ComputeNormals()
{
    if (!attrib.normals.empty())
        return;

    size_t num_vertices = attrib.vertices.size() / 3;

    std::vector<int> num_triangles_per_vertex(num_vertices, 0);
    std::vector<glm::vec4> vertex_normals(num_vertices, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

    for (size_t shape = 0; shape < shapes.size(); ++shape)
    {
        size_t num_triangles = shapes[shape].mesh.num_face_vertices.size();

        for (size_t triangle = 0; triangle < num_triangles; ++triangle)
        {
            assert(shapes[shape].mesh.num_face_vertices[triangle] == 3);

            glm::vec4 vertices[3];
            for (size_t vertex = 0; vertex < 3; ++vertex)
            {
                tinyobj::index_t idx = shapes[shape].mesh.indices[3 * triangle + vertex];
                const float vx = attrib.vertices[3 * idx.vertex_index + 0];
                const float vy = attrib.vertices[3 * idx.vertex_index + 1];
                const float vz = attrib.vertices[3 * idx.vertex_index + 2];
                vertices[vertex] = glm::vec4(vx, vy, vz, 1.0);
            }

            const glm::vec4 a = vertices[0];
            const glm::vec4 b = vertices[1];
            const glm::vec4 c = vertices[2];

            const glm::vec4 n = Matrices::CrossProduct(b - a, c - a);

            for (size_t vertex = 0; vertex < 3; ++vertex)
            {
                tinyobj::index_t idx = shapes[shape].mesh.indices[3 * triangle + vertex];
                num_triangles_per_vertex[idx.vertex_index] += 1;
                vertex_normals[idx.vertex_index] += n;
                shapes[shape].mesh.indices[3 * triangle + vertex].normal_index = idx.vertex_index;
            }
        }
    }

    attrib.normals.resize(3 * num_vertices);

    for (size_t i = 0; i < vertex_normals.size(); ++i)
    {
        glm::vec4 n = vertex_normals[i] / (float)num_triangles_per_vertex[i];
        n /= Matrices::Norm(n);
        attrib.normals[3 * i + 0] = n.x;
        attrib.normals[3 * i + 1] = n.y;
        attrib.normals[3 * i + 2] = n.z;
    }
}

void SceneObject::initBuffers()
{
    glGenVertexArrays(1, &vertex_array_object_id);
    glBindVertexArray(vertex_array_object_id);

    std::vector<GLuint> indices;
    std::vector<float> model_coefficients;
    std::vector<float> normal_coefficients;
    std::vector<float> texture_coefficients;

    for (size_t shape = 0; shape < shapes.size(); ++shape)
    {
        size_t first_index = indices.size();
        size_t num_triangles = shapes[shape].mesh.num_face_vertices.size();


        for (size_t triangle = 0; triangle < num_triangles; ++triangle)
        {
            assert(shapes[shape].mesh.num_face_vertices[triangle] == 3);

            for (size_t vertex = 0; vertex < 3; ++vertex)
            {
                tinyobj::index_t idx = shapes[shape].mesh.indices[3 * triangle + vertex];

                indices.push_back(first_index + 3 * triangle + vertex);

                const float vx = attrib.vertices[3 * idx.vertex_index + 0];
                const float vy = attrib.vertices[3 * idx.vertex_index + 1];
                const float vz = attrib.vertices[3 * idx.vertex_index + 2];

                model_coefficients.push_back(vx);   
                model_coefficients.push_back(vy);   
                model_coefficients.push_back(vz);   
                model_coefficients.push_back(1.0f); 


                if (idx.normal_index != -1)
                {
                    const float nx = attrib.normals[3 * idx.normal_index + 0];
                    const float ny = attrib.normals[3 * idx.normal_index + 1];
                    const float nz = attrib.normals[3 * idx.normal_index + 2];
                    normal_coefficients.push_back(nx);   
                    normal_coefficients.push_back(ny);   
                    normal_coefficients.push_back(nz);   
                    normal_coefficients.push_back(0.0f); 
                }

                if (idx.texcoord_index != -1)
                {
                    const float u = attrib.texcoords[2 * idx.texcoord_index + 0];
                    const float v = attrib.texcoords[2 * idx.texcoord_index + 1];
                    texture_coefficients.push_back(u);
                    texture_coefficients.push_back(v);
                }
            }
        }

        size_t last_index = indices.size() - 1;

        this->first_index = first_index;
        this->num_indices = last_index - first_index + 1;
        this->vertex_array_object_id = vertex_array_object_id;
    }

    GLuint VBO_model_coefficients_id;
    glGenBuffers(1, &VBO_model_coefficients_id);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_model_coefficients_id);
    glBufferData(GL_ARRAY_BUFFER, model_coefficients.size() * sizeof(float), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, model_coefficients.size() * sizeof(float), model_coefficients.data());
    GLuint location = 0;
    GLint number_of_dimensions = 4;
    glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(location);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    if (!normal_coefficients.empty())
    {
        GLuint VBO_normal_coefficients_id;
        glGenBuffers(1, &VBO_normal_coefficients_id);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_normal_coefficients_id);
        glBufferData(GL_ARRAY_BUFFER, normal_coefficients.size() * sizeof(float), NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, normal_coefficients.size() * sizeof(float), normal_coefficients.data());
        location = 1;
        number_of_dimensions = 4;
        glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(location);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    if (!texture_coefficients.empty())
    {
        GLuint VBO_texture_coefficients_id;
        glGenBuffers(1, &VBO_texture_coefficients_id);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_texture_coefficients_id);
        glBufferData(GL_ARRAY_BUFFER, texture_coefficients.size() * sizeof(float), NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, texture_coefficients.size() * sizeof(float), texture_coefficients.data());
        location = 2;
        number_of_dimensions = 2;
        glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(location);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    GLuint indices_id;
    glGenBuffers(1, &indices_id);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(GLuint), indices.data());

    glBindVertexArray(0);
}

SceneObject::SceneObject(const char *filename)
{
    std::string fullpath(filename);
    auto i = fullpath.find_last_of("/");
    const char *basepath = NULL;
    if (i != std::string::npos)
    {
        std::string dirname = fullpath.substr(0, i + 1);
        basepath = dirname.c_str();
    }

    std::string warn;
    std::string err;
    bool ret = tinyobj::LoadObj(&this->attrib, &this->shapes, &this->materials, &warn, &err, filename, basepath, true);

    if (!err.empty())
    {
        fprintf(stderr, "\n%s\n", err.c_str());
        std::exit(EXIT_FAILURE);
    }

    if (!ret)
    {
        fprintf(stderr, "Error loading .obj %s", filename);
        std::exit(EXIT_FAILURE);
    }

    for (size_t shape = 0; shape < shapes.size(); ++shape)
    {
        if (shapes[shape].name.empty())
        {
            fprintf(stderr, "No name at .obj %s", filename);
            std::exit(EXIT_FAILURE);
        }
    }

    ComputeNormals();
    initBuffers();
    calculateHitbox();
}

void SceneObject::render(GpuProgramController& gpuProgramController)
{
    gpuProgramController.DrawObject(vertex_array_object_id, model_matrix,this->object_id,this->object_color);
    glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, (void *)(first_index * sizeof(GLuint)));
    glBindVertexArray(0);
    drawHitbox();
}


bool SceneObject::getPlaneInfo(glm::vec3& planePoint, glm::vec3& planeNormal) {
    if (shapes.empty() || shapes[0].mesh.indices.size() < 3) {
        return false; 
    }

    tinyobj::index_t idx0 = shapes[0].mesh.indices[0];
    tinyobj::index_t idx1 = shapes[0].mesh.indices[1];
    tinyobj::index_t idx2 = shapes[0].mesh.indices[2];

    glm::vec3 v0(attrib.vertices[3 * idx0.vertex_index + 0], attrib.vertices[3 * idx0.vertex_index + 1], attrib.vertices[3 * idx0.vertex_index + 2]);
    glm::vec3 v1(attrib.vertices[3 * idx1.vertex_index + 0], attrib.vertices[3 * idx1.vertex_index + 1], attrib.vertices[3 * idx1.vertex_index + 2]);
    glm::vec3 v2(attrib.vertices[3 * idx2.vertex_index + 0], attrib.vertices[3 * idx2.vertex_index + 1], attrib.vertices[3 * idx2.vertex_index + 2]);

    glm::vec3 edge1 = v1 - v0;
    glm::vec3 edge2 = v2 - v0;
    planeNormal = glm::normalize(glm::cross(edge1, edge2));

    planePoint = v0;

    return true;
}

void SceneObject::calculateHitbox(){
  if(attrib.vertices.empty()){
    return;
  }

  hitboxMin = glm::vec3(attrib.vertices[0], attrib.vertices[1], attrib.vertices[2]);
  hitboxMax = hitboxMin;
  for (size_t i = 0; i < attrib.vertices.size(); i += 3) {
    glm::vec3 vertex(attrib.vertices[i], attrib.vertices[i + 1], attrib.vertices[i + 2]);
    hitboxMin = glm::min(hitboxMin, vertex);
    hitboxMax = glm::max(hitboxMax, vertex);
  }
  /*std::cout<<"Hitbox min: "<<hitboxMin.x<<" "<<hitboxMin.y<<" "<<hitboxMin.z<<" Hitbox max: "<<hitboxMax.x<<" "<<hitboxMax.y<<" "<<hitboxMax.z<<std::endl;*/
}

void SceneObject::UpdateHitbox() {
    glm::vec3 transformedMin = glm::vec3(model_matrix * glm::vec4(hitboxMin, 1.0f));
    glm::vec3 transformedMax = glm::vec3(model_matrix * glm::vec4(hitboxMax, 1.0f));

    hitboxMin = glm::min(transformedMin, transformedMax);
    hitboxMax = glm::max(transformedMin, transformedMax);
}

glm::vec3 SceneObject::getHitboxMin(){
  return hitboxMin;
}

glm::vec3 SceneObject::getHitboxMax(){
  return hitboxMax;
}

bool SceneObject::checkCollision( SceneObject& other) {
     glm::vec4 hitBoxMin= glm::vec4(this->getHitboxMin(),1.0f); 
     glm::vec4 hitBoxMax= glm::vec4(this->getHitboxMax(),1.0f);
    
     glm::vec4 otherHitBoxMin= glm::vec4(other.getHitboxMin(),1.0f);
     glm::vec4 otherHitBoxMax= glm::vec4(other.getHitboxMax(),1.0f);
      
    if(hitBoxMin.x > hitBoxMax.x){
      std::swap(hitBoxMin.x, hitBoxMax.x);
    }
    if(hitBoxMin.y > hitBoxMax.y){
      std::swap(hitBoxMin.y, hitBoxMax.y);
    }
    if(hitBoxMin.z > hitBoxMax.z){
      std::swap(hitBoxMin.z, hitBoxMax.z);
    }
    
    if(otherHitBoxMin.x > otherHitBoxMax.x){
      std::swap(otherHitBoxMin.x, otherHitBoxMax.x);
    }
    if(otherHitBoxMin.y > otherHitBoxMax.y){
      std::swap(otherHitBoxMin.y, otherHitBoxMax.y);
    }
    if(otherHitBoxMin.z > otherHitBoxMax.z){
      std::swap(otherHitBoxMin.z, otherHitBoxMax.z);
    }
  
    /*
     1.5228 > 5 || 
      */
    std::cout<<"Hitbox min: "<<hitBoxMin.x<<" "<<hitBoxMin.y<<" "<<hitBoxMin.z<<" Hitbox max: "<<hitBoxMax.x<<" "<<hitBoxMax.y<<" "<<hitBoxMax.z<<" Position: "<<this->model_matrix[3][0]<<" "<<this->model_matrix[3][1]<<" "<<this->model_matrix[3][2]<<std::endl;
    
    std::cout<<"Other Hitbox min: "<<otherHitBoxMin.x<<" "<<otherHitBoxMin.y<<" "<<otherHitBoxMin.z<<" Other Hitbox max: "<<otherHitBoxMax.x<<" "<<otherHitBoxMax.y<<" "<<otherHitBoxMax.z<<" Position: "<<other.model_matrix[3][0]<<" "<<other.model_matrix[3][1]<<" "<<other.model_matrix[3][2]<<std::endl;

  return hitBoxMin.x <= otherHitBoxMax.x && hitBoxMax.x >= otherHitBoxMin.x &&
         hitBoxMin.y <= otherHitBoxMax.y && hitBoxMax.y >= otherHitBoxMin.y &&
         hitBoxMin.z <= otherHitBoxMax.z && hitBoxMax.z >= otherHitBoxMin.z;

}

void SceneObject::scale(const glm::vec3& scale) {
    /*model_matrix = Matrices::Scale(scale.x, scale.y, scale.z) * model_matrix;*/
    /*std::cout<<"hitbox max antes: "<<hitboxMax.x<<" "<<hitboxMax.y<<" "<<hitboxMax.z<<std::endl;*/
    hitboxMax = Matrices::Scale(scale.x, scale.y, scale.z) * glm::vec4(hitboxMax, 1.0f);
    hitboxMin = Matrices::Scale(scale.x, scale.y, scale.z) * glm::vec4(hitboxMin, 1.0f);
    /*std::cout<<"hitbox max depois: "<<hitboxMax.x<<" "<<hitboxMax.y<<" "<<hitboxMax.z<<std::endl;*/
}

void SceneObject::translate(float x, float y, float z) {
    model_matrix = Matrices::Translate(x, y, z) * model_matrix;
    hitboxMax = Matrices::Translate(x, y, z) * glm::vec4(hitboxMax, 1.0f);
    hitboxMin = Matrices::Translate(x, y, z) * glm::vec4(hitboxMin, 1.0f); 
}

void SceneObject::rotateX(float angle) {
    model_matrix = Matrices::RotateX(angle) * model_matrix;
    hitboxMax = Matrices::RotateX(angle) * glm::vec4(hitboxMax, 1.0f);
    hitboxMin = Matrices::RotateX(angle) * glm::vec4(hitboxMin, 1.0f);
}

void SceneObject::rotateY(float angle) {
    model_matrix = Matrices::RotateY(angle) * model_matrix;
    hitboxMax = Matrices::RotateY(angle) * glm::vec4(hitboxMax, 1.0f);
    hitboxMin = Matrices::RotateY(angle) * glm::vec4(hitboxMin, 1.0f);
}

void SceneObject::rotateZ(float angle) {
  model_matrix = Matrices::RotateZ(angle) * model_matrix;
  hitboxMax = Matrices::RotateZ(angle) * glm::vec4(hitboxMax, 1.0f);
  hitboxMin = Matrices::RotateZ(angle) * glm::vec4(hitboxMin, 1.0f);
}

void SceneObject::setObjectID(int object_id){
  this->object_id = object_id;
}
void SceneObject::setModelMatrix(glm::mat4 model_matrix){
  this->model_matrix = model_matrix;
}
void SceneObject::setObjectColor(glm::vec3 object_color){
  this->object_color = object_color;
}
void SceneObject::drawHitbox() {
  /*calculateHitbox();*/
  std::cout<<"Drawing hitbox"<<"Hitbox min: "<<hitboxMin.x<<" "<<hitboxMin.y<<" "<<hitboxMin.z<<" Hitbox max: "<<hitboxMax.x<<" "<<hitboxMax.y<<" "<<hitboxMax.z<<std::endl;
    glm::vec3 vertices[8] = {
        hitboxMin,
        glm::vec3(hitboxMax.x, hitboxMin.y, hitboxMin.z),
        glm::vec3(hitboxMax.x, hitboxMax.y, hitboxMin.z),
        glm::vec3(hitboxMin.x, hitboxMax.y, hitboxMin.z),
        glm::vec3(hitboxMin.x, hitboxMin.y, hitboxMax.z),
        glm::vec3(hitboxMax.x, hitboxMin.y, hitboxMax.z),
        hitboxMax,
        glm::vec3(hitboxMin.x, hitboxMax.y, hitboxMax.z)
    };

    GLuint indices[24] = {
        0, 1, 1, 2, 2, 3, 3, 0,
        4, 5, 5, 6, 6, 7, 7, 4,
        0, 4, 1, 5, 2, 6, 3, 7
    };

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glBindVertexArray(VAO);
    glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void SceneObject::resetModelMatrix(){
  model_matrix = Matrices::Identity();
  calculateHitbox();
}
