#ifndef _PLAYER_H
#define _PLAYER_H

#include "lookAtCamera.h"
#include "freeCamera.hpp"

#include "matrices.h"
#include <iostream>
#include <string>

// OpenGl
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// Matrices
#include <glm/vec4.hpp>

enum struct CameraMode
{
    Free,
    LookAt,
};

class Player
{
private:
    CameraMode camera_mode;

    const float PLAYER_SPEED = 0.1f;
    const float GRAVITY = 0.010f;
    const float JUMP_STRENGHT = 0.15f;

    void UpdatePosition();

    // Keys
    bool pressing_W;
    bool pressing_A;
    bool pressing_S;
    bool pressing_D;

    bool pressing_Space;
    bool is_jumping;
    float jump_velocity;

    // Mouse
    double cursorPosX, cursorPosY;
    bool pressing_LeftButton;
    
public:
    Player();
    void Update();
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