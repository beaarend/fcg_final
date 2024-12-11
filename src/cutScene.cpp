#include <iostream>
#include <algorithm>
#include "cutScene.hpp"
#include <bits/stdc++.h>

CutScene::CutScene(GpuProgramController *gpu_controller)
{
    this->gpu_controller = gpu_controller;
    createSceneObjects();
    this->current_time = 0.0f;
    this->total_time = 10.0f;
    this->camera_position = glm::vec4(0.0f, INIT_Y, INIT_Z, 1.0f); 
    this->final_camera_position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    this->bezier_time = 0.0f; 
    this->control_point = glm::vec4(0.0f, 1.0f, 4.0f, 1.0f);
    this->control_point2 = glm::vec4(0.0f, 2.0f, 2.0f, 1.0f);
}

void CutScene::Update(float delta_time) 
{
    if (this->current_time > 4.0f){
        AdjustCameraAngle();
        ComputeBezier(delta_time);
    }
    this->look_at_camera->Update(this->camera_position);
    renderSceneObjects();
}

void CutScene::AdjustCameraAngle()
{
    float t = this->bezier_time / BEZIER_DURATION;

    float start_angle = 0.0f;
    float end_angle = 0.5f;
    float angle = (1.0f - t) * start_angle + t * end_angle;
    
    this->look_at_camera->setAnglePhi(angle);
}

glm::vec4 CutScene::GetBezierPoint()
{
    float t = this->bezier_time / BEZIER_DURATION;

    glm::vec4 c12 = this->camera_position + t * (this->control_point - this->camera_position);
    glm::vec4 c23 = this->control_point + t * (this->control_point2 - this->control_point);
    glm::vec4 c34 = this->control_point2 + t * (this->final_camera_position - this->control_point2);
    glm::vec4 c123 = c12 + t * (c23 - c12);
    glm::vec4 c234 = c23 + t * (c34 - c23);

    return c123 + t * (c234 - c123);
}

void CutScene::ComputeBezier(float delta_time)
{
    if (this->bezier_time + delta_time <= this->total_time)
    {
        this->bezier_time += delta_time;
        this->camera_position = GetBezierPoint();
    }
    else
    {
        this->camera_position = this->final_camera_position;
    }
}

void CutScene::AddLookAtCamera(LookAtCamera *look_at_camera)
{
    this->look_at_camera = look_at_camera;
    this->look_at_camera->setAnglePhi(0.0f);
}

void CutScene::createSceneObjects() {
    
    SceneObject sphereObject("../../resources/objects/sphere.obj");
    sphereObject.setObjectID(0);
    sphereObject.translate(0.0f, INIT_Y, INIT_Z);
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