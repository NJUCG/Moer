/**
 * @file TileGenerator.h
 * @author orbitchen
 * @brief Generate tiles for different threads in renderer.
 * @version 0.1
 * @date 2022-04-30
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 *
 */
#pragma once

#include "CoreLayer/Geometry/Geometry.h"

#include <vector>

// @brief Iterator for Point2i.
class PointIterator
{
	Point2i pBegin;
	Point2i pEnd;
	Point2i p;

public:

	PointIterator(const Point2i& _pBegin, const Point2i& _pEnd, const Point2i& p);

	Point2i operator*() const;

	PointIterator& operator++();

	bool operator!=(const PointIterator& anotherIt);

};

class Tile
{
	Point2i pBegin;
	Point2i pEnd;

public:

	Tile(const Point2i pBegin, const Point2i pEnd);

	PointIterator begin() const;
	PointIterator end() const;
};

class TileGenerator
{
public:

	TileGenerator();

	// @brief generate all tiles.
	std::vector<Tile> generateTiles();

	// @brief generate next tile.
	Tile generateNext();

};