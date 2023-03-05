#include "MineField.h"

#include <sstream>

/// <summary>
/// Convert Cell to character
/// </summary>
/// <param name="cell">cell value</param>
/// <returns>Character representing cell value</returns>
__forceinline char CellToChar(MineField::Cell cell)
{
	assert(cell >= MineField::Cell::EMPTY && cell <= MineField::Cell::MINE);

	static const char* s_cellToChar = ".123456789M";
	return s_cellToChar[(int)cell];
}

void MineField::Init(unsigned int a_width, unsigned int a_height)
{
	m_width = a_width;
	m_height = a_height;
	m_numMines = 0;

	m_cells.clear();
	m_cells.resize(a_width * a_height, Cell::EMPTY);
}

bool MineField::AddRandomMine()
{
	if (m_numMines >= m_width * m_height)
		return false;

	while (!AddMine(rand() % m_width, rand() % m_height));

	return true;
}

bool MineField::AddMine(unsigned int x, unsigned int y)
{
	if (GetCellAt(x, y) == Cell::MINE)
		return false;

	SetCellAt(x, y, Cell::MINE);

	// get bounds of surrounding cells
	int mini = std::max<int>(x - 1, 0);
	int minj = std::max<int>(y - 1, 0);
	int maxi = std::min<int>(x + 1, m_width - 1);
	int maxj = std::min<int>(y + 1, m_height - 1);

	// increment mine count of surrounding cells
	for (int j = minj; j <= maxj; ++j)
		for (int i = mini; i <= maxi; ++i)
			IncMineCount(i, j);

	m_numMines++;
	return true;
}

std::string MineField::toString() const
{
	std::stringstream ss;

	for (unsigned int y = 0; y < m_height; ++y)
	{
		for (unsigned int x = 0; x < m_width; ++x)
			ss << CellToChar(GetCellAt(x, y));

		ss << "\n";
	}

	return ss.str();
}
