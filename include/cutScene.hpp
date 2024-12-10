#ifndef CUTSCENE_HPP
#define CUTSCENE_HPP

#include <vector>
#include "SceneObject.hpp"
#include "freeCamera.hpp"
#include "lookAtCamera.h"
#include "gpuProgramController.h"

class CutScene {
public:
    CutScene(GpuProgramController *gpu_controller); // Add this line

    LookAtCamera *look_at_camera;
    void AddLookAtCamera(LookAtCamera *look_at_camera);
    glm::vec4 camera_position;
    glm::vec4 final_camera_position;
    glm::vec4 control_point;
    float bezier_time;

    GpuProgramController *gpu_controller;
    
    void Update(float delta_time);
    void UpdateCameraPosition(float delta_time);

    std::vector<SceneObject> objects;

    void addSceneObject(const SceneObject& obj);
    void clearSceneObjects();

    void createSceneObjects();
    void renderSceneObjects();
};

#endif 
// CUTSCENE_HPP