#pragma once

namespace hemlock {
    namespace io {
        namespace Image {
            enum class FileFormat : ILenum {
                UNKNOWN = IL_TYPE_UNKNOWN,
                BMP = IL_BMP,
                CUT = IL_CUT,
                DDS = IL_DDS,
                DOOM = IL_DOOM,
                DOOM_FLAT = IL_DOOM_FLAT,
                GIF = IL_GIF,
                ICO = IL_ICO,
                JPG = IL_JPG,
                LIF = IL_LIF,
                MNG = IL_MNG,
                PCD = IL_PCD,
                PCX = IL_PCX,
                PIC = IL_PIC,
                PNG = IL_PNG,
                PNM = IL_PNM,
                PSD = IL_PSD,
                PSP = IL_PSP,
                SGI = IL_SGI,
                TGA = IL_TGA,
                TIF = IL_TIF,
                RAW = IL_RAW,
                JASC_PAL = IL_JASC_PAL
            };

            enum class Format : ILenum {
                RGB = IL_RGB,
                RGBA = IL_RGBA,
                BGR = IL_BGR,
                BGRA = IL_BGRA,
                LUMINANCE = IL_LUMINANCE
            };

            enum class Type : ILenum {
                I8 = IL_BYTE,
                I16 = IL_SHORT,
                I32 = IL_INT,
                F32 = IL_FLOAT,
                F64 = IL_DOUBLE,
                UI8 = IL_UNSIGNED_BYTE,
                UI16 = IL_UNSIGNED_SHORT,
                UI32 = IL_UNSIGNED_INT
            };

            enum class Error : ILenum {
                NONE = IL_NO_ERROR,
                ILLEGAL_OPERATION = IL_ILLEGAL_OPERATION,
                INVALID_FILEPATH = IL_INVALID_PARAM,
                COULD_NOT_OPEN_FILE = IL_COULD_NOT_OPEN_FILE,
                CORRUPT_HEADER = IL_INVALID_FILE_HEADER,
                CORRUPT_DATA = IL_ILLEGAL_FILE_VALUE,
                OUT_OF_MEMORY = IL_OUT_OF_MEMORY,
                JPEG_LOAD_FAIL = IL_LIB_JPEG_ERROR,
                PNG_LOAD_FAIL = IL_LIB_PNG_ERROR
            };

            void** allocate(std::vector<ui32> widths, std::vector<ui32> heights, std::vector<Format> formats, std::vector<Type> types, std::vector<ui32>& sizes);
            void** allocate(ui32* widths, ui32* heights, Format* formats, Type* types, ui32*& sizes, ui32 count);
            void*  allocate(ui32 width, ui32 height, Format format, Type type, ui32& size);

            ui32*  load(std::vector<const char*> filepaths, std::vector<FileFormat> types);
            ui32*  load(const char** filepaths, FileFormat* types, ui32 count);
            ui32   load(const char* filepath, FileFormat type = FileFormat::UNKNOWN);

            ui32*  load(std::vector<const void*> data, std::vector<ui32> sizes, std::vector<FileFormat> types);
            ui32*  load(const void** data, ui32* sizes, FileFormat* types, ui32 count);
            ui32   load(const void* data, ui32 size, FileFormat type);

            ui8**  getData(std::vector<ui32> imageIDs);
            ui8**  getData(ui32* imageIDs, ui32 count);
            ui8*   getData(ui32 imageID);

            void   unload(std::vector<ui32> imageIDs);
            void   unload(ui32* imageIDs, ui32 count);
            void   unload(ui32 imageID);

            bool   save(std::vector<ui32> imageIDs, std::vector<const char*> filepaths, std::vector<FileFormat> types, bool overwrite = true);
            bool   save(ui32* imageIDs, const char** filepaths, ui32 count, FileFormat* types = nullptr, bool overwrite = true);
            bool   save(ui32 imageID, const char* filepath, FileFormat type = FileFormat::UNKNOWN, bool overwrite = true);

            void** convert(std::vector<ui32> imageIds, std::vector<Format> formats, std::vector<Type> types, std::vector<ui32>& sizes);
            void** convert(ui32* imageIDs, Format* formats, Type* types, ui32*& sizes, ui32 count);
            void*  convert(ui32 imageID, Format format, Type type, ui32& size);

            ui32*  convertAndLoad(std::vector<ui32> imageIDs, std::vector<Format> formats, std::vector<Type> types, std::vector<FileFormat> fformats);
            ui32*  convertAndLoad(ui32* imageIDs, Format* formats, Type* types, FileFormat* fformats, ui32 count);
            ui32   convertAndLoad(ui32 imageID, Format format, Type type, FileFormat fformat);

            Error getError();
        };
    }
}
namespace hio = hemlock::io;
