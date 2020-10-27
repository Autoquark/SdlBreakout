#pragma once

#include "stdafx.h"

#include "AxisAlignedRectF.h"
#include <SDL.h>

SDL_FRect ToSdlShape(const AxisAlignedRectF& shape);