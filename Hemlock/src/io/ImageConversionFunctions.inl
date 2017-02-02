// Credit to Cristian Zaloj 1000%
template<typename FROM, typename TO>
void convertToRGB(FIBITMAP* bmp, hio::BitmapResource& res) {
    size_t i = 0;
    TO* data = (TO*)res.data;
    for (ui32 y = 0; y < res.height; y++) {
        FROM* bits = (FROM*)FreeImage_GetScanLine(bmp, SCAN_Y(y, res.height));
        for (ui32 x = 0; x < res.width; x++) {
            TO v = (TO)bits[x];
            data[i + R_OFFSET] = v;
            data[i + G_OFFSET] = v;
            data[i + A_OFFSET] = v;
            i += 3;
        }
    }
}
template<typename FROM, typename TO, ui32 SHIFT>
void convertToRGBLShift(FIBITMAP* bmp, hio::BitmapResource& res) {
    size_t i = 0;
    TO* data = (TO*)res.data;
    for (ui32 y = 0; y < res.height; y++) {
        FROM* bits = (FROM*)FreeImage_GetScanLine(bmp, SCAN_Y(y, res.height));
        for (ui32 x = 0; x < res.width; x++) {
            TO v = (TO)bits[x] << SHIFT;
            data[i + R_OFFSET] = v;
            data[i + G_OFFSET] = v;
            data[i + B_OFFSET] = v;
            i += 3;
        }
    }
}
template<typename FROM, typename TO, ui32 SHIFT>
void convertToRGBRShift(FIBITMAP* bmp, hio::BitmapResource& res) {
    size_t i = 0;
    TO* data = (TO*)res.data;
    for (ui32 y = 0; y < res.height; y++) {
        FROM* bits = (FROM*)FreeImage_GetScanLine(bmp, SCAN_Y(y, res.height));
        for (ui32 x = 0; x < res.width; x++) {
            TO v = (TO)(bits[x] >> SHIFT);
            data[i + R_OFFSET] = v;
            data[i + G_OFFSET] = v;
            data[i + B_OFFSET] = v;
            i += 3;
        }
    }
}
template<typename FROM, typename TO>
void convertToRGBA(FIBITMAP* bmp, hio::BitmapResource& res) {
    size_t i = 0;
    TO* data = (TO*)res.data;
    for (ui32 y = 0; y < res.height; y++) {
        FROM* bits = (FROM*)FreeImage_GetScanLine(bmp, SCAN_Y(y, res.height));
        for (ui32 x = 0; x < res.width; x++) {
            TO v = (TO)bits[x];
            data[i + R_OFFSET] = v;
            data[i + G_OFFSET] = v;
            data[i + B_OFFSET] = v;
            data[i + A_OFFSET] = v;
            i += 4;
        }
    }
}
template<typename FROM, typename TO, ui32 SHIFT>
void convertToRGBALShift(FIBITMAP* bmp, hio::BitmapResource& res) {
    size_t i = 0;
    TO* data = (TO*)res.data;
    for (ui32 y = 0; y < res.height; y++) {
        FROM* bits = (FROM*)FreeImage_GetScanLine(bmp, SCAN_Y(y, res.height));
        for (ui32 x = 0; x < res.width; x++) {
            TO v = (TO)bits[x] << SHIFT;
            data[i + R_OFFSET] = v;
            data[i + G_OFFSET] = v;
            data[i + B_OFFSET] = v;
            data[i + A_OFFSET] = v;
            i += 4;
        }
    }
}
template<typename FROM, typename TO, ui32 SHIFT>
void convertToRGBARShift(FIBITMAP* bmp, hio::BitmapResource& res) {
    size_t i = 0;
    TO* data = (TO*)res.data;
    for (ui32 y = 0; y < res.height; y++) {
        FROM* bits = (FROM*)FreeImage_GetScanLine(bmp, SCAN_Y(y, res.height));
        for (ui32 x = 0; x < res.width; x++) {
            TO v = (TO)(bits[x] >> SHIFT);
            data[i + R_OFFSET] = v;
            data[i + G_OFFSET] = v;
            data[i + B_OFFSET] = v;
            data[i + A_OFFSET] = v;
            i += 4;
        }
    }
}

template<typename FROM, typename TO, FROM DIV>
void convertToRGBF(FIBITMAP* bmp, hio::BitmapResource& res) {
    size_t i = 0;
    TO* data = (TO*)res.data;
    for (ui32 y = 0; y < res.height; y++) {
        FROM* bits = (FROM*)FreeImage_GetScanLine(bmp, SCAN_Y(y, res.height));
        for (ui32 x = 0; x < res.width; x++) {
            TO v = (TO)bits[x] / (TO)DIV;
            data[i + R_OFFSET] = v;
            data[i + G_OFFSET] = v;
            data[i + B_OFFSET] = v;
            i += 3;
        }
    }
}
template<typename FROM, typename TO, FROM DIV>
void convertToRGBAF(FIBITMAP* bmp, hio::BitmapResource& res) {
    size_t i = 0;
    TO* data = (TO*)res.data;
    for (ui32 y = 0; y < res.height; y++) {
        FROM* bits = (FROM*)FreeImage_GetScanLine(bmp, SCAN_Y(y, res.height));
        for (ui32 x = 0; x < res.width; x++) {
            TO v = (TO)bits[x] / (TO)DIV;
            data[i + R_OFFSET] = v;
            data[i + G_OFFSET] = v;
            data[i + B_OFFSET] = v;
            data[i + A_OFFSET] = v;
            i += 4;
        }
    }
}

template<typename FROM, typename TO, TO MUL>
void convertFToRGB(FIBITMAP* bmp, hio::BitmapResource& res) {
    size_t i = 0;
    TO* data = (TO*)res.data;
    for (ui32 y = 0; y < res.height; y++) {
        FROM* bits = (FROM*)FreeImage_GetScanLine(bmp, SCAN_Y(y, res.height));
        for (ui32 x = 0; x < res.width; x++) {
            TO v = (TO)(bits[x] * (FROM)MUL);
            data[i + R_OFFSET] = v;
            data[i + G_OFFSET] = v;
            data[i + B_OFFSET] = v;
            i += 3;
        }
    }
}
template<typename FROM, typename TO, TO MUL>
void convertFToRGBA(FIBITMAP* bmp, hio::BitmapResource& res) {
    size_t i = 0;
    TO* data = (TO*)res.data;
    for (ui32 y = 0; y < res.height; y++) {
        FROM* bits = (FROM*)FreeImage_GetScanLine(bmp, SCAN_Y(y, res.height));
        for (ui32 x = 0; x < res.width; x++) {
            TO v = (TO)(bits[x] * (FROM)MUL);
            data[i + R_OFFSET] = v;
            data[i + G_OFFSET] = v;
            data[i + B_OFFSET] = v;
            data[i + A_OFFSET] = v;
            i += 4;
        }
    }
}

template<typename FROM, typename TO, ui32 RSHIFT>
void convertRGBToRGB(FIBITMAP* bmp, hio::BitmapResource& res) {
    size_t i = 0;
    TO* data = (TO*)res.data;
    ui32 endX = res.width * 3;
    for (ui32 y = 0; y < res.height; y++) {
        FROM* bits = (FROM*)FreeImage_GetScanLine(bmp, SCAN_Y(y, res.height));
        for (ui32 x = 0; x < endX;) {
            data[i + R_OFFSET] = (TO)(bits[x++] >> RSHIFT);
            data[i + G_OFFSET] = (TO)(bits[x++] >> RSHIFT);
            data[i + B_OFFSET] = (TO)(bits[x++] >> RSHIFT);
            i += 3;
        }
    }
}
template<typename FROM, typename TO, ui32 RSHIFT>
void convertRGBToRGBA(FIBITMAP* bmp, hio::BitmapResource& res) {
    size_t i = 0;
    TO* data = (TO*)res.data;
    ui32 endX = res.width * 3;
    for (ui32 y = 0; y < res.height; y++) {
        FROM* bits = (FROM*)FreeImage_GetScanLine(bmp, SCAN_Y(y, res.height));
        for (ui32 x = 0; x < endX;) {
            data[i + R_OFFSET] = (TO)(bits[x++] >> RSHIFT);
            data[i + G_OFFSET] = (TO)(bits[x++] >> RSHIFT);
            data[i + B_OFFSET] = (TO)(bits[x++] >> RSHIFT);
            data[i + A_OFFSET] = (TO)(~0u);
            i += 4;
        }
    }
}
template<typename FROM, typename TO, ui32 RSHIFT>
void convertRGBAToRGB(FIBITMAP* bmp, hio::BitmapResource& res) {
    size_t i = 0;
    TO* data = (TO*)res.data;
    ui32 endX = res.width * 4;
    for (ui32 y = 0; y < res.height; y++) {
        FROM* bits = (FROM*)FreeImage_GetScanLine(bmp, SCAN_Y(y, res.height));
        for (ui32 x = 0; x < endX;) {
            data[i + R_OFFSET] = (TO)(bits[x++] >> RSHIFT);
            data[i + G_OFFSET] = (TO)(bits[x++] >> RSHIFT);
            data[i + B_OFFSET] = (TO)(bits[x++] >> RSHIFT);
            i += 3;
            x++;
        }
    }
}
template<typename FROM, typename TO, ui32 RSHIFT>
void convertRGBAToRGBA(FIBITMAP* bmp, hio::BitmapResource& res) {
    size_t i = 0;
    TO* data = (TO*)res.data;
    ui32 endX = res.width * 4;
    for (ui32 y = 0; y < res.height; y++) {
        FROM* bits = (FROM*)FreeImage_GetScanLine(bmp, SCAN_Y(y, res.height));
        for (ui32 x = 0; x < endX;) {
            data[i + R_OFFSET] = (TO)(bits[x++] >> RSHIFT);
            data[i + G_OFFSET] = (TO)(bits[x++] >> RSHIFT);
            data[i + B_OFFSET] = (TO)(bits[x++] >> RSHIFT);
            data[i + A_OFFSET] = (TO)(bits[x++] >> RSHIFT);
            i += 4;
        }
    }
}

template<typename FROM, typename TO, ui32 LSHIFT>
void convertRGBToRGBLShift(FIBITMAP* bmp, hio::BitmapResource& res) {
    size_t i = 0;
    TO* data = (TO*)res.data;
    ui32 endX = res.width * 3;
    for (ui32 y = 0; y < res.height; y++) {
        FROM* bits = (FROM*)FreeImage_GetScanLine(bmp, SCAN_Y(y, res.height));
        for (ui32 x = 0; x < endX;) {
            data[i + R_OFFSET] = (TO)bits[x++] << LSHIFT;
            data[i + G_OFFSET] = (TO)bits[x++] << LSHIFT;
            data[i + B_OFFSET] = (TO)bits[x++] << LSHIFT;
            i += 3;
        }
    }
}
template<typename FROM, typename TO, ui32 LSHIFT>
void convertRGBToRGBALShift(FIBITMAP* bmp, hio::BitmapResource& res) {
    size_t i = 0;
    TO* data = (TO*)res.data;
    ui32 endX = res.width * 3;
    for (ui32 y = 0; y < res.height; y++) {
        FROM* bits = (FROM*)FreeImage_GetScanLine(bmp, SCAN_Y(y, res.height));
        for (ui32 x = 0; x < endX;) {
            data[i + R_OFFSET] = (TO)bits[x++] << LSHIFT;
            data[i + G_OFFSET] = (TO)bits[x++] << LSHIFT;
            data[i + B_OFFSET] = (TO)bits[x++] << LSHIFT;
            data[i + A_OFFSET] = (TO)(~0u);
            i += 4;
        }
    }
}
template<typename FROM, typename TO, ui32 LSHIFT>
void convertRGBAToRGBLShift(FIBITMAP* bmp, hio::BitmapResource& res) {
    size_t i = 0;
    TO* data = (TO*)res.data;
    ui32 endX = res.width * 4;
    for (ui32 y = 0; y < res.height; y++) {
        FROM* bits = (FROM*)FreeImage_GetScanLine(bmp, SCAN_Y(y, res.height));
        for (ui32 x = 0; x < endX;) {
            data[i + R_OFFSET] = (TO)bits[x++] << LSHIFT;
            data[i + G_OFFSET] = (TO)bits[x++] << LSHIFT;
            data[i + B_OFFSET] = (TO)bits[x++] << LSHIFT;
            i += 3;
            x++;
        }
    }
}
template<typename FROM, typename TO, ui32 LSHIFT>
void convertRGBAToRGBALShift(FIBITMAP* bmp, hio::BitmapResource& res) {
    size_t i = 0;
    TO* data = (TO*)res.data;
    ui32 endX = res.width * 4;
    for (ui32 y = 0; y < res.height; y++) {
        FROM* bits = (FROM*)FreeImage_GetScanLine(bmp, SCAN_Y(y, res.height));
        for (ui32 x = 0; x < endX;) {
            data[i + R_OFFSET] = (TO)bits[x++] << LSHIFT;
            data[i + G_OFFSET] = (TO)bits[x++] << LSHIFT;
            data[i + B_OFFSET] = (TO)bits[x++] << LSHIFT;
            data[i + A_OFFSET] = (TO)bits[x++] << LSHIFT;
            i += 4;
        }
    }
}

template<typename FROM, typename TO, FROM DIV>
void convertRGBToRGBF(FIBITMAP* bmp, hio::BitmapResource& res) {
    size_t i = 0;
    TO* data = (TO*)res.data;
    ui32 endX = res.width * 3;
    for (ui32 y = 0; y < res.height; y++) {
        FROM* bits = (FROM*)FreeImage_GetScanLine(bmp, SCAN_Y(y, res.height));
        for (ui32 x = 0; x < endX;) {
            data[i + R_OFFSET] = (TO)bits[x++] / (TO)DIV;
            data[i + G_OFFSET] = (TO)bits[x++] / (TO)DIV;
            data[i + B_OFFSET] = (TO)bits[x++] / (TO)DIV;
            i += 3;
        }
    }
}
template<typename FROM, typename TO, FROM DIV>
void convertRGBToRGBAF(FIBITMAP* bmp, hio::BitmapResource& res) {
    size_t i = 0;
    TO* data = (TO*)res.data;
    ui32 endX = res.width * 3;
    for (ui32 y = 0; y < res.height; y++) {
        FROM* bits = (FROM*)FreeImage_GetScanLine(bmp, SCAN_Y(y, res.height));
        for (ui32 x = 0; x < endX;) {
            data[i + R_OFFSET] = (TO)bits[x++] / (TO)DIV;
            data[i + G_OFFSET] = (TO)bits[x++] / (TO)DIV;
            data[i + B_OFFSET] = (TO)bits[x++] / (TO)DIV;
            data[i + A_OFFSET] = DIV;
            i += 4;
        }
    }
}
template<typename FROM, typename TO, FROM DIV>
void convertRGBAToRGBF(FIBITMAP* bmp, hio::BitmapResource& res) {
    size_t i = 0;
    TO* data = (TO*)res.data;
    ui32 endX = res.width * 4;
    for (ui32 y = 0; y < res.height; y++) {
        FROM* bits = (FROM*)FreeImage_GetScanLine(bmp, SCAN_Y(y, res.height));
        for (ui32 x = 0; x < endX;) {
            data[i + R_OFFSET] = (TO)bits[x++] / (TO)DIV;
            data[i + G_OFFSET] = (TO)bits[x++] / (TO)DIV;
            data[i + B_OFFSET] = (TO)bits[x++] / (TO)DIV;
            i += 3;
            x++;
        }
    }
}
template<typename FROM, typename TO, FROM DIV>
void convertRGBAToRGBAF(FIBITMAP* bmp, hio::BitmapResource& res) {
    size_t i = 0;
    TO* data = (TO*)res.data;
    ui32 endX = res.width * 4;
    for (ui32 y = 0; y < res.height; y++) {
        FROM* bits = (FROM*)FreeImage_GetScanLine(bmp, SCAN_Y(y, res.height));
        for (ui32 x = 0; x < endX;) {
            data[i + R_OFFSET] = (TO)bits[x++] / (TO)DIV;
            data[i + G_OFFSET] = (TO)bits[x++] / (TO)DIV;
            data[i + B_OFFSET] = (TO)bits[x++] / (TO)DIV;
            data[i + A_OFFSET] = (TO)bits[x++] / (TO)DIV;
            i += 4;
        }
    }
}

template<typename FROM, typename TO>
void convertRGBFToRGBF(FIBITMAP* bmp, hio::BitmapResource& res) {
    size_t i = 0;
    TO* data = (TO*)res.data;
    ui32 endX = res.width * 3;
    for (ui32 y = 0; y < res.height; y++) {
        FROM* bits = (FROM*)FreeImage_GetScanLine(bmp, SCAN_Y(y, res.height));
        for (ui32 x = 0; x < endX;) {
            data[i + R_OFFSET] = (TO)bits[x++];
            data[i + G_OFFSET] = (TO)bits[x++];
            data[i + B_OFFSET] = (TO)bits[x++];
            i += 3;
        }
    }
}
template<typename FROM, typename TO>
void convertRGBFToRGBAF(FIBITMAP* bmp, hio::BitmapResource& res) {
    size_t i = 0;
    TO* data = (TO*)res.data;
    ui32 endX = res.width * 3;
    for (ui32 y = 0; y < res.height; y++) {
        FROM* bits = (FROM*)FreeImage_GetScanLine(bmp, SCAN_Y(y, res.height));
        for (ui32 x = 0; x < endX;) {
            data[i + R_OFFSET] = (TO)bits[x++];
            data[i + G_OFFSET] = (TO)bits[x++];
            data[i + B_OFFSET] = (TO)bits[x++];
            data[i + A_OFFSET] = (TO)1.0;
            i += 4;
        }
    }
}
template<typename FROM, typename TO>
void convertRGBAFToRGBF(FIBITMAP* bmp, hio::BitmapResource& res) {
    size_t i = 0;
    TO* data = (TO*)res.data;
    ui32 endX = res.width * 4;
    for (ui32 y = 0; y < res.height; y++) {
        FROM* bits = (FROM*)FreeImage_GetScanLine(bmp, SCAN_Y(y, res.height));
        for (ui32 x = 0; x < endX;) {
            data[i + R_OFFSET] = (TO)bits[x++];
            data[i + G_OFFSET] = (TO)bits[x++];
            data[i + B_OFFSET] = (TO)bits[x++];
            i += 3;
            x++;
        }
    }
}
template<typename FROM, typename TO>
void convertRGBAFToRGBAF(FIBITMAP* bmp, hio::BitmapResource& res) {
    size_t i = 0;
    TO* data = (TO*)res.data;
    ui32 endX = res.width * 4;
    for (ui32 y = 0; y < res.height; y++) {
        FROM* bits = (FROM*)FreeImage_GetScanLine(bmp, SCAN_Y(y, res.height));
        for (ui32 x = 0; x < endX;) {
            data[i + R_OFFSET] = (TO)bits[x++];
            data[i + G_OFFSET] = (TO)bits[x++];
            data[i + B_OFFSET] = (TO)bits[x++];
            data[i + A_OFFSET] = (TO)bits[x++];
            i += 4;
        }
    }
}

template<typename FROM, typename TO, TO MUL>
void convertRGBFToRGB(FIBITMAP* bmp, hio::BitmapResource& res) {
    size_t i = 0;
    TO* data = (TO*)res.data;
    ui32 endX = res.width * 3;
    for (ui32 y = 0; y < res.height; y++) {
        FROM* bits = (FROM*)FreeImage_GetScanLine(bmp, SCAN_Y(y, res.height));
        for (ui32 x = 0; x < endX;) {
            data[i + R_OFFSET] = (TO)(bits[x++] * (FROM)MUL);
            data[i + G_OFFSET] = (TO)(bits[x++] * (FROM)MUL);
            data[i + B_OFFSET] = (TO)(bits[x++] * (FROM)MUL);
            i += 3;
        }
    }
}
template<typename FROM, typename TO, TO MUL>
void convertRGBFToRGBA(FIBITMAP* bmp, hio::BitmapResource& res) {
    size_t i = 0;
    TO* data = (TO*)res.data;
    ui32 endX = res.width * 3;
    for (ui32 y = 0; y < res.height; y++) {
        FROM* bits = (FROM*)FreeImage_GetScanLine(bmp, SCAN_Y(y, res.height));
        for (ui32 x = 0; x < endX;) {
            data[i + R_OFFSET] = (TO)(bits[x++] * (FROM)MUL);
            data[i + B_OFFSET] = (TO)(bits[x++] * (FROM)MUL);
            data[i + G_OFFSET] = (TO)(bits[x++] * (FROM)MUL);
            data[i + A_OFFSET] = MUL;
            i += 4;
        }
    }
}
template<typename FROM, typename TO, TO MUL>
void convertRGBAFToRGB(FIBITMAP* bmp, hio::BitmapResource& res) {
    size_t i = 0;
    TO* data = (TO*)res.data;
    ui32 endX = res.width * 4;
    for (ui32 y = 0; y < res.height; y++) {
        FROM* bits = (FROM*)FreeImage_GetScanLine(bmp, SCAN_Y(y, res.height));
        for (ui32 x = 0; x < endX;) {
            data[i + R_OFFSET] = (TO)(bits[x++] * (FROM)MUL);
            data[i + G_OFFSET] = (TO)(bits[x++] * (FROM)MUL);
            data[i + B_OFFSET] = (TO)(bits[x++] * (FROM)MUL);
            i += 3;
            x++;
        }
    }
}
template<typename FROM, typename TO, TO MUL>
void convertRGBAFToRGBA(FIBITMAP* bmp, hio::BitmapResource& res) {
    size_t i = 0;
    TO* data = (TO*)res.data;
    ui32 endX = res.width * 4;
    for (ui32 y = 0; y < res.height; y++) {
        FROM* bits = (FROM*)FreeImage_GetScanLine(bmp, SCAN_Y(y, res.height));
        for (ui32 x = 0; x < endX;) {
            data[i + R_OFFSET] = (TO)(bits[x++] * (FROM)MUL);
            data[i + G_OFFSET] = (TO)(bits[x++] * (FROM)MUL);
            data[i + B_OFFSET] = (TO)(bits[x++] * (FROM)MUL);
            data[i + A_OFFSET] = (TO)(bits[x++] * (FROM)MUL);
            i += 4;
        }
    }
}

hio::BMPConvFunc convUI16[] = {
    nullptr,
    convertToRGBRShift<ui16, ui8, 8>,
    convertToRGBARShift<ui16, ui8, 8>,
    convertToRGB<ui16, ui16>,
    convertToRGBA<ui16, ui16>,
    convertToRGBF<ui16, f32, 65535u>,
    convertToRGBAF<ui16, f32, 65535u>,
    convertToRGBF<ui16, f64, 65535u>,
    convertToRGBAF<ui16, f64, 65535u>
};
hio::BMPConvFunc convI16[] = {
    nullptr,
    convertToRGBRShift<i16, ui8, 8>,
    convertToRGBARShift<i16, ui8, 8>,
    convertToRGB<i16, ui16>,
    convertToRGBA<i16, ui16>,
    convertToRGBF<i16, f32, 32767>,
    convertToRGBAF<i16, f32, 32767>,
    convertToRGBF<i16, f64, 32767>,
    convertToRGBAF<i16, f64, 32767>
};
hio::BMPConvFunc convUI32[] = {
    nullptr,
    convertToRGBRShift<ui32, ui8, 24>,
    convertToRGBARShift<ui32, ui8, 24>,
    convertToRGBRShift<ui32, ui16, 16>,
    convertToRGBARShift<ui32, ui16, 16>,
    convertToRGBF<ui32, f32, 4294967295u>,
    convertToRGBAF<ui32, f32, 4294967295u>,
    convertToRGBF<ui32, f64, 4294967295u>,
    convertToRGBAF<ui32, f64, 4294967295u>
};
hio::BMPConvFunc convI32[] = {
    nullptr,
    convertToRGBRShift<i32, ui8, 24>,
    convertToRGBARShift<i32, ui8, 24>,
    convertToRGBRShift<i32, ui16, 16>,
    convertToRGBARShift<i32, ui16, 16>,
    convertToRGBF<i32, f32, 2147483647>,
    convertToRGBAF<i32, f32, 2147483647>,
    convertToRGBF<i32, f64, 2147483647>,
    convertToRGBAF<i32, f64, 2147483647>
};
hio::BMPConvFunc convF32[] = {
    nullptr,
    convertFToRGB<f32, ui8, 255u>,
    convertFToRGBA<f32, ui8, 255u>,
    convertFToRGB<f32, ui16, 65535u>,
    convertFToRGBA<f32, ui16, 65535u>,
    convertToRGB<f32, f32>,
    convertToRGBA<f32, f32>,
    convertToRGB<f32, f64>,
    convertToRGBA<f32, f64>
};
hio::BMPConvFunc convF64[] = {
    nullptr,
    convertFToRGB<f64, ui8, 255u>,
    convertFToRGBA<f64, ui8, 255u>,
    convertFToRGB<f64, ui16, 65535u>,
    convertFToRGBA<f64, ui16, 65535u>,
    convertToRGB<f64, f32>,
    convertToRGBA<f64, f32>,
    convertToRGB<f64, f64>,
    convertToRGBA<f64, f64>
};
hio::BMPConvFunc convRGB8[] = {
    nullptr,
    convertRGBToRGB<ui8, ui8, 0>,
    convertRGBToRGBA<ui8, ui8, 0>,
    convertRGBToRGBLShift<ui8, ui16, 8>,
    convertRGBToRGBALShift<ui8, ui16, 8>,
    convertRGBToRGBF<ui8, f32, 255u>,
    convertRGBToRGBAF<ui8, f32, 255u>,
    convertRGBToRGBF<ui8, f64, 255u>,
    convertRGBToRGBAF<ui8, f64, 255u>
};
hio::BMPConvFunc convRGBA8[] = {
    nullptr,
    convertRGBAToRGB<ui8, ui8, 0>,
    convertRGBAToRGBA<ui8, ui8, 0>,
    convertRGBAToRGBLShift<ui8, ui16, 8>,
    convertRGBAToRGBALShift<ui8, ui16, 8>,
    convertRGBAToRGBF<ui8, f32, 255u>,
    convertRGBAToRGBAF<ui8, f32, 255u>,
    convertRGBAToRGBF<ui8, f64, 255u>,
    convertRGBAToRGBAF<ui8, f64, 255u>
};
hio::BMPConvFunc convRGB16[] = {
    nullptr,
    convertRGBToRGB<ui16, ui8, 8>,
    convertRGBToRGBA<ui16, ui8, 8>,
    convertRGBToRGB<ui16, ui16, 0>,
    convertRGBToRGBA<ui16, ui16, 0>,
    convertRGBToRGBF<ui16, f32, 65535u>,
    convertRGBToRGBAF<ui16, f32, 65535u>,
    convertRGBToRGBF<ui16, f64, 65535u>,
    convertRGBToRGBAF<ui16, f64, 65535u>
};
hio::BMPConvFunc convRGBA16[] = {
    nullptr,
    convertRGBAToRGB<ui16, ui8, 8>,
    convertRGBAToRGBA<ui16, ui8, 8>,
    convertRGBAToRGB<ui16, ui16, 0>,
    convertRGBAToRGBA<ui16, ui16, 0>,
    convertRGBAToRGBF<ui16, f32, 65535u>,
    convertRGBAToRGBAF<ui16, f32, 65535u>,
    convertRGBAToRGBF<ui16, f64, 65535u>,
    convertRGBAToRGBAF<ui16, f64, 65535u>
};
hio::BMPConvFunc convRGBF[] = {
    nullptr,
    convertRGBFToRGB<f32, ui8, 255u>,
    convertRGBFToRGBA<f32, ui8, 255u>,
    convertRGBFToRGB<f32, ui16, 65535u>,
    convertRGBFToRGBA<f32, ui16, 65535u>,
    convertRGBFToRGBF<f32, f32>,
    convertRGBFToRGBAF<f32, f32>,
    convertRGBFToRGBF<f32, f64>,
    convertRGBFToRGBAF<f32, f64>
};
hio::BMPConvFunc convRGBAF[] = {
    nullptr,
    convertRGBAFToRGB<f32, ui8, 255u>,
    convertRGBAFToRGBA<f32, ui8, 255u>,
    convertRGBAFToRGB<f32, ui16, 65535u>,
    convertRGBAFToRGBA<f32, ui16, 65535u>,
    convertRGBAFToRGBF<f32, f32>,
    convertRGBAFToRGBAF<f32, f32>,
    convertRGBAFToRGBF<f32, f64>,
    convertRGBAFToRGBAF<f32, f64>
};