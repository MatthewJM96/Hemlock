#pragma once

#include <app\IScreen.h>
#include <camera\BasicCamera.h>
#include <graphics\GLSLProgram.h>

#include <graphics\Mesh.hpp>

class MainMenuScreen : public happ::IScreen {
public:
    void init(char* name);
    void dispose();

    void update(TimeData time);
    void draw(TimeData time);
private:
    hcam::BasicCamera m_camera;
    hg::GLSLProgram   m_shader;
    GLuint            m_vao1;
    GLuint            m_vao2;
    GLuint            m_texture;
};
