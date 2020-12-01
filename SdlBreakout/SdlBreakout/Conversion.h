#pragma once
#include "stdafx.h"

#include <SDL.h>

class AxisAlignedRectF;

SDL_FRect ToSdlShape(const AxisAlignedRectF& shape);