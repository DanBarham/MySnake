#include "Goal.h"

Goal::Goal(std::mt19937 & rng, const Board & brd, const Snake & snake)
{
	Respawn(rng, brd, snake);
}

void Goal::Respawn(std::mt19937 & rng, const Board & brd, const Snake & snake)
{
	std::uniform_int_distribution<int> xDist(0, brd.GetGridWidth() - 1);
	std::uniform_int_distribution<int> yDist(0, brd.GetGridHeight() - 1);

	Location temp_loc;

	do
	{
		temp_loc.x = xDist(rng);
		temp_loc.y = yDist(rng);
	} while (snake.IsInTile(temp_loc, true));

	loc = temp_loc;
}

void Goal::Respawn(std::mt19937 & rng, const Board & brd, const Snake & snake, const Obstacle * obstacles, int obstacleCount)
{
	//Some random bullshit
	std::uniform_int_distribution<int> xDist(0, brd.GetGridWidth() - 1);
	std::uniform_int_distribution<int> yDist(0, brd.GetGridHeight() - 1);

	bool in_Obstacle;
	Location temp_loc;
	do
	{
		in_Obstacle = false;
		temp_loc.x = xDist(rng);
		temp_loc.y = yDist(rng);

		for (int i = 0; i < obstacleCount; ++i) {
			if (obstacles[i].GetLocation() == temp_loc)
			{
				in_Obstacle = true;
			}
		}
	} while (snake.IsInTile(temp_loc, true) || in_Obstacle);

	loc = temp_loc;
}

void Goal::Draw(Board & brd) const
{
	brd.DrawCell(loc, c);
}

const Location & Goal::GetLocation() const
{
	return loc;
}