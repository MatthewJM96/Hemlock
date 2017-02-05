#include "stdafx.h"

#include "MainMenuScreen.h"

#include <IL\il.h>

#include <io\ImageLoader.h>
#include <graphics\Texture.h>

void MainMenuScreen::init(char* name) {
    if (m_initialised) return;
    IScreen::init(name);

    ilInit();

    m_camera.setPosition(glm::f32vec3(0.0f, 0.0f, -1.5f));
    m_camera.setFov(45.0f);
    m_camera.update();

    m_shader.init();
    m_shader.addShaders("shaders/simple.frag", "shaders/simple.vert");
    m_shader.link();

    //GLuint mvpLoc = shader->getUniformLocation("MVP");
    
    m_texture1 = hg::Texture::load("textures/container.jpg", true);
    m_texture2 = hg::Texture::load("textures/anfo.png", true);

    hg::Vertex3D<f32> vertices[4] = {
        { 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },  // Top Right
        { 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f },  // Bottom Right
        { -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f },  // Bottom Left
        { -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f }   // Top Left 
    };
    GLuint* indices = new GLuint[6]{
        0, 1, 3,  // First Triangle
        1, 2, 3   // Second Triangle
    };

    hg::MeshData3D<f32> m_vData = {};
    m_vData.vertices = &vertices[0];
    m_vData.vertexCount = 4;
    m_vData.indices = &indices[0];
    m_vData.indexCount = 6;

    m_vao1 = hg::createVAO(m_vData);

    //hg::Vertex3D<f32> vertices2[4] = {
    //    {  1.5f,  1.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f },  // Top Right
    //    {  1.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },  // Bottom Right
    //    {  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f },  // Bottom Left
    //    {  0.5f,  1.5f, 0.0f, 0.6f, 0.3f, 0.5f, 1.0f }   // Top Left 
    //};
    //GLuint* indices2 = new GLuint[6]{
    //    0, 1, 3,  // First Triangle
    //    1, 2, 3   // Second Triangle
    //};

    //hg::MeshData3D<f32> m_vData2 = {};
    //m_vData2.vertices = &vertices2[0];
    //m_vData2.vertexCount = 4;
    //m_vData2.indices = &indices2[0];
    //m_vData2.indexCount = 6;

    //m_vao2 = hg::createVAO(m_vData2);

    GLint nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    printf("Maximum # of vertex attributes supported: %d.\n", nrAttributes);

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
    // Empty
}

void MainMenuScreen::draw(TimeData time) {
    m_camera.update();
    m_shader.use();

    //glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT /*| GL_DEPTH_BUFFER_BIT*/);

    //glm::f32mat4 mvp = camera->getViewProjectionMatrix();
    //glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &mvp[0][0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture1);
    glUniform1i(m_shader.getUniformLocation("tex1"), 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_texture2);
    glUniform1i(m_shader.getUniformLocation("tex2"), 1);


    glBindVertexArray(m_vao1);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    //glBindVertexArray(m_vao2);
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    m_shader.unuse();
}
