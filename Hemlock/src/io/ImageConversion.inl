// Credit to Cristian Zaloj 1000%
namespace hemlock {
    namespace io {

#if defined(OS_WINDOWS)
#define R_OFFSET 2
#define G_OFFSET 1
#define B_OFFSET 0
#define A_OFFSET 3
#else
#define R_OFFSET 0
#define G_OFFSET 1
#define B_OFFSET 2
#define A_OFFSET 3
#error Please research and test wtf is FreeImage doing?
#endif


        typedef void(*BMPConvFunc)(FIBITMAP*, hio::BitmapResource&);

        namespace flip {
#define SCAN_Y(Y, H) (Y)
#include "ImageConversionFunctions.inl"
#undef SCAN_Y
        }
        namespace noflip {
#define SCAN_Y(Y, H) (H - 1 - Y)
#include "ImageConversionFunctions.inl"
#undef SCAN_Y
        }

        template <typename T>
        void moveRGB(T* destination, T* source, ui32 width, ui32 height) {
            ui32 end = width * height * 3;
            ui32 si = 0, di = 0;
            while (si < end) {
                destination[di++] = source[si + R_OFFSET];
                destination[di++] = source[si + G_OFFSET];
                destination[di++] = source[si + B_OFFSET];
                si += 3;
            }
        }
        template <typename T>
        void moveRGBA(T* destination, T* source, ui32 width, ui32 height) {
            size_t end = width * height * 4;
            size_t si = 0, di = 0;
            while (si < end) {
                destination[di++] = source[si + R_OFFSET];
                destination[di++] = source[si + G_OFFSET];
                destination[di++] = source[si + B_OFFSET];
                destination[di++] = source[si + A_OFFSET];
                si += 4;
            }
        }
    }
}