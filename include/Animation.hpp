#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <random>
#include "SceneObject.hpp"

class Animation
{
public:
    Animation(SceneObject* object, const glm::vec3& start, const glm::vec3& end, float duration, float startDelay);

    void Update(float deltaTime);
    void Restart();
    void RestartAleatorio();

private:
    SceneObject* object;
    glm::vec3 start;
    glm::vec3 end;
    float duration;
    float elapsedTime;
    float startDelay;
    bool started=false;
    bool ended;
};

