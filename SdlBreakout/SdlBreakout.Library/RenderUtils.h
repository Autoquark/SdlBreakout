#pragma once

#include <SDL.h>
#include <SDL_ttf.h>

#include "Vector2.h"

class RenderUtils
{
public:
	RenderUtils() = delete;

	/// <summary>
	/// Renders a string to the given renderer
	/// </summary>
	/// <param name="renderer"></param>
	/// <param name="position">Pixel position to place the anchor point of the text rectangle</param>
	/// <param name="anchorPoint">A vector between (0,0) and (1,1) representing where within the text's bounding rectangle the anchor point should be. E.g. 0,0 positions the top left of the text at the given
	/// position while (1, 1) would position the bottom right of the text at the position</param>
	/// <param name="text"></param>
	/// <param name="font"></param>
	/// <param name="color"></param>
	/// <param name="preciseHeight">If false, the height of the text's bounding rectangle will be based on the height maximum height needed by any combination of glyphs in the font. If true, the
	/// </param>height will be trimmed to the minimum required to fit the characters actually being rendered.
	static void RenderText(SDL_Renderer* renderer,
		Vector2<int> position,
		Vector2F anchorPoint,
		const std::string& text,
		TTF_Font* font,
		const SDL_Color& color = SDL_Color{ 255, 255, 255 },
		bool preciseHeight = false)
	{
		auto surfaceMessage = TTF_RenderText_Shaded(font, text.c_str(), color, SDL_Color());
		auto message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

		auto sourceRect = SDL_Rect{ 0, 0, surfaceMessage->w, surfaceMessage->h };

		auto descent = TTF_FontDescent(font);
		auto fontHeight = TTF_FontHeight(font);
		auto lineSkip = TTF_FontLineSkip(font);

		// The height of the generated surface will be based on the font height which is set to accomodate the tallest character in the font. If preciseHeight is true, we trim the height to
		// accomodate the tallest character we're actually using
		if (preciseHeight)
		{
			//auto glyph = TTF_RenderGlyph_Shaded(font, '0', SDL_Color{ 255, 255, 255 }, SDL_Color{ 255, 0, 0 });

			auto fontAscent = TTF_FontAscent(font);
			//auto baselineY = height - fontAscent;

			// This will be the greatest descent below the baseline of any glyph being rendered. We get this as a negative number
			int miny = 0;
			// This will be the greatest ascent above the baseline of any glyph being rendered. We get this as a positive number
			int maxy = 0;
			for (char character : text)
			{
				int charMiny, charMaxy;
				if (TTF_GlyphMetrics(font, character, nullptr, nullptr, &charMiny, &charMaxy, nullptr) != 0)
				{
					throw new std::exception();
				}

				miny = std::min(miny, charMiny);
				maxy = std::max(maxy, charMaxy);
			}

			// Bear in mind that we're converting from font coordinates, where up is +y, to SDL coordinates, where up is -y. Fun!
			sourceRect.y = fontAscent - maxy;
			sourceRect.h = maxy - miny;
		}

		auto topLeftPosition = position - (Vector2<int>)anchorPoint.ElementMultiply(Vector2F((float)sourceRect.w, (float)sourceRect.h));
		auto destinationRect = SDL_Rect{ topLeftPosition.x, topLeftPosition.y, sourceRect.w, sourceRect.h };
		
		SDL_RenderCopy(renderer, message, &sourceRect, &destinationRect);
	}
};

