#include <iostream>
#include <sstream>
#include <string>
#include <set>
#include <assert.h>
#include <stdexcept>
#include "MineField.h"

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
        << "Usage:\n  MineSweeper <WIDTH> <HEIGHT> <COUNT>\n\n"
        << "Where:\n"
        << "  WIDTH  - width of the mine field from 1 to " << MineField::MAX_SIZE << "\n"
        << "  HEIGHT - height of the mine field from 1 to " << MineField::MAX_SIZE << "\n"
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

    if (!width || width > MineField::MAX_SIZE)
    {
        std::cout << "Error: invalid width, expected number from 1 to " << MineField::MAX_SIZE << "\n";
        return false;
    }

    if (!height || height > MineField::MAX_SIZE)
    {
        std::cout << "Error: invalid height, expected number from 1 to " << MineField::MAX_SIZE << "\n";
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

/// <summary>
/// Generates a mine field. 
/// </summary>
/// <param name="width">width of mine field</param>
/// <param name="height">height of mine field</param>
/// <param name="count">number of mines</param>
/// <param name="field">IN mine field to update</param>
void GenerateMineField(unsigned int width, unsigned int height, unsigned int count, MineField &field)
{
    std::cout << "Generating " << width << "x" << height << " mine field with " << count << " mines\n\n";

    // I moved mine field generation to the MineField class as part of its initialisation.
    field.Init(width, height, count);

    std::cout << field.toString(true);
}

/// Get random success message
std::string GetSuccessMessage()
{
    switch (rand() % 5)
    {
    case 0: return "Success! Mine field solved!";
    case 1: return "Success! I can't believe it either!";
    case 2: return "Success! Would you like to play again?";
    case 3: return "Success! How did that hap... I mean I never doubted myself for a moment!";
    case 4: return "Success! 100% successful 14.38% of the time :D";
    }
    return "Success!";
}

/// Get random failed message
std::string GetFailedMessage()
{
    switch (rand() % 5)
    {
    case 0: return "Failed! Oh Noes!1!";
    case 1: return "Failed! We lost again?";
    case 2: return "Failed! This game is rigged, can we play TicTacToe instead?";
    case 3: return "Failed! Maybe the next one will be easier? :/";
    case 4: return "Failed! Oops! :P";
    }
    return "Failed!";
}

/// <summary>
/// Attempt to solve the given mine field.
/// </summary>
/// <param name="field">IN mine field to solve</param>
void SolveMineField(MineField& field)
{
    std::cout << "Solving " << field.Width() << "x" << field.Height() << " mine field\n\n";

    typedef std::pair<unsigned int, unsigned int> Pos;
    typedef std::vector<Pos> PosList;

    PosList closed, numbered, temp;
    closed.reserve(field.Width() * field.Height());
    numbered.reserve(field.Width() * field.Height());
    temp.reserve(8);

    while (!field.IsSolved() && !field.IsFailed())
    {
        std::cout << field.toString();

        closed.clear();
        numbered.clear();

        int prevNumClosed = field.NumClosed();

        // gather cells by type
        for (unsigned int y = 0; y < field.Height(); ++y)
            for (unsigned int x = 0; x < field.Width(); ++x)
            {
                auto cell = field.GetCellAt(x, y);
                if (cell >= MineField::Cell::M1 && cell <= MineField::Cell::M9)
                {
                    numbered.push_back(std::make_pair(x, y));
                }
                else if (cell == MineField::Cell::CLOSED)
                {
                    closed.push_back(std::make_pair(x, y));
                }
            }

        // for each numbered cell, 
        //   if mine hint count == number adjacent closed cells then mark adjacent closed cells as mines
        //   else if mine hint count == number adjacent flagged mines then open unflagged adjacent closed cells
        for (auto const& numberedPos : numbered)
        {
            unsigned int x = numberedPos.first, y = numberedPos.second;

            auto cell = field.GetCellAt(x, y);
            int hintCount = (int)cell - (int)MineField::Cell::EMPTY;
            int flagCount = 0;

            // get adjacent closed cells
            temp.clear();
            for (int j = (int)y - 1; j <= (int)y + 1; ++j)
                for (int i = (int)x - 1; i <= (int)x + 1; ++i)
                    if (field.InBounds(i, j) && field.GetCellAt(i, j) == MineField::Cell::CLOSED)
                    {
                        if (field.IsFlagged(i, j)) 
                            flagCount++;

                        temp.push_back(std::make_pair(i, j));
                    }
            int closedCount = temp.size();

            if (hintCount == closedCount)
            {
                // flag all adjacent closed cells as mines
                for (auto const& closedPos : temp)
                    field.Flag(closedPos.first, closedPos.second);
            }
            else if (hintCount == flagCount)
            {
                // open all unflagged adjacent closed cells
                for (auto const& closedPos : temp)
                    if (!field.IsFlagged(closedPos.first, closedPos.second))
                        field.Open(closedPos.first, closedPos.second);
            }
        }

        // choose random closed cell if nothing opened
        if (prevNumClosed == field.NumClosed())
        {
            std::cout << "Wish me luck! Choosing a random square...\n\n";

            // get random pos from closed cells
            const Pos& pos = closed[rand() % closed.size()];

            // open cell
            field.Open(pos.first, pos.second);
        }
    }

    std::cout << field.toString();

    std::cout << (field.IsSolved() ? GetSuccessMessage() : GetFailedMessage()) << "\n\n";
}

int main()
{
    ShowTitle();

    unsigned int width = 0, height = 0, count = 0;
    if (!ParseArgs(width, height, count))
        return 1;

    MineField field;
    GenerateMineField(width, height, count, field);
    SolveMineField(field);

    return 0;
}
