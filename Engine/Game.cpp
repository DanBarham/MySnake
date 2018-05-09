/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
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
#include "MainWindow.h"
#include "Game.h"
#include "SpriteCodex.h"
#include <assert.h>

//Main game loop

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	brd( gfx ),
	rng( std::random_device()() ),
	snake( {4,4}, rng ),	//Snake starting position (x,y)
	goal(rng, brd, snake)
{
}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	const float frameTime = ft.Mark();

	if (!gameStart)
	{
		//Starting splash screen VK_RETURN = Enter
		//After gameStart = true, then we start updating the game
		if (wnd.kbd.KeyIsPressed(VK_RETURN))
		{
			gameStart = true;
		}
	}
	else if (!gameOver)	//While end game conditions = false
	{
		/*Keyboard controls (up,down,left,right)
		The second condition in the if statements is to prevent the snake from immediately turning back on itself*/
		if (wnd.kbd.KeyIsPressed(VK_UP) && prev_delta_loc.y != 1)
		{
				delta_loc = { 0,-1 };
		}
		else if (wnd.kbd.KeyIsPressed(VK_DOWN) && prev_delta_loc.y != -1)
		{
			delta_loc = { 0,1 };
		}
		else if (wnd.kbd.KeyIsPressed(VK_LEFT) && prev_delta_loc.x != 1)
		{
			delta_loc = { -1,0 };
		}
		else if (wnd.kbd.KeyIsPressed(VK_RIGHT) && prev_delta_loc.x != -1)
		{
			delta_loc = { 1,0 };
		}

		/*snakeMoveCounter and snakeMovePeriod work so that the snake doesn't move every frame
		the snake ends up moving every 60/snakeMovePeriod frames*/
		++snakeMoveCounter;
		if (snakeMoveCounter >= snakeMovePeriod)
		{
			snakeMoveCounter = 0;

			//The for loop + first if statement test for game over conditions
			const Location next = snake.GetNextHeadLocation(delta_loc);
			for (int i = 0; i < obstacleCount; ++i)	//if you collide w/ an obstacle
			{
				if (obstacles[i].GetLocation() == next)
				{
					gameOver = true;
					return;
				}
			}
			if (!brd.IsInsideBoard(next) || snake.IsInTile(next, false) || gameOver)  //if you collide w/ the edge or w/ yourself
			{
				gameOver = true;
			}
			else {	//if the game isn't over update the game
				const bool eating = (next == goal.GetLocation());	//tests if you are colliding w/ the goal
				if (eating)
				{
					snake.Grow();
				}
				snake.MoveBy(delta_loc);
				prev_delta_loc = delta_loc;
				if (eating)
				{
					goal.Respawn(rng, brd, snake, obstacles, obstacleCount);
					if (obstacleCount < Obstacle::maxObstacle)
					{
						obstacles[obstacleCount].Spawn(rng, brd, snake, goal.GetLocation());
						++obstacleCount;
					}
		
					++score;
					if (score % 2 == 0)	//speed up the game every 2 goals
					{
						--snakeMovePeriod;
						if (snakeMovePeriod < 5)	//Max speed, remember lower = faster
						{
							snakeMovePeriod = 5;
						}
					}
				}
			}
		}
	}
}

void Game::ComposeFrame()
{
	if (!gameStart)
	{
		SpriteCodex::DrawTitle(Graphics::ScreenWidth / 2 - 106, Graphics::ScreenHeight / 2 - 80, gfx);
	}
	else
	{
		brd.DrawBorder();
		snake.Draw(brd);
		for (int i = 0; i < obstacleCount; ++i)
		{
			obstacles[i].Draw(brd);
		}
		goal.Draw(brd);
		if (gameOver)
		{
			SpriteCodex::DrawGameOver(Graphics::ScreenWidth / 2 - 41, Graphics::ScreenHeight / 2 - 31, gfx);
		}
	}
}
