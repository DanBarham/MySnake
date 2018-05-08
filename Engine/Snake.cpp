#include "Snake.h"
#include <assert.h>

void Snake::Segment::InitHead(const Location & _loc, Color _c)
{
	loc = _loc;
	c = _c;
}

void Snake::Segment::InitBody(Color _c)
{
	c = _c;
}

void Snake::Segment::InitBody(const Location & _loc, Color _c)
{
	loc = _loc;
	c = _c;
}

void Snake::Segment::Follow(const Segment & next)
{
	loc = next.loc;
}

void Snake::Segment::MoveBy(const Location & delta_loc)
{
	assert(abs(delta_loc.x) + abs(delta_loc.y) == 1);
	loc.Add(delta_loc);
}

void Snake::Segment::Draw(Board & brd) const
{
	brd.DrawCell(loc, c);
}

const Location& Snake::Segment::GetLocation() const
{
	return loc;
}

Snake::Snake(const Location& loc, std::mt19937 & _rng)
	:
	cDist(75, 255)
{
	headColor = Colors::Yellow;
	segments[0].InitHead(loc, headColor);
	rng = _rng;

	for (int i = 1; i < nSegments; ++i)
	{
		Location temp_loc = loc;
		temp_loc.x -= i;
		segments[i].InitBody(temp_loc, Color(0u, cDist(rng), 0u));
	}
}

void Snake::MoveBy(const Location& delta_loc)
{
	for (int i = nSegments - 1; i > 0; --i)
	{
		segments[i].Follow(segments[i - 1]);
	}
	segments[0].MoveBy(delta_loc);
}

Location Snake::GetNextHeadLocation(const Location& delta_loc) const
{
	Location _loc(segments[0].GetLocation());
	_loc.Add(delta_loc);
	return _loc;
}

void Snake::Grow()
{
	if (nSegments < nSegmentsMax)
	{
		segments[nSegments].InitBody(Color(0u, cDist(rng), 0u));
		++nSegments;
	}
}

void Snake::Draw(Board& brd) const
{
	for (int i = 0; i < nSegments; ++i)
	{
		segments[i].Draw(brd);
	}
}

bool Snake::IsInTile(const Location& loc, bool testEnd) const
{
	int testSegments = nSegments;
	if (!testEnd)
		--testSegments;
	for (int i = 0; i < testSegments; ++i)
	{
		if (segments[i].GetLocation() == loc)
		{
			return true;
		}
	}

	return false;
}