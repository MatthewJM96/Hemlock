#include "stdafx.h"

#include "MainMenuScreen.h"

void MainMenuScreen::init(char* name) {
    IScreen::init(name);

    m_camera.setPosition(glm::f32vec3(0.0f, 0.0f, -1.5f));
    m_camera.setFov(45.0f);
    m_camera.update();

    m_shader.init();
    m_shader.addShaders("shaders/simple.frag", "shaders/simple.vert");
    m_shader.link();

    //GLuint mvpLoc = shader->getUniformLocation("MVP");

    GLfloat* vertices = new GLfloat[12] {
        0.5f,  0.5f, 0.0f,  // Top Right
        0.5f, -0.5f, 0.0f,  // Bottom Right
        -0.5f, -0.5f, 0.0f,  // Bottom Left
        -0.5f,  0.5f, 0.0f   // Top Left 
    };
    GLuint* indices = new GLuint[6] {
        0, 1, 3,  // First Triangle
        1, 2, 3   // Second Triangle
    };

    m_vData.positions = &vertices[0];
    m_vData.positionCount = 4;
    m_vData.indices = &indices[0];
    m_vData.indexCount = 6;

    m_vao = hg::createVAO(m_vData);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void MainMenuScreen::update(TimeData time) {
    m_shader.dispose();
    glDeleteVertexArrays(1, &m_vao);
}

void MainMenuScreen::draw(TimeData time) {
    m_camera.update();
    m_shader.use();

    //glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT /*| GL_DEPTH_BUFFER_BIT*/);

    //glm::f32mat4 mvp = camera->getViewProjectionMatrix();
    //glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &mvp[0][0]);

    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    m_shader.unuse();
}
