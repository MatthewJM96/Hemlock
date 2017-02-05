#include "stdafx.h"

#include "MainMenuScreen.h"

#include <IL\il.h>

#include <io\ImageLoader.h>
#include <graphics\Texture.h>
#include <glm\gtc\matrix_transform.hpp>

void MainMenuScreen::init(char* name) {
    if (m_initialised) return;
    IScreen::init(name);

    const auto& inputDispatcher = hui::InputDispatcher::getInstance();
    inputDispatcher->onMouseMove += h::makeDelegate(*this, &MainMenuScreen::handleMouseMovement);
    inputDispatcher->onKeyboardButtonDown += h::makeDelegate(*this, &MainMenuScreen::handleKeyboardDown);
    inputDispatcher->onKeyboardButtonUp += h::makeDelegate(*this, &MainMenuScreen::handleKeyboardUp);
    
    m_camera.setPosition(glm::f32vec3(5.0f, 0.0f, -1.5f));
    m_camera.setFov(45.0f);
    m_camera.update();

    m_shader.init();
    m_shader.addShaders("shaders/simple.frag", "shaders/simple.vert");
    m_shader.link();
    //m_mvpLoc = m_shader.getUniformLocation("MVP");
        
    m_texture1 = hg::Texture::load("textures/container.jpg", true);
    m_texture2 = hg::Texture::load("textures/anfo.png", true);

    hg::Vertex3D<f32> vertices[36] = {
        {-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f, },
        { 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f,  1.0f, 0.0f, },
        { 0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f,  1.0f, 1.0f, },
        { 0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f,  1.0f, 1.0f, },
        { -0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f,  0.0f, 1.0f, },
        { -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, },
        
        { -0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f, },
        { 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, },
        { 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, },
        { 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, },
        { -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f,  0.0f, 1.0f, },
        { -0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f, },

        { -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, },
        { -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, },
        { -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, },
        { -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f,  0.0f, 1.0f, },
        { -0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f, },
        { -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f,  1.0f, 0.0f, },

        { 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, },
        { 0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, },
        { 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, },
        { 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, },
        { 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, },
        { 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, },

        { -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f,  0.0f, 1.0f, },
        { 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, },
        { 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, },
        { 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, },
        { -0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f, },
        { -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, },

        { -0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f, 1.0f,0.0f, 1.0f, },
        { 0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, },
        { 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,},
        { 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,},
        { -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f,},
        { -0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f,  0.0f, 1.0f }
    };
   /* GLuint* indices = new GLuint[6]{
        0, 1, 3,  // First Triangle
        1, 2, 3   // Second Triangle
    };*/

    hg::MeshData3D<f32> m_vData = {};
    m_vData.vertices = &vertices[0];
    m_vData.vertexCount = 36;
    m_vData.indices = nullptr;
    //m_vData.indices = &indices[0];
    //m_vData.indexCount = 6;

    m_vao1 = hg::createVAO(m_vData);

    GLint nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    printf("Maximum # of vertex attributes supported: %d.\n", nrAttributes);

    glEnable(GL_DEPTH_TEST);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void MainMenuScreen::dispose() {
    if (!m_initialised) return;

    m_shader.dispose();
    glDeleteVertexArrays(1, &m_vao1);
    glDeleteVertexArrays(1, &m_vao2);

    IScreen::dispose();
}

void MainMenuScreen::update(TimeData time) {
    if (m_wPressed) {
        m_camera.offsetPosition(0.05f * m_camera.getDirection());
    }
    if (m_aPressed) {
        m_camera.offsetPosition(-0.05f * m_camera.getRight());
    }
    if (m_sPressed) {
        m_camera.offsetPosition(-0.05f * m_camera.getDirection());
    }
    if (m_dPressed) {
        m_camera.offsetPosition(0.05f * m_camera.getRight());
    }
}

void MainMenuScreen::draw(TimeData time) {
    static glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    m_camera.update();
    m_shader.use();

    //glClearDepth(1.0);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Crappy but simple way to test.
    //glm::f32mat4 mvp = m_camera.getViewProjectionMatrix();
    glm::f32mat4 view;
    glm::f32mat4 projection;
    view = m_camera.getViewMatrix();
    projection = m_camera.getProjectionMatrix();
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

    glBindVertexArray(m_vao1);
    for (ui32 i = 0; i < 10; ++i) {
        glm::f32mat4 model = glm::translate(glm::f32mat4(), cubePositions[i]);
        GLfloat angle = 20.0f * i;
        //model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
        model = glm::rotate(model, glm::radians((GLfloat)SDL_GetTicks()) / 10.0f, glm::vec3((f32)i * 0.05f, 1.0f - ((f32)i / 10.0f), (i < 5 ? 0.5f : -0.5f)));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

        glDrawArrays(GL_TRIANGLES, 0, 36);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
    glBindVertexArray(0);

    m_shader.unuse();
}

void MainMenuScreen::handleMouseMovement(h::Sender sender, hui::MouseMoveEvent event) {
    m_camera.rotateFromMouse(-event.dx, -event.dy, 0.01f);
}

void MainMenuScreen::handleKeyboardDown(h::Sender sender, hui::KeyboardButtonEvent event) {
    switch (event.physicalKey) {
    case SDL_SCANCODE_W:
        m_wPressed = true;
        break;
    case SDL_SCANCODE_A:
        m_aPressed = true;
        break;
    case SDL_SCANCODE_S:
        m_sPressed = true;
        break;
    case SDL_SCANCODE_D:
        m_dPressed = true;
        break;
    }
}

void MainMenuScreen::handleKeyboardUp(h::Sender sender, hui::KeyboardButtonEvent event) {
    switch (event.physicalKey) {
        case SDL_SCANCODE_W:
            m_wPressed = false;
            break;
        case SDL_SCANCODE_A:
            m_aPressed = false;
            break;
        case SDL_SCANCODE_S:
            m_sPressed = false;
            break;
        case SDL_SCANCODE_D:
            m_dPressed = false;
            break;
    }
}
