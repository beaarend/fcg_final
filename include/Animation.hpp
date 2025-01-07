#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <random>
#include <SceneObject.hpp>

class SceneObject;

class Animation
{
public:
    Animation(const glm::vec3& start, const glm::vec3& end, float duration, float startDelay);

    void Update(float deltaTime,SceneObject* object);
    void Restart(SceneObject* object);
    void RestartAleatorio(SceneObject* object);

private:
    glm::vec3 start;
    glm::vec3 end;
    float duration;
    float elapsedTime;
    float startDelay;
    bool started=false;
    bool ended=false;
};

