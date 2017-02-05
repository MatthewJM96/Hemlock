#pragma once

#include "stdafx.h"

#include "camera\BasicCamera.h"
#include "graphics\GLSLProgram.h"
#include "graphics\Window.h"

namespace hemlock {
    void init() {
        ilInit();
    }

    void dispose() {
        ilShutDown();
    }
}
namespace h = hemlock;
