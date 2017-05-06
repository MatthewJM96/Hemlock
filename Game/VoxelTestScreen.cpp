#include "stdafx.h"

#include "VoxelTestScreen.h"

#include <graphics/Mesh.hpp>
#include <graphics/Texture.h>

#include "ChunkGenerator.h"

// Won't need these once greedy merging is in, but for now let's just 
// figure out simple occlusion. As such don't worry about indexing for now.
static hg::Vertex3D<f32> FRONT_QUAD[6] = {
    { -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
    { 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
    { 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    { 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    { -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f },
    { -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
};
static hg::Vertex3D<f32> BACK_QUAD[6] = {
    { -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
    { 0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
    { 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    { 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    { -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f },
    { -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
};
static hg::Vertex3D<f32> LEFT_QUAD[6] = {
    { -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
    { -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    { -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f },
    { -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f },
    { -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
    { -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
};
static hg::Vertex3D<f32> RIGHT_QUAD[6] = {
    { 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
    { 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    { 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f },
    { 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f },
    { 0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
    { 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
};
static hg::Vertex3D<f32> BOTTOM_QUAD[6] = {
    { -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f },
    { 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    { 0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
    { 0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
    { -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
    { -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f },
};
static hg::Vertex3D<f32> TOP_QUAD[6] = {
    { -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f },
    { 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    { 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
    { 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
    { -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
    { -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f }
};

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

    m_chunkGrid.init(CHUNK_SIZE, new ChunkGenerator(), new hvox::ChunkMesher());
    for (i32 x = -VIEW_DIST; x <= VIEW_DIST; ++x) {
        for (i32 y = -VIEW_DIST; y <= VIEW_DIST; ++y) {
            for (i32 z = -VIEW_DIST; z <= VIEW_DIST; ++z) {
                m_chunkGrid.submitGenTask(hvox::ChunkLOD::FULL, hvox::ChunkGenType::TERRAIN, { x, y, z });
            }
        }
    }
    m_chunkGrid.update();
    
    hg::MeshData3D<f32> vData = {};
    vData.vertexCount = 6;
    vData.indices = nullptr;
    
    vData.vertices = FRONT_QUAD;
    m_frontVAO = hg::createVAO(vData);
    
    vData.vertices = BACK_QUAD;
    m_backVAO = hg::createVAO(vData);
    
    vData.vertices = LEFT_QUAD;
    m_leftVAO = hg::createVAO(vData);
    
    vData.vertices = RIGHT_QUAD;
    m_rightVAO = hg::createVAO(vData);
    
    vData.vertices = BOTTOM_QUAD;
    m_bottomVAO = hg::createVAO(vData);
    
    vData.vertices = TOP_QUAD;
    m_topVAO = hg::createVAO(vData);

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

    f64 xDel = glm::round(pos.x - m_chunkLoc.x);
    if (xDel != 0.0) {
        i32 x = xDel * VIEW_DIST;
        for (i32 y = -VIEW_DIST; y <= VIEW_DIST; ++y) {
            for (i32 z = -VIEW_DIST; z <= VIEW_DIST; ++z) {
                m_chunkGrid.submitGenTask(hvox::ChunkLOD::FULL, hvox::ChunkGenType::TERRAIN, { x + (i32)pos.x, y + (i32)pos.y, z + (i32)pos.z });
            }
        }
        runGridUpdate = true;
    }
    f64 yDel = glm::round(pos.y - m_chunkLoc.y);
    if (yDel != 0.0) {
        i32 y = yDel * VIEW_DIST;
        for (i32 x = -VIEW_DIST; x <= VIEW_DIST; ++x) {
            for (i32 z = -VIEW_DIST; z <= VIEW_DIST; ++z) {
                m_chunkGrid.submitGenTask(hvox::ChunkLOD::FULL, hvox::ChunkGenType::TERRAIN, { x + (i32)pos.x, y + (i32)pos.y, z + (i32)pos.z });
            }
        }
        runGridUpdate = true;
    }
    f64 zDel = glm::round(pos.z - m_chunkLoc.z);
    if (zDel != 0.0) {
        i32 z = zDel * VIEW_DIST;
        for (i32 x = -VIEW_DIST; x <= VIEW_DIST; ++x) {
            for (i32 y = -VIEW_DIST; y <= VIEW_DIST; ++y) {
                m_chunkGrid.submitGenTask(hvox::ChunkLOD::FULL, hvox::ChunkGenType::TERRAIN, { x + (i32)pos.x, y + (i32)pos.y, z + (i32)pos.z });
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
    
    for (i32 x = -VIEW_DIST; x <= VIEW_DIST; ++x) {
        for (i32 y = -VIEW_DIST; y <= VIEW_DIST; ++y) {
            for (i32 z = -VIEW_DIST; z <= VIEW_DIST; ++z) {
                hvox::ChunkGridPosition pos = { x + m_chunkLoc.x, y + m_chunkLoc.y, z + m_chunkLoc.z };
                hvox::Chunk* const chunk = m_chunkGrid.getChunks().at(pos);

                for (const hvox::Quad& quad : chunk->mesh.quads) {
                    switch (quad.face) {
                    case hvox::Face::TOP:
                        glBindVertexArray(m_topVAO);
                        break;
                    case hvox::Face::BOTTOM:
                        glBindVertexArray(m_bottomVAO);
                        break;
                    case hvox::Face::LEFT:
                        glBindVertexArray(m_leftVAO);
                        break;
                    case hvox::Face::RIGHT:
                        glBindVertexArray(m_rightVAO);
                        break;
                    case hvox::Face::FRONT:
                        glBindVertexArray(m_frontVAO);
                        break;
                    case hvox::Face::BACK:
                        glBindVertexArray(m_backVAO);
                        break;
                    }
                    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &quad.translationMatrix[0][0]);
                    glDrawArrays(GL_TRIANGLES, 0, 36);
                    glBindVertexArray(0);
                }
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
