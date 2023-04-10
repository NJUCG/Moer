/**
 * @file SequenceTileGenerator.cpp
 * @author orbitchen
 * @brief Implemention of SequenceTileGenerator.
 * @version 0.1
 * @date 2022-7-7
 *
 * @copyright NJUMeta (c) 2022
 * www.njumeta.com
 *
 */

#include "SequenceTileGenerator.h"

SequenceTileGenerator::SequenceTileGenerator(const Point2i &_resolution, int _size)
    : TileGenerator(_resolution) {
    size = _size;

    int xCount = resolution.x / size;
    int yCount = resolution.y / size;

    for (int i = 0; i < xCount; i++)
        xList.push_back(i * size);
    if (resolution.x % size != 0)
        xList.push_back(xCount * size);
    xList.push_back(resolution.x);

    for (int i = 0; i < yCount; i++)
        yList.push_back(i * size);
    if (resolution.y % size != 0)
        yList.push_back(yCount * size);
    yList.push_back(resolution.y);

    currentBeginIndex = Point2i(0, 0);

    tileCount = xList.size() * yList.size();
}

std::vector<std::shared_ptr<Tile>> SequenceTileGenerator::generateTiles() {
    std::vector<std::shared_ptr<Tile>> retVal;

    int yListLen = yList.size();
    int xListLen = xList.size();
    for (int j = 0; j < yListLen - 1; j++) {
        for (int i = 0; i < xListLen - 1; i++) {
            retVal.push_back(std::make_shared<SquareTile>(
                Point2i(xList[i], yList[j]),
                Point2i(xList[i + 1], yList[j + 1])));
        }
    }

    return retVal;
}

std::optional<std::shared_ptr<Tile>> SequenceTileGenerator::generateNextTile() {
    //lock
    mute.lock();

    if (reachedEnd) {
        mute.unlock();
        return std::nullopt;
    }

    int yListLen = yList.size();
    int xListLen = xList.size();

    // last one
    if (currentBeginIndex.x == xListLen - 2 && currentBeginIndex.y == yListLen - 2 && reachedEnd == false) {
        reachedEnd = true;
        auto retVal = std::make_optional<std::shared_ptr<Tile>>(
            std::make_shared<SquareTile>(
                Point2i(xList[currentBeginIndex.x], yList[currentBeginIndex.y]),
                Point2i(xList[currentBeginIndex.x + 1], yList[currentBeginIndex.y + 1])));
        mute.unlock();
        return retVal;
    }

    std::optional<std::shared_ptr<Tile>> retVal =
        std::make_optional<std::shared_ptr<Tile>>(std::make_shared<SquareTile>(
            Point2i(xList[currentBeginIndex.x], yList[currentBeginIndex.y]),
            Point2i(xList[currentBeginIndex.x + 1], yList[currentBeginIndex.y + 1])));

    if (currentBeginIndex.x < xListLen - 2) {
        currentBeginIndex.x++;
    } else if (currentBeginIndex.y < yListLen - 2) {
        currentBeginIndex.x = 0;
        currentBeginIndex.y++;
    }
    // unlock and return
    mute.unlock();
    return retVal;
}
