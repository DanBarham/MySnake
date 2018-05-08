#pragma once

#include "Snake.h"
#include "Board.h"
#include <random>

class Obstacle
{
public:
	void Spawn(std::mt19937& rng, const Board& brd, const Snake& snake, const Location& goal_loc);
	void Draw(Board& brd) const;
	const Location& GetLocation() const;
	static constexpr int maxObstacle = 50;
private:
	bool init = false;
	static constexpr Color c = Color(150,150,150);
	Location loc;
};