#pragma once

#include <vector>
#include <cstdint>
#include "Utils.hpp"

static int8_t nextSlotmapId = 1;

// Generation is used to get a unique token so that a handle isn't accidentally deemed valid
struct Handle {
	uint16_t index = 0;
	uint16_t generation = 0;
	int8_t id = -1;

	static constexpr size_t serializedSize() noexcept {
		return sizeof(uint16_t) * 2 + sizeof(int8_t);
	}

	// If this handle is part of a SlotMap
	bool valid() const noexcept {
		return id > 0;
	}

	friend bool operator==(const Handle&, const Handle&) = default;
};

template<typename T>
class SlotMap {

public:

	struct Slot {
		uint16_t denseIndex;
		uint16_t generation;
	};

private:
	std::vector<T> dense;
	std::vector<Slot> slots;
	std::vector<uint32_t> freeList;
	std::vector<uint32_t> denseToSlotIdx;

	int8_t thisId{};
	
public:

	// Iterator
	using iterator = std::vector<T>::iterator;
	using constIterator = std::vector<T>::const_iterator;

	iterator       begin()        { return dense.begin(); }
    iterator       end()          { return dense.end();   }
    constIterator  begin()  const { return dense.begin(); }
    constIterator  end()    const { return dense.end();   }
    constIterator  cbegin() const { return dense.cbegin(); }
    constIterator  cend()   const { return dense.cend();   }

	SlotMap(uint16_t size = 0) {
		if (size > 0) {
			reserve(size);
		}

		thisId = nextSlotmapId++;
	}

	Handle add(T&& obj) {
		uint16_t slotIdx{};

		if (freeList.empty()) {
			slotIdx = slots.size();
			slots.push_back({ 0, 0 });
		}
		else { // Take an index from the free list
			slotIdx = freeList.back();
			freeList.pop_back();
		}

		slots[slotIdx].denseIndex = dense.size();
		denseToSlotIdx.push_back(slotIdx);
		dense.push_back(std::forward<T>(obj));

		return { slotIdx, slots[slotIdx].generation, thisId };
	}

	void remove(Handle h) {
		if (!valid(h)) return;

		uint32_t slotIdx = h.index;
		uint32_t denseIdx = slots[slotIdx].denseIndex;
		uint32_t last = dense.size() - 1;
		
		// Move and pop
		if (dense.size() > 1) {
			dense[denseIdx] = std::move(dense[last]);

			denseToSlotIdx[denseIdx] = denseToSlotIdx[last];

			// Update the slot so that it indexes to the correct object
			slots[denseToSlotIdx[denseIdx]].denseIndex = denseIdx;
		}

		dense.pop_back();
		denseToSlotIdx.pop_back();

		// Invalidate handles
		slots[slotIdx].generation++;

		// Add this slot to the free list
		freeList.push_back(slotIdx);
	}

	bool valid(Handle h) const noexcept {
		return h.index < slots.size() && h.generation == slots[h.index].generation && h.id == thisId;
	}

	T* get(Handle h) {
		if (!valid(h)) return nullptr;

		return &dense[slots[h.index].denseIndex];
	}

	uint32_t size() const noexcept {
		return dense.size();
	}

	Handle getNextHandle() const noexcept {
		uint16_t slotIdx{};
		uint16_t generation = 0;

		if (freeList.empty()) {
			slotIdx = slots.size();
		}
		else {
			slotIdx = freeList.back();
			generation = slots[slotIdx].generation;
		} 

		return { slotIdx, generation, thisId };
	}

	std::vector<T>& getDense() {
		return dense;
	}

	void reserve(uint32_t size) {
		dense.reserve(size);
		slots.reserve(size);
		denseToSlotIdx.reserve(size);
	}
};
