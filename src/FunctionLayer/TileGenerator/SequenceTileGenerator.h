/**
 * @file SequenceTileGenerator.h
 * @author orbitchen
 * @brief TileGenerator thats generate tiles sequentially, from top-left to down-right.
 * @version 0.1
 * @date 2022-7-7
 *
 * @copyright NJUMeta (c) 2022
 * www.njumeta.com
 *
 */

#pragma once

#include "TileGenerator.h"

class SequenceTileGenerator : public TileGenerator
{

private:

	std::vector<int> xList;
	std::vector<int> yList;

	Point2i currentBeginIndex;
	// currentEndIndex=[currentBeginIndex.x+1,currentBeginIndex.y+1]
	bool reachedEnd=false;

protected:

	//@brief the size of a tile. The default size of a tile is 16x16.
	int size;

public:

	SequenceTileGenerator(const Point2i& _resolution,int _size=16);

	virtual std::vector<std::shared_ptr<Tile>> generateTiles();

	virtual std::optional<std::shared_ptr<Tile>> generateNextTile();

};