#include "stdafx.h"

#include "io\ImageLoader.h"

#include <FreeImage.h>

#include "ImageConversion.inl"

namespace hemlock {
    namespace io {
        FIBITMAP* makeRGB(FIBITMAP* bmp) {
            FIBITMAP* tmp = bmp;
            bmp = FreeImage_ConvertTo24Bits(bmp);
            FreeImage_Unload(tmp);
            return bmp;
        }
        FIBITMAP* makeRGBA(FIBITMAP* bmp) {
            FIBITMAP* tmp = bmp;
            bmp = FreeImage_ConvertTo32Bits(bmp);
            FreeImage_Unload(tmp);
            return bmp;
        }
    }
}

void hemlock::io::free(BitmapResource& resource) {
    delete[] resource.bytesUI8;
    resource.bytesUI8 = nullptr;
}

hio::BitmapResource hemlock::io::allocate(ImageIOFormat format, ui32 width, ui32 height) {
    BitmapResource resource = { width, height, nullptr };
    ui32 pixelSize;
    switch (format) {
        case ImageIOFormat::RGB_UI8:
            pixelSize = sizeof(ui8) * 3;
            break;
        case ImageIOFormat::RGBA_UI8:
            pixelSize = sizeof(ui8) * 4;
            break;
        case ImageIOFormat::RGB_UI16:
            pixelSize = sizeof(ui16) * 3;
            break;
        case ImageIOFormat::RGBA_UI16:
            pixelSize = sizeof(ui16) * 4;
            break;
        case ImageIOFormat::RGB_F32:
            pixelSize = sizeof(f32) * 3;
            break;
        case ImageIOFormat::RGBA_F32:
            pixelSize = sizeof(f32) * 4;
            break;
        case ImageIOFormat::RGB_F64:
            pixelSize = sizeof(f64) * 3;
            break;
        case ImageIOFormat::RGBA_F64:
            pixelSize = sizeof(f64) * 4;
            break;
        default:
            return resource;
    }
    resource.bytesUI8 = new ui8[pixelSize * width * height];
    return resource;
}

// TODO(Matthew): Better errors (here and basically everywhere).
hio::BitmapResource hemlock::io::load(const char* filepath, ImageIOFormat format, bool flipV) {
    BitmapResource resource = { 0, 0, nullptr };

    FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(filepath);
    if (fif == FIF_UNKNOWN) {
        printf("ERROR: Could not load file: %s!\n", filepath);
        return resource;
    }

    FIBITMAP* bmp = FreeImage_Load(fif, filepath);
    FREE_IMAGE_TYPE bmpType = FreeImage_GetImageType(bmp);
    if (bmpType == FIT_UNKNOWN) {
        printf("ERROR: Image from file: %s is of unknown type.\n", filepath);
        FreeImage_Unload(bmp);
        return resource;
    }
    resource.width = FreeImage_GetWidth(bmp);
    resource.height = FreeImage_GetHeight(bmp);
    
    // Return raw data obtained if no conversion needed.
    if (format == ImageIOFormat::RAW) {
        ui32 bytes = resource.height * FreeImage_GetPitch(bmp);
        resource.data = new ui8[bytes];
        memcpy(resource.data, FreeImage_GetBits(bmp), bytes);
        FreeImage_Unload(bmp);
        return resource;
    }

    bool error = true;
    resource = allocate(format, resource.width, resource.height);
    BMPConvFunc f;
    switch (bmpType) {
        case FIT_BITMAP:
            switch (format) {
                case ImageIOFormat::RGB_UI8:
                case ImageIOFormat::RGB_UI16:
                case ImageIOFormat::RGB_F32:
                case ImageIOFormat::RGB_F64:
                    bmp = makeRGB(bmp);
                    f = flipV ? flip::convRGB8[(size_t)format] : noflip::convRGB8[(size_t)format];
                    f(bmp, resource);
                    break;
                case ImageIOFormat::RGBA_UI8:
                case ImageIOFormat::RGBA_UI16:
                case ImageIOFormat::RGBA_F32:
                case ImageIOFormat::RGBA_F64:
                    bmp = makeRGBA(bmp);
                    f = flipV ? flip::convRGBA8[(size_t)format] : noflip::convRGBA8[(size_t)format];
                    f(bmp, resource);
                    break;
                default:
                    printf("Invalid format requested for file: %s.", filepath);
                    error = true;
                    break;
            }
            break;
        case FIT_UINT16:
            f = flipV ? flip::convUI16[(size_t)format] : noflip::convUI16[(size_t)format];
            f(bmp, resource);
            break;
        case FIT_INT16:
            f = flipV ? flip::convI16[(size_t)format] : noflip::convI16[(size_t)format];
            f(bmp, resource);
            break;
        case FIT_UINT32:
            f = flipV ? flip::convUI32[(size_t)format] : noflip::convUI32[(size_t)format];
            f(bmp, resource);
            break;
        case FIT_INT32:
            f = flipV ? flip::convI32[(size_t)format] : noflip::convI32[(size_t)format];
            f(bmp, resource);
            break;
        case FIT_FLOAT:
            f = flipV ? flip::convF32[(size_t)format] : noflip::convF32[(size_t)format];
            f(bmp, resource);
            break;
        case FIT_DOUBLE:
            f = flipV ? flip::convF64[(size_t)format] : noflip::convF64[(size_t)format];
            f(bmp, resource);
            break;
        case FIT_RGB16:
            f = flipV ? flip::convRGB16[(size_t)format] : noflip::convRGB16[(size_t)format];
            f(bmp, resource);
            break;
        case FIT_RGBA16:
            f = flipV ? flip::convRGBA16[(size_t)format] : noflip::convRGBA16[(size_t)format];
            f(bmp, resource);
            break;
        case FIT_RGBF:
            f = flipV ? flip::convRGBF[(size_t)format] : noflip::convRGBF[(size_t)format];
            f(bmp, resource);
            break;
        case FIT_RGBAF:
            f = flipV ? flip::convRGBAF[(size_t)format] : noflip::convRGBAF[(size_t)format];
            f(bmp, resource);
            break;
        case FIT_COMPLEX:
            puts("Cannae do complex images.");
            error = true;
            break;
        default:
            puts("Cannae do complex images.");
            error = true;
            break;
    }
    FreeImage_Unload(bmp);
    if (error) {
        free(resource);
        resource.data = nullptr;
    }
    return resource;
}

bool hemlock::io::save(const char* filepath, ImageIOFormat format, const void* data, ui32 width, ui32 height) {
    FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(filepath);
    if (fif == FIF_UNKNOWN) {
        printf("Unable to save to file of unknown type: %s!\n", filepath);
        return false;
    }

    ui32 bytes;
    switch (format) {
        case ImageIOFormat::RGB_UI8:   bytes = 3 * sizeof(ui8);  break;
        case ImageIOFormat::RGBA_UI8:  bytes = 4 * sizeof(ui8);  break;
        case ImageIOFormat::RGB_UI16:  bytes = 3 * sizeof(ui16); break;
        case ImageIOFormat::RGBA_UI16: bytes = 4 * sizeof(ui16); break;
        case ImageIOFormat::RGB_F32:   bytes = 3 * sizeof(f32);  break;
        case ImageIOFormat::RGBA_F32:  bytes = 4 * sizeof(f32);  break;
        case ImageIOFormat::RGB_F64:   bytes = 3 * sizeof(f64);  break;
        case ImageIOFormat::RGBA_F64:  bytes = 4 * sizeof(f64);  break;
        default:
            printf("Cannot save image without a format being specified!\n");
            return false;
    }

    FIBITMAP* bmp = FreeImage_Allocate(width, height, bytes << 3);
    
    switch (format) {
        case ImageIOFormat::RGB_UI8:   moveRGB<ui8>  ((ui8*) FreeImage_GetBits(bmp), (ui8*) data, width, height); break;
        case ImageIOFormat::RGBA_UI8:  moveRGBA<ui8> ((ui8*) FreeImage_GetBits(bmp), (ui8*) data, width, height); break;
        case ImageIOFormat::RGB_UI16:  moveRGB<ui16> ((ui16*)FreeImage_GetBits(bmp), (ui16*)data, width, height); break;
        case ImageIOFormat::RGBA_UI16: moveRGBA<ui16>((ui16*)FreeImage_GetBits(bmp), (ui16*)data, width, height); break;
        case ImageIOFormat::RGB_F32:   moveRGB<f32>  ((f32*) FreeImage_GetBits(bmp), (f32*) data, width, height); break;
        case ImageIOFormat::RGBA_F32:  moveRGBA<f32> ((f32*) FreeImage_GetBits(bmp), (f32*) data, width, height); break;
        case ImageIOFormat::RGB_F64:   moveRGB<f64>  ((f64*) FreeImage_GetBits(bmp), (f64*) data, width, height); break;
        case ImageIOFormat::RGBA_F64:  moveRGBA<f64> ((f64*) FreeImage_GetBits(bmp), (f64*) data, width, height); break;
        default:
            printf("Cannot save image without a format being specified!\n");
            return false;
    }

    FreeImage_Save(fif, bmp, filepath);
    FreeImage_Unload(bmp);

    return true;
}
