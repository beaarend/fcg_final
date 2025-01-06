#include "Player.hpp"
#include "SceneObject.hpp"
#include <vector>

Player::Player()
{
    this->camera_mode = CameraMode::LookAt;
    this->position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

    this->pressing_LeftButton = false;

    this->pressing_W = false;
    this->pressing_A = false;
    this->pressing_S = false;
    this->pressing_D = false;

    this->pressed_P = false;

    this->pressing_Space = false;
    this->is_jumping = false;
    this->jump_velocity = 0.0f;

    this->cursorPosX = 0.0f;
    this->cursorPosY = 0.0f;   

}

void Player::Print()
{
    std::cout << "Player position: " << this->position.x << " " << this->position.y << " " << this->position.z << std::endl;
}

void Player::AddLookAtCamera(LookAtCamera *look_at_camera)
{
    this->look_at_camera = look_at_camera;
}

void Player::AddFreeCamera(FreeCamera *free_camera)
{
    this->free_camera = free_camera;
}

void Player::UpdatePosition(float delta_time, std::vector<SceneObject*> objects,SceneObject* floor)
{
    if (this->pressing_W){
        this->position.z -= PLAYER_SPEED * delta_time;
        std::cout<<"new player position: "<<this->position.x<<" "<<this->position.y<<" "<<this->position.z<<std::endl;
    }
    if (this->pressing_A){
        this->position.x -= PLAYER_SPEED * delta_time;
        std::cout<<"new player position: "<<this->position.x<<" "<<this->position.y<<" "<<this->position.z<<std::endl;
    }
    if (this->pressing_S){
        this->position.z += PLAYER_SPEED * delta_time;
        std::cout<<"new player position: "<<this->position.x<<" "<<this->position.y<<" "<<this->position.z<<std::endl;
    }
    if (this->pressing_D){
        this->position.x += PLAYER_SPEED * delta_time;
        std::cout<<"new player position: "<<this->position.x<<" "<<this->position.y<<" "<<this->position.z<<std::endl;
    }
    /*if (this->pressing_C){*/
    /*  this->position.y -= PLAYER_SPEED * delta_time;*/
    /*}*/

    
  if (this->is_jumping)
  {
    this->position.y += this->jump_velocity * delta_time;
    this->jump_velocity -= GRAVITY;
    if (this->position.y <= 0.0f)
    {
      this->position.y = 0.0f;
      this->is_jumping = false;
    }
  }
  /*else{*/
  /*  for(auto object : objects){*/
  /*    if(object->checkCollision(*floor)){*/
  /*      /*std::cout<<"COLIDIU"<<std::endl;*/
  /*      this->is_jumping = false;*/
  /*    }else{*/
  /*      if(!this->is_jumping)*/
  /*        /*this->position.y -= GRAVITY;*/
  /*    } */
  /*  }*/
  /*}*/
   
    
}

void Player::Update(float delta_time, std::vector<SceneObject*> objects,SceneObject* floor)
{   
    this->UpdatePosition(delta_time,objects,floor);

    switch(this->camera_mode)
    {
        case CameraMode::Free:
            this->free_camera->Update(this->position, PLAYER_SPEED);
            break;
        case CameraMode::LookAt:
            this->look_at_camera->Update(this->position);
            break;
    }
}

void Player::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mod)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    // Debug
    std::string key_pressed = "";
    switch (key)
        {
        case GLFW_KEY_W:
            key_pressed = "W";
            break; 
        case GLFW_KEY_A:
            key_pressed = "A";
            break;
        case GLFW_KEY_S:
            key_pressed = "S";
            break;
        case GLFW_KEY_D:
            key_pressed = "D";
            break;
        case GLFW_KEY_SPACE:
            key_pressed = "SPACE";
            break;
        case GLFW_KEY_P:
            key_pressed = "P";
            break;
        case GLFW_KEY_C:
            key_pressed = "C";
            break;
        }
    std::string action_string = "";
    if (action == GLFW_PRESS)
        action_string = "PRESS";
    else if (action == GLFW_RELEASE)
        action_string = "RELEASE";

    // Movement keys
    if (key == GLFW_KEY_W && action == GLFW_PRESS)
    {
        this->pressing_W = true;
    }
    if (key == GLFW_KEY_W && action == GLFW_RELEASE)
    {
        this->pressing_W = false;
    }
    if (key == GLFW_KEY_A && action == GLFW_PRESS)
    {
        this->pressing_A = true;
    }
    if (key == GLFW_KEY_A && action == GLFW_RELEASE)
    {
        this->pressing_A = false;
    }
    if (key == GLFW_KEY_S && action == GLFW_PRESS)
    {
        this->pressing_S = true;
    }
    if (key == GLFW_KEY_S && action == GLFW_RELEASE)
    {
        this->pressing_S = false;
    }
    if (key == GLFW_KEY_D && action == GLFW_PRESS)
    {
        this->pressing_D = true;
    }
    if (key == GLFW_KEY_D && action == GLFW_RELEASE)
    {
        this->pressing_D = false;
    }
    if (key == GLFW_KEY_C && action == GLFW_PRESS)
    {
        this->pressing_C = true;
    }

    // Jump key
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        this->pressing_Space = true;
        this->is_jumping = true;
        this->jump_velocity = JUMP_STRENGHT;
    }

    // Change cameras
    if (key == GLFW_KEY_P && action == GLFW_PRESS)
    {
        this->pressed_P = !this->pressed_P;
        if (this->pressed_P)
        {
            this->camera_mode = CameraMode::Free;
        }
        else
        {
            this->camera_mode = CameraMode::LookAt;
        }
    }

    std::cout << "Key: " << key_pressed << " | " << "Action: " << action_string << std::endl;
    std::cout << "PAUSE" << this->pressed_P << std::endl;
    
}

void Player::MouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        glfwGetCursorPos(window, &cursorPosX, &cursorPosY);
        this->pressing_LeftButton = true;
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        this->pressing_LeftButton = false;
    }
}

void Player::CursorPosCallback(GLFWwindow *window, double xpos, double ypos)
{

    float dx = xpos - this->cursorPosX;
    float dy = ypos - this->cursorPosY;

    float phimax = 3.141592f / 2;
    float phimin = -phimax;

    switch (camera_mode)
    {
    case CameraMode::LookAt:
        if (this->pressing_LeftButton)
        {
            this->look_at_camera->view_angle_theta -= 0.001f * dx;
            this->look_at_camera->view_angle_phi += 0.001f * dy;

            if (this->look_at_camera->view_angle_phi > phimax)
                this->look_at_camera->view_angle_phi = phimax;

            if (this->look_at_camera->view_angle_phi < phimin)
                this->look_at_camera->view_angle_phi = phimin;

            this->cursorPosX = xpos;
            this->cursorPosY = ypos;
        }
        break;

    case CameraMode::Free:
        free_camera->view_angle_theta -= 0.001f * dx;
        free_camera->view_angle_phi -= 0.001f * dy;

        if (free_camera->view_angle_phi > phimax)
            free_camera->view_angle_phi = phimax;

        if (free_camera->view_angle_phi < phimin)
            free_camera->view_angle_phi = phimin;

        this->cursorPosX = xpos;
        this->cursorPosY = ypos;
        break;
    default:
        break;
    }
}

void Player::ScrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    float zoom_delta = 0.1f * yoffset;

    const float verysmallnumber = std::numeric_limits<float>::epsilon();
    if (this->look_at_camera->distance - zoom_delta >= verysmallnumber)
        this->look_at_camera->distance -= zoom_delta;
}

void Player::KeyCallbackStatic(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Player* player = static_cast<Player*>(glfwGetWindowUserPointer(window));
    if (player)
        player->KeyCallback(window, key, scancode, action, mods);
}

void Player::MouseButtonCallbackStatic(GLFWwindow* window, int button, int action, int mods)
{
    Player* player = static_cast<Player*>(glfwGetWindowUserPointer(window));
    if (player)
        player->MouseButtonCallback(window, button, action, mods);
}

void Player::CursorPosCallbackStatic(GLFWwindow* window, double xpos, double ypos)
{
    Player* player = static_cast<Player*>(glfwGetWindowUserPointer(window));
    if (player)
        player->CursorPosCallback(window, xpos, ypos);
}

void Player::ScrollCallbackStatic(GLFWwindow *window, double xoffset, double yoffset)
{
    Player* player = static_cast<Player*>(glfwGetWindowUserPointer(window));
    if (player)
        player->ScrollCallback(window, xoffset, yoffset);
}





