/**
 * @file TileGenerator.cpp
 * @author orbitchen
 * @brief Implemention of SquareTile and pointIterator.
 * @version 0.1
 * @date 2022-7-7
 *
 * @copyright NJUMeta (c) 2022
 * www.njumeta.com
 *
 */

#include "TileGenerator.h"

// implemention of PointIterator

PointIterator::PointIterator(const Point2i& _pBegin, const Point2i& _pEnd,const Point2i& p)
{
	pBegin = _pBegin;
	pEnd = _pEnd;
	currentP = p;

	xMin = pBegin.x;
	yMin = pBegin.y;
	xMax = pEnd.x;
	yMax = pEnd.y;
}

Point2i PointIterator::operator*() const
{
	return currentP;
}

PointIterator& PointIterator::operator++()
{
	if (currentP.x < xMax-1) {
		currentP.x++;
	}
	else if (currentP.y < yMax-1) {
		// currentP.x = xMax-1
		currentP.x = xMin;
		currentP.y++;
	}

	// do nothing if iteration is over.

	return *this;
}

bool PointIterator::operator!=(const PointIterator& anotherIt)
{
	// try to make it stop earlier.
	return currentP != anotherIt.currentP
		&& pBegin != anotherIt.pBegin 
		&& pEnd != anotherIt.pEnd;
}

// implemention of SquareTile

SquareTile::SquareTile(const Point2i& _pBegin, const Point2i& _pEnd)
{
	pBegin = _pBegin;
	pEnd = _pEnd;

	// TODO: check pBegin < pEnd and assert/log.

	beginIte = PointIterator(pBegin, pEnd, pBegin);
	endIte = PointIterator(pBegin, pEnd, Point2i(pEnd.x - 1, pEnd.y - 1));
}

PointIterator SquareTile::begin() const
{
	return beginIte;
}

PointIterator SquareTile::end() const
{
	return endIte;
}