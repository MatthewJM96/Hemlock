#include "stdafx.h"

#include "MainMenuScreen.h"

#include <IL\il.h>

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

    f32 ratio = m_camera.getAspectRatio(); // Cheap trick to make image square
    hg::Vertex3D<f32> vertices[4] = {
        {  0.06f,  0.06f * ratio, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },  // Top Right
        {  0.06f, -0.06f * ratio, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f },  // Bottom Right
        { -0.06f, -0.06f * ratio, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f },  // Bottom Left
        { -0.06f,  0.06f * ratio, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f }   // Top Left 
    };
    GLuint* indices = new GLuint[6] {
        0, 1, 3,  // First Triangle
        1, 2, 3   // Second Triangle
    };

    //////////////////////////////////////////////////////////////
    // TODO(Matthew): Replace current image loading with DevIL. //
    //     Also, figure out ILUT and utilise as it likely hand- //
    //     -les metadata better than we could. Finally, make a  //
    //     class/function that handles uploading mesh data AND  //
    //     preparing the textures associated.                   //
    //////////////////////////////////////////////////////////////

    //hio::BitmapResource bmp = hio::load("textures/uvtemplate.bmp", hio::ImageIOFormat::RAW);
    ILuint image;
    ilGenImages(1, &image);
    ilBindImage(image);

    ilLoadImage("textures/anfo.png");

    ILuint width = ilGetInteger(IL_IMAGE_WIDTH);
    ILuint height = ilGetInteger(IL_IMAGE_HEIGHT);
    ILubyte* data = ilGetData();
    
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    hg::MeshData3D<f32> m_vData = {};
    m_vData.vertices = &vertices[0];
    m_vData.vertexCount = 4;
    m_vData.indices = &indices[0];
    m_vData.indexCount = 6;

    m_vao1 = hg::createVAO(m_vData);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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

    glBindTexture(GL_TEXTURE_2D, m_texture);
    glBindVertexArray(m_vao1);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    //glBindVertexArray(m_vao2);
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    m_shader.unuse();
}
