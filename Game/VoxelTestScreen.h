#pragma once

#include <app\IScreen.h>
#include <camera\BasicFPSCamera.h>
#include <graphics\GLSLProgram.h>
#include <ui\InputDispatcher.h>
#include <ui\InputManager.h>
#include <procedural\Noise.hpp>
#include <voxel\ChunkGrid.hpp>
#include <voxel\Chunk.hpp>

#include "Constants.hpp"

class VoxelTestScreen : public happ::IScreen {
public:
    VoxelTestScreen(hui::InputManager* inputManager) : m_inputManager(inputManager) {}

    void init(char* name);
    void dispose();

    void update(TimeData time);
    void draw(TimeData time);

    bool handleMouseMovement(h::Sender sender, hui::MouseMoveEvent event);
    bool handleMouseCapture(h::Sender sender, hui::KeyboardButtonEvent event);
private:
    hcam::BasicFPSCamera  m_camera;
    hg::GLSLProgram    m_shader;
    hui::InputManager* m_inputManager;
    
    GLuint m_texture1;
    GLuint m_texture2;
    GLuint m_voxVAO;

    hvox::ChunkGrid<CHUNK_SIZE> m_chunkGrid;

    /*f32* m_heightmap;
    hproc::Noise::NoiseData<f64> m_mapDesc;*/

    glm::f64vec3 m_chunkLoc = glm::f64vec3(0, 0, 0);
};
