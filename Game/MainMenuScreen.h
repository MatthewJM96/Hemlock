#pragma once

#include <ui\InputDispatcher.h>
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

    void handleMouseMovement(h::Sender sender, hui::MouseMoveEvent event);
    void handleKeyboardDown(h::Sender sender, hui::KeyboardButtonEvent event);
    void handleKeyboardUp(h::Sender sender, hui::KeyboardButtonEvent event);
private:
    hcam::BasicCamera m_camera;
    hg::GLSLProgram   m_shader;
    GLuint            m_vao1;
    GLuint            m_vao2;
    GLuint            m_texture1;
    GLuint            m_texture2;
    GLuint            m_mvpLoc;

    bool m_wPressed = false;
    bool m_aPressed = false;
    bool m_sPressed = false;
    bool m_dPressed = false;
};
