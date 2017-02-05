#pragma once

#include <type_traits>
#include <gl\glew.h>
#include "../types.h"

namespace hemlock {
    namespace graphics {
        enum class VertexDataVolatility {
            STATIC  = GL_STATIC_DRAW,
            DYNAMIC = GL_DYNAMIC_DRAW,
            STREAM  = GL_STREAM_DRAW
        };

        template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
        struct Vertex3D {
            FXX x, y, z, r, g, b, a, u, v;
        };

        template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
        struct MeshData3D {
            Vertex3D<FXX>* vertices;
            ui32           vertexCount;
            ui32*          indices;
            ui32           indexCount;
        };

        // TODO(Matthew): Figure out if certain bits (looking at glEnableVertexAttribArray) can't be so-generalised.
        // TODO(Matthew): Make class so we can properly dispose of VBO and EBO.
        template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
        inline GLuint createVAO(MeshData3D<FXX> vertexData, VertexDataVolatility volatility = VertexDataVolatility::STATIC) {
            if (!vertexData.vertices) return 0;

            GLuint vao;
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);

            GLuint vbo;
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex3D<FXX>) * vertexData.vertexCount, vertexData.vertices, (int)volatility);

            if (vertexData.indices != nullptr) {
                GLuint ebo;
                glGenBuffers(1, &ebo);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(FXX) * vertexData.indexCount, vertexData.indices, (int)volatility);
            }

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D<FXX>), 0);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex3D<FXX>), (GLvoid*)(3 * sizeof(FXX)));
            glEnableVertexAttribArray(1);

            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D<FXX>), (GLvoid*)(7 * sizeof(FXX)));
            glEnableVertexAttribArray(2);

            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            if (vertexData.indices != nullptr) glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            return vao;
        }
    }
}
namespace hg = hemlock::graphics;
