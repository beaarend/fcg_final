#include <iostream>
#include <algorithm>
#include "cutScene.hpp"
#include <Hitbox.hpp>

CutScene::CutScene(GpuProgramController *gpu_controller)
{
    this->gpu_controller = gpu_controller;
    createSceneObjects();
    this->current_time = 0.0f;
    this->total_time = 10.0f;
    this->camera_position = glm::vec4(0.0f, INIT_Y+2.0f, INIT_Z, 1.0f); 
    this->final_camera_position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    this->bezier_time = 0.0f; 
    this->control_point = glm::vec4(0.0f, 5.5f, 4.5f, 1.0f);
    this->control_point2 = glm::vec4(0.0f, 3.5f, 2.5f, 1.0f);
}

void CutScene::Update(float delta_time) 
{
    if (this->current_time > 4.0f){
        AdjustCameraAngle();
        ComputeBezier(delta_time);

        if (this->current_time > 9.1f && !is_cleared)
        {
            clearSceneObjects();
            is_cleared = true;
        }
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

#define WALL 3
#define FLOOR 7
#define TV 8
#define SCREEN 9

void CutScene::createSceneObjects() {
    
    // SceneObject sphereObject("../../resources/objects/sphere.obj");
    // sphereObject.setObjectID(0);
    // sphereObject.translate(0.0f, INIT_Y, INIT_Z+4.0f);
    // addSceneObject(sphereObject);

    // SceneObject cowObject("../../resources/objects/cow.obj", "unique", HitboxType::AABB);
    // cowObject.translate(0.0f, 5.0f, 4.0f);
    // cowObject.setObjectID(3);
    // addSceneObject(cowObject);

    //FLOOR
    SceneObject floor("../../resources/objects/cutscene/plane.obj", "unique", HitboxType::AABB);
    floor.translate(0.0f, 9.0f, 0.0f); // Floor
    floor.scale(glm::vec3(5.0f, 0.5f, 15.0f));
    floor.setObjectID(FLOOR);
    addSceneObject(floor);
    
    // BACKWALL
    SceneObject wall1("../../resources/objects/cutscene/plane.obj", "unique", HitboxType::AABB);
    wall1.scale(glm::vec3(5.0f, 0.5f, 15.0f));
    wall1.rotateX(1.57f);
    wall1.translate(0.0f, 9.0f, 7.0f); 
    wall1.setObjectID(WALL);
    addSceneObject(wall1);

    SceneObject wall2("../../resources/objects/cutscene/plane.obj", "unique", HitboxType::AABB);
    wall2.scale(glm::vec3(5.0f, 0.5f, 20.0f));
    wall2.rotateX(1.57f);
    wall2.rotateY(1.25f);
    wall2.translate(-3.0f, 9.0f, 7.0f); 
    wall2.setObjectID(WALL);
    addSceneObject(wall2);

    SceneObject wall3("../../resources/objects/cutscene/plane.obj", "unique", HitboxType::AABB);
    wall3.scale(glm::vec3(5.0f, 0.5f, 20.0f));
    wall3.rotateX(1.57f);
    wall3.rotateY(-1.25f);
    wall3.translate(3.0f, 9.0f, 7.0f); 
    wall3.setObjectID(WALL);
    addSceneObject(wall3);

    SceneObject tvObject("../../resources/objects/cutscene/tv.obj", "unique", HitboxType::AABB);
    tvObject.scale(glm::vec3(2.0f, 2.0f, 2.0f));
    tvObject.rotateY(1.57f);
    tvObject.translate(0.0f, 5.0f, 10.0f);
    tvObject.setObjectID(TV);
    addSceneObject(tvObject);

    SceneObject screenObject("../../resources/objects/cutscene/plane.obj", "unique", HitboxType::AABB);
    //screenObject.scale(glm::vec3(1.0f, 1.0f, 0.1f));
    screenObject.rotateX(1.57f);
    screenObject.scale(glm::vec3(0.65f, 0.45f, 0.1f));
    screenObject.translate(0.0f, 5.6f, 10.5f);
    screenObject.setObjectID(SCREEN);
    addSceneObject(screenObject);
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
