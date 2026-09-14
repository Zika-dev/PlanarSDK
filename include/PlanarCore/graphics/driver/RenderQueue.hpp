#pragma once

#include "Core.hpp"
#include "runtime/GameObject.hpp"

#include "graphics/library/ParticleSystemHandle.hpp"
#include "graphics/library/Shapes.hpp"

#include "RenderObjects.hpp"

namespace core::graphics {
	class PLANAR_API RenderQueue {
	public:
		RenderQueue(size_t size = 16000, size_t maxRenderObjects = 2048);

		void addGameObject(const runtime::ObjectRenderData& g, Vec2<float> screenPos);

		void addParticles(const ParticleSystemHandle& handle, Vec2<int16_t> pos, int16_t order);

		void addLine(Line line);

		void addRect(Rectangle rect);

		void addCircle(Circle circle);

		void sort();

		size_t getIndex() const noexcept {
			return index;
		}

		void reset() noexcept;

		AnyRenderObject& getObject(size_t i);

		uint16_t getNumObjects() const noexcept {
			return numSortObjects;
		}

	private:

		struct SortObject {
			uint32_t order;
			uint16_t index;
		};

		uint16_t numSortObjects = 0;

		std::vector<AnyRenderObject> renderObjects;
		std::vector<SortObject> sortList;

		size_t index = 0;

		const char* TAG = "RenderQueue";
	};
}