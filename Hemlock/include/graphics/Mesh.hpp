#pragma once

#include <gl\glew.h>
#include "../types.h"

namespace hemlock {
    namespace graphics {
        enum class VertexDataVolatility {
            STATIC  = GL_STATIC_DRAW,
            DYNAMIC = GL_DYNAMIC_DRAW,
            STREAM  = GL_STREAM_DRAW
        };

        struct VertexData {
            VertexData() {};
            VertexData(f32* positions, f32* colours, ui32* indices, ui32 positionCount, ui32 indexCount) :
                positions(positions), colours(colours), indices(indices), positionCount(positionCount), indexCount(indexCount) {};
            f32* positions;
            f32* colours;
            ui32* indices;
            ui32 positionCount;
            ui32 indexCount;
        };

        // TODO(Matthew): Figure out if certain bits (looking at glEnableVertexAttribArray) can't be so-generalised.
        // TODO(Matthew): Make class so we can properly dispose of VBO and EBO.
        inline GLuint createVAO(VertexData vertexData, VertexDataVolatility volatility = VertexDataVolatility::STATIC) {
            GLuint vao;
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);

            GLuint vbo;
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * 3 * vertexData.positionCount, vertexData.positions, (int)volatility);

            GLuint ebo;
            glGenBuffers(1, &ebo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(f32) * vertexData.indexCount, vertexData.indices, (int)volatility);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(f32), 0);
            glEnableVertexAttribArray(0);

            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            return vao;
        }
    }
}
namespace hg = hemlock::graphics;
