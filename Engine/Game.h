/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.h																				  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#pragma once

#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include "Board.h"
#include "Snake.h"
#include "Goal.h"
#include "Obstacle.h"
#include "FrameTimer.h"
#include <random>

class Game
{
public:
	Game( class MainWindow& wnd );
	Game( const Game& ) = delete;
	Game& operator=( const Game& ) = delete;
	void Go();
private:
	void ComposeFrame();
	void UpdateModel();
	/********************************/
	/*  User Functions              */
	/********************************/
private:
	enum class GameState
	{
		GAMESTART,
		PLAYING,
		GAMEOVER
	};
private:
	MainWindow& wnd;
	Graphics gfx;
	/********************************/
	/*  User Variables              */
	FrameTimer ft;
	std::mt19937 rng;
	Board brd;
	Snake snake;
	Location delta_loc = { 1,0 };
	Location prev_delta_loc = delta_loc;
	Goal goal;
	Obstacle obstacles[Obstacle::maxObstacle];
	GameState gState = GameState::GAMESTART;
	int obstacleCount = 0;
	int score = 0;
	float snakeMoveCounter = 0.0f;
	float snakeMovePeriod = 12.0f/60.0f;  //I use as a fraction of 60 assuming a framerate of 60 hz
	float maxSpeed = 5.0f / 60.0f;
	/********************************/
};