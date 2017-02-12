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

		struct Quad {
			Face		 face;
			glm::f32mat4 translationMatrix;
		};

		struct ChunkMesh {
			std::vector<Quad> quads;
		};
	}
}
namespace hvox = hemlock::voxel;
