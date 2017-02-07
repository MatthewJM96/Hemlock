#include "stdafx.h"

#include "VoxelTestScreen.h"

#include <Event.hpp>
#include <graphics\Mesh.hpp>
#include <graphics\Texture.h>

#include <glm\gtc\matrix_transform.hpp>

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

void VoxelTestScreen::init(char* name) {
    if (m_initialised) return;
    IScreen::init(name);

    m_camera.setPosition(glm::f32vec3(0.0f, 0.0f, 50.0f));
    m_camera.setFov(45.0f);
    m_camera.update();

    hui::InputDispatcher::getInstance()->onMouseMove += { 2, h::makeRDelegate(*this, &VoxelTestScreen::handleMouseMovement) };

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
    
    m_mapDesc.type = hproc::Noise::Type::RAW;
    m_mapDesc.octaves = 6;
    m_mapDesc.persistence = 0.85;
    m_mapDesc.frequency = 0.1;
    m_mapDesc.modifier = 1.0;
    m_mapDesc.op = hproc::Noise::Operation::ADD;
    m_mapDesc.multiplier = hproc::Noise::Multiplier::NONE;
    m_mapDesc.bound = { -2.0, 6.0 };
    m_mapDesc.clamp = { 0.0,  0.0 };

    m_heightmap = new f32[101 * 101];
    for (i32 x = -50; x <= 50; ++x) {
        for (i32 y = -50; y <= 50; ++y) {
            m_heightmap[(x + 50) + 101 * (y + 50)] = hproc::Noise::getNoiseValue(glm::f64vec2(x, y), m_mapDesc);
            //m_heightmap[(x + 50) + 101 * (y + 50)] = hm::Fractal::genSimplexWithOctavesScaled(m_mapDesc.octaves, m_mapDesc.persistence, m_mapDesc.frequency, m_mapDesc.bound.lo, m_mapDesc.bound.hi, glm::f64vec2(x, y));
        }
    }

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
    pos += glm::f64vec3(50.0, 50.0, 0.0);
    pos /= 100.0;
    pos = glm::floor(pos);
    if (m_chunkLoc.x != pos.x || m_chunkLoc.y != pos.y) {
        for (i32 x = -50; x <= 50; ++x) {
            for (i32 y = -50; y <= 50; ++y) {
                m_heightmap[(x + 50) + 101 * (y + 50)] = hproc::Noise::getNoiseValue(glm::f64vec2(x + (101 * pos.x), y + (101 * pos.y)), m_mapDesc);
                //m_heightmap[(x + 50) + 101 * (y + 50)] = hm::Fractal::genSimplexWithOctavesScaled(m_mapDesc.octaves, m_mapDesc.persistence, m_mapDesc.frequency, m_mapDesc.bound.lo, m_mapDesc.bound.hi, glm::f64vec2(x, y));
            }
        }
        m_chunkLoc.x = pos.x;
        m_chunkLoc.y = pos.y;
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
    for (f32 x = -50.0f; x <= 50.0f; ++x) {
        for (f32 y = -50.0f; y <= 50.0f; ++y) {
            for (f32 z = -2.0f; z <= m_heightmap[((ui32)x + 50) + 101 * ((ui32)y + 50)]; ++z) {
                glm::f32mat4 model = glm::translate(glm::f32mat4(), glm::f32vec3(x + (101 * m_chunkLoc.x), y + (101 * m_chunkLoc.y), z));
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

                glDrawArrays(GL_TRIANGLES, 0, 36);
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
