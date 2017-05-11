#include "stdafx.h"

#include "VoxelTestScreen.h"

#include <graphics/Mesh.hpp>
#include <graphics/Texture.h>
#include <procedural/Noise.hpp>
#include <voxel/ChunkGenerator.h>

static i32 generateHeightmap(hvox::ColumnRectilinearWorldPosition pos) {
    // Set up noise data for small, medium and large-scale terrain details.
    hproc::Noise::NoiseData<f64> smallDetails;
    smallDetails.type = hproc::Noise::Type::ABS;
    smallDetails.octaves = 8;
    smallDetails.persistence = 0.85;
    smallDetails.frequency = 0.05;
    smallDetails.modifier = 0.0;
    smallDetails.op = hproc::Noise::Operation::ADD;
    smallDetails.multiplier = hproc::Noise::Multiplier::NONE;
    smallDetails.bound = { -2.0, 7.0 };
    smallDetails.clamp = { 0.0,  0.0 };

    hproc::Noise::NoiseData<f64> medDetails;
    medDetails.type = hproc::Noise::Type::CELLULAR_CUBIC;
    medDetails.octaves = 6;
    medDetails.persistence = 0.8;
    medDetails.frequency = 0.0025;
    medDetails.modifier = 1.0;
    medDetails.op = hproc::Noise::Operation::ADD;
    medDetails.multiplier = hproc::Noise::Multiplier::NONE;
    medDetails.bound = { -10.0, 10.0 };
    medDetails.clamp = { 0.0,  0.0 };

    hproc::Noise::NoiseData<f64> medDetails2;
    medDetails2.type = hproc::Noise::Type::RIDGED;
    medDetails2.octaves = 6;
    medDetails2.persistence = 0.8;
    medDetails2.frequency = 0.0025;
    medDetails2.modifier = 1.0;
    medDetails2.op = hproc::Noise::Operation::ADD;
    medDetails2.multiplier = hproc::Noise::Multiplier::NONE;
    medDetails2.bound = { -20.0, 10.0 };
    medDetails2.clamp = { 0.0,  0.0 };

    hproc::Noise::NoiseData<f64> bigDetails;
    bigDetails.type = hproc::Noise::Type::RIDGED;
    bigDetails.octaves = 6;
    bigDetails.persistence = 0.7;
    bigDetails.frequency = 0.0005;
    bigDetails.modifier = 1.0;
    bigDetails.op = hproc::Noise::Operation::ADD;
    bigDetails.multiplier = hproc::Noise::Multiplier::NONE;
    bigDetails.bound = { -30.0, 250.0 };
    bigDetails.clamp = { 0.0,  0.0 };

    glm::f64vec2 pos_ = { pos.x, pos.z };
    return (i32)glm::floor(hproc::Noise::getNoiseValue(pos_, smallDetails) + hproc::Noise::getNoiseValue(pos_, medDetails) + hproc::Noise::getNoiseValue(pos_, medDetails2) + hproc::Noise::getNoiseValue(pos_, bigDetails));
}

void VoxelTestScreen::init(const char* name) {
    if (m_initialised) return;
    IScreen::init(name);

    m_camera.setPosition(glm::f32vec3(0.0f, 0.0f, 0.0f));
    m_camera.setFov(45.0f);
    m_camera.update();

    hui::InputDispatcher::getInstance()->onMouseMove          += { 2, h::makeDelegate(this, &VoxelTestScreen::handleMouseMovement) };
    hui::InputDispatcher::getInstance()->onKeyboardButtonDown += { 2, h::makeDelegate(this, &VoxelTestScreen::handleMouseCapture) };

    m_shader.init();
    m_shader.addShaders("shaders/simple.frag", "shaders/simple.vert");
    m_shader.link();

    m_texture1 = hg::Texture::load("textures/container.jpg", true);
    m_texture2 = hg::Texture::load("textures/anfo.png", true);
    
    m_chunkGrid.init(new hvox::ChunkGenerator(), new hvox::ChunkMesher());
    auto heightmapper = h::makeDelegate(&generateHeightmap);
    for (i32 x = -VIEW_DIST_XZ; x <= VIEW_DIST_XZ; ++x) {
        for (i32 y = -VIEW_DIST_Y; y <= VIEW_DIST_Y; ++y) {
            for (i32 z = -VIEW_DIST_XZ; z <= VIEW_DIST_XZ; ++z) {
                m_chunkGrid.submitGenTask(hvox::ChunkLOD::FULL, hvox::ChunkGenType::TERRAIN, { x, y, z }, heightmapper);
            }
        }
    }
    m_chunkGrid.update();

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
    const auto& modifiers = m_inputManager->getCurrentKeyModifierState();
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

    bool runGridUpdate = false;

    auto heightmapper = h::makeDelegate(&generateHeightmap);

    f64 xDel = glm::round(pos.x - m_chunkLoc.x);
    if (xDel != 0.0) {
        i32 x = xDel * VIEW_DIST_XZ;
        for (i32 y = -VIEW_DIST_Y; y <= VIEW_DIST_Y; ++y) {
            for (i32 z = -VIEW_DIST_XZ; z <= VIEW_DIST_XZ; ++z) {
                m_chunkGrid.submitGenTask(hvox::ChunkLOD::FULL, hvox::ChunkGenType::TERRAIN, { x + (i32)pos.x, y + (i32)pos.y, z + (i32)pos.z }, heightmapper);
            }
        }
        runGridUpdate = true;
    }
    f64 yDel = glm::round(pos.y - m_chunkLoc.y);
    if (yDel != 0.0) {
        i32 y = yDel * VIEW_DIST_Y;
        for (i32 x = -VIEW_DIST_XZ; x <= VIEW_DIST_XZ; ++x) {
            for (i32 z = -VIEW_DIST_XZ; z <= VIEW_DIST_XZ; ++z) {
                m_chunkGrid.submitGenTask(hvox::ChunkLOD::FULL, hvox::ChunkGenType::TERRAIN, { x + (i32)pos.x, y + (i32)pos.y, z + (i32)pos.z }, heightmapper);
            }
        }
        runGridUpdate = true;
    }
    f64 zDel = glm::round(pos.z - m_chunkLoc.z);
    if (zDel != 0.0) {
        i32 z = zDel * VIEW_DIST_XZ;
        for (i32 x = -VIEW_DIST_XZ; x <= VIEW_DIST_XZ; ++x) {
            for (i32 y = -VIEW_DIST_Y; y <= VIEW_DIST_Y; ++y) {
                m_chunkGrid.submitGenTask(hvox::ChunkLOD::FULL, hvox::ChunkGenType::TERRAIN, { x + (i32)pos.x, y + (i32)pos.y, z + (i32)pos.z }, heightmapper);
            }
        }
        runGridUpdate = true;
    }

    if (runGridUpdate) {
        m_chunkGrid.update();
        m_chunkLoc = pos;
    }

    static size_t _i = 0;
    if (++_i % 10 == 0) {
        std::cout << "Pos:  x - " << m_chunkLoc.x << "  --  y - " << m_chunkLoc.y << "  --  z - " << m_chunkLoc.z << std::endl;
        _i = 0;
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
	
    for (i32 x = -VIEW_DIST_XZ; x <= VIEW_DIST_XZ; ++x) {
        for (i32 y = -VIEW_DIST_Y; y <= VIEW_DIST_Y; ++y) {
            for (i32 z = -VIEW_DIST_XZ; z <= VIEW_DIST_XZ; ++z) {
				hvox::ChunkGridPosition pos = { x + m_chunkLoc.x, y + m_chunkLoc.y, z + m_chunkLoc.z };
				hvox::Chunk* const chunk = m_chunkGrid.getChunks().at(pos);

				if (chunk->mesh.vao <= 0) continue;
				
				glBindVertexArray(chunk->mesh.vao);
				
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &chunk->mesh.translationMatrix[0][0]);
				glDrawArrays(GL_TRIANGLES, 0, chunk->mesh.indices);
				glBindVertexArray(0);
            }
        }
    }

    m_shader.unuse();
}

bool VoxelTestScreen::handleMouseMovement(h::Sender sender, hui::MouseMoveEvent event) {
    m_camera.rotateFromMouse(-(f32)event.dx, -(f32)event.dy, 0.005f);
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
