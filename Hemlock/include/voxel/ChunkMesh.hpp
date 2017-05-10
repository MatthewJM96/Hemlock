#pragma once

namespace hemlock {
    namespace voxel {
        enum class Face {
            TOP,
            BOTTOM,
            LEFT,
            RIGHT,
            FRONT,
            BACK
        };
        
        struct ChunkMesh {
            GLuint       vao;
            ui64         indices;
            glm::f32mat4 translationMatrix;
        };
    }
}
namespace hvox = hemlock::voxel;
