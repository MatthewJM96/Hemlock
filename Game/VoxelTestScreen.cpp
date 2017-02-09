#include "stdafx.h"

#include "VoxelTestScreen.h"

#include <Event.hpp>
#include <graphics\Mesh.hpp>
#include <graphics\Texture.h>

#include <glm\gtc\matrix_transform.hpp>

#include "ChunkGenerator.h"

// TODO(Matthew): Colourless variation.
static hg::Vertex3D<f32> VOXEL_VERTICES[36] = {
    { -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, },
    {  0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, },
    {  0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, },
    {  0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, },
    { -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, },
    { -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, },

    { -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, },
    {  0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, },
    {  0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, },
    {  0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, },
    { -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, },
    { -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, },

    { -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, },
    { -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, },
    { -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, },
    { -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, },
    { -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, },
    { -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, },

    {  0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, },
    {  0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, },
    {  0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, },
    {  0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, },
    {  0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, },
    {  0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, },

    { -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, },
    {  0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, },
    {  0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, },
    {  0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, },
    { -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, },
    { -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, },

    { -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, },
    {  0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, },
    {  0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, },
    {  0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, },
    { -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, },
    { -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f }
};

//#define MAPSIZE 100
//
//f32 getChillHills(glm::f32vec2 pos) {
//    hproc::Noise::NoiseData<f64> smallDetails;
//    smallDetails.type = hproc::Noise::Type::CELLULAR_CUBIC;
//    smallDetails.octaves = 6;
//    smallDetails.persistence = 0.85;
//    smallDetails.frequency = 0.1;
//    smallDetails.modifier = 1.0;
//    smallDetails.op = hproc::Noise::Operation::ADD;
//    smallDetails.multiplier = hproc::Noise::Multiplier::NONE;
//    smallDetails.bound = { 0.0, 10.0 };
//    smallDetails.clamp = { 0.0,  0.0 };
//
//    hproc::Noise::NoiseData<f64> bigDetails;
//    bigDetails.type = hproc::Noise::Type::CELLULAR;
//    bigDetails.octaves = 5;
//    bigDetails.persistence = 0.7;
//    bigDetails.frequency = 0.005;
//    bigDetails.modifier = 1.0;
//    bigDetails.op = hproc::Noise::Operation::ADD;
//    bigDetails.multiplier = hproc::Noise::Multiplier::NONE;
//    bigDetails.bound = { -20.0, 80.0 };
//    bigDetails.clamp = { 0.0,  0.0 };
//
//    return glm::floor(hproc::Noise::getNoiseValue((glm::f64vec2)pos, smallDetails) + hproc::Noise::getNoiseValue((glm::f64vec2)pos, bigDetails));
//}
//
//f32 getMountains(glm::f32vec2 pos) {
//    hproc::Noise::NoiseData<f64> smallDetails;
//    smallDetails.type = hproc::Noise::Type::RIDGED;
//    smallDetails.octaves = 8;
//    smallDetails.persistence = 0.85;
//    smallDetails.frequency = 0.05;
//    smallDetails.modifier = 0.0;
//    smallDetails.op = hproc::Noise::Operation::ADD;
//    smallDetails.multiplier = hproc::Noise::Multiplier::NONE;
//    smallDetails.bound = { -2.0, 7.0 };
//    smallDetails.clamp = { 0.0,  0.0 };
//
//    hproc::Noise::NoiseData<f64> medDetails;
//    medDetails.type = hproc::Noise::Type::RIDGED;
//    medDetails.octaves = 6;
//    medDetails.persistence = 0.8;
//    medDetails.frequency = 0.0025;
//    medDetails.modifier = 1.0;
//    medDetails.op = hproc::Noise::Operation::ADD;
//    medDetails.multiplier = hproc::Noise::Multiplier::NONE;
//    medDetails.bound = { -50.0, 50.0 };
//    medDetails.clamp = { 0.0,  0.0 };
//
//    hproc::Noise::NoiseData<f64> bigDetails;
//    bigDetails.type = hproc::Noise::Type::CELLULAR;
//    bigDetails.octaves = 6;
//    bigDetails.persistence = 0.7;
//    bigDetails.frequency = 0.0005;
//    bigDetails.modifier = 1.0;
//    bigDetails.op = hproc::Noise::Operation::ADD;
//    bigDetails.multiplier = hproc::Noise::Multiplier::NONE;
//    bigDetails.bound = { 0.0, 200.0 };
//    bigDetails.clamp = { 0.0,  0.0 };
//
//    return glm::floor(hproc::Noise::getNoiseValue((glm::f64vec2)pos, smallDetails) + hproc::Noise::getNoiseValue((glm::f64vec2)pos, medDetails) + hproc::Noise::getNoiseValue((glm::f64vec2)pos, bigDetails));
//}
//
//
//f32 getNoise(glm::f32vec2 pos) {
//    return getChillHills(pos);
//}

void VoxelTestScreen::init(char* name) {
    if (m_initialised) return;
    IScreen::init(name);

    m_camera.setPosition(glm::f32vec3(0.0f, 0.0f, 20.0f));
    m_camera.setFov(45.0f);
    m_camera.update();

    hui::InputDispatcher::getInstance()->onMouseMove += { 2, h::makeRDelegate(*this, &VoxelTestScreen::handleMouseMovement) };
    hui::InputDispatcher::getInstance()->onKeyboardButtonDown += { 2, h::makeRDelegate(*this, &VoxelTestScreen::handleMouseCapture) };

    m_shader.init();
    m_shader.addShaders("shaders/simple.frag", "shaders/simple.vert");
    m_shader.link();

    m_texture1 = hg::Texture::load("textures/container.jpg", true);
    m_texture2 = hg::Texture::load("textures/anfo.png", true);
    
    hg::MeshData3D<f32> m_vData = {};
    m_vData.vertices = &VOXEL_VERTICES[0];
    m_vData.vertexCount = 36;
    m_vData.indices = nullptr;

    m_voxVAO = hg::createVAO(m_vData);
    
    m_chunkGrid.init(new ChunkGenerator());

    for (i32 x = -VIEW_DIST; x < VIEW_DIST; ++x) {
        for (i32 y = -VIEW_DIST; y < VIEW_DIST; ++y) {
            for (i32 z = -VIEW_DIST; z < VIEW_DIST; ++z) {
                m_chunkGrid.submitGenTask(hvox::ChunkLOD::FULL, hvox::ChunkGenType::TERRAIN, { x, y, z });
            }
        }
    }
    m_chunkGrid.update();

    //m_heightmap = new f32[(2 * MAPSIZE + 1) * (2 * MAPSIZE + 1)];
    //for (i32 x = -MAPSIZE; x <= MAPSIZE; ++x) {
    //    for (i32 y = -MAPSIZE; y <= MAPSIZE; ++y) {
    //        m_heightmap[(x + MAPSIZE) + (2 * MAPSIZE + 1) * (y + MAPSIZE)] = getNoise({ x, y }/*, m_mapDesc*/);
    //        //m_heightmap[(x + 50) + 101 * (y + 50)] = hm::Fractal::genSimplexWithOctavesScaled(m_mapDesc.octaves, m_mapDesc.persistence, m_mapDesc.frequency, m_mapDesc.bound.lo, m_mapDesc.bound.hi, glm::f64vec2(x, y));
    //    }
    //}

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void VoxelTestScreen::dispose() {
    if (!m_initialised) return;

    m_shader.dispose();

    IScreen::dispose();
}

void VoxelTestScreen::update(TimeData time) {
    f32 speed = 0.1f;
    auto& modifiers = m_inputManager->getCurrentKeyModifierState();
    if (modifiers.lctrl) {
        speed *= 5;
    }
    if (modifiers.lalt) {
        speed *= 10;
    }
    if (m_inputManager->isPressed(hui::PhysicalKey::H_W)) {        
        m_camera.offsetPosition(speed * m_camera.getDirection());
    }
    if (m_inputManager->isPressed(hui::PhysicalKey::H_A)) {
        m_camera.offsetPosition(-speed * m_camera.getRight());
    }
    if (m_inputManager->isPressed(hui::PhysicalKey::H_S)) {
        m_camera.offsetPosition(-speed * m_camera.getDirection());
    }
    if (m_inputManager->isPressed(hui::PhysicalKey::H_D)) {
        m_camera.offsetPosition(speed * m_camera.getRight());
    }
    
    // Update heightmap to be of new location if we move out of range of current loc.
    glm::f64vec3 pos = glm::f64vec3(m_camera.getPosition());
    pos /= CHUNK_SIZE;
    pos = glm::floor(pos);
    if (m_chunkLoc.x != pos.x || m_chunkLoc.y != pos.y || m_chunkLoc.z != pos.z) {
        for (i32 x = -VIEW_DIST; x < VIEW_DIST; ++x) { // lazy but this is all lazy...
            for (i32 y = -VIEW_DIST; y < VIEW_DIST; ++y) {
                for (i32 z = -VIEW_DIST; z < VIEW_DIST; ++z) {
                    m_chunkGrid.submitGenTask(hvox::ChunkLOD::FULL, hvox::ChunkGenType::TERRAIN, { x + (i32)pos.x, y + (i32)pos.y, z + (i32)pos.z });
                }
            }
        }
        m_chunkGrid.update();
        m_chunkLoc = pos;
        //i32 diff = (i32)(m_chunkLoc.x - pos.x);
        //if (diff < 0) {
        //    diff = glm::abs(diff);
        //    for (i32 i = 0; i < diff; ++i) {
        //        i32 x = i + pos.x - VIEW_DIST;
        //        m_chunkGrid.submitGenTask(hvox::ChunkLOD::FULL, hvox::ChunkGenType::TERRAIN, { x, y, z });
        //    }
        //} else {

        //}
    }

    m_camera.update();
}

void VoxelTestScreen::draw(TimeData time) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   
    m_shader.use();

    glm::f32mat4 view = m_camera.getViewMatrix();
    glm::f32mat4 projection = m_camera.getProjectionMatrix();

    GLuint modelLoc = m_shader.getUniformLocation("model");
    GLuint viewLoc = m_shader.getUniformLocation("view");
    GLuint ProjLoc = m_shader.getUniformLocation("proj");

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(ProjLoc, 1, GL_FALSE, &projection[0][0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture1);
    glUniform1i(m_shader.getUniformLocation("tex1"), 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_texture2);
    glUniform1i(m_shader.getUniformLocation("tex2"), 1);

    glBindVertexArray(m_voxVAO);
    for (i32 i = -VIEW_DIST; i < VIEW_DIST; ++i) {
        for (i32 j = -VIEW_DIST; j < VIEW_DIST; ++j) {
            for (i32 k = -VIEW_DIST; k < VIEW_DIST; ++k) {
                char* hash = new char[256];
                snprintf(hash, 256, "%d|%d|%d", i + (i32)m_chunkLoc.x, j + (i32)m_chunkLoc.y, k + (i32)m_chunkLoc.z);
                hvox::Chunk<CHUNK_SIZE>* chunk = m_chunkGrid.getChunks().at(/*{ i + (i32)m_chunkLoc.x, j + (i32)m_chunkLoc.y, k + (i32)m_chunkLoc.z }*/std::string(hash));
                hvox::Block* blocks = chunk->getBlocks();
                for (ui32 x = 0; x < CHUNK_SIZE; ++x) {
                    for (ui32 y = 0; y < CHUNK_SIZE; ++y) {
                        for (ui32 z = 0; z < CHUNK_SIZE; ++z) {
                            if (!blocks[x + y * CHUNK_SIZE + z * CHUNK_SIZE * CHUNK_SIZE].present) continue;
                            glm::f32mat4 model = glm::translate(glm::f32mat4(), glm::f32vec3(x + CHUNK_SIZE * (i + m_chunkLoc.x), y + CHUNK_SIZE * (j + m_chunkLoc.y), z + CHUNK_SIZE * (k + m_chunkLoc.z)));
                            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

                            glDrawArrays(GL_TRIANGLES, 0, 36);
                        }
                    }
                }
            }
        }
    }
    glBindVertexArray(0);

    m_shader.unuse();
}

bool VoxelTestScreen::handleMouseMovement(h::Sender sender, hui::MouseMoveEvent event) {
    m_camera.rotateFromMouse(-event.dx, -event.dy, 0.005f);
    return false;
}

bool VoxelTestScreen::handleMouseCapture(h::Sender sender, hui::KeyboardButtonEvent event) {
    if (event.physicalKey == hui::PhysicalKey::H_LSHIFT) {
        SDL_SetRelativeMouseMode(SDL_TRUE);
    } else if (event.physicalKey == hui::PhysicalKey::H_RSHIFT) {
        SDL_SetRelativeMouseMode(SDL_FALSE);
    }
    return true;
}
