#pragma once

#include <vector>
#include <string>
#include <assert.h>

class MineField
{
public:
	enum class Cell
	{
		EMPTY,
		M1,
		M2,
		M3,
		M4,
		M5,
		M6,
		M7,
		M8,
		M9,
		MINE,
		CLOSED,
	};

	unsigned int Width() const { return m_width; }
	unsigned int Height() const { return m_height; }
	unsigned int NumMines() const { return m_numMines; }

	MineField(): m_width(0), m_height(0), m_numMines(0) {}

	/// <summary>
	/// Initialise the mine field dimensions.
	/// </summary>
	/// <param name="a_width"></param>
	/// <param name="a_height"></param>
	void Init(unsigned int width, unsigned int height);

	/// <summary>
	/// Adds a mine at a random location
	/// </summary>
	/// <returns>true on success or false if mine field is full of mines</returns>
	bool AddRandomMine();

	/// <summary>
	/// Adds a mine at given location.
	/// </summary>
	/// <param name="x">x position</param>
	/// <param name="y">y position</param>
	/// <returns>true on success or false if mine already at location</returns>
	bool AddMine(unsigned int x, unsigned int y);

	/// <summary>
	/// Get cell at given location.
	/// </summary>
	/// <param name="x">x position</param>
	/// <param name="y">y position</param>
	/// <returns>Cell of given location</returns>
	Cell GetCellAt(unsigned int x, unsigned int y) const;

	/// <summary>
	/// 
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <returns></returns>
	Cell Open(unsigned int x, unsigned int y);

	/// <summary>
	/// Convert mine field to string for display.
	/// </summary>
	/// <returns>Mine field in string form</returns>
	std::string toString() const;

private:
	unsigned int m_width;
	unsigned int m_height;
	unsigned int m_numMines;

	std::vector<Cell> m_cells;

	/// <summary>
	/// Checks if position is valid
	/// </summary>
	/// <param name="x">x position</param>
	/// <param name="y">y position</param>
	/// <returns>true if position is valid else false</returns>
	bool InBounds(unsigned int x, unsigned int y) const;

	/// <summary>
	/// Helper to checks if position contains a mine
	/// </summary>
	/// <param name="x">x position</param>
	/// <param name="y">y position</param>
	/// <returns>true if position contains a mine else false</returns>
	bool IsMine(unsigned int x, unsigned int y) const;

	/// <summary>
	/// Set cell at given location
	/// </summary>
	/// <param name="x">x position</param>
	/// <param name="y">y position</param>
	/// <param name="value">Cell value</param>
	void SetCellAt(unsigned int x, unsigned int y, Cell value);

	/// <summary>
	/// Increment mine count for given cell
	/// </summary>
	/// <param name="x">x position</param>
	/// <param name="y">y position</param>
	/// <returns>true on success or false if cell is mine</returns>
	bool IncMineCount(unsigned int x, unsigned int y);
};


inline MineField::Cell MineField::GetCellAt(unsigned int x, unsigned int y) const
{ 
	assert(InBounds(x, y));
	return m_cells[y * m_width + x];
}

inline void MineField::SetCellAt(unsigned int x, unsigned int y, Cell value)
{
	assert(InBounds(x, y));
	m_cells[y * m_width + x] = value;
}

inline bool MineField::InBounds(unsigned int x, unsigned int y) const
{ 
	return x < m_width && y < m_height;
}

inline bool MineField::IsMine(unsigned int x, unsigned int y) const
{
	return GetCellAt(x, y) == Cell::MINE;
}

inline bool MineField::IncMineCount(unsigned int x, unsigned int y)
{
	if (IsMine(x, y))
		return false;

	Cell cell = GetCellAt(x, y);
	SetCellAt(x, y, Cell((int)cell + 1));
	return true;
}
