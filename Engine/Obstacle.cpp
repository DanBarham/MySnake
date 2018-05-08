#include "Obstacle.h"

void Obstacle::Spawn(std::mt19937 & rng, const Board & brd, const Snake & snake, const Location& goal_loc)
{
	if (!init)
	{
		init = true;
		std::uniform_int_distribution<int> xDist(0, brd.GetGridWidth() - 1);
		std::uniform_int_distribution<int> yDist(0, brd.GetGridHeight() - 1);

		Location temp_loc;

		do
		{
			temp_loc.x = xDist(rng);
			temp_loc.y = yDist(rng);
		} while (snake.IsInTile(temp_loc, true) && temp_loc != goal_loc);

		loc = temp_loc;
	}
}

void Obstacle::Draw(Board & brd) const
{
	if (init)
	{
		brd.DrawCell(loc, c);
	}
}

const Location & Obstacle::GetLocation() const
{
	return loc;
}