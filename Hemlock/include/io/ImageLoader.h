#pragma once

namespace hemlock {
    namespace io {
        enum class ImageIOFormat {
            RAW = 0,
            RGB_UI8,
            RGBA_UI8,
            RGB_UI16,
            RGBA_UI16,
            RGB_F32,
            RGBA_F32,
            RGB_F64,
            RGBA_F64
        };

        struct BitmapResource {
            ui32 width, height;
            union {
                void*         data = nullptr;
                ui8*          bytesUI8;
                glm::u8vec2*  bytesUI8v2;
                glm::u8vec3*  bytesUI8v3;
                glm::u8vec4*  bytesUI8v4;
                ui16*         bytesUI16;
                glm::u16vec2* bytesUI16v2;
                glm::u16vec3* bytesUI16v3;
                glm::u16vec4* bytesUI16v4;
                f32*          bytesF32;
                glm::f32vec2* bytesF32v2;
                glm::f32vec3* bytesF32v3;
                glm::f32vec4* bytesF32v4;
                f64*          bytesF64;
                glm::f64vec2* bytesF64v2;
                glm::f64vec3* bytesF64v3;
                glm::f64vec4* bytesF64v4;
            };
        };

        void free(BitmapResource& resource);

        BitmapResource allocate(ImageIOFormat format, ui32 width, ui32 height);

        BitmapResource load(const char* filepath, ImageIOFormat format = ImageIOFormat::RGBA_UI8, bool flipV = false);

        bool save(const char* filepath, ImageIOFormat format, const void* data, ui32 width, ui32 height);
    }
}
namespace hio = hemlock::io;
