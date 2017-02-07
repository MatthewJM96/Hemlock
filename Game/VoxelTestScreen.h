#pragma once

#include <app\IScreen.h>
#include <camera\BasicCamera.h>
#include <graphics\GLSLProgram.h>
#include <ui\InputDispatcher.h>
#include <ui\InputManager.h>
#include <procedural\Noise.hpp>

class VoxelTestScreen : public happ::IScreen {
public:
    VoxelTestScreen(hui::InputManager* inputManager) : m_inputManager(inputManager) {}

    void init(char* name);
    void dispose();

    void update(TimeData time);
    void draw(TimeData time);

    bool handleMouseMovement(h::Sender sender, hui::MouseMoveEvent event);
private:
    hcam::BasicCamera  m_camera;
    hg::GLSLProgram    m_shader;
    hui::InputManager* m_inputManager;
    
    GLuint m_texture1;
    GLuint m_texture2;
    GLuint m_voxVAO;

    f32* m_heightmap;
    hproc::Noise::NoiseData<f64> m_mapDesc;

    glm::f64vec2 m_chunkLoc;
};
