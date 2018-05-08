#pragma once

#include "Board.h"
#include <random>

class Snake
{
private:
	class Segment
	{
	public:
		void InitHead(const Location& _loc, Color _c);
		void InitBody(Color _c);
		void InitBody(const Location& _loc, Color _c);
		void Follow(const Segment& next);
		void MoveBy(const Location& delta_loc);
		void Draw(Board& brd) const;
		const Location& GetLocation() const;
	private:
		Location loc;
		Color c;
	};
public:
	Snake(const Location& loc, std::mt19937& _rng);
	void MoveBy(const Location& delta_loc);
	Location GetNextHeadLocation(const Location & delta_loc) const;
	void Grow();
	void Draw(Board& brd) const;
	bool IsInTile(const Location & loc, bool testEnd) const;
private:
	Color headColor;
	static constexpr int nSegmentsMax = 100;
	Segment segments[nSegmentsMax];
	std::mt19937 rng;
	std::uniform_int_distribution<int> cDist;
	int nSegments = 4;
};