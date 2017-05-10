#pragma once

#include <map>
#include <queue>
#include <set>
#include <tuple>
#include <unordered_map>
#include <vector>

#include <cstring>
#include <string>

#include <algorithm>
#include <functional>
#include <memory>
#include <type_traits>
#include <utility>

#include <iostream>
#include <fstream>

#include <sdl/SDL.h>
#include <gl/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <IL/il.h>
#define ILUT_USE_OPENGL
#include <IL/ilut.h>

#define STX_NO_STD_ANY
#include <Any.hpp>

#include "types.h"

#include "io/FileLoader.h"
#include "Event.hpp"

#if !defined(CHUNK_SIZE)
#define CHUNK_SIZE 16
#endif
