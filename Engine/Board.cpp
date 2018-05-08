#include "Board.h"
#include <assert.h>

Board::Board( Graphics& _gfx )
	:
	gfx( _gfx )
{
}

void Board::DrawCell(const Location & loc, Color c)
{
	assert(loc.x >= 0);
	assert(loc.x < width);
	assert(loc.y >= 0);
	assert(loc.y < height);

	gfx.DrawRectDim((loc.x * dimension) + x, (loc.y * dimension) + y, dimension - cellPadding, dimension - cellPadding, c);
}

void Board::DrawBorder()
{
	//Top
	gfx.DrawRectDim(x - borderPadding, y - borderPadding,
		(dimension * width) + (borderPadding * 2) - cellPadding, borderPadding - cellPadding,
		Colors::White);
	//Bottom
	gfx.DrawRectDim(x - borderPadding, y + (dimension * height),
		(dimension * width) + (borderPadding * 2) - cellPadding, borderPadding - cellPadding,
		Colors::White);
	//Left
	gfx.DrawRectDim(x - borderPadding, y - cellPadding,
		borderPadding - cellPadding, (dimension*height) + cellPadding,
		Colors::White);
	//Right
	gfx.DrawRectDim(x + (dimension * width), y - cellPadding,
		borderPadding - cellPadding, (dimension*height) + cellPadding,
		Colors::White);
}

int Board::GetGridHeight() const
{
	return height;
}

int Board::GetGridWidth() const
{
	return width;
}

bool Board::IsInsideBoard(const Location& loc) const
{
	return loc.x >= 0 && loc.x < width
		&& loc.y >= 0 && loc.y < height;
}