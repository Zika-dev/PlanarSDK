#pragma once

#include <memory>

#include "Core.hpp"
#include "Log.hpp"
#include "graphics/library/TextureHandle.hpp"

#include "Utils.hpp"
#include "IDMaker.hpp"

#include "tiled/TiledTypes.hpp"
#include "tiled/ObjectLayer.hpp"
#include "ComponentEventBus.hpp"

namespace core::runtime {

	class PLANAR_API ObjectIdentity {
	public:

		ObjectIdentity();

		ObjectIdentity(const ObjectIdentity& other);
		ObjectIdentity& operator=(const ObjectIdentity& other);

		ObjectIdentity(ObjectIdentity&& other) noexcept;
		ObjectIdentity& operator=(ObjectIdentity&& other) noexcept;

		~ObjectIdentity();

		int get() const noexcept {
			return id;
		}

	private:
		int id = -1;
		std::weak_ptr<ComponentEventBus> eventBus;
	};

	class PLANAR_API ObjectRenderData {

	public:

		Vec2<float> pos{};
		int16_t order = 0;

		ObjectRenderData() = default;

		ObjectRenderData(graphics::TextureHandle textureHandle, Vec2<float> pos = { 0.0f, 0.0f }, uint8_t alpha = 255, bool flipX = false, bool flipY = false, bool isStatic = false) : 
			pos(pos), 
			handle(textureHandle), 
			size(textureHandle.getSize()), 
			alpha(alpha), 
			flipX(flipX), 
			flipY(flipY), 
			isStatic(isStatic)
		{

		}

		void setTextureHandle(graphics::TextureHandle handle) noexcept {
			if (!handle.isValid()) {
				CORE_LOG_ERROR("Could not set texture handle: texture handle is not valid!");
				return;
			}

			size = handle.getSize();
			this->handle = handle;
		}

		graphics::TextureHandle getTextureHandle() const noexcept {
			return handle;
		}

		void setSize(Vec2<uint16_t> newSize) {
			size = newSize;
		}

		Vec2<uint16_t> getSize() const noexcept {
			return size;
		}

		Vec2<int> getRoundedPos() const noexcept {
			return pos.round();
		}

		Vec2<float> getMidPoint() const noexcept {
			return pos + Vec2<float>(size.x / 2, size.y / 2);
		}

		void setMidPoint(Vec2<float> pos) {
			this->pos = pos - Vec2<float>(size.x / 2, size.y / 2);
		}

		void setAlpha(uint8_t alpha) noexcept {
			this->alpha = alpha;
		}

		uint8_t getAlpha() const noexcept {
			return alpha;
		}

		void setFlipX(bool enable) noexcept {
			flipX = enable;
		}

		bool getFlipX() const noexcept {
			return flipX;
		}

		void setFlipY(bool enable) noexcept {
			flipY = enable;
		}

		bool getFlipY() const noexcept {
			return flipY;
		}

		void setStatic(bool enable) noexcept {
			isStatic = enable;
		}

		bool getStatic() const noexcept {
			return isStatic;
		}

	protected:
		uint8_t alpha = 255;
		BlendMode blendMode = BlendMode::NORMAL;

		bool flipX = false;
		bool flipY = false;
		bool isStatic = false;

	private:
		graphics::TextureHandle handle{};
		Vec2<uint16_t> size; // Texture size in pixels

		friend class GameObject;
	};

	class PLANAR_API GameObject : public ObjectRenderData
	{
	public:

		using ObjectRenderData::ObjectRenderData;

		GameObject(tilemap::TileObject object, tilemap::ObjectLayer& layer);

		int getId() const noexcept {
			return id.get();
		}

		bool hasId() const noexcept {
			return id.get() >= 0;
		}

		void copyFromTileObject(tilemap::TileObject object, tilemap::ObjectLayer& layer);

	private:
		ObjectIdentity id;
	};
}