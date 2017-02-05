#pragma once

namespace hemlock {
    namespace graphics {
        namespace Texture {
            ui32* load(std::vector<const char*> filepaths, bool mipmap = false);
            ui32* load(const char** filepaths, ui32 count, bool mipmap = false);
            ui32  load(const char* filepath, bool mipmap = false);

            ui32* load(std::vector<ui32> imageIDs, bool mipmap = false);
            ui32* load(const ui32* imageIDs, ui32 count, bool mipmap = false);
            ui32  load(ui32 imageID, bool mipmap = false);

            // TODO(Matthew): Actually implement these.
            ui32* load(std::vector<const void*> data, std::vector<hio::Image::Format> formats, std::vector<hio::Image::Type> types, bool mipmap = false);
            ui32* load(const void** data, hio::Image::Format* formats, hio::Image::Type* types, ui32 count, bool mipmap = false);
            ui32  load(const void* data, hio::Image::Format format, hio::Image::Type type, bool mipmap = false);
        }
    }
}
namespace hg = hemlock::graphics;
