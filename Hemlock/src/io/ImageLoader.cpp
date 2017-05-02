#include "stdafx.h"

#include "io/ImageLoader.h"

void** hio::Image::allocate(std::vector<ui32> widths, std::vector<ui32> heights, std::vector<Format> formats, std::vector<Type> types, std::vector<ui32>& sizes) {
    if (widths.size() != heights.size() ||
        widths.size() != formats.size() ||
        widths.size() != types.size()) return nullptr;

    ui32* tempSizes = nullptr;
    void** data = allocate(&widths[0], &heights[0], &formats[0], &types[0], tempSizes, (ui32)widths.size());
    sizes = std::vector<ui32>(&tempSizes[0], &tempSizes[widths.size() - 1]);

    return data;
}
void** hio::Image::allocate(ui32* widths, ui32* heights, Format* formats, Type* types, ui32*& sizes, ui32 count) {
    if (widths  == nullptr ||
        heights == nullptr ||
        formats == nullptr ||
        types   == nullptr) return nullptr;

    sizes = new ui32[count];
    void** data = new void*[count];
    for (ui32 i = 0; i < count; ++i) {
        data[i] = allocate(widths[i], heights[i], formats[i], types[i], sizes[i]);
    }
    return data;
}
void* hio::Image::allocate(ui32 width, ui32 height, Format format, Type type, ui32& size) {
    ui32 channels = 4;
    if (format == Format::RGB ||
        format == Format::BGR) channels = 3;
            
    switch (type) {
        case Type::I8:
        case Type::UI8:
            return new ui8[width * height * channels];
        case Type::I16:
        case Type::UI16:
            size = sizeof(ui16) * width * height * channels;
            return new ui16[width * height * channels];
        case Type::I32:
        case Type::UI32:
        case Type::F32:
            size = sizeof(ui32) * width * height * channels;
            return new ui32[width * height * channels];
        case Type::F64:
            size = sizeof(f64) * width * height * channels;
            return new f64[width * height * channels];
        default:
            return nullptr;
    }
}

ui32* hio::Image::load(std::vector<const char*> filepaths, std::vector<FileFormat> types) {
    if (filepaths.size() != types.size()) return nullptr;

    return load(&filepaths[0], &types[0], (ui32)filepaths.size());
}
ui32* hio::Image::load(const char** filepaths, FileFormat* types, ui32 count) {
    if (filepaths == nullptr ||
        types     == nullptr) return nullptr;

    ui32* ids = new ui32[count];
    for (ui32 i = 0; i < count; ++i) {
        ids[i] = load(filepaths[i], types[i]);
    }
    return ids;
}
ui32 hio::Image::load(const char* filepath, FileFormat type /*= FileFormat::UNKNOWN*/) {
    if (filepath == nullptr) return 0;
            
    ILuint imageID;
    ilGenImages(1, &imageID);
    ilBindImage(imageID);

    bool res = ilLoad((ILenum)type, filepath);

    ilBindImage(0);
    if (!res) {
        ilDeleteImages(1, &imageID);
        return 0;
    }
    return imageID;
}

ui32* hio::Image::load(std::vector<const void*> data, std::vector<ui32> sizes, std::vector<FileFormat> types) {
    if (data.size() != sizes.size() ||
        data.size() != types.size()) return nullptr;

    return load(&data[0], &sizes[0], &types[0], (ui32)data.size());
}
ui32* hio::Image::load(const void** data, ui32* sizes, FileFormat* types, ui32 count) {
    if (data  == nullptr ||
        sizes == nullptr ||
        types == nullptr) return nullptr;

    ui32* ids = new ui32[count];
    for (ui32 i = 0; i < count; ++i) {
        ids[i] = load(data[i], sizes[i], types[i]);
    }
    return ids;
}
ui32 hio::Image::load(const void* data, ui32 size, FileFormat type) {
    if (data == nullptr) return 0;

    ILuint imageID;
    ilGenImages(1, &imageID);
    ilBindImage(imageID);

    // NOTE: Does not support IL_TIF format, but let it be called to generate error.
    bool res = ilLoadL((ILenum)type, data, size);

    ilBindImage(0);
    if (!res) {
        ilDeleteImages(1, &imageID);
        return 0;
    }
    return imageID;
}

ui8** hio::Image::getData(std::vector<ui32> imageIDs) {
    return getData(&imageIDs[0], (ui32)imageIDs.size());
}
ui8** hio::Image::getData(ui32* imageIDs, ui32 count) {
    ui8** data = new ui8*[count];
    for (ui32 i = 0; i < count; ++i) {
        data[i] = getData(imageIDs[i]);
    }
    return data;
}
ui8* hio::Image::getData(ui32 imageID) {
    if (!ilIsImage(imageID)) return nullptr;

    ilBindImage(imageID);
    ui8* data = ilGetData();

    ilBindImage(0);
    return data;
}
        
void hio::Image::unload(std::vector<ui32> imageIDs) {
    ilDeleteImages(imageIDs.size(), &imageIDs[0]);
}
void hio::Image::unload(ui32* imageIDs, ui32 count) {
    ilDeleteImages(count, imageIDs);
}
void hio::Image::unload(ui32 imageID) {
    ilDeleteImages(1, &imageID);
}

bool hio::Image::save(std::vector<ui32> imageIDs, std::vector<const char*> filepaths, std::vector<FileFormat> types, bool overwrite /*= true*/) {
    if (imageIDs.size() != filepaths.size() ||
        imageIDs.size() != types.size()) return false;

    return save(&imageIDs[0], &filepaths[0], (ui32)imageIDs.size(), &types[0], overwrite);
}
bool hio::Image::save(ui32* imageIDs, const char** filepaths, ui32 count, FileFormat* types /*= nullptr*/, bool overwrite /*= true*/) {
    if (imageIDs  == nullptr ||
        filepaths == nullptr) return false;

    bool res = true;
    for (ui32 i = 0; i < count; ++i) {
        if (!save(imageIDs[i], filepaths[i], (types == nullptr ? FileFormat::UNKNOWN : types[i]), overwrite)) {
            res = false;
        }
    }
    return res;
}
bool hio::Image::save(ui32 imageID, const char* filepath, FileFormat type /*= FileFormat::UNKNOWN*/, bool overwrite /*= true*/) {
    if (!ilIsImage(imageID)) return false;

    if (overwrite) ilEnable(IL_FILE_OVERWRITE);

    ilBindImage(imageID);
    bool res = ilSave((ILenum)type, filepath);

    ilDisable(IL_FILE_OVERWRITE);
    ilBindImage(0);
    return res;
}

void** hio::Image::convert(std::vector<ui32> imageIDs, std::vector<Format> formats, std::vector<Type> types, std::vector<ui32>& sizes) {
    if (imageIDs.size() != formats.size() ||
        imageIDs.size() != types.size()) return nullptr;

    ui32* tempSizes = nullptr;
    void** data = convert(&imageIDs[0], &formats[0], &types[0], tempSizes, (ui32)imageIDs.size());
    sizes = std::vector<ui32>(&tempSizes[0], &tempSizes[imageIDs.size() - 1]);

    return data;
}
void** hio::Image::convert(ui32* imageIDs, Format* formats, Type* types, ui32*& sizes, ui32 count) {
    if (imageIDs == nullptr ||
        formats  == nullptr ||
        types    == nullptr) return nullptr;

    sizes = new ui32[count];
    void** data = new void*[count];
    for (ui32 i = 0; i < count; ++i) {
        data[i] = convert(imageIDs[i], formats[i], types[i], sizes[i]);
    }
    return data;
}
void* hio::Image::convert(ui32 imageID, Format format, Type type, ui32& size) {
    if (!ilIsImage(imageID)) return nullptr;

    ilBindImage(imageID);
            
    ui32 width  = ilGetInteger(IL_IMAGE_WIDTH);
    ui32 height = ilGetInteger(IL_IMAGE_HEIGHT);

    void* data = allocate(width, height, format, type, size);

    bool res = ilCopyPixels(0, 0, 0, width, height, 1, (ILenum)format, (ILenum)type, data);

    ilBindImage(0);
    if (!res) {
        delete[] data;
        return nullptr;
    }
    return data;
}

ui32* hio::Image::convertAndLoad(std::vector<ui32> imageIDs, std::vector<Format> formats, std::vector<Type> types, std::vector<FileFormat> fformats) {
    if (imageIDs.size() != formats.size() ||
        imageIDs.size() != types.size()   ||
        imageIDs.size() != fformats.size()) return nullptr;

    ui32* newIDs = convertAndLoad(&imageIDs[0], &formats[0], &types[0], &fformats[0], (ui32)imageIDs.size());

    return newIDs;
}
ui32* hio::Image::convertAndLoad(ui32* imageIDs, Format* formats, Type* types, FileFormat* fformats, ui32 count) {
    if (imageIDs == nullptr ||
        formats  == nullptr ||
        types    == nullptr ||
        fformats == nullptr) return nullptr;

    ui32* newIDs = new ui32[count];
    for (ui32 i = 0; i < count; ++i) {
        newIDs[i] = convertAndLoad(imageIDs[i], formats[i], types[i], fformats[i]);
    }
    return newIDs;
}
ui32 hio::Image::convertAndLoad(ui32 imageID, Format format, Type type, FileFormat fformat) {
    if (!ilIsImage(imageID)) return 0;

    ui32 size;
    void* data = convert(imageID, format, type, size);

    return load(data, size, fformat);
}

hio::Image::Error hio::Image::getError() {
    return (Error)ilGetError();
}
