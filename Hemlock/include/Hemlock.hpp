#pragma once

#include "stdafx.h"

#include "camera\BasicCamera.h"
#include "graphics\GLSLProgram.h"
#include "graphics\Mesh.hpp"
#include "graphics\Window.h"

namespace hemlock {
    void init() {
        FreeImage_Initialise();

        // TODO(Matthew): Move this elsewhere.
        SDL_Init(SDL_INIT_EVERYTHING);
        SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    }

    void dispose() {
        FreeImage_DeInitialise();
    }

    // TODO(Matthew): Once screens are implemented make one in Game and move this there.
    void runTEMP(hg::Window* window, hg::GLSLProgram* shader, hcam::BasicCamera* camera) {
        //GLuint mvpLoc = shader->getUniformLocation("MVP");

        GLfloat vertices[] = {
            0.5f,  0.5f, 0.0f,  // Top Right
            0.5f, -0.5f, 0.0f,  // Bottom Right
            -0.5f, -0.5f, 0.0f,  // Bottom Left
            -0.5f,  0.5f, 0.0f   // Top Left 
        };
        GLuint indices[] = {
            0, 1, 3,  // First Triangle
            1, 2, 3   // Second Triangle
        };

        hg::VertexData vData = {};
        vData.positions = &vertices[0];
        vData.positionCount = 4;
        vData.indices = &indices[0];
        vData.indexCount = 6;

        GLuint vao = hg::createVAO(vData);

        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        while (true) {
            SDL_PumpEvents();
            camera->update();
            shader->use();

            //glClearDepth(1.0);
            glClear(GL_COLOR_BUFFER_BIT /*| GL_DEPTH_BUFFER_BIT*/);

            //glm::f32mat4 mvp = camera->getViewProjectionMatrix();
            //glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &mvp[0][0]);

            glBindVertexArray(vao);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            shader->unuse();
            window->sync();
        }

        glDeleteVertexArrays(1, &vao);
    }
}
namespace h = hemlock;
