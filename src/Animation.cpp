#include "Animation.hpp"
#include "SceneObject.hpp"

Animation::Animation(const glm::vec3& start, const glm::vec3& end, float duration, float startDelay)
{
    this->start = start;
    this->end = end;
    this->duration = duration;
    this->startDelay = startDelay;
    this->elapsedTime = 0.0f;
    this->ended = false;
}

void Animation::Update(float deltaTime,SceneObject* object)
{
    /*std::cout<<"Update animation "<<this->ended<<std::endl;*/
    if(!object)
    {
        std::cout<<"Object is null\n";
        return;
    }
    if (ended) return;

    elapsedTime += deltaTime;

    if (!started)
    {
        if (elapsedTime >= startDelay)
        {
            started = true;
            elapsedTime -= startDelay;
            object->translate(start.x, start.y, start.z);
        }
        else
        {
            return;
        }
    }

    float t = elapsedTime / duration;
    if (t > 1.0f) t = 1.0f;

    glm::vec3 newPosition = start + t * (end - start);
    object->resetModelMatrix();
    object->translate(newPosition.x, newPosition.y, newPosition.z);

    // Verifica se chegou no final da animação
    if (t == 1.0f)
    {
        ended = true;
        /*Restart();*/
        RestartAleatorio(object);
    }
}

void Animation::Restart(SceneObject* object)
{
    elapsedTime = 0.0f;
    ended = false;
    object->resetModelMatrix();
    object->translate(start.x, start.y, start.z);
}
void Animation::RestartAleatorio(SceneObject* object)
{
    elapsedTime = 0.0f;
    ended = false;
    started = false;
    object->resetModelMatrix();
    object->translate(start.x, start.y, start.z);
    //novo delay aleatorio
    startDelay = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/4.0f));
}

