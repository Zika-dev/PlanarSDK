#pragma once

#include "Core.hpp"
#include "SlotMap.hpp"
#include <variant>
#include "runtime/GameObject.hpp"
#include "graphics/library/Shapes.hpp"

namespace core::graphics {

	class SDKGraphics;

	enum class RenderType : uint8_t {
		SPRITE,
		PARTICLES,
		RECTANGLE,
		TEXT,
		LINE,
		CIRCLE,

		NONE,
	};

	struct PLANAR_API RenderObject {

		RenderType TYPE = RenderType::NONE;
		int16_t order = 0;
		int16_t originalOrder = 0;

		RenderObject() = default;

		virtual ~RenderObject() = default;

		virtual void draw(int tileY, SDKGraphics* consoleGraphics) const = 0;
	};

	struct PLANAR_API SpriteRenderObject : RenderObject {
		SpriteRenderObject(const runtime::ObjectRenderData& g) :
			handle(g.getTextureHandle().getHandle()), position(g.getRoundedPos()),
			alpha (g.getAlpha()), flipX(g.getFlipX()), flipY(g.getFlipY()) {
			TYPE = RenderType::SPRITE;
			order = g.order;
		}

		SpriteRenderObject() {
			TYPE = RenderType::SPRITE;
		}

		Handle handle;
		Vec2<int16_t> position;

		uint8_t alpha = 255;
		bool flipX = false;
		bool flipY = false;

		static constexpr size_t serializedSize() noexcept {
			return sizeof(RenderType) + sizeof(position.x) + sizeof(position.y)
				+ Handle::serializedSize() + sizeof(alpha)
				+ sizeof(flipX)
				+ sizeof(flipY);
		}

		void draw(int tileY, SDKGraphics* consoleGraphics) const override;
	};

	struct PLANAR_API  ParticleRenderObject : RenderObject {
		ParticleRenderObject() {
			TYPE = RenderType::PARTICLES;
		}

		Handle handle;
		Vec2<int16_t> position;

		static constexpr size_t serializedSize() noexcept {
			return sizeof(RenderType) + sizeof(position.x) + sizeof(position.y)
				+ Handle::serializedSize();
		}

		void draw(int tileY, SDKGraphics* consoleGraphics) const override;
	};

	struct PLANAR_API  LineRenderObject : RenderObject {
		LineRenderObject() {
			TYPE = RenderType::LINE;
		}

		Line line;

		static constexpr size_t serializedSize() noexcept {
			return sizeof(RenderType) + sizeof(Line) +
				+ Handle::serializedSize();
		}

		void draw(int tileY, SDKGraphics* consoleGraphics) const override;
	};

	struct PLANAR_API  RectRenderObject : RenderObject {
		RectRenderObject() {
			TYPE = RenderType::RECTANGLE;
		}

		Rectangle rect;

		static constexpr size_t serializedSize() noexcept {
			return sizeof(RenderType) + sizeof(Rectangle) +
				+Handle::serializedSize();
		}

		void draw(int tileY, SDKGraphics* consoleGraphics) const override;
	};

	struct PLANAR_API  CircleRenderObject : RenderObject {
		CircleRenderObject() {
			TYPE = RenderType::LINE;
		}

		Circle circle;

		static constexpr size_t serializedSize() noexcept {
			return sizeof(RenderType) + sizeof(Circle) +
				+Handle::serializedSize();
		}

		void draw(int tileY, SDKGraphics* consoleGraphics) const override;
	};

	using AnyRenderObject = std::variant<
		SpriteRenderObject, 
		ParticleRenderObject,
		LineRenderObject,
		RectRenderObject,
		CircleRenderObject
	>;
}