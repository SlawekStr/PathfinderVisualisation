#include "RecursiveDivisionGenerator.h"
#include <iostream>

////////////////////////////////////////////////////////////
RecursiveDivisionGenerator::RecursiveDivisionGenerator(Mesh* mesh) : MazeGenerator(mesh), m_generator(std::random_device()())
{}

////////////////////////////////////////////////////////////
void RecursiveDivisionGenerator::updateGenerator()
{
    if (!m_isFinished)
    {
        sf::Vector2i mazeSize = m_mesh->getMeshSize();
        divideMaze(0, 0, mazeSize.x, mazeSize.y, getOrientation(mazeSize.x, mazeSize.y));
    }
}

////////////////////////////////////////////////////////////
bool RecursiveDivisionGenerator::isFinished()
{
	return m_isFinished;
}

////////////////////////////////////////////////////////////
void RecursiveDivisionGenerator::divideMaze(int x, int y, int sectionWidth, int sectionHeight,int orientation)
{
    std::vector<Cell>& cellVec = m_mesh->getSquareVec();
    sf::Vector2i mazeSize = m_mesh->getMeshSize();

    if (sectionWidth < 5 || sectionHeight < 5)
        return;

    bool horizontal = orientation == 0;

    int wallX = x + (horizontal ? 0 : getRandomInt(2, sectionWidth - 3) / 2 * 2);
    int wallY = y + (horizontal ? getRandomInt(2, sectionHeight - 3) / 2 * 2 : 0);

    int openingX = wallX + (horizontal ? getRandomInt(1, sectionWidth - 2) / 2 * 2 + 1 : 0);
    int openingY = wallY + (horizontal ? 0 : getRandomInt(1, sectionHeight - 2) / 2 * 2 + 1);

    int directionX = horizontal ? 1 : 0;
    int directionY = horizontal ? 0 : 1;

    int wallLength = horizontal ? sectionWidth : sectionHeight;

    int cellX = wallX;
    int cellY = wallY;

    bool putWall{ false };
    for (int i = 0; i < wallLength; ++i)
    {
        if (cellX != openingX || cellY != openingY)
        {
            cellVec[cellY * mazeSize.x + cellX].cellType = SquareType::FULL;
            m_mesh->setCellColor(SquareType::FULL, cellY * mazeSize.x + cellX);
        }
        cellX += directionX;
        cellY += directionY;
    }

    int subWidth = horizontal ? sectionWidth : wallX - x;
    int subHeight = horizontal ? wallY - y + 1 : sectionHeight;

    divideMaze(x, y, subWidth, subHeight, getOrientation(subWidth, subHeight));
    
    int subX = horizontal ? x : wallX;
    int subY = horizontal ? wallY : y;

    subWidth = horizontal ? sectionWidth : x + sectionWidth - wallX;
    subHeight = horizontal ? y + sectionHeight - wallY : sectionHeight;

    divideMaze(subX, subY, subWidth, subHeight, getOrientation(subWidth, subHeight));
    
    m_isFinished = true;
}

////////////////////////////////////////////////////////////
int RecursiveDivisionGenerator::getOrientation(int width, int height)
{
    if (width < height)
    {
        return 0;
    }
    else if (width > height)
    {
        return 1;
    }
    else
    {
        return getRandomInt(0, 1);
    }
}

////////////////////////////////////////////////////////////
int RecursiveDivisionGenerator::getRandomInt(int min, int max)
{
    return std::uniform_int_distribution<int>(min, max)(m_generator);
}
