#pragma once

#undef min
#undef max
#include <glm/glm.hpp>
//#include <glm\gtx\extended_min_max.hpp>

namespace hemlock {
    namespace maths {
        namespace Fractal {
            // Cellular noise ("Worley noise") in 2D & 3D in GLSL.
            // Copyright (c) Stefan Gustavson 2011-04-19. All rights reserved.
            // This code is released under the conditions of the MIT license.
            // See LICENSE file for details.
            // https://github.com/stegu/webgl-noise
            //
            // Ported to C++ by Matthew Marshall

            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            static inline glm::tvec3<FXX, glm::highp> mod289(glm::tvec3<FXX, glm::highp> x) {
                return x - glm::floor(x * (FXX)(1.0 / 289.0)) * (FXX)289.0;
            }

            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            static inline glm::tvec2<FXX, glm::highp> mod289(glm::tvec2<FXX, glm::highp> x) {
                return x - glm::floor(x * (FXX)(1.0 / 289.0)) * (FXX)289.0;
            }

            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            static inline glm::tvec3<FXX, glm::highp> mod7(glm::tvec3<FXX, glm::highp> x) {
                return x - glm::floor(x * (FXX)(1.0 / 7.0)) * (FXX)7.0;
            }

            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            static inline glm::tvec3<FXX, glm::highp> permute(glm::tvec3<FXX, glm::highp> x) {
                return mod289(((FXX)34.0 * x + (FXX)1.0) * x);
            }

            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            glm::tvec2<FXX, glm::highp> genCellular(glm::tvec2<FXX, glm::highp> coords) {
                using FXXv2 = glm::tvec2<FXX, glm::highp>;
                using FXXv3 = glm::tvec3<FXX, glm::highp>;
#define K 0.142857142857 // 1/7
#define Ko 0.428571428571 // 3/7
#define jitter 1.0 // Lower values gives more regular patterns.
                FXXv2 Pi = mod289(glm::floor(coords));
                FXXv2 Pf = glm::fract(coords);

                static FXXv3 oi = FXXv3((FXX)-1.0, (FXX)0.0, (FXX)1.0);
                static FXXv3 of = FXXv3((FXX)-0.5, (FXX)0.5, (FXX)1.5);

                FXXv3 px = permute(Pi.x + oi);
                FXXv3 p = permute(px.x + Pi.y + oi);

                FXXv3 ox = glm::fract(p * K) - Ko;
                FXXv3 oy = mod7(glm::floor(p * K)) * K - Ko;

                FXXv3 dx = Pf.x + (FXX)0.5 + jitter * ox;
                FXXv3 dy = Pf.y - of + jitter * oy;
                FXXv3 d1 = dx * dx + dy * dy;

                p = permute(px.y + Pi.y + oi);
                ox = glm::fract(p * K) - Ko;
                oy = mod7(glm::floor(p * K)) * K - Ko;

                dx = Pf.x - (FXX)0.5 + jitter * ox;
                dy = Pf.y - of + jitter * oy;
                FXXv3 d2 = dx * dx + dy * dy;

                p = permute(px.z + Pi.y + oi);
                ox = glm::fract(p * K) - Ko;
                oy = mod7(glm::floor(p * K)) * K - Ko;

                dx = Pf.x - (FXX)1.5 + jitter * ox;
                dy = Pf.y - of + jitter * oy;
                FXXv3 d3 = dx * dx + dy * dy;

                FXXv3 d1a = glm::min(d1, d2);

                d2 = glm::max(d1, d2);
                d2 = glm::min(d2, d3);
                d1 = glm::min(d1a, d2);
                d2 = glm::max(d1a, d2);
                if (d1.x >= d1.y) std::swap(d1.x, d1.y);
                if (d1.x >= d1.z) std::swap(d1.x, d1.z);
                d1.y = glm::min(d1.y, d2.y);
                d1.z = glm::min(d1.z, d2.z);
                d1.y = glm::min(d1.y, d1.z);
                d1.y = glm::min(d1.y, d2.x);
                return glm::sqrt(FXXv2(d1.x, d1.y));
#undef K
#undef Ko
#undef jitter
            }
            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            glm::tvec2<FXX, glm::highp> genCellular(glm::tvec3<FXX, glm::highp> coords) {
                using FXXv2 = glm::tvec2<FXX, glm::highp>;
                using FXXv3 = glm::tvec3<FXX, glm::highp>;
#define K 0.142857142857 // 1/7
#define Ko 0.428571428571 // 1/2-K/2
#define K2 0.020408163265306 // 1/(7*7)
#define Kz 0.166666666667 // 1/6
#define Kzo 0.416666666667 // 1/2-1/6*2
#define jitter 1.0 // Lower values gives more regular patterns.

                FXXv3 Pi = mod289(floor(P));
                FXXv3 Pf = glm::fract(P) - 0.5;

                FXXv3 Pfx = Pf.x + FXXv3(1.0, 0.0, -1.0);
                FXXv3 Pfy = Pf.y + FXXv3(1.0, 0.0, -1.0);
                FXXv3 Pfz = Pf.z + FXXv3(1.0, 0.0, -1.0);

                FXXv3 p = permute(Pi.x + FXXv3(-1.0, 0.0, 1.0));
                FXXv3 p1 = permute(p + Pi.y - 1.0);
                FXXv3 p2 = permute(p + Pi.y);
                FXXv3 p3 = permute(p + Pi.y + 1.0);

                FXXv3 p11 = permute(p1 + Pi.z - 1.0);
                FXXv3 p12 = permute(p1 + Pi.z);
                FXXv3 p13 = permute(p1 + Pi.z + 1.0);

                FXXv3 p21 = permute(p2 + Pi.z - 1.0);
                FXXv3 p22 = permute(p2 + Pi.z);
                FXXv3 p23 = permute(p2 + Pi.z + 1.0);

                FXXv3 p31 = permute(p3 + Pi.z - 1.0);
                FXXv3 p32 = permute(p3 + Pi.z);
                FXXv3 p33 = permute(p3 + Pi.z + 1.0);

                FXXv3 ox11 = glm::fract(p11 * K) - Ko;
                FXXv3 oy11 = mod7(floor(p11 * K)) * K - Ko;
                FXXv3 oz11 = glm::floor(p11 * K2) * Kz - Kzo;

                FXXv3 ox12 = glm::fract(p12 * K) - Ko;
                FXXv3 oy12 = mod7(glm::floor(p12 * K)) * K - Ko;
                FXXv3 oz12 = glm::floor(p12 * K2) * Kz - Kzo;

                FXXv3 ox13 = glm::fract(p13 * K) - Ko;
                FXXv3 oy13 = mod7(floor(p13 * K)) * K - Ko;
                FXXv3 oz13 = glm::floor(p13 * K2) * Kz - Kzo;

                FXXv3 ox21 = glm::fract(p21 * K) - Ko;
                FXXv3 oy21 = mod7(floor(p21 * K)) * K - Ko;
                FXXv3 oz21 = glm::floor(p21 * K2) * Kz - Kzo;

                FXXv3 ox22 = glm::fract(p22 * K) - Ko;
                FXXv3 oy22 = mod7(glm::floor(p22 * K)) * K - Ko;
                FXXv3 oz22 = glm::floor(p22 * K2) * Kz - Kzo;

                FXXv3 ox23 = glm::fract(p23 * K) - Ko;
                FXXv3 oy23 = mod7(glm::floor(p23 * K)) * K - Ko;
                FXXv3 oz23 = glm::floor(p23 * K2) * Kz - Kzo;

                FXXv3 ox31 = glm::fract(p31 * K) - Ko;
                FXXv3 oy31 = mod7(glm::floor(p31 * K)) * K - Ko;
                FXXv3 oz31 = glm::floor(p31 * K2) * Kz - Kzo;

                FXXv3 ox32 = glm::fract(p32 * K) - Ko;
                FXXv3 oy32 = mod7(glm::floor(p32 * K)) * K - Ko;
                FXXv3 oz32 = glm::floor(p32 * K2) * Kz - Kzo;

                FXXv3 ox33 = glm::fract(p33 * K) - Ko;
                FXXv3 oy33 = mod7(glm::floor(p33 * K)) * K - Ko;
                FXXv3 oz33 = glm::floor(p33 * K2) * Kz - Kzo;

                FXXv3 dx11 = Pfx + jitter * ox11;
                FXXv3 dy11 = Pfy.x + jitter * oy11;
                FXXv3 dz11 = Pfz.x + jitter * oz11;

                FXXv3 dx12 = Pfx + jitter * ox12;
                FXXv3 dy12 = Pfy.x + jitter * oy12;
                FXXv3 dz12 = Pfz.y + jitter * oz12;

                FXXv3 dx13 = Pfx + jitter * ox13;
                FXXv3 dy13 = Pfy.x + jitter * oy13;
                FXXv3 dz13 = Pfz.z + jitter * oz13;

                FXXv3 dx21 = Pfx + jitter * ox21;
                FXXv3 dy21 = Pfy.y + jitter * oy21;
                FXXv3 dz21 = Pfz.x + jitter * oz21;

                FXXv3 dx22 = Pfx + jitter * ox22;
                FXXv3 dy22 = Pfy.y + jitter * oy22;
                FXXv3 dz22 = Pfz.y + jitter * oz22;

                FXXv3 dx23 = Pfx + jitter * ox23;
                FXXv3 dy23 = Pfy.y + jitter * oy23;
                FXXv3 dz23 = Pfz.z + jitter * oz23;

                FXXv3 dx31 = Pfx + jitter * ox31;
                FXXv3 dy31 = Pfy.z + jitter * oy31;
                FXXv3 dz31 = Pfz.x + jitter * oz31;

                FXXv3 dx32 = Pfx + jitter * ox32;
                FXXv3 dy32 = Pfy.z + jitter * oy32;
                FXXv3 dz32 = Pfz.y + jitter * oz32;

                FXXv3 dx33 = Pfx + jitter * ox33;
                FXXv3 dy33 = Pfy.z + jitter * oy33;
                FXXv3 dz33 = Pfz.z + jitter * oz33;

                FXXv3 d11 = dx11 * dx11 + dy11 * dy11 + dz11 * dz11;
                FXXv3 d12 = dx12 * dx12 + dy12 * dy12 + dz12 * dz12;
                FXXv3 d13 = dx13 * dx13 + dy13 * dy13 + dz13 * dz13;
                FXXv3 d21 = dx21 * dx21 + dy21 * dy21 + dz21 * dz21;
                FXXv3 d22 = dx22 * dx22 + dy22 * dy22 + dz22 * dz22;
                FXXv3 d23 = dx23 * dx23 + dy23 * dy23 + dz23 * dz23;
                FXXv3 d31 = dx31 * dx31 + dy31 * dy31 + dz31 * dz31;
                FXXv3 d32 = dx32 * dx32 + dy32 * dy32 + dz32 * dz32;
                FXXv3 d33 = dx33 * dx33 + dy33 * dy33 + dz33 * dz33;

                // Sort out the two smallest distances (F1, F2)
#if 0
                // Cheat and sort out only F1
                FXXv3 d1 = glm::min(glm::min(d11, d12), d13);
                FXXv3 d2 = glm::min(glm::min(d21, d22), d23);
                FXXv3 d3 = glm::min(glm::min(d31, d32), d33);
                FXXv3 d = glm::min(glm::min(d1, d2), d3);
                d.x = glm::min(glm::min(d.x, d.y), d.z);
                return FXXv2(sqrt(d.x)); // F1 duplicated, no F2 computed
#else
                // Do it right and sort out both F1 and F2
                FXXv3 d1a = glm::min(d11, d12);
                d12 = glm::max(d11, d12);
                d11 = glm::min(d1a, d13); // Smallest now not in d12 or d13
                d13 = glm::max(d1a, d13);
                d12 = glm::min(d12, d13); // 2nd smallest now not in d13
                FXXv3 d2a = glm::min(d21, d22);
                d22 = glm::max(d21, d22);
                d21 = glm::min(d2a, d23); // Smallest now not in d22 or d23
                d23 = glm::max(d2a, d23);
                d22 = glm::min(d22, d23); // 2nd smallest now not in d23
                FXXv3 d3a = glm::min(d31, d32);
                d32 = glm::max(d31, d32);
                d31 = glm::min(d3a, d33); // Smallest now not in d32 or d33
                d33 = glm::max(d3a, d33);
                d32 = glm::min(d32, d33); // 2nd smallest now not in d33
                FXXv3 da = glm::min(d11, d21);
                d21 = glm::max(d11, d21);
                d11 = glm::min(da, d31); // Smallest now in d11
                d31 = glm::max(da, d31); // 2nd smallest now not in d31
                if (d11.x < d11.y) std::swap(d11.x, d11.y);
                if (d11.x < d11.z) std::swap(d11.x, d11.z);
                d12 = glm::min(d12, d21); // 2nd smallest now not in d21
                d12 = glm::min(d12, d22); // nor in d22
                d12 = glm::min(d12, d31); // nor in d31
                d12 = glm::min(d12, d32); // nor in d32
                d11.y = glm::min(d11.y, d12.x); // nor in d12.yz
                d11.z = glm::min(d11.z, d12.y); // nor in d12.yz
                d11.y = glm::min(d11.y, d12.z); // Only two more to go
                d11.y = glm::min(d11.y, d11.z); // Done! (Phew!)
                return glm::sqrt(FXXv2(d11.x, d11.y)); // F1, F2
#endif
            }
            //TODO(Matthew): Scaled cellular and octave-based cellular?

            /* Copyright (c) 2007-2012 Eliot Eshelman
            *
            * This program is free software: you can redistribute it and/or modify
            * it under the terms of the GNU General Public License as published by
            * the Free Software Foundation, either version 3 of the License, or
            * (at your option) any later version.
            *
            * This program is distributed in the hope that it will be useful,
            * but WITHOUT ANY WARRANTY; without even the implied warranty of
            * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
            * GNU General Public License for more details.
            *
            * You should have received a copy of the GNU General Public License
            * along with this program. If not, see <http://www.gnu.org/licenses/>.
            *
            * Modified by Matthew Marshall
            */

            // The gradients are the midpoints of the vertices of a cube.
            static const int grad3[12][3] = {
                { 1,1,0 },{ -1,1,0 },{ 1,-1,0 },{ -1,-1,0 },
                { 1,0,1 },{ -1,0,1 },{ 1,0,-1 },{ -1,0,-1 },
                { 0,1,1 },{ 0,-1,1 },{ 0,1,-1 },{ 0,-1,-1 }
            };


            // The gradients are the midpoints of the vertices of a hypercube.
            static const int grad4[32][4] = {
                { 0,1,1,1 },{ 0,1,1,-1 },{ 0,1,-1,1 },{ 0,1,-1,-1 },
                { 0,-1,1,1 },{ 0,-1,1,-1 },{ 0,-1,-1,1 },{ 0,-1,-1,-1 },
                { 1,0,1,1 },{ 1,0,1,-1 },{ 1,0,-1,1 },{ 1,0,-1,-1 },
                { -1,0,1,1 },{ -1,0,1,-1 },{ -1,0,-1,1 },{ -1,0,-1,-1 },
                { 1,1,0,1 },{ 1,1,0,-1 },{ 1,-1,0,1 },{ 1,-1,0,-1 },
                { -1,1,0,1 },{ -1,1,0,-1 },{ -1,-1,0,1 },{ -1,-1,0,-1 },
                { 1,1,1,0 },{ 1,1,-1,0 },{ 1,-1,1,0 },{ 1,-1,-1,0 },
                { -1,1,1,0 },{ -1,1,-1,0 },{ -1,-1,1,0 },{ -1,-1,-1,0 }
            };


            // Permutation table.  The same list is repeated twice.
            static const int perm[512] = {
                151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,
                8,99,37,240,21,10,23,190,6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,
                35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168,68,175,74,165,71,
                134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,
                55,46,245,40,244,102,143,54,65,25,63,161,1,216,80,73,209,76,132,187,208, 89,
                18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,186,3,64,52,217,226,
                250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,
                189,28,42,223,183,170,213,119,248,152,2,44,154,163,70,221,153,101,155,167,43,
                172,9,129,22,39,253,19,98,108,110,79,113,224,232,178,185,112,104,218,246,97,
                228,251,34,242,193,238,210,144,12,191,179,162,241,81,51,145,235,249,14,239,
                107,49,192,214,31,181,199,106,157,184,84,204,176,115,121,50,45,127,4,150,254,
                138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180,

                151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,
                8,99,37,240,21,10,23,190,6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,
                35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168,68,175,74,165,71,
                134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,
                55,46,245,40,244,102,143,54,65,25,63,161,1,216,80,73,209,76,132,187,208, 89,
                18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,186,3,64,52,217,226,
                250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,
                189,28,42,223,183,170,213,119,248,152,2,44,154,163,70,221,153,101,155,167,43,
                172,9,129,22,39,253,19,98,108,110,79,113,224,232,178,185,112,104,218,246,97,
                228,251,34,242,193,238,210,144,12,191,179,162,241,81,51,145,235,249,14,239,
                107,49,192,214,31,181,199,106,157,184,84,204,176,115,121,50,45,127,4,150,254,
                138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
            };


            // A lookup table to traverse the simplex around a given point in 4D.
            static const int simplex[64][4] = {
                { 0,1,2,3 },{ 0,1,3,2 },{ 0,0,0,0 },{ 0,2,3,1 },{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 },{ 1,2,3,0 },
                { 0,2,1,3 },{ 0,0,0,0 },{ 0,3,1,2 },{ 0,3,2,1 },{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 },{ 1,3,2,0 },
                { 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 },
                { 1,2,0,3 },{ 0,0,0,0 },{ 1,3,0,2 },{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 },{ 2,3,0,1 },{ 2,3,1,0 },
                { 1,0,2,3 },{ 1,0,3,2 },{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 },{ 2,0,3,1 },{ 0,0,0,0 },{ 2,1,3,0 },
                { 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 },
                { 2,0,1,3 },{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 },{ 3,0,1,2 },{ 3,0,2,1 },{ 0,0,0,0 },{ 3,1,2,0 },
                { 2,1,0,3 },{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 },{ 3,1,0,2 },{ 0,0,0,0 },{ 3,2,0,1 },{ 3,2,1,0 }
            };

            // Could probably reduce code repetitiveness if we used templates even more clever-like...
            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            FXX genSimplexWithOctaves(ui8 octaves, FXX persistence, FXX scale, glm::tvec2<FXX, glm::highp> coords) {
                FXX total = (FXX)0.0;
                FXX frequency = scale;
                FXX amplitude = (FXX)1.0;

                // We have to keep track of the largest possible amplitude,
                // because each octave adds more, and we need a value in [-1, 1].
                FXX maxAmplitude = (FXX)0.0;

                for (ui8 i = 0; i < octaves; ++i) {
                    total += genSimplex(coords * frequency) * amplitude;

                    frequency *= (FXX)2.0;
                    maxAmplitude += amplitude;
                    amplitude *= persistence;
                }

                return total / maxAmplitude;
            }
            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            FXX genSimplexWithOctaves(ui8 octaves, FXX persistence, FXX scale, glm::tvec3<FXX, glm::highp> coords) {
                FXX total = (FXX)0.0;
                FXX frequency = scale;
                FXX amplitude = (FXX)1.0;

                // We have to keep track of the largest possible amplitude,
                // because each octave adds more, and we need a value in [-1, 1].
                FXX maxAmplitude = (FXX)0.0;

                for (ui8 i = 0; i < octaves; ++i) {
                    total += genSimplex(coords * frequency) * amplitude;

                    frequency *= (FXX)2.0;
                    maxAmplitude += amplitude;
                    amplitude *= persistence;
                }

                return total / maxAmplitude;
            }
            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            FXX genSimplexWithOctaves(ui8 octaves, FXX persistence, FXX scale, glm::tvec4<FXX, glm::highp> coords) {
                FXX total = (FXX)0.0;
                FXX frequency = scale;
                FXX amplitude = (FXX)1.0;

                // We have to keep track of the largest possible amplitude,
                // because each octave adds more, and we need a value in [-1, 1].
                FXX maxAmplitude = (FXX)0.0;

                for (ui8 i = 0; i < octaves; ++i) {
                    total += genSimplex(coords * frequency) * amplitude;

                    frequency *= (FXX)2.0;
                    maxAmplitude += amplitude;
                    amplitude *= persistence;
                }

                return total / maxAmplitude;
            }

            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            FXX genSimplexWithOctavesScaled(ui8 octaves, FXX persistence, FXX scale, FXX loBound, FXX hiBound, glm::tvec2<FXX, glm::highp> coords) {
                return genSimplexWithOctaves(octaves, persistence, scale, coords) * (hiBound - loBound) / (FXX)2.0 + (hiBound + loBound) / (FXX)2.0;
            }
            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            FXX genSimplexWithOctavesScaled(ui8 octaves, FXX persistence, FXX scale, FXX loBound, FXX hiBound, glm::tvec3<FXX, glm::highp> coords) {
                return genSimplexWithOctaves(octaves, persistence, scale, coords) * (hiBound - loBound) / (FXX)2.0 + (hiBound + loBound) / (FXX)2.0;
            }
            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            FXX genSimplexWithOctavesScaled(ui8 octaves, FXX persistence, FXX scale, FXX loBound, FXX hiBound, glm::tvec4<FXX, glm::highp> coords) {
                return genSimplexWithOctaves(octaves, persistence, scale, coords) * (hiBound - loBound) / (FXX)2.0 + (hiBound + loBound) / (FXX)2.0;
            }

            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            static inline int fastfloor(const FXX x) { return x > 0 ? (int)x : (int)x - 1; }

            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            static inline FXX dot(const int* g, const FXX x, const FXX y) { return g[0] * x + g[1] * y; }
            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            static inline FXX dot(const int* g, const FXX x, const FXX y, const FXX z) { return g[0] * x + g[1] * y + g[2] * z; }
            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            static  FXX dot(const int* g, const FXX x, const FXX y, const FXX z, const FXX w) { return g[0] * x + g[1] * y + g[2] * z + g[3] * w; }


            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            FXX genSimplex(glm::tvec2<FXX, glm::highp> coords) {
                // Noise contributions from the three corners
                FXX n0, n1, n2;

                // Skew the input space to determine which simplex cell we're in
                FXX F2 = (FXX)0.5 * ((FXX)sqrt(3.0) - (FXX)1.0);
                // Hairy factor for 2D
                FXX s = (coords.x + coords.y) * F2;
                int i = fastfloor(coords.x + s);
                int j = fastfloor(coords.y + s);

                FXX G2 = ((FXX)3.0 - (FXX)glm::sqrt(3.0)) / (FXX)6.0;
                FXX t = (i + j) * G2;
                // Unskew the cell origin back to (x,y) space
                FXX X0 = i - t;
                FXX Y0 = j - t;
                // The x,y distances from the cell origin
                FXX x0 = coords.x - X0;
                FXX y0 = coords.y - Y0;

                // For the 2D case, the simplex shape is an equilateral triangle.
                // Determine which simplex we are in.
                int i1, j1; // Offsets for second (middle) corner of simplex in (i,j) coords
                if (x0 > y0) { // lower triangle, XY order: (0,0)->(1,0)->(1,1)
                    i1 = 1;
                    j1 = 0;
                } else { // upper triangle, YX order: (0,0)->(0,1)->(1,1)
                    i1 = 0;
                    j1 = 1;
                }

                // A step of (1,0) in (i,j) means a step of (1-c,-c) in (x,y), and
                // a step of (0,1) in (i,j) means a step of (-c,1-c) in (x,y), where
                // c = (3-sqrt(3))/6
                FXX x1 = x0 - i1 + G2; // Offsets for middle corner in (x,y) unskewed coords
                FXX y1 = y0 - j1 + G2;
                FXX x2 = x0 - (FXX)1.0 + (FXX)2.0 * G2; // Offsets for last corner in (x,y) unskewed coords
                FXX y2 = y0 - (FXX)1.0 + (FXX)2.0 * G2;

                // Work out the hashed gradient indices of the three simplex corners
                int ii = i & 255;
                int jj = j & 255;
                int gi0 = perm[ii + perm[jj]] % 12;
                int gi1 = perm[ii + i1 + perm[jj + j1]] % 12;
                int gi2 = perm[ii + 1 + perm[jj + 1]] % 12;

                // Calculate the contribution from the three corners
                FXX t0 = 0.5 - x0 * x0 - y0 * y0;
                if (t0 < 0) {
                    n0 = 0.0;
                } else {
                    t0 *= t0;
                    n0 = t0 * t0 * dot(grad3[gi0], x0, y0); // (x,y) of grad3 used for 2D gradient
                }

                FXX t1 = 0.5 - x1 * x1 - y1 * y1;
                if (t1 < (FXX)0.0) {
                    n1 = (FXX)0.0;
                } else {
                    t1 *= t1;
                    n1 = t1 * t1 * dot(grad3[gi1], x1, y1);
                }

                FXX t2 = (FXX)0.5 - x2 * x2 - y2 * y2;
                if (t2 < (FXX)0.0) {
                    n2 = (FXX)0.0;
                } else {
                    t2 *= t2;
                    n2 = t2 * t2 * dot(grad3[gi2], x2, y2);
                }

                // Add contributions from each corner to get the final noise value.
                // The result is scaled to return values in the interval [-1,1].
                return (FXX)70.0 * (n0 + n1 + n2);
            }
            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            FXX genSimplex(glm::tvec3<FXX, glm::highp> coords) {
                FXX n0, n1, n2, n3; // Noise contributions from the four corners

                                    // Skew the input space to determine which simplex cell we're in
                FXX F3 = (FXX)1.0 / (FXX)3.0;
                FXX s = (coords.x + coords.y + coords.z)*F3; // Very nice and simple skew factor for 3D
                int i = fastfloor(coords.x + s);
                int j = fastfloor(coords.y + s);
                int k = fastfloor(coords.z + s);

                FXX G3 = (FXX)1.0 / (FXX)6.0; // Very nice and simple unskew factor, too
                FXX t = (i + j + k)*G3;
                FXX X0 = i - t; // Unskew the cell origin back to (x,y,z) space
                FXX Y0 = j - t;
                FXX Z0 = k - t;
                FXX x0 = coords.x - X0; // The x,y,z distances from the cell origin
                FXX y0 = coords.y - Y0;
                FXX z0 = coords.z - Z0;

                // For the 3D case, the simplex shape is a slightly irregular tetrahedron.
                // Determine which simplex we are in.
                int i1, j1, k1; // Offsets for second corner of simplex in (i,j,k) coords
                int i2, j2, k2; // Offsets for third corner of simplex in (i,j,k) coords

                if (x0 >= y0) {
                    if (y0 >= z0) { // X Y Z order
                        i1 = 1;
                        j1 = 0;
                        k1 = 0;
                        i2 = 1;
                        j2 = 1;
                        k2 = 0;
                    } else if (x0 >= z0) { // X Z Y order
                        i1 = 1;
                        j1 = 0;
                        k1 = 0;
                        i2 = 1;
                        j2 = 0;
                        k2 = 1;
                    } else { // Z X Y order
                        i1 = 0;
                        j1 = 0;
                        k1 = 1;
                        i2 = 1;
                        j2 = 0;
                        k2 = 1;
                    }
                } else { // x0 < y0
                    if (y0 < z0) { // Z Y X order
                        i1 = 0;
                        j1 = 0;
                        k1 = 1;
                        i2 = 0;
                        j2 = 1;
                        k2 = 1;
                    } else if (x0 < z0) { // Y Z X order
                        i1 = 0;
                        j1 = 1;
                        k1 = 0;
                        i2 = 0;
                        j2 = 1;
                        k2 = 1;
                    } else { // Y X Z order
                        i1 = 0;
                        j1 = 1;
                        k1 = 0;
                        i2 = 1;
                        j2 = 1;
                        k2 = 0;
                    }
                }

                // A step of (1,0,0) in (i,j,k) means a step of (1-c,-c,-c) in (x,y,z),
                // a step of (0,1,0) in (i,j,k) means a step of (-c,1-c,-c) in (x,y,z), and
                // a step of (0,0,1) in (i,j,k) means a step of (-c,-c,1-c) in (x,y,z), where
                // c = 1/6.
                FXX x1 = x0 - i1 + G3; // Offsets for second corner in (x,y,z) coords
                FXX y1 = y0 - j1 + G3;
                FXX z1 = z0 - k1 + G3;
                FXX x2 = x0 - i2 + (FXX)2.0*G3; // Offsets for third corner in (x,y,z) coords
                FXX y2 = y0 - j2 + (FXX)2.0*G3;
                FXX z2 = z0 - k2 + (FXX)2.0*G3;
                FXX x3 = x0 - (FXX)1.0 + (FXX)3.0*G3; // Offsets for last corner in (x,y,z) coords
                FXX y3 = y0 - (FXX)1.0 + (FXX)3.0*G3;
                FXX z3 = z0 - (FXX)1.0 + (FXX)3.0*G3;

                // Work out the hashed gradient indices of the four simplex corners
                int ii = i & 255;
                int jj = j & 255;
                int kk = k & 255;
                int gi0 = perm[ii + perm[jj + perm[kk]]] % 12;
                int gi1 = perm[ii + i1 + perm[jj + j1 + perm[kk + k1]]] % 12;
                int gi2 = perm[ii + i2 + perm[jj + j2 + perm[kk + k2]]] % 12;
                int gi3 = perm[ii + 1 + perm[jj + 1 + perm[kk + 1]]] % 12;

                // Calculate the contribution from the four corners
                FXX t0 = (FXX)0.6 - x0*x0 - y0*y0 - z0*z0;
                if (t0 < (FXX)0.0) {
                    n0 = (FXX)0.0;
                } else {
                    t0 *= t0;
                    n0 = t0 * t0 * dot(grad3[gi0], x0, y0, z0);
                }

                FXX t1 = (FXX)0.6 - x1*x1 - y1*y1 - z1*z1;
                if (t1 < (FXX)0.0) {
                    n1 = (FXX)0.0;
                } else {
                    t1 *= t1;
                    n1 = t1 * t1 * dot(grad3[gi1], x1, y1, z1);
                }

                FXX t2 = (FXX)0.6 - x2*x2 - y2*y2 - z2*z2;
                if (t2 < (FXX)0.0) {
                    n2 = (FXX)0.0;
                } else {
                    t2 *= t2;
                    n2 = t2 * t2 * dot(grad3[gi2], x2, y2, z2);
                }

                FXX t3 = (FXX)0.6 - x3*x3 - y3*y3 - z3*z3;
                if (t3 < (FXX)0.0) {
                    n3 = (FXX)0.0;
                } else {
                    t3 *= t3;
                    n3 = t3 * t3 * dot(grad3[gi3], x3, y3, z3);
                }

                // Add contributions from each corner to get the final noise value.
                // The result is scaled to stay just inside [-1,1]
                return 32.0*(n0 + n1 + n2 + n3);
            }
            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            FXX genSimplex(glm::tvec4<FXX, glm::highp> coords) {
                // The skewing and unskewing factors are hairy again for the 4D case
                FXX F4 = (glm::sqrt(5.0) - (FXX)1.0) / (FXX)4.0;
                FXX G4 = ((FXX)5.0 - glm::sqrt((FXX)5.0)) / (FXX)20.0;
                FXX n0, n1, n2, n3, n4; // Noise contributions from the five corners

                                        // Skew the (x,y,z,w) space to determine which cell of 24 simplices we're in
                FXX s = (coords.x + coords.y + coords.z + coords.w) * F4; // Factor for 4D skewing
                int i = fastfloor(coords.x + s);
                int j = fastfloor(coords.y + s);
                int k = fastfloor(coords.z + s);
                int l = fastfloor(coords.w + s);
                FXX t = (i + j + k + l) * G4; // Factor for 4D unskewing
                FXX X0 = i - t; // Unskew the cell origin back to (x,y,z,w) space
                FXX Y0 = j - t;
                FXX Z0 = k - t;
                FXX W0 = l - t;

                FXX x0 = coords.x - X0; // The x,y,z,w distances from the cell origin
                FXX y0 = coords.y - Y0;
                FXX z0 = coords.z - Z0;
                FXX w0 = coords.w - W0;

                // For the 4D case, the simplex is a 4D shape I won't even try to describe.
                // To find out which of the 24 possible simplices we're in, we need to
                // determine the magnitude ordering of x0, y0, z0 and w0.
                // The method below is a good way of finding the ordering of x,y,z,w and
                // then find the correct traversal order for the simplex we're in.
                // First, six pair-wise comparisons are performed between each possible pair
                // of the four coordinates, and the results are used to add up binary bits
                // for an integer index.
                int c1 = (x0 > y0) ? 32 : 0;
                int c2 = (x0 > z0) ? 16 : 0;
                int c3 = (y0 > z0) ? 8 : 0;
                int c4 = (x0 > w0) ? 4 : 0;
                int c5 = (y0 > w0) ? 2 : 0;
                int c6 = (z0 > w0) ? 1 : 0;
                int c = c1 + c2 + c3 + c4 + c5 + c6;

                int i1, j1, k1, l1; // The integer offsets for the second simplex corner
                int i2, j2, k2, l2; // The integer offsets for the third simplex corner
                int i3, j3, k3, l3; // The integer offsets for the fourth simplex corner

                                    // simplex[c] is a 4-vector with the numbers 0, 1, 2 and 3 in some order.
                                    // Many values of c will never occur, since e.g. x>y>z>w makes x<z, y<w and x<w
                                    // impossible. Only the 24 indices which have non-zero entries make any sense.
                                    // We use a thresholding to set the coordinates in turn from the largest magnitude.
                                    // The number 3 in the "simplex" array is at the position of the largest coordinate.
                i1 = simplex[c][0] >= 3 ? 1 : 0;
                j1 = simplex[c][1] >= 3 ? 1 : 0;
                k1 = simplex[c][2] >= 3 ? 1 : 0;
                l1 = simplex[c][3] >= 3 ? 1 : 0;
                // The number 2 in the "simplex" array is at the second largest coordinate.
                i2 = simplex[c][0] >= 2 ? 1 : 0;
                j2 = simplex[c][1] >= 2 ? 1 : 0;
                k2 = simplex[c][2] >= 2 ? 1 : 0;
                l2 = simplex[c][3] >= 2 ? 1 : 0;
                // The number 1 in the "simplex" array is at the second smallest coordinate.
                i3 = simplex[c][0] >= 1 ? 1 : 0;
                j3 = simplex[c][1] >= 1 ? 1 : 0;
                k3 = simplex[c][2] >= 1 ? 1 : 0;
                l3 = simplex[c][3] >= 1 ? 1 : 0;
                // The fifth corner has all coordinate offsets = 1, so no need to look that up.

                FXX x1 = x0 - i1 + G4; // Offsets for second corner in (x,y,z,w) coords
                FXX y1 = y0 - j1 + G4;
                FXX z1 = z0 - k1 + G4;
                FXX w1 = w0 - l1 + G4;
                FXX x2 = x0 - i2 + (FXX)2.0*G4; // Offsets for third corner in (x,y,z,w) coords
                FXX y2 = y0 - j2 + (FXX)2.0*G4;
                FXX z2 = z0 - k2 + (FXX)2.0*G4;
                FXX w2 = w0 - l2 + (FXX)2.0*G4;
                FXX x3 = x0 - i3 + (FXX)3.0*G4; // Offsets for fourth corner in (x,y,z,w) coords
                FXX y3 = y0 - j3 + (FXX)3.0*G4;
                FXX z3 = z0 - k3 + (FXX)3.0*G4;
                FXX w3 = w0 - l3 + (FXX)3.0*G4;
                FXX x4 = x0 - (FXX)1.0 + (FXX)4.0*G4; // Offsets for last corner in (x,y,z,w) coords
                FXX y4 = y0 - (FXX)1.0 + (FXX)4.0*G4;
                FXX z4 = z0 - (FXX)1.0 + (FXX)4.0*G4;
                FXX w4 = w0 - (FXX)1.0 + (FXX)4.0*G4;

                // Work out the hashed gradient indices of the five simplex corners
                int ii = i & 255;
                int jj = j & 255;
                int kk = k & 255;
                int ll = l & 255;
                int gi0 = perm[ii + perm[jj + perm[kk + perm[ll]]]] % 32;
                int gi1 = perm[ii + i1 + perm[jj + j1 + perm[kk + k1 + perm[ll + l1]]]] % 32;
                int gi2 = perm[ii + i2 + perm[jj + j2 + perm[kk + k2 + perm[ll + l2]]]] % 32;
                int gi3 = perm[ii + i3 + perm[jj + j3 + perm[kk + k3 + perm[ll + l3]]]] % 32;
                int gi4 = perm[ii + 1 + perm[jj + 1 + perm[kk + 1 + perm[ll + 1]]]] % 32;

                // Calculate the contribution from the five corners
                FXX t0 = (FXX)0.6 - x0 * x0 - y0 * y0 - z0 * z0 - w0 * w0;
                if (t0 < (FXX)0.0) {
                    n0 = (FXX)0.0;
                } else {
                    t0 *= t0;
                    n0 = t0 * t0 * dot(grad4[gi0], x0, y0, z0, w0);
                }

                FXX t1 = (FXX)0.6 - x1 * x1 - y1 * y1 - z1 * z1 - w1 * w1;
                if (t1 < (FXX)0.0) {
                    n1 = (FXX)0.0;
                } else {
                    t1 *= t1;
                    n1 = t1 * t1 * dot(grad4[gi1], x1, y1, z1, w1);
                }

                FXX t2 = (FXX)0.6 - x2 * x2 - y2 * y2 - z2 * z2 - w2 * w2;
                if (t2 < (FXX)0.0) {
                    n2 = (FXX)0.0;
                } else {
                    t2 *= t2;
                    n2 = t2 * t2 * dot(grad4[gi2], x2, y2, z2, w2);
                }

                FXX t3 = (FXX)0.6 - x3 * x3 - y3 * y3 - z3 * z3 - w3 * w3;
                if (t3 < (FXX)0.0) {
                    n3 = (FXX)0.0;
                } else {
                    t3 *= t3;
                    n3 = t3 * t3 * dot(grad4[gi3], x3, y3, z3, w3);
                }

                FXX t4 = (FXX)0.6 - x4 * x4 - y4 * y4 - z4 * z4 - w4 * w4;
                if (t4 < (FXX)0.0) {
                    n4 = (FXX)0.0;
                } else {
                    t4 *= t4;
                    n4 = t4 * t4 * dot(grad4[gi4], x4, y4, z4, w4);
                }

                // Sum up and scale the result to cover the range [-1,1]
                return 27.0 * (n0 + n1 + n2 + n3 + n4);
            }

            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            FXX genSimplexScaled(FXX loBound, FXX hiBound, glm::tvec2<FXX, glm::highp> coords) {
                return genSimplex(coords) * (hiBound - loBound) / (FXX)2.0 + (hiBound + loBound) / (FXX)2.0;
            }
            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            FXX genSimplexScaled(FXX loBound, FXX hiBound, glm::tvec3<FXX, glm::highp> coords) {
                return genSimplex(coords) * (hiBound - loBound) / (FXX)2.0 + (hiBound + loBound) / (FXX)2.0;
            }
            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            FXX genSimplexScaled(FXX loBound, FXX hiBound, glm::tvec4<FXX, glm::highp> coords) {
                return genSimplex(coords) * (hiBound - loBound) / (FXX)2.0 + (hiBound + loBound) / (FXX)2.0;
            }
        }
    }
}
namespace hm = hemlock::maths;
