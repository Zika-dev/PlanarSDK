#pragma once

#include <cstdint>
#include <vector>
#include <algorithm>
#include <map>

#include "Utils.hpp"

// Spatial hash grid, implementation inspired by Matthias Müller
class SpatialHash {

public:
	
	SpatialHash(uint32_t cellSize = 128, uint32_t maxObjects = 1024, uint32_t tableSize = 1024);

	void reset();

	void createGrid(std::vector<Vec2<float>> coords, uint32_t n);

	void query(Vec2<float> pos, float distance);

	void queryCell(Vec2<int32_t> pos);

	void clearQuery();

	std::vector<int>& getQueryResult() {
		return queryResult;
	}

	uint32_t getCellSize() const noexcept {
		return cellSize;
	}

	float getDiagonalLength() const noexcept {
		return diagonalLen;
	}

	std::map<Vec2<int32_t>, bool>& getCellPositions() {
		return cellPositions;
	}

private:

	uint32_t hash(Vec2<int32_t> pos);

	Vec2<int32_t> getCellCoords(Vec2<float> pos);

	int32_t getCellCoord(float coord);

	uint32_t hashObject(Vec2<float> pos);

	/*
		The cell size determines the side length of each cell in the spatial grid in pixels.
		It should be bigger than the biggest object's size to prevent objects from being in multiple cells, but smaller cell sizes can lead to more efficient queries.
		Thus you should try not to create hit boxes that are much bigger than the cell size. It is better to subdivide them.

		Table size determines the maximum number of cells in the grid. It should be a power of 2 for faster modulus operation.
		Increasing this value not only adds more cells to the grid, but also decreases the chance of hash collisions, which can lead to more efficient queries.

		Max objects is the maximum number of objects that can be stored in the grid. It should be bigger than the expected number of objects in the scene.
	*/

	uint32_t cellSize = 128;
	uint32_t tableSize = 1024;
	uint32_t maxObjects = 1024;

	float diagonalLen = 0;

	std::map<Vec2<int32_t>, bool> cellPositions;

	std::vector<uint32_t> hashTable; // Each entry corresponds to a cell and contains the index of the first object in that cell
	std::vector<uint32_t> objectIds; // List of object ids sorted by their cell, used for querying objects in a cell
	std::vector<int> queryResult;
};