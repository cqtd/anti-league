#ifndef _PCH_H_
#define _PCH_H_

#pragma once

#include <dwmapi.h>
#include <thread>

#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include <imconfig.h>
#include <imgui.h>
#include <imgui_freetype.h>
#include <imgui_impl_dx9.h>

#define _EXTERNAL
#include "../shared/offset.hpp"

/// COMMON ///
#include <cstdio>
#include <d3dx9.h>
#include <ctime>
#include "Shellscalingapi.h"

/// UTILITY ///
#include "Vector3f.hpp"
#include "macro.hpp"

/// CORE ///
#include "renderer.h"
#include "d3d.h"
#include "loop.h"
#include "memory.h"

#endif