#include "stdafx.h"

#include <Hemlock.hpp>
#include <camera\BasicCamera.h>
#include <graphics\GLSLProgram.h>
#include <graphics\Window.h>
#include <ui\InputDispatcher.h>

int main(int argc, char* argv[]) {
    h::init();
    hg::Window window = hg::Window();

    hui::InputManager manager;
    hui::InputDispatcher* dispatcher = hui::InputDispatcher::getInstance();
    dispatcher->init(&window, &manager);
    
    hcam::BasicCamera camera;
    camera.attachToWindow(&window);
    camera.setPosition(glm::f32vec3(0.0f, 0.0f, -1.5f));
    camera.setFov(45.0f);
    camera.update();
        
    hg::GLSLProgram shader = hg::GLSLProgram();
    shader.init();
    shader.addShaders("shaders/simple.frag", "shaders/simple.vert");
    shader.link();
    
    h::runTEMP(&window, &shader, &camera);

    shader.dispose();
    h::dispose();

    getchar();
    return 0;
}
