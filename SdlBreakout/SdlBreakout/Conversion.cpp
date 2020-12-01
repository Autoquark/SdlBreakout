#include "stdafx.h"
#include "Conversion.h"

#include "AxisAlignedRectF.h"

SDL_FRect ToSdlShape(const AxisAlignedRectF& shape)
{
	return SDL_FRect
	{
		shape.position.x,
		shape.position.y,
		shape.size.x,
		shape.size.y,
	};
}
