#ifndef _PLAYER_H
#define _PLAYER_H

#include "lookAtCamera.h"
#include "matrices.h"
#include <iostream>
#include <string>

// OpenGl
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// Matrices
#include <glm/vec4.hpp>

class Player
{
private:
    void UpdatePosition();
    const float PLAYER_SPEED = 0.1f;

    bool pressing_LeftButton;

    // Keys
    bool pressing_W;
    bool pressing_A;
    bool pressing_S;
    bool pressing_D;

    bool pressing_Space;

    double cursorPosX, cursorPosY;
    
public:
    Player();
    glm::vec4 position;

    LookAtCamera *look_at_camera;
    void AddLookAtCamera(LookAtCamera *look_at_camera);

    void ScrollCallback(GLFWwindow *window, double xoffset, double yoffset);
    void MouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
    void CursorPosCallback(GLFWwindow *window, double xpos, double ypos);
    void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mod);

    // Static callback functions
    static void KeyCallbackStatic(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void MouseButtonCallbackStatic(GLFWwindow* window, int button, int action, int mods);
    static void CursorPosCallbackStatic(GLFWwindow* window, double xpos, double ypos);
    static void ScrollCallbackStatic(GLFWwindow *window, double xoffset, double yoffset);

    void Update();
    void Print();
    
};

#endif // _PLAYER_H