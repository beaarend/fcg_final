#ifndef CUTSCENE_HPP
#define CUTSCENE_HPP

#include <vector>
#include "SceneObject.hpp"
#include "freeCamera.hpp"
#include "lookAtCamera.h"
#include "gpuProgramController.h"

class CutScene {
public:
    CutScene(GpuProgramController *gpu_controller); 

    const float INIT_Y = 5.0f;
    const float INIT_Z = 5.0f;

    const float BEZIER_DURATION = 6.0f;

    float total_time;
    float current_time;

    bool is_cleared = false;

    LookAtCamera *look_at_camera;
    void AddLookAtCamera(LookAtCamera *look_at_camera);
    glm::vec4 camera_position;
    glm::vec4 final_camera_position;
    glm::vec4 control_point;
    glm::vec4 control_point2;
    float bezier_time;

    GpuProgramController *gpu_controller;
    
    void Update(float delta_time);
    void ComputeBezier(float delta_time);
    glm::vec4 GetBezierPoint();
    void AdjustCameraAngle();

    std::vector<SceneObject> objects;

    void addSceneObject(const SceneObject& obj);
    void clearSceneObjects();

    void createSceneObjects();
    void renderSceneObjects();
};

#endif 
// CUTSCENE_HPP