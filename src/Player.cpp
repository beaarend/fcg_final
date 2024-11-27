#include "Player.hpp"

Player::Player()
{
    this->position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

    this->pressing_LeftButton = false;

    this->pressing_W = false;
    this->pressing_A = false;
    this->pressing_S = false;
    this->pressing_D = false;

    this->pressing_Space = false;

    this->cursorPosX = 0.0f;
    this->cursorPosY = 0.0f;   

}

void Player::AddLookAtCamera(LookAtCamera *look_at_camera)
{
    this->look_at_camera = look_at_camera;
}

void Player::UpdatePosition()
{
    // create the function that when the player press the key, the player moves
    if (this->pressing_W)
        this->position.x += PLAYER_SPEED;
    if (this->pressing_A)
        this->position.z += PLAYER_SPEED;
    if (this->pressing_S)
        this->position.x -= PLAYER_SPEED;
    if (this->pressing_D)
        this->position.z -= PLAYER_SPEED;
}

void Player::Update()
{
    this->UpdatePosition();
    this->look_at_camera->Update(this->position);
}

void Player::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mod)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);


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

    // Jump key
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        this->pressing_Space = true;
    }
    
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

    if (this->pressing_LeftButton)
    {
        this->look_at_camera->view_angle_theta -= 0.01f * dx;
        this->look_at_camera->view_angle_phi += 0.01f * dy;

        if (this->look_at_camera->view_angle_phi > phimax)
            this->look_at_camera->view_angle_phi = phimax;

        if (this->look_at_camera->view_angle_phi < phimin)
            this->look_at_camera->view_angle_phi = phimin;

        this->cursorPosX = xpos;
        this->cursorPosY = ypos;
    }
}

// // Função callback chamada sempre que o usuário movimenta a "rodinha" do mouse.
// void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
// {
//     // Atualizamos a distância da câmera para a origem utilizando a
//     // movimentação da "rodinha", simulando um ZOOM.
//     g_CameraDistance -= 0.1f*yoffset;

//     // Uma câmera look-at nunca pode estar exatamente "em cima" do ponto para
//     // onde ela está olhando, pois isto gera problemas de divisão por zero na
//     // definição do sistema de coordenadas da câmera. Isto é, a variável abaixo
//     // nunca pode ser zero. Versões anteriores deste código possuíam este bug,
//     // o qual foi detectado pelo aluno Vinicius Fraga (2017/2).
//     const float verysmallnumber = std::numeric_limits<float>::epsilon();
//     if (g_CameraDistance < verysmallnumber)
//         g_CameraDistance = verysmallnumber;
// }



