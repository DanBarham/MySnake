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
	snake( {4,4},rng ),	//Snake starting position (x,y)
	goal( rng,brd,snake ),
	eat( L"Eat.wav" ),
	gameMusic( L"Music_Loop.wav",.0f,3.42f ),
	gameOverSound( L"Fart.wav" ),
	titleSound( L"Title.wav" ),
	slitherSound( { L"Slither0.wav",L"Slither1.wav",L"Slither2.wav" } )
{
	titleSound.Play();
	gameMusic.Play();
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


	if ( gState == GameState::PLAYING )
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

		/*snakeMoveCounter adds up the accumulated time and updates the game logic after
		it has reached the snakeMovePeriod, it gets reset and process begins again*/
		snakeMoveCounter += frameTime;
		if (snakeMoveCounter >= snakeMovePeriod)
		{
			snakeMoveCounter = 0.0f;

			//The for loop + first if statement test for game over conditions
			const Location next = snake.GetNextHeadLocation(delta_loc);
			for (int i = 0; i < obstacleCount; ++i)	//if you collide w/ an obstacle
			{
				if (obstacles[i].GetLocation() == next)
				{
					gState = GameState::GAMEOVER;
					gameOverSound.Play();
					gameMusic.StopAll();
					return;
				}
			}
			if (!brd.IsInsideBoard(next) || snake.IsInTile(next, false) || gState == GameState::GAMEOVER)  //if you collide w/ the edge or w/ yourself
			{
				gState = GameState::GAMEOVER;
				gameMusic.StopAll();
				gameOverSound.Play();
			}
			else {	//if the game isn't over update the game
				const bool eating = (next == goal.GetLocation());	//tests if you are colliding w/ the goal
				if (eating)
				{
					snake.Grow();
					eat.Play();
				}
				slitherSound.Play( rng );
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
						snakeMovePeriod -= 1.0f/60.0f;
						if (snakeMovePeriod < maxSpeed)
						{
							snakeMovePeriod = maxSpeed;
						}
					}
				}
			}
		}
	}
	else if ( gState == GameState::GAMESTART )
	{
		//Starting splash screen VK_RETURN = Enter
		//After gameStart = true, then we start updating the game
		if (wnd.kbd.KeyIsPressed(VK_RETURN))
		{
			gState = GameState::PLAYING;
		}
	}
}

void Game::ComposeFrame()
{
	if (gState == GameState::GAMESTART)
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
		if (gState == GameState::GAMEOVER)
		{
			SpriteCodex::DrawGameOver(Graphics::ScreenWidth / 2 - 41, Graphics::ScreenHeight / 2 - 31, gfx);
		}
	}
}
