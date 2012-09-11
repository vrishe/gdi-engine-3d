#pragma once

#ifdef GDIENGINE3D_EXPORTS
#define GDIENGINE3D_API __declspec(dllexport)
#else
#define GDIENGINE3D_API __declspec(dllimport)
#endif

#include "Render.h"

#ifdef __cplusplus
extern "C" {
#endif

GDIENGINE3D_API void testFunc();

#ifdef __cplusplus
}
#endif

