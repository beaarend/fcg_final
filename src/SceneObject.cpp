#include "Hitbox.hpp"
#include "glm/ext/vector_float4.hpp"
#include "gpuProgramController.h"
#include <SceneObject.hpp>
#include <iostream>
#include <glm/glm.hpp>
#include <matrices.h>

struct ShapeRenderData {
    size_t first_index;
    size_t num_indices;
};

std::vector<ShapeRenderData> shape_render_data;

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

        const float minval = std::numeric_limits<float>::min();
        const float maxval = std::numeric_limits<float>::max();

        this->bbox_min = glm::vec3(maxval,maxval,maxval);
        this->bbox_max = glm::vec3(minval,minval,minval);

        for (size_t triangle = 0; triangle < num_triangles; ++triangle)
        {
            assert(shapes[shape].mesh.num_face_vertices[triangle] == 3);

            for (size_t vertex = 0; vertex < 3; ++vertex)
            {
                tinyobj::index_t idx = shapes[shape].mesh.indices[3 * triangle + vertex];
                indices.push_back(indices.size());

                const float vx = attrib.vertices[3 * idx.vertex_index + 0];
                const float vy = attrib.vertices[3 * idx.vertex_index + 1];
                const float vz = attrib.vertices[3 * idx.vertex_index + 2];
                model_coefficients.push_back(vx);
                model_coefficients.push_back(vy);
                model_coefficients.push_back(vz);
                model_coefficients.push_back(1.0f);

                bbox_min.x = std::min(bbox_min.x, vx);
                bbox_min.y = std::min(bbox_min.y, vy);
                bbox_min.z = std::min(bbox_min.z, vz);
                bbox_max.x = std::max(bbox_max.x, vx);
                bbox_max.y = std::max(bbox_max.y, vy);
                bbox_max.z = std::max(bbox_max.z, vz);

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
        shape_render_data.push_back({first_index, last_index - first_index + 1});
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

SceneObject::SceneObject(const tinyobj::attrib_t &attrib, 
                         const tinyobj::shape_t &shape, 
                         const std::vector<tinyobj::material_t> &materials)
{
    this->attrib = attrib;
    this->shapes.push_back(shape); // Only include this shape
    this->materials = materials;

    ComputeNormals();
    initBuffers();
    this->hitbox = new AxisAlignedBoundingBox(this->attrib);
}

SceneObject::SceneObject(const char *filename, const char *flag)
{
    std::cout<<"carregando objeto: "<<filename<<std::endl;
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

    if (std::strcmp(flag, "unique") == 0)
    {
        ComputeNormals();
        initBuffers();
        this->hitbox = new AxisAlignedBoundingBox(attrib);
    }
}


glm::vec3 SceneObject::getHitboxMax() {
    return this->hitbox->getHitboxMax();
}

glm::vec3 SceneObject::getHitboxMin() {
    return this->hitbox->getHitboxMin();
}


void SceneObject::render(GpuProgramController& gpuProgramController)
{
    glBindVertexArray(vertex_array_object_id);

    this->hitboxMax = this->hitbox->getHitboxMax();
    this->hitboxMin = this->hitbox->getHitboxMin();

    for (const auto &shape : shape_render_data)
    {
        //gpuProgramController.DrawObject(vertex_array_object_id, model_matrix, object_id, object_color);
        //gpuProgramController.DrawObjectHitbox(vertex_array_object_id, model_matrix, object_id, object_color, hitboxMin, hitboxMax);
        gpuProgramController.DrawObjectHitbox(vertex_array_object_id, model_matrix, object_id, object_color, this->bbox_min, this->bbox_max);
        glDrawElements(GL_TRIANGLES, shape.num_indices, GL_UNSIGNED_INT, (void *)(shape.first_index * sizeof(GLuint)));
    }
    glBindVertexArray(0);
    this->hitbox->draw(gpuProgramController);
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

bool SceneObject::checkCollision(SceneObject& other) {
      return this->hitbox->checkCollision(other.getHitbox());
}

Hitbox* SceneObject::getHitbox(){
  return this->hitbox;
}

void SceneObject::scale(const glm::vec3& scale) {
    model_matrix = Matrices::Scale(scale.x, scale.y, scale.z) * model_matrix;
    this->hitbox->UpdateHitbox(model_matrix);
}

void SceneObject::translate(float x, float y, float z) {
    model_matrix = Matrices::Translate(x, y, z) * model_matrix;
    this->hitbox->UpdateHitbox(model_matrix);
}

void SceneObject::rotateX(float angle) {
    model_matrix = Matrices::RotateX(angle) * model_matrix; 
    this->hitbox->UpdateHitbox(model_matrix);
}

void SceneObject::rotateY(float angle) {
    model_matrix = Matrices::RotateY(angle) * model_matrix;
    this->hitbox->UpdateHitbox(model_matrix);
}

void SceneObject::rotateZ(float angle) {
  model_matrix = Matrices::RotateZ(angle) * model_matrix;
  this->hitbox->UpdateHitbox(model_matrix);
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

void SceneObject::resetModelMatrix(){
  model_matrix = Matrices::Identity();
  HitboxType hitboxType = this->hitbox->getHitboxType();
  delete this->hitbox;
  if(hitboxType == HitboxType::AABB){
    this->hitbox = new AxisAlignedBoundingBox(attrib);
  }
  else if(hitboxType == HitboxType::OBB){
    this->hitbox = new OrientedBoundingBox(attrib);
  }
}
