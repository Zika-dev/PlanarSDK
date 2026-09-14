#pragma once

#include "Core.hpp"
#include "Utils.hpp"

#include "graphics/library/ParticleSystem.hpp"
#include "graphics/library/Camera.hpp"
#include "graphics/library/Texture.hpp"
#include "graphics/library/Shapes.hpp"

namespace core::graphics {

	// Virtual render buffer
	class PLANAR_API  Rasterizer {

	public:
		Rasterizer(SDL_Renderer* renderer);

		void resetBuffer();

		void clear(RGB565 color);

		void rasterizerToScreen();

		void fastBlit(int tileY, Texture& texture, Vec2<int16_t> pos, int alpha = 255, bool flipX = false, bool flipY = false, BlendMode mode = BlendMode::NORMAL);

		void putPixel(int tileY, Vec2<int16_t> pos, RGB565 color, uint8_t alpha);

		void drawParticleSystem(int tileY, ParticleSystem& particleSystem, Camera& camera, Texture* texture);

		void drawLine(int tileY, Line line);

		void drawCircle(int tileY, Circle circle);
		
		void drawRectangle(int tileY, Rectangle rect);

		int getTileHeight() const noexcept;

		void copyTileBufferToScreenBuffer(int tileY);

		SDL_Texture& getRenderTexture() noexcept {
			return *bufferTex;
		}

		SDL_Texture* consoleTextureToSDLTexture(Texture& texture);

	private:

		int fastAdditiveBlend(int alpha, int color2, int color1, int clamp) const noexcept ;
		int fastNormalBlend(int alpha, int src, int dst, int clamp) const noexcept ;

		void blendCopy(RGB565* dest, const RGB565* src, uint32_t pixelCount, int alpha, bool flipX, BlendMode mode = BlendMode::NORMAL) const noexcept;

		void blendSet(RGB565* dest, uint32_t pixelCount, RGB565 color, int alpha, BlendMode mode = BlendMode::NORMAL);

		RGB565 blendPixel(RGB565 a, RGB565 b, int alpha, BlendMode mode = BlendMode::NORMAL) const noexcept;

		void drawHLine(int tileY, Vec2<int16_t> start, int16_t len, RGB565 color, uint8_t alpha);

		void drawVLine(int tileY, Vec2<int16_t> start, int16_t len, RGB565 color, uint8_t alpha);
		
		void drawRectFilled(int tileY, Rectangle rect);

		void drawRectHollow(int tileY, Rectangle rect);

		void fillStep(int tileY, int x, int y, int xSinceYIncrement, int prevX, const Circle& circle);

		void fillColorFast(RGB565* buf, uint32_t pixelCount, RGB565 color);

		std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> bufferTex{nullptr, &SDL_DestroyTexture};
		
		SDL_Renderer* renderer;

		static constexpr int TILE_HEIGHT = 80;

		std::unique_ptr<RGB565[]> buffer;
		std::unique_ptr<RGB565[]> tileBuffer;
	};
}