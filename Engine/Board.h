#pragma once

#include "Graphics.h"
#include "Location.h"

class Board
{
public:
	Board( Graphics& _gfx );
	void DrawCell(const Location& loc, Color c);
	void DrawBorder();
	int GetGridHeight() const;
	int GetGridWidth() const;
	bool IsInsideBoard(const Location& loc) const;
private:
	static constexpr int dimension = 23;
	static constexpr int width = 33;
	static constexpr int height = 25;
	static constexpr int x = (Graphics::ScreenWidth - dimension * width) / 2;
	static constexpr int y = (Graphics::ScreenHeight - dimension * height) / 2;
	static constexpr int cellPadding = 2;
	static constexpr int borderPadding = 5;
	Graphics& gfx;
};