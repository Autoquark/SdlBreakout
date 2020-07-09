// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Exclude min and max macros which conflict with those in <algorithm>
#define NOMINMAX
// Windows Header Files
#include <windows.h>

// if this is not before the STL one, it multiply defines M_PI
#include <corecrt_math_defines.h>



// reference additional headers your program requires here
