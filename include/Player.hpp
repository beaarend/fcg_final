#ifndef _PLAYER_H
#define _PLAYER_H

#include "SceneObject.hpp"
#include "lookAtCamera.h"
#include "freeCamera.hpp"
#include "Animation.hpp"

#include "matrices.h"
#include <iostream>
#include <string>

// OpenGl
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// Matrices
#include <glm/vec4.hpp>
#include <vector>

enum struct CameraMode
{
    Free,
    LookAt,
};

class Player
{
private:
    CameraMode camera_mode;

    const float PLAYER_SPEED = 3.0f;
    const float GRAVITY = 0.1f;
    const float JUMP_STRENGHT = 2.5f;

    void UpdatePosition(float delta_time,std::vector<SceneObject*> objects,SceneObject* floor,std::vector<SceneObject*> spheres);

    // Keys
    bool pressing_W;
    bool pressing_A;
    bool pressing_S;
    bool pressing_D;
    bool pressing_C;

    bool pressed_P;

    bool pressing_Space;
    bool is_jumping;
    float jump_velocity;

    // Mouse
    double cursorPosX, cursorPosY;
    bool pressing_LeftButton;
    std::vector<Animation*> animations;
    
public:
    Player();
    void Update(float delta_time,std::vector<SceneObject*> objects,SceneObject* floor,std::vector<SceneObject*> spheres);
    void Print();
    glm::vec4 position;

    LookAtCamera *look_at_camera;
    void AddLookAtCamera(LookAtCamera *look_at_camera);

    FreeCamera *free_camera;
    void AddFreeCamera(FreeCamera *free_camera);

    void ScrollCallback(GLFWwindow *window, double xoffset, double yoffset);
    void MouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
    void CursorPosCallback(GLFWwindow *window, double xpos, double ypos);
    void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mod);

    // Static callback functions
    static void KeyCallbackStatic(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void MouseButtonCallbackStatic(GLFWwindow* window, int button, int action, int mods);
    static void CursorPosCallbackStatic(GLFWwindow* window, double xpos, double ypos);
    static void ScrollCallbackStatic(GLFWwindow *window, double xoffset, double yoffset);
};

#endif // _PLAYER_H
