#pragma once

#include <vector>
#include <bitset>
#include <string>
#include <assert.h>

class MineField
{
public:
	static const unsigned int MAX_SIZE = 25;

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

	MineField() : m_width(0), m_height(0), m_numMines(0), m_numClosed(0), m_failed(false) {}

	unsigned int Width() const { return m_width; }
	unsigned int Height() const { return m_height; }
	unsigned int NumMines() const { return m_numMines; }
	unsigned int NumClosed() const { return m_numClosed; }

	/// <summary>
	/// Test if mine field is solved (!failed and remaining closed positions == num mines)
	/// </summary>
	/// <returns>true if solved</returns>
	bool IsSolved() const { return !m_failed && NumClosed() == m_numMines; }

	/// <summary>
	/// Test if a mine was opened
	/// </summary>
	/// <returns>true if failed</returns>
	bool IsFailed() const { return m_failed; }

	/// <summary>
	/// Initialise the mine field dimensions and places mines.
	/// </summary>
	/// <param name="width">width of mine field</param>
	/// <param name="height">height of mine field</param>
	/// <param name="count">number of mines</param>
	/// <param name="seed">seed value for random number generator or 0 to use Time</param>
	void Init(unsigned int width, unsigned int height, unsigned int count, unsigned int seed = 0);

	/// <summary>
	/// Get cell at given location.
	/// </summary>
	/// <param name="x">x position</param>
	/// <param name="y">y position</param>
	/// <returns>Cell of given location</returns>
	Cell GetCellAt(unsigned int x, unsigned int y) const;

	/// <summary>
	/// Open cell at given location.
	/// </summary>
	/// <param name="x">x position</param>
	/// <param name="y">y position</param>
	/// <returns>Cell value of opened location</returns>
	Cell Open(unsigned int x, unsigned int y);

	/// <summary>
	/// Helper to check if position is open
	/// </summary>
	/// <param name="x">x position</param>
	/// <param name="y">y position</param>
	/// <returns>true if position is open</returns>
	bool IsOpen(unsigned int x, unsigned int y) const;

	/// <summary>
	/// Flag cell at given location as probably mined
	/// </summary>
	/// <param name="x">x position</param>
	/// <param name="y">y position</param>
	void Flag(unsigned int x, unsigned int y, bool flag = true);

	/// <summary>
	/// Check if cell at given location as flagged
	/// </summary>
	/// <param name="x">x position</param>
	/// <param name="y">y position</param>
	/// <returns>true if flagged</returns>
	bool IsFlagged(unsigned int x, unsigned int y) const;

	/// <summary>
	/// Convert mine field to string for display.
	/// </summary>
	/// <returns>Mine field in string form</returns>
	std::string toString(bool reveal = false) const;

	/// <summary>
	/// Checks if position is valid
	/// </summary>
	/// <param name="x">x position</param>
	/// <param name="y">y position</param>
	/// <returns>true if position is valid else false</returns>
	bool InBounds(unsigned int x, unsigned int y) const;

private:
	unsigned int m_width;
	unsigned int m_height;
	unsigned int m_numMines;
	unsigned int m_numClosed;
	bool m_failed;

	std::vector<Cell> m_cells;
	std::bitset<MAX_SIZE * MAX_SIZE> m_open;
	std::bitset<MAX_SIZE * MAX_SIZE> m_flags;

	/// <summary>
	/// Helper to get index of given position
	/// </summary>
	/// <param name="x">x position</param>
	/// <param name="y">y position</param>
	/// <returns>true if position is valid else false</returns>
	size_t GetPosIndex(unsigned int x, unsigned int y) const;

	/// <summary>
	/// Set cell open at given location
	/// </summary>
	/// <param name="x">x position</param>
	/// <param name="y">y position</param>
	void SetOpenAt(unsigned int x, unsigned int y);

	/// <summary>
	/// Helper to checks if position contains a mine
	/// </summary>
	/// <param name="x">x position</param>
	/// <param name="y">y position</param>
	/// <returns>true if position contains a mine else false</returns>
	bool IsMine(unsigned int x, unsigned int y) const;

	/// <summary>
	/// Adds a mine at given location.
	/// </summary>
	/// <param name="x">x position</param>
	/// <param name="y">y position</param>
	/// <returns>true on success or false if mine already at location</returns>
	bool AddMine(unsigned int x, unsigned int y);

	/// <summary>
	/// Increment mine count for given cell
	/// </summary>
	/// <param name="x">x position</param>
	/// <param name="y">y position</param>
	/// <returns>true on success or false if cell is mine</returns>
	bool IncMineCount(unsigned int x, unsigned int y);

	/// <summary>
	/// Get raw cell at given location. Ignores open state.
	/// </summary>
	/// <param name="x">x position</param>
	/// <param name="y">y position</param>
	/// <returns>Cell of given location</returns>
	Cell GetRawCellAt(unsigned int x, unsigned int y) const;

	/// <summary>
	/// Set cell at given location
	/// </summary>
	/// <param name="x">x position</param>
	/// <param name="y">y position</param>
	/// <param name="value">Cell value</param>
	void SetRawCellAt(unsigned int x, unsigned int y, Cell value);
};

inline size_t MineField::GetPosIndex(unsigned int x, unsigned int y) const
{
	assert(InBounds(x, y));
	return y * m_width + x;
}

inline MineField::Cell MineField::GetCellAt(unsigned int x, unsigned int y) const
{ 
	return IsOpen(x, y) ? GetRawCellAt(x, y) : Cell::CLOSED;
}

inline MineField::Cell MineField::GetRawCellAt(unsigned int x, unsigned int y) const
{
	return m_cells[GetPosIndex(x, y)];
}

inline void MineField::SetRawCellAt(unsigned int x, unsigned int y, Cell value)
{
	m_cells[GetPosIndex(x, y)] = value;
}

inline bool MineField::InBounds(unsigned int x, unsigned int y) const
{ 
	return x < m_width && y < m_height;
}

inline bool MineField::IsOpen(unsigned int x, unsigned int y) const
{
	return m_open.test(GetPosIndex(x, y));
}

inline void MineField::Flag(unsigned int x, unsigned int y, bool value)
{
	m_flags.set(GetPosIndex(x, y), value);
}

inline bool MineField::IsFlagged(unsigned int x, unsigned int y) const
{
	return m_flags.test(GetPosIndex(x, y));
}

inline void MineField::SetOpenAt(unsigned int x, unsigned int y)
{
	m_open.set(GetPosIndex(x, y));
}

inline bool MineField::IsMine(unsigned int x, unsigned int y) const
{
	return m_cells[GetPosIndex(x, y)] == Cell::MINE;
}
