#pragma once

namespace hemlock {
    namespace maths {
        namespace Fractal {
            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            glm::tvec2<FXX, glm::highp> genCellular(glm::tvec2<FXX, glm::highp> coords);
            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            glm::tvec2<FXX, glm::highp> genCellular(glm::tvec3<FXX, glm::highp> coords);
            //TODO(Matthew): Scaled cellular and octave-based cellular?

            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            FXX genSimplexWithOctaves(ui8 octaves, FXX persistence, FXX scale, glm::tvec2<FXX, glm::highp> coords);
            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            FXX genSimplexWithOctaves(ui8 octaves, FXX persistence, FXX scale, glm::tvec3<FXX, glm::highp> coords);
            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            FXX genSimplexWithOctaves(ui8 octaves, FXX persistence, FXX scale, glm::tvec4<FXX, glm::highp> coords);

            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            FXX genSimplexWithOctavesScaled(ui8 octaves, FXX persistence, FXX scale, FXX loBound, FXX hiBound, glm::tvec2<FXX, glm::highp> coords);
            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            FXX genSimplexWithOctavesScaled(ui8 octaves, FXX persistence, FXX scale, FXX loBound, FXX hiBound, glm::tvec3<FXX, glm::highp> coords);
            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            FXX genSimplexWithOctavesScaled(ui8 octaves, FXX persistence, FXX scale, FXX loBound, FXX hiBound, glm::tvec4<FXX, glm::highp> coords);

            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            FXX genSimplex(glm::tvec2<FXX, glm::highp> coords);
            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            FXX genSimplex(glm::tvec3<FXX, glm::highp> coords);
            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            FXX genSimplex(glm::tvec4<FXX, glm::highp> coords);

            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            FXX genSimplexScaled(FXX loBound, FXX hiBound, glm::tvec2<FXX, glm::highp> coords);
            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            FXX genSimplexScaled(FXX loBound, FXX hiBound, glm::tvec3<FXX, glm::highp> coords);
            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            FXX genSimplexScaled(FXX loBound, FXX hiBound, glm::tvec4<FXX, glm::highp> coords);
        }
    }
}
namespace hm = hemlock::maths;
