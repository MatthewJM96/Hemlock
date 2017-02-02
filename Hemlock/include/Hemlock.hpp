#pragma once

#include "stdafx.h"

#include "camera\BasicCamera.h"
#include "graphics\GLSLProgram.h"
#include "graphics\Window.h"

namespace hemlock {
    void init() {
        FreeImage_Initialise();
    }

    void dispose() {
        FreeImage_DeInitialise();
    }
}
namespace h = hemlock;
