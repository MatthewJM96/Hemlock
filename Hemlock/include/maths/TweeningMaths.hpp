#pragma once

#include <glm\glm.hpp>
#include "types.h"

namespace hemlock {
    namespace maths {
        // TODO(Matthew): Support double precision tweening.

        /*! @brief Computes a linear tweening.
        */
        inline f32 linear(f32 startVal, f32 finalVal, f32 alpha) {
            f32 range = finalVal - startVal;
            return startVal + range * alpha;
        }
        inline f32 linear(f32 startVal, f32 finalVal, unsigned int stageCount, unsigned int stage) {
            if (stageCount == 0) return;
            return linear(startVal, finalVal, (f32)stage / (f32)stageCount);
        }

        /*! @brief Computes an easeInQuad tweening.
        */
        inline f32 easeInQuad(f32 startVal, f32 finalVal, f32 alpha) {
            f32 range = finalVal - startVal;
            return range * alpha * alpha + startVal;
        }
        inline f32 easeInQuad(f32 startVal, f32 finalVal, unsigned int stageCount, unsigned int stage) {
            return easeInQuad(startVal, finalVal, (f32)stage / (f32)stageCount);
        }

        /*! @brief Computes an easeOutQuad tweening.
        */
        inline f32 easeOutQuad(f32 startVal, f32 finalVal, f32 alpha) {
            f32 range = finalVal - startVal;
            return -1.0f * range * alpha * (alpha - 2.0f) + startVal;
        }
        inline f32 easeOutQuad(f32 startVal, f32 finalVal, unsigned int stageCount, unsigned int stage) {
            return easeOutQuad(startVal, finalVal, (f32)stage / (f32)stageCount);
        }

        /*! @brief Computes an easeInOutQuad tweening.
        */
        inline f32 easeInOutQuad(f32 startVal, f32 finalVal, f32 alpha) {
            f32 range = finalVal - startVal;
            f32 alpha2 = alpha * 2.0f;
            if (alpha2 < 1.0f) {
                return range / 2.0f * alpha2 * alpha2 + startVal;
            }
            f32 alpha2Minus1 = alpha2 - 1.0f;
            return -1.0f * range / 2.0f * (alpha2Minus1 * (alpha2Minus1 - 2.0f) - 1.0f) + startVal;
        }
        inline f32 easeInOutQuad(f32 startVal, f32 finalVal, unsigned int stageCount, unsigned int stage) {
            return easeInOutQuad(startVal, finalVal, (f32)stage / (f32)stageCount);
        }

        /*! @brief Computes an easeInCubic tweening.
        */
        inline f32 easeInCubic(f32 startVal, f32 finalVal, f32 alpha) {
            f32 range = finalVal - startVal;
            return range * alpha * alpha * alpha + startVal;
        }
        inline f32 easeInCubic(f32 startVal, f32 finalVal, unsigned int stageCount, unsigned int stage) {
            return easeInCubic(startVal, finalVal, (f32)stage / (f32)stageCount);
        }

        /*! @brief Computes an easeOutCubic tweening.
        */
        inline f32 easeOutCubic(f32 startVal, f32 finalVal, f32 alpha) {
            f32 range = finalVal - startVal;
            f32 alphaMinus1 = alpha - 1.0f;
            return range * (alphaMinus1 * alphaMinus1 * alphaMinus1 + 1.0f) + startVal;
        }
        inline f32 easeOutCubic(f32 startVal, f32 finalVal, unsigned int stageCount, unsigned int stage) {
            return easeOutCubic(startVal, finalVal, (f32)stage / (f32)stageCount);
        }

        /*! @brief Computes an easeInOutCubic tweening.
        */
        inline f32 easeInOutCubic(f32 startVal, f32 finalVal, f32 alpha) {
            f32 range = finalVal - startVal;
            f32 alpha2 = alpha * 2.0f;
            if (alpha2 < 1.0f) {
                return range / 2.0f * alpha2 * alpha2 * alpha2 + startVal;
            }
            f32 alpha2Minus2 = alpha2 - (f32)2.0f;
            return range / 2.0f * (alpha2Minus2 * alpha2Minus2 * alpha2Minus2 + 2.0f) + startVal;
        }
        inline f32 easeInOutCubic(f32 startVal, f32 finalVal, unsigned int stageCount, unsigned int stage) {
            return easeInOutCubic(startVal, finalVal, (f32)stage / (f32)stageCount);
        }

        /*! @brief Computes an easeInQuart tweening.
        */
        inline f32 easeInQuart(f32 startVal, f32 finalVal, f32 alpha) {
            f32 range = finalVal - startVal;
            return range * alpha * alpha * alpha * alpha + startVal;
        }
        inline f32 easeInQuart(f32 startVal, f32 finalVal, unsigned int stageCount, unsigned int stage) {
            return easeInQuart(startVal, finalVal, (f32)stage / (f32)stageCount);
        }

        /*! @brief Computes an easeOutQuart tweening.
        */
        inline f32 easeOutQuart(f32 startVal, f32 finalVal, f32 alpha) {
            f32 range = finalVal - startVal;
            f32 alphaMinus1 = alpha - 1.0f;
            return -1.0f * range * (alphaMinus1 * alphaMinus1 * alphaMinus1 * alphaMinus1 - 1.0f) + startVal;
        }
        inline f32 easeOutQuart(f32 startVal, f32 finalVal, unsigned int stageCount, unsigned int stage) {
            return easeOutQuart(startVal, finalVal, (f32)stage / (f32)stageCount);
        }

        /*! @brief Computes an easeInOutQuart tweening.
        */
        inline f32 easeInOutQuart(f32 startVal, f32 finalVal, f32 alpha) {
            f32 range = finalVal - startVal;
            f32 alpha2 = alpha * 2.0f;
            if (alpha2 < 1.0f) {
                return range / 2.0f * alpha2 * alpha2 * alpha2 * alpha2 + startVal;
            }
            f32 alpha2Minus2 = alpha2 - 2.0f;
            return -1.0f * range / 2.0f * (alpha2Minus2 * alpha2Minus2 * alpha2Minus2 * alpha2Minus2 - 2.0f) + startVal;
        }
        inline f32 easeInOutQuart(f32 startVal, f32 finalVal, unsigned int stageCount, unsigned int stage) {
            return easeInOutQuart(startVal, finalVal, (f32)stage / (f32)stageCount);
        }

        /*! @brief Computes an easeInQuint tweening.
        */
        inline f32 easeInQuint(f32 startVal, f32 finalVal, f32 alpha) {
            f32 range = finalVal - startVal;
            return range * alpha * alpha * alpha * alpha * alpha + startVal;
        }
        template <typename f32>
        inline f32 easeInQuint(f32 startVal, f32 finalVal, unsigned int stageCount, unsigned int stage) {
            return easeInQuint(startVal, finalVal, (f32)stage / (f32)stageCount);
        }

        /*! @brief Computes an easeOutQuint tweening.
        */
        inline f32 easeOutQuint(f32 startVal, f32 finalVal, f32 alpha) {
            f32 range = finalVal - startVal;
            f32 alphaMinus1 = alpha - 1.0f;
            return range * (alphaMinus1 * alphaMinus1 * alphaMinus1 * alphaMinus1 * alphaMinus1 + (f32)1.0f) + startVal;
        }
        inline f32 easeOutQuint(f32 startVal, f32 finalVal, unsigned int stageCount, unsigned int stage) {
            return easeOutQuint(startVal, finalVal, (f32)stage / (f32)stageCount);
        }

        /*! @brief Computes an easeInOutQuint tweening.
        */
        inline f32 easeInOutQuint(f32 startVal, f32 finalVal, f32 alpha) {
            f32 range = finalVal - startVal;
            f32 alpha2 = alpha * 2.0f;
            if (alpha2 < 1.0f) {
                return range / 2.0f * alpha2 * alpha2 * alpha2 * alpha2 * alpha2 + startVal;
            }
            f32 alpha2Minus2 = alpha2 - (f32)2.0f;
            return range / 2.0f * (alpha2Minus2 * alpha2Minus2 * alpha2Minus2 * alpha2Minus2 * alpha2Minus2 + (f32)2.0f) + startVal;
        }
        inline f32 easeInOutQuint(f32 startVal, f32 finalVal, unsigned int stageCount, unsigned int stage) {
            return easeInOutQuint(startVal, finalVal, (f32)stage / (f32)stageCount);
        }

        /*! @brief Computes an easeInSine tweening.
        */
        inline f32 easeInSine(f32 startVal, f32 finalVal, f32 alpha) {
            f32 range = finalVal - startVal;
            return (f32)-1.0f * range * glm::cos(alpha * M_TAUF) + range + startVal;
        }
        inline f32 easeInSine(f32 startVal, f32 finalVal, unsigned int stageCount, unsigned int stage) {
            return easeInSine(startVal, finalVal, (f32)stage / (f32)stageCount);
        }

        /*! @brief Computes an easeOutSine tweening.
        */
        inline f32 easeOutSine(f32 startVal, f32 finalVal, f32 alpha) {
            f32 range = finalVal - startVal;
            return range * glm::sin(alpha * M_TAUF) + startVal;
        }
        inline f32 easeOutSine(f32 startVal, f32 finalVal, unsigned int stageCount, unsigned int stage) {
            return easeOutSine(startVal, finalVal, (f32)stage / (f32)stageCount);
        }

        /*! @brief Computes an easeInOutSine tweening.
        */
        inline f32 easeInOutSine(f32 startVal, f32 finalVal, f32 alpha) {
            f32 range = finalVal - startVal;
            return -1.0f * range / 2.0f * (glm::cos(alpha * M_PIF) - 1.0f) + startVal;
        }
        inline f32 easeInOutSine(f32 startVal, f32 finalVal, unsigned int stageCount, unsigned int stage) {
            return easeInOutSine(startVal, finalVal, (f32)stage / (f32)stageCount);
        }

        /*! @brief Computes an easeInExpo tweening.
        */
        inline f32 easeInExpo(f32 startVal, f32 finalVal, f32 alpha) {
            f32 range = finalVal - startVal;
            return (alpha == 0.0f) ? startVal : range * glm::pow(2.0f, 10.0f * (alpha - 1.0f)) + startVal;
        }
        inline f32 easeInExpo(f32 startVal, f32 finalVal, unsigned int stageCount, unsigned int stage) {
            return easeInExpo(startVal, finalVal, (f32)stage / (f32)stageCount);
        }

        /*! @brief Computes an easeOutExpo tweening.
        */
        inline f32 easeOutExpo(f32 startVal, f32 finalVal, f32 alpha) {
            f32 range = finalVal - startVal;
            return (alpha == 1.0f) ? finalVal : range * (-1.0f * glm::pow(2.0f, -10.0f * alpha) + 1.0f) + startVal;
        }
        inline f32 easeOutExpo(f32 startVal, f32 finalVal, unsigned int stageCount, unsigned int stage) {
            return easeOutExpo(startVal, finalVal, (f32)stage / (f32)stageCount);
        }

        /*! @brief Computes an easeInOutExpo tweening.
        */
        inline f32 easeInOutExpo(f32 startVal, f32 finalVal, f32 alpha) {
            f32 range = finalVal - startVal;
            //if (alpha == 0.0f) return startVal;
            //if (alpha == 1.0f) return finalVal;
            f32 alpha2 = alpha * 2.0f;
            if (alpha2 < 1.0f) {
                return range / 2.0f * glm::pow(2.0f, 10.0f * (alpha2 - 1.0f)) + startVal;
            }
            return range / 2.0f * -1.0f * glm::pow(2.0f, -10.0f * (alpha2 - 1.0f) + 2.0f) + startVal;
        }
        inline f32 easeInOutExpo(f32 startVal, f32 finalVal, unsigned int stageCount, unsigned int stage) {
            return easeInOutExpo(startVal, finalVal, (f32)stage / (f32)stageCount);
        }

        /*! @brief Computes an easeInCirc tweening.
        */
        inline f32 easeInCirc(f32 startVal, f32 finalVal, f32 alpha) {
            f32 range = finalVal - startVal;
            return -1.0f * range * (glm::sqrt(1.0f - alpha * alpha) - 1.0f) + startVal;
        }
        inline f32 easeInCirc(f32 startVal, f32 finalVal, unsigned int stageCount, unsigned int stage) {
            return easeInCirc(startVal, finalVal, (f32)stage / (f32)stageCount);
        }

        /*! @brief Computes an easeOutCirc tweening.
        */
        inline f32 easeOutCirc(f32 startVal, f32 finalVal, f32 alpha) {
            f32 range = finalVal - startVal;
            f32 alphaMinus1 = alpha - 1.0f;
            return range * glm::sqrt(1.0f - alphaMinus1 * alphaMinus1) + startVal;
        }
        inline f32 easeOutCirc(f32 startVal, f32 finalVal, unsigned int stageCount, unsigned int stage) {
            return easeOutCirc(startVal, finalVal, (f32)stage / (f32)stageCount);
        }

        /*! @brief Computes an easeInOutCirc tweening.
        */
        inline f32 easeInOutCirc(f32 startVal, f32 finalVal, f32 alpha) {
            f32 range = finalVal - startVal;
            f32 alpha2 = alpha * 2.0f;
            if (alpha2 < 1.0f) {
                return -1.0f * range / 2.0f * (glm::sqrt(1.0f - alpha2 * alpha2) - 1.0f) + startVal;
            }
            f32 alpha2Minus2 = alpha2 - 2.0f;
            return range / 2.0f * (glm::sqrt(1.0f - alpha2Minus2 * alpha2Minus2) + 1.0f) + startVal;
        }
        inline f32 easeInOutCirc(f32 startVal, f32 finalVal, unsigned int stageCount, unsigned int stage) {
            return easeInOutCirc(startVal, finalVal, (f32)stage / (f32)stageCount);
        }

        /*! @brief Computes an easeInElastic tweening.
        */
        inline f32 easeInElastic(f32 startVal, f32 finalVal, f32 alpha) {
            f32 range = finalVal - startVal;
            //if (alpha == 0.0f) return startVal;
            //if (alpha == 1.0f) return finalVal;
            f32 alphaMinus1 = alpha - 1;
            return -1.0f * range * glm::pow(2.0f, 10.0f * alphaMinus1) * glm::sin(M_PIF * (20.0f / 3.0f * alphaMinus1 - 0.5f)) + startVal;
        }
        inline f32 easeInElastic(f32 startVal, f32 finalVal, unsigned int stageCount, unsigned int stage) {
            return easeInElastic(startVal, finalVal, (f32)stage / (f32)stageCount);
        }

        /*! @brief Computes an easeOutElastic tweening.
        */
        inline f32 easeOutElastic(f32 startVal, f32 finalVal, f32 alpha) {
            f32 range = finalVal - startVal;
            //if (alpha == 0.0f) return startVal;
            //if (alpha == 1.0f) return finalVal;
            return range * glm::pow(2.0f, -10.0f * alpha) * glm::sin(M_PIF * (20.0f / 3.0f * alpha - 0.5f)) * 0.5f + finalVal;
        }
        inline f32 easeOutElastic(f32 startVal, f32 finalVal, unsigned int stageCount, unsigned int stage) {
            return easeOutElastic(startVal, finalVal, (f32)stage / (f32)stageCount);
        }

        /*! @brief Computes an easeInOutElastic tweening.
        */
        inline f32 easeInOutElastic(f32 startVal, f32 finalVal, f32 alpha) {
            f32 range = finalVal - startVal;
            //if (alpha == 0.0f) return startVal;
            //if (alpha == 1.0f) return finalVal;
            f32 alpha2 = 2.0f * alpha;
            f32 alpha2Minus1 = alpha2 - 1.0f;
            if (alpha2 < 1.0f) {
                return -0.5f * range * glm::pow(2.0f, 10.0f * alpha2Minus1) * glm::sin(M_PIF * (40.0f / 9.0f * alpha2Minus1 - 0.5f)) + startVal;
            }
            return range * glm::pow(2.0f, -10.0f * alpha2Minus1) * glm::sin(M_PIF * (40.0f / 9.0f * alpha2Minus1 - 0.5f)) * 0.5f + finalVal;
        }
        inline f32 easeInOutElastic(f32 startVal, f32 finalVal, unsigned int stageCount, unsigned int stage) {
            return easeInOutElastic(startVal, finalVal, (f32)stage / (f32)stageCount);
        }

        /*! @brief Computes an easeInBack tweening.
        */
        inline f32 easeInBack(f32 startVal, f32 finalVal, f32 alpha, f32 s = 1.70158f) {
            f32 range = finalVal - startVal;
            return range * alpha * alpha * ((s + 1.0f) * alpha - s) + startVal;
        }
        inline f32 easeInBack(f32 startVal, f32 finalVal, unsigned int stageCount, unsigned int stage, f32 s = 1.70158f) {
            return easeInBack(startVal, finalVal, (f32)stage / (f32)stageCount, s);
        }

        /*! @brief Computes an easeOutBack tweening.
        */
        inline f32 easeOutBack(f32 startVal, f32 finalVal, f32 alpha, f32 s = 1.70158f) {
            f32 range = finalVal - startVal;
            f32 alphaMinus1 = alpha - 1;
            return range * (alphaMinus1 * alphaMinus1 * ((s + (f32)1.0f) * alphaMinus1 + s) + 1.0f) + startVal;
        }
        inline f32 easeOutBack(f32 startVal, f32 finalVal, unsigned int stageCount, unsigned int stage, f32 s = 1.70158f) {
            return easeOutBack(startVal, finalVal, (f32)stage / (f32)stageCount, s);
        }

        /*! @brief Computes an easeInOutBack tweening.
        */
        inline f32 easeInOutBack(f32 startVal, f32 finalVal, f32 alpha, f32 s = 1.70158f) {
            f32 range = finalVal - startVal;
            f32 alpha2 = alpha * 2.0f;
            f32 sPrime = s * 1.525f;
            if (alpha2 < 1.0f) {
                return range / 2.0f * (alpha2 * alpha2 * ((sPrime + 1.0f) * alpha2 - sPrime)) + startVal;
            }
            f32 alpha2Minus2 = alpha2 - 2.0f;
            return range / 2.0f * (alpha2Minus2 * alpha2Minus2 * ((sPrime + 1.0f) * alpha2Minus2 + sPrime) + 2.0f) + startVal;
        }
        inline f32 easeInOutBack(f32 startVal, f32 finalVal, unsigned int stageCount, unsigned int stage, f32 s = 1.70158f) {
            return easeInOutBack(startVal, finalVal, (f32)stage / (f32)stageCount, s);
        }

        /*! @brief Computes an easeInBounce tweening.
        */
        inline f32 easeInBounce(f32 startVal, f32 finalVal, f32 alpha) {
            f32 range = finalVal - startVal;
            return range - easeOutBounce(0, finalVal - startVal, 1 - alpha) + startVal;
        }
        inline f32 easeInBounce(f32 startVal, f32 finalVal, unsigned int stageCount, unsigned int stage) {
            return easeInBounce(startVal, finalVal, (f32)stage / (f32)stageCount);
        }

        /*! @brief Computes an easeOutBounce tweening.
        */
        inline f32 easeOutBounce(f32 startVal, f32 finalVal, f32 alpha) {
            f32 range = finalVal - startVal;
            if (alpha < 1.0f / 2.75f) {
                return range * (7.5625f * alpha * alpha) + startVal;
            } else if (alpha < 2.0f / 2.75f) {
                f32 alphaPrime = alpha - (1.5f / 2.75f);
                return range * (7.5625f * alphaPrime * alphaPrime * 0.75f) + startVal;
            } else if (alpha < 2.5f / 2.75f) {
                f32 alphaPrime = alpha - (2.25f / 2.75f);
                return range * (7.5625f * alphaPrime * alphaPrime * 0.9375f) + startVal;
            } else {
                f32 alphaPrime = alpha - (2.625f / 2.75f);
                return range * (7.5625f * alphaPrime * alphaPrime * 0.984375f) + startVal;
            }
        }
        inline f32 easeOutBounce(f32 startVal, f32 finalVal, unsigned int stageCount, unsigned int stage) {
            return easeOutBounce(startVal, finalVal, (f32)stage / (f32)stageCount);
        }

        /*! @brief Computes an easeInOutBounce tweening.
        */
        inline f32 easeInOutBounce(f32 startVal, f32 finalVal, f32 alpha) {
            f32 range = finalVal - startVal;
            if (alpha < 0.5f) {
                return easeInBounce(0, finalVal - startVal, 2.0f * alpha) * 0.5f + startVal;
            }
            return easeOutBounce(0, finalVal - startVal, 2.0f * alpha - 1.0f) * range * 0.25f + startVal;
        }
        inline f32 easeInOutBounce(f32 startVal, f32 finalVal, unsigned int stageCount, unsigned int stage) {
            return easeInOutBounce(startVal, finalVal, (f32)stage / (f32)stageCount);
        }
    }
}
namespace hm = hemlock::maths;
