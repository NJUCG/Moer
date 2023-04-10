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

#include <vector>
#include <mutex>
#include <optional>
#include <memory>
#include "CoreLayer/Geometry/Geometry.h"

/*@brief Iterator for Point2i.
* It will generate points between pBegin and pEnd but pEnd is not included.
* e.g, pBegin=[0,0], pEnd=[2,2], and the generated points will be 
* [0,0], [1,0], [0,1] and [1,1].
* */
class PointIterator {

protected:
    // @brief the first point of a tile.
    Point2i pBegin;

    // @brief the last point of a tile.
    Point2i pEnd;

    // @brief current point in tile.
    Point2i currentP;

    int xMin, xMax;
    int yMin, yMax;

public:
    PointIterator(const Point2i &_pBegin, const Point2i &_pEnd, const Point2i &p);

    Point2i operator*() const;

    PointIterator &operator++();

    bool operator!=(const PointIterator &anotherIt);
};

/*
* @brief Tile representation, and generate points within the tile.
* Check PointIterator for more information.
*/
class Tile {

protected:
    Point2i pBegin;
    Point2i pEnd;

public:
    Tile(const Point2i &_pBegin, const Point2i &_pEnd);

    virtual PointIterator begin() const = 0;
    virtual PointIterator end() const = 0;
};

/*
* @brief Basic Tile implemention. Generate points in a square.
*/
class SquareTile : public Tile {
private:
    PointIterator beginIte;
    PointIterator endIte;

public:
    SquareTile(const Point2i &_pBegin, const Point2i &_pEnd);

    virtual PointIterator begin() const;
    virtual PointIterator end() const;
};

class TileGenerator {

protected:
    // @brief the resolution of render image.
    Point2i resolution;

    // @brief lock for generateNextTile.
    std::mutex mute;

public:
    TileGenerator(const Point2i &_resolution);

    // @brief generate all tiles. No mutex.
    virtual std::vector<std::shared_ptr<Tile>> generateTiles() = 0;

    /*
	* @brief generate next tile.
	* Use mutex to make sure that threads will have different tiles.
	*/
    virtual std::optional<std::shared_ptr<Tile>> generateNextTile() = 0;

    /*
     * avoid delete mutex
     */
    virtual ~TileGenerator(){};

    // @brief the number of tiles
    int tileCount;
};