#ifndef _MATRICES_H
#define _MATRICES_H

#include <cstdio>
#include <cstdlib>
#include <cmath>

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Matrices
{
public:
    static glm::mat4 New(
        float m00, float m01, float m02, float m03, // line 1
        float m10, float m11, float m12, float m13, // line 2
        float m20, float m21, float m22, float m23, // line 3
        float m30, float m31, float m32, float m33  // line 4
    );
    static glm::mat4 Identity();
    static glm::mat4 Translate(float tx, float ty, float tz);
    static glm::mat4 Scale(float sx, float sy, float sz);
    static glm::mat4 RotateX(float angle);
    static glm::mat4 RotateY(float angle);
    static glm::mat4 RotateZ(float angle);
    static float Norm(glm::vec4 v);
    static glm::vec4 Vectorize(glm::vec4 point);
    static bool IsVectorNull(glm::vec4 vector);
    static glm::vec4 Normalize(glm::vec4 vector);
    static glm::mat4 Rotate(float angle, glm::vec4 axis);
    static glm::mat4 FaceDirection(glm::vec4 source_direction, glm::vec4 target_direction);
    static glm::vec4 CrossProduct(glm::vec4 u, glm::vec4 v);
    static float DotProduct(glm::vec4 u, glm::vec4 v);
    static glm::mat4 CameraView(glm::vec4 position_c, glm::vec4 view_vector, glm::vec4 up_vector);
    static glm::mat4 Orthographic(float l, float r, float b, float t, float n, float f);
    static glm::mat4 Perspective(float field_of_view, float aspect, float n, float f);
};

#endif // _MATRICES_H