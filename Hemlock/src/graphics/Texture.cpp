#include "stdafx.h"

#include "graphics/Texture.h"
#include "io/ImageLoader.h"

ui32* hg::Texture::load(std::vector<const char*> filepaths, bool mipmap /*= false*/) {
    return load(&filepaths[0], (ui32)filepaths.size());
}
ui32* hg::Texture::load(const char ** filepaths, ui32 count, bool mipmap /*= false*/) {
    if (filepaths == nullptr) return nullptr;
    
    ui32* texIDs = new ui32[count];
    for (ui32 i = 0; i < count; ++i) {
        texIDs[i] = load(filepaths[i]);
    }
    return texIDs;
}
ui32 hg::Texture::load(const char * filepath, bool mipmap /*= false*/) {
    if (filepath == nullptr) return 0;

    ui32 imgID = hio::Image::load(filepath);
    if (imgID == 0) return 0;

    ilBindImage(imgID);

    ui32 texID;
    if (mipmap) {
        texID = ilutGLBindMipmaps();
    } else {
        texID = ilutGLBindTexImage();
    }

    ilBindImage(0);
    ilDeleteImages(1, &imgID);
    return texID;
}

ui32* hg::Texture::load(std::vector<ui32> imageIDs, bool mipmap /*= false*/) {
    return load(&imageIDs[0], (ui32)imageIDs.size(), mipmap);
}
ui32* hg::Texture::load(const ui32* imageIDs, ui32 count, bool mipmap /*= false*/) {
    if (imageIDs == nullptr) return nullptr;

    ui32* texIDs = new ui32[count];
    for (ui32 i = 0; i < count; ++i) {
        texIDs[i] = load(imageIDs[i], mipmap);
    }
    return texIDs;
}
ui32 hg::Texture::load(ui32 imageID, bool mipmap /*= false*/) {
    if (!ilIsImage(imageID)) return 0;

    ilBindImage(imageID);

    ui32 texID;
    if (mipmap) {
        texID = ilutGLBindMipmaps();
    } else {
        texID = ilutGLBindTexImage();
    }

    ilBindImage(0);
    return texID;
}

//ui32* hg::Texture::load(std::vector<const void*> data, std::vector<hio::Image::Format> formats, std::vector<hio::Image::Type> types, bool mipmap /*= false*/) {
//    return nullptr;
//}
//ui32* hg::Texture::load(const void** data, hio::Image::Format* format, hio::Image::Type* type, ui32 count, bool mipmap /*= false*/) {
//    return nullptr;
//}
//ui32 hg::Texture::load(const void* data, hio::Image::Format format, hio::Image::Type type, bool mipmap /*= false*/) {
//    return 0;
//    /*if (data == nullptr) return 0;
//
//
//
//    ilBindImage(imgID);
//
//    ui32 texID;
//    if (mipmap) {
//        texID = ilutGLBindMipmaps();
//    } else {
//        texID = ilutGLBindTexImage();
//    }
//
//    ilBindImage(0);
//    ilDeleteImages(1, &imgID);
//    return texID;*/
//}
