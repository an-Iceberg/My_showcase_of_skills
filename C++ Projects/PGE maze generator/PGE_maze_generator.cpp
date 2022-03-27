#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <stack>

class MazeGenerator : public olc::PixelGameEngine
{
	public:
		MazeGenerator()
		{
			sAppName = "Maze generator";
		}

	private:
		enum Direction
		{
			NOT_SET,
			NORTH,
			WEST,
			SOUTH,
			EAST
		};

		// ? In pixels or in maze cells? (probably maze cells)
		int iMazeWidth;

		// ? In pixels or in maze cells? (probably maze cells)
		int iMazeHeight;

		int iPathWidth; // Path width in pixels
		std::vector<Direction> vMaze; // Vector of directions

		int iVisitedCells;

		std::stack<std::pair<int, int>> iStack;

	public:
		bool OnUserCreate() override
		{
			// Initializing the random number generator
			srand(time(nullptr));

			// TODO OPTIONAL: make maze width and height user adjustable
			iMazeHeight = 50;
			iMazeWidth = 50;

			for (int i = 0; i < iMazeWidth * iMazeHeight; i++)
			{
				vMaze.push_back(NOT_SET);
			}

			iStack.push(std::make_pair(0, 0));
			iVisitedCells = 1;

			iPathWidth = 3;

			return true;
		}

		// TODO: user adjustable maze creation speed delay
		// TODO: generate new maze when pressing enter key
		bool OnUserUpdate(float fElapsedTime) override
		{
			// Updating the maze
			if (iVisitedCells < iMazeWidth * iMazeHeight)
			{
				std::vector<int> iNeighbours;

				// checks if neighbours exist and if they have been visited
				CheckNeighbours(iNeighbours);

				// If there are unvisited neighbours
				if (!iNeighbours.empty())
				{
				  // Choose a random neighbour
					int iNextCellDirection = iNeighbours[rand() % iNeighbours.size()];

					switch (iNextCellDirection)
					{
						// North
						case 1:
							ChangeValueAndPush(0, -1, NORTH);
						break;

						// West
						case 2:
						  ChangeValueAndPush(-1, 0, WEST);
						break;

						// South
						case 3:
						  ChangeValueAndPush(0, 1, SOUTH);
						break;

						// East
						case 4:
						  ChangeValueAndPush(1, 0, EAST);
						break;
					}

					iVisitedCells++;
				}
				else
				{
					iStack.pop();
				}
			}

			// Drawing the maze
			Clear(olc::VERY_DARK_BLUE);

			// Draws cell positions
			for (int x = 0; x < iMazeWidth; x++)
			{
				for (int y = 0; y < iMazeHeight; y++)
				{

					// Fills in cell positions with cell interior and walls
					for (int py = 0; py < iPathWidth; py++)
					{
						for (int px = 0; px < iPathWidth; px++)
						{
							// Cell has been visited
							if (vMaze[y * iMazeWidth + x] != NOT_SET)
							{
								Draw(x * (iPathWidth + 1) + px, y * (iPathWidth + 1) + py);
							}
							// Cell has not yet been visited
							else
							{
								Draw(x * (iPathWidth + 1) + px, y * (iPathWidth + 1) + py, olc::BLUE);
							}
						}
					}

					// Connects cells by overriding the walls
					for (int p = 0; p < iPathWidth; p++)
					{
						if (vMaze[y * iMazeWidth + x] == SOUTH)
						{
							Draw(x * (iPathWidth + 1) + p, y * (iPathWidth + 1) + iPathWidth);
						}

						if (vMaze[y * iMazeWidth + x] == EAST)
						{
							Draw(x * (iPathWidth + 1) + iPathWidth, y * (iPathWidth + 1) + p);
						}

						if (vMaze[y * iMazeWidth + x] == WEST)
						{
							Draw(x * (iPathWidth + 1) - 1, y * (iPathWidth + 1) + p);
						}

						if (vMaze[y * iMazeWidth + x] == NORTH)
						{
							Draw(x * (iPathWidth + 1) + p, y * (iPathWidth + 1) - 1);
						}
					}
				}
			}

			// Drawing the top of the stack
			for (int py = 0; py < iPathWidth; py++)
			{
				for (int px = 0; px < iPathWidth; px++)
				{
					Draw(iStack.top().first * (iPathWidth + 1) + px, iStack.top().second * (iPathWidth + 1) + py, olc::GREEN);
				}
			}

			return true;
		}

		// Functions
		void CheckNeighbours(std::vector<int>& neighbour)
		{
			// North
			if (iStack.top().second > 0)
			{
				CheckNeighbour(0, -1, NORTH, neighbour);
			}

			// West
			if (iStack.top().first > 0)
			{
				CheckNeighbour(-1, 0, WEST, neighbour);
			}

			// South
			if (iStack.top().second < iMazeWidth - 1)
			{
				CheckNeighbour(0, 1, SOUTH, neighbour);
			}

			// East
			if (iStack.top().first < iMazeHeight - 1)
			{
				CheckNeighbour(1, 0, EAST, neighbour);
			}
		}

		void CheckNeighbour(int x, int y, int direction, std::vector<int>& vector)
		{
			if (vMaze[Position(x, y)] == NOT_SET)
			{
				vector.push_back(direction);
			}
		}

		void ChangeValueAndPush(int x, int y, Direction direction)
		{
			if(vMaze[Position(0, 0)] == 0)
			{
				vMaze[Position(0, 0)] = direction;
			}

			switch (direction)
			{
				case 1:
					if(vMaze[Position(x, y)] == 0)
					{
						vMaze[Position(x, y)] = SOUTH;
					}
				break;

				case 2:
					if (vMaze[Position(x, y)] == 0)
					{
						vMaze[Position(x, y)] = EAST;
					}
				break;

				case 3:
					if (vMaze[Position(x, y)] == 0)
					{
						vMaze[Position(x, y)] = NORTH;
					}
				break;

				case 4:
					if (vMaze[Position(x, y)] == 0)
					{
						vMaze[Position(x, y)] = WEST;
					}
				break;
			}

			iStack.push(std::make_pair(iStack.top().first + x, iStack.top().second + y));
		}

		int Position(int x, int y)
		{
			return (iStack.top().second + y) * iMazeWidth + (iStack.top().first + x);
		}
};

int main()
{
	MazeGenerator demo;
	if (demo.Construct(200, 200, 4, 4))
		demo.Start();
	return 0;
}
