#include "matrices.h"

// Transpose New constructor
glm::mat4 Matrices::New(
    float m00, float m01, float m02, float m03, // line 1
    float m10, float m11, float m12, float m13, // line 2
    float m20, float m21, float m22, float m23, // line 3
    float m30, float m31, float m32, float m33  // line 4
)
{
    return glm::mat4(
        m00, m10, m20, m30, // column 1
        m01, m11, m21, m31, // column 2
        m02, m12, m22, m32, // column 3
        m03, m13, m23, m33  // column 4
    );
}

glm::mat4 Matrices::Identity()
{
    return New(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);
}

float Matrices::Norm(glm::vec4 v)
{
    float vx = v.x;
    float vy = v.y;
    float vz = v.z;

    return sqrt(vx * vx + vy * vy + vz * vz);
}

bool Matrices::IsVectorNull(glm::vec4 vector)
{
    if (vector.w != 0.0f)
    {
        fprintf(stderr, "[ERROR] IsVectorNull: input is a point\n");
        std::exit(EXIT_FAILURE);
    }

    return vector.x == 0.0f && vector.y == 0.0f && vector.z == 0.0f;
}

glm::vec4 Matrices::Normalize(glm::vec4 v)
{
    if (IsVectorNull(v))
    {
        fprintf(stderr, "[ERROR] Normalize: vector is null, it can't be normalized\n");
        std::exit(EXIT_FAILURE);
    }

    return v / Norm(v);
}

glm::mat4 Matrices::Translate(float tx, float ty, float tz)
{
    return New(
        1.0f, 0.0f, 0.0f, tx,
        0.0f, 1.0f, 0.0f, ty,
        0.0f, 0.0f, 1.0f, tz,
        0.0f, 0.0f, 0.0f, 1.0f);
}

glm::mat4 Matrices::Scale(float sx, float sy, float sz)
{
    return New(
        sx, 0.0f, 0.0f, 0.0f,
        0.0f, sy, 0.0f, 0.0f,
        0.0f, 0.0f, sz, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);
}

glm::mat4 Matrices::RotateX(float angle)
{
    float c = cos(angle);
    float s = sin(angle);
    return New(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, c, -s, 0.0f,
        0.0f, s, c, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);
}

glm::mat4 Matrices::RotateY(float angle)
{
    float c = cos(angle);
    float s = sin(angle);
    return New(
        c, 0.0f, s, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        -s, 0.0f, c, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);
}

glm::mat4 Matrices::RotateZ(float angle)
{
    float c = cos(angle);
    float s = sin(angle);
    return New(
        c, -s, 0.0f, 0.0f,
        s, c, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);
}

glm::vec4 Matrices::Vectorize(glm::vec4 point)
{
    glm::vec4 vector = point;
    vector.w = 0.0f;
    return vector;
}

glm::mat4 Matrices::Rotate(float angle, glm::vec4 axis)
{
    float c = cos(angle);
    float s = sin(angle);

    glm::vec4 v = Normalize(axis);

    float vx = v.x;
    float vy = v.y;
    float vz = v.z;

    return New(
        vx * vx * (1.0f - c) + c, vx * vy * (1.0f - c) - vz * s, vx * vz * (1 - c) + vy * s, 0.0f,
        vx * vy * (1.0f - c) + vz * s, vy * vy * (1.0f - c) + c, vy * vz * (1 - c) - vx * s, 0.0f,
        vx * vz * (1 - c) - vy * s, vy * vz * (1 - c) + vx * s, vz * vz * (1.0f - c) + c, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);
}

glm::mat4 Matrices::FaceDirection(glm::vec4 source_direction, glm::vec4 target_direction)
{
    float angle_cos = Matrices::DotProduct(source_direction, target_direction);
    float rotation_angle = (float)acos(angle_cos);

    glm::vec4 rotation_axis = Matrices::CrossProduct(source_direction, target_direction);
    glm::vec4 normalized_rotation_axis = Matrices::Normalize(rotation_axis);

    return Matrices::Rotate(rotation_angle, normalized_rotation_axis);
}

glm::vec4 Matrices::CrossProduct(glm::vec4 u, glm::vec4 v)
{
    float u1 = u.x;
    float u2 = u.y;
    float u3 = u.z;
    float v1 = v.x;
    float v2 = v.y;
    float v3 = v.z;

    return glm::vec4(
        u2 * v3 - u3 * v2,
        u3 * v1 - u1 * v3,
        u1 * v2 - u2 * v1,
        0.0f);
}

float Matrices::DotProduct(glm::vec4 u, glm::vec4 v)
{
    float u1 = u.x;
    float u2 = u.y;
    float u3 = u.z;
    float u4 = u.w;
    float v1 = v.x;
    float v2 = v.y;
    float v3 = v.z;
    float v4 = v.w;

    if (u4 != 0.0f || v4 != 0.0f)
    {
        fprintf(stderr, "[ERROR] DotProduct: this operation is undefined for points.\n");
        std::exit(EXIT_FAILURE);
    }

    return u1 * v1 + u2 * v2 + u3 * v3;
}

glm::mat4 Matrices::CameraView(glm::vec4 position_c, glm::vec4 view_vector, glm::vec4 up_vector)
{
    glm::vec4 w = -view_vector;
    glm::vec4 u = CrossProduct(up_vector, w);

    w = Normalize(w);
    u = Normalize(u);

    glm::vec4 v = CrossProduct(w, u);

    glm::vec4 origin_o = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

    float ux = u.x;
    float uy = u.y;
    float uz = u.z;
    float vx = v.x;
    float vy = v.y;
    float vz = v.z;
    float wx = w.x;
    float wy = w.y;
    float wz = w.z;

    return New(
        ux, uy, uz, -DotProduct(u, position_c - origin_o),
        vx, vy, vz, -DotProduct(v, position_c - origin_o),
        wx, wy, wz, -DotProduct(w, position_c - origin_o),
        0.0f, 0.0f, 0.0f, 1.0f);
}

glm::mat4 Matrices::Orthographic(float l, float r, float b, float t, float n, float f)
{
    glm::mat4 M = New(
        2.0f / (r - l), 0.0f, 0.0f, -(r + l) / (r - l),
        0.0f, 2.0f / (t - b), 0.0f, -(t + b) / (t - b),
        0.0f, 0.0f, 2.0f / (f - n), -(f + n) / (f - n),
        0.0f, 0.0f, 0.0f, 1.0f);

    return M;
}

glm::mat4 Matrices::Perspective(float field_of_view, float aspect, float n, float f)
{
    float t = fabs(n) * tanf(field_of_view / 2.0f);
    float b = -t;
    float r = t * aspect;
    float l = -r;

    glm::mat4 P = New(
        n, 0.0f, 0.0f, 0.0f,
        0.0f, n, 0.0f, 0.0f,
        0.0f, 0.0f, n + f, -f * n,
        0.0f, 0.0f, 1.0f, 0.0f);

    glm::mat4 M = Matrices::Orthographic(l, r, b, t, n, f);

    return -M * P;
}