#include "MineField.h"

#include <iostream>
#include <sstream>

/// <summary>
/// Convert Cell to character
/// </summary>
/// <param name="cell">cell value</param>
/// <returns>Character representing cell value</returns>
__forceinline char CellToChar(MineField::Cell cell)
{
	assert(cell >= MineField::Cell::EMPTY && cell <= MineField::Cell::CLOSED);

	static const char* s_cellToChar = ".123456789M?";
	return s_cellToChar[(int)cell];
}

void MineField::Init(unsigned int width, unsigned int height, unsigned int count, unsigned int seed)
{
	assert(width > 0 && width <= MAX_SIZE);
	assert(height > 0 && height <= MAX_SIZE);
	assert(count > 0 && count <= width * height);

	m_width = width;
	m_height = height;
	m_numMines = 0;
	m_numClosed = width * height;
	m_failed = false;

	m_cells.clear();
	m_cells.resize(width * height, Cell::EMPTY);

	m_open.reset();
	m_flags.reset();

	srand(seed != 0 ? seed : (int)time(NULL));

	while (m_numMines < count)
		AddMine(rand() % width, rand() % height);
}

bool MineField::AddMine(unsigned int x, unsigned int y)
{
	if (IsMine(x, y))
		return false;

	SetRawCellAt(x, y, Cell::MINE);
	m_numMines++;

	// increment mine count of surrounding cells
	for (int j = (int)y - 1; j <= (int)y + 1; ++j)
	{
		for (int i = (int)x - 1; i <= (int)x + 1; ++i)
		{
			if (InBounds(i, j) && (i != x || j != y))
				IncMineCount(i, j);
		}
	}

	return true;
}

bool MineField::IncMineCount(unsigned int x, unsigned int y)
{
	if (IsMine(x, y))
		return false;

	Cell cell = GetRawCellAt(x, y);
	SetRawCellAt(x, y, Cell((int)cell + 1));
	return true;
}

MineField::Cell MineField::Open(unsigned int x, unsigned int y)
{
	Cell cell = GetRawCellAt(x, y);
	if (IsOpen(x, y) || IsFlagged(x, y))
		return cell;

	SetOpenAt(x, y);
	m_numClosed--;

	switch (cell)
	{
		case Cell::EMPTY:
		{
			// reveal surrounding cells
			for (int j = (int)y - 1; j <= (int)y + 1; ++j)
				for (int i = (int)x - 1; i <= (int)x + 1; ++i)
					if (InBounds(i, j))
						Open(i, j);

			break;
		}
		case Cell::MINE:
		{
			// oh no!
			m_failed = true;
			break;
		}
	}

	return cell;
}

std::string MineField::toString(bool reveal) const
{
	std::stringstream ss;

	for (unsigned int y = 0; y < m_height; ++y)
	{
		for (unsigned int x = 0; x < m_width; ++x)
			ss << CellToChar(reveal ? GetRawCellAt(x, y) : GetCellAt(x, y)) << " ";

		ss << "\n";
	}

	if (!reveal)
		ss << "Mines: " << m_numMines << ", Closed: " << m_numClosed << ", Flagged: " << m_flags.count() << "\n";

	ss << "\n";

	return ss.str();
}
