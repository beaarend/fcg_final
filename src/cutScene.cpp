#include <iostream>
#include <algorithm>
#include "cutScene.hpp"

CutScene::CutScene(GpuProgramController *gpu_controller)
{
    this->gpu_controller = gpu_controller;
    createSceneObjects();
    this->camera_position = glm::vec4(2.0f, 2.0f, 2.0f, 1.0f); 
    this->final_camera_position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    this->bezier_time = 0.0f; 
    this->control_point = glm::vec4(1.0f, 3.0f, 1.0f, 1.0f);
}

void CutScene::Update(float delta_time) 
{
    UpdateCameraPosition(delta_time);
    this->look_at_camera->Update(this->camera_position);
    renderSceneObjects();
}

void CutScene::UpdateCameraPosition(float delta_time)
{
    // TODO Implement Bezier curve THIS IS NOT WORKING
    this->bezier_time += delta_time;
    if (this->bezier_time > 2.0f)
    {
        this->bezier_time = 2.0f;
    }
    this->camera_position = (1.0f - this->bezier_time) * this->camera_position + this->bezier_time * this->final_camera_position;
}


void CutScene::AddLookAtCamera(LookAtCamera *look_at_camera)
{
    this->look_at_camera = look_at_camera;
}

void CutScene::createSceneObjects() {
    
    SceneObject sphereObject("../../resources/objects/sphere.obj");
    sphereObject.setObjectID(0);
    sphereObject.translate(2.0f, 2.0f, 2.0f);
    addSceneObject(sphereObject);

    // TODO ADD FAUSTÃO NÃO SEI PQ ELE N APARECE >:/
    SceneObject faustaoObject("../../resources/objects/faustao.obj");
    faustaoObject.setObjectID(3);
    addSceneObject(faustaoObject);

}

void CutScene::renderSceneObjects() {
    for (auto& obj : objects) {
        obj.render(*this->gpu_controller);
    }
}

void CutScene::addSceneObject(const SceneObject& obj) {
    objects.push_back(obj);
}

void CutScene::clearSceneObjects() {
    objects.clear();
}