#include <iostream>
#include <sstream>
#include <string>
#include <assert.h>
#include <stdexcept>
#include "MineField.h"

const int MAX_SIZE = 25;

template <class T>
std::string toString(const T& value)
{
    std::stringstream ss;
    ss << value;
    return ss.str();
}

/// <summary>
/// Shows program title
/// </summary>
void ShowTitle()
{
    std::cout << "MineSweeper Generator & Solver, Brendan Brewster\n\n";
}

/// <summary>
/// Shows program usage
/// </summary>
void ShowUsage()
{
    std::cout
        << "Usage:\n  MineSweeper <WIDTH> <HEIGHT> <COUNT>\n"
        << "\nWhere:\n"
        << "  WIDTH  - width of the mine field from 1 to " << MAX_SIZE << "\n"
        << "  HEIGHT - height of the mine field from 1 to " << MAX_SIZE << "\n"
        << "  COUNT  - number of mines on the field from 1 to number of cells\n";
}

/// <summary>
/// Parse program arguments
/// </summary>
/// <param name="width">OUT width of mine field</param>
/// <param name="height">OUT height of mine field</param>
/// <param name="count">OUT number of mines</param>
/// <returns>true if arguments parsed and valid else false</returns>
bool ParseArgs(unsigned int& width, unsigned int& height, unsigned int& count)
{
    if (__argc != 4)
    {
        ShowUsage();
        return false;
    }

    width = atoi(__argv[1]);
    height = atoi(__argv[2]);
    count = atoi(__argv[3]);

    if (!width || width > MAX_SIZE)
    {
        std::cout << "Error: invalid width, expected number from 1 to " << MAX_SIZE << "\n";
        return false;
    }

    if (!height || height > MAX_SIZE)
    {
        std::cout << "Error: invalid height, expected number from 1 to " << MAX_SIZE << "\n";
        return false;
    }

    unsigned int maxCount = width * height;
    if (!count || count > maxCount)
    {
        std::cout << "Error: invalid count, expected number from 1 to " << maxCount << "\n";
        return false;
    }

    return true;
}

void GenerateMineField(unsigned int width, unsigned int height, unsigned int count, MineField &field)
{
    assert(width > 0 && width <= MAX_SIZE);
    assert(height > 0 && height <= MAX_SIZE);
    assert(count > 0 && count <= width * height);

    std::cout << "Generating " << width << "x" << height << " mine field with " << count << " mines\n\n";

    field.Init(width, height);
    while (field.NumMines() < count)
        field.AddRandomMine();

    std::cout << field.toString();
}

void SolveMineField(MineField& field)
{
    // TODO
}

int main()
{
    srand((int)time(NULL));

    ShowTitle();

    unsigned int width = 0, height = 0, count = 0;
    if (!ParseArgs(width, height, count))
        return 1;

    MineField field;
    GenerateMineField(width, height, count, field);

    SolveMineField(field);

    return 0;
}
