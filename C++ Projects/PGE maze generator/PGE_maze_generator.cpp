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

		// Maze width in maze cells
		int iMazeWidth;

		// Maze height in maze cells
		int iMazeHeight;

		// Path width in pixels
		int iPathWidth;

		// Vector of directions
		std::vector<Direction> vMaze;

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
			// Generate new maze upon pressing the ENTER key
			if (GetKey(olc::Key::ENTER).bPressed)
			{
				iVisitedCells = 1;

				// Clearing the stack of all elements
				while (!iStack.empty())
				{
					iStack.pop();
				}

				// Setting all maze cell's directions to NOT_SET
				for (int element = 0; element < vMaze.size(); element++)
				{
					vMaze[element] = NOT_SET;
				}

				// The top leftmost cell is going to be the starting point for the maze
				iStack.push(std::make_pair(0, 0));
			}

			// As long as there are unvisited cells
			if (iVisitedCells < iMazeWidth * iMazeHeight)
			{
				std::vector<int> vValidNeighbours;

				// Checks if neighbours exist and if they have been visited
				CheckForValidNeighbours(vValidNeighbours);

				// If there are unvisited neighbours
				if (!vValidNeighbours.empty())
				{
				  // Choosing a random neighbour from all valid neighbours
					int iNextCellDirection = vValidNeighbours[rand() % vValidNeighbours.size()];

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

			// TODO: create function DrawEmptyMaze to be reused elsewhere
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

		/**
		 * @brief Returns an array of valid directions to choose from.
		 * Accounts for the edge-case of maze cells on the literal edge of the maze
		 * returning directions that would lead outside the maze.
		 *
		 * @param neighbours Vector with all valid directions to choose from
		 */
		void CheckForValidNeighbours(std::vector<int>& neighbours)
		{
			// Check for valid northern neighbour
			if (iStack.top().second > 0)
			{
				CheckNeighbour(0, -1, NORTH, neighbours);
			}

			// Check for valid western neighbour
			if (iStack.top().first > 0)
			{
				CheckNeighbour(-1, 0, WEST, neighbours);
			}

			// Check for valid southern neighbour
			if (iStack.top().second < iMazeWidth - 1)
			{
				CheckNeighbour(0, 1, SOUTH, neighbours);
			}

			// Check for valid eastern neighbour
			if (iStack.top().first < iMazeHeight - 1)
			{
				CheckNeighbour(1, 0, EAST, neighbours);
			}
		}

		/**
		 * @brief If the neighbour's direction is not set, push the 'direction' parameter onto the 'neighbours' vector.
		 *
		 * @param x North/south neighbour indicator
		 * @param y East/west neighbour indicator
		 * @param direction The direction the neighbour is currently facing
		 * @param neighbours Vector with all valid directions to choose from
		 */
		void CheckNeighbour(int x, int y, int direction, std::vector<int>& neighbours)
		{
			if (vMaze[Position(x, y)] == NOT_SET)
			{
				neighbours.push_back(direction);
			}
		}

		// ... and pushes a new pair onto the stack
		// Only changes direction values if they were NOT_SET before
		void ChangeValueAndPush(int x, int y, Direction direction)
		{
			// ? why do we check for the top element?
			if(vMaze[Position(0, 0)] == NOT_SET)
			{
				vMaze[Position(0, 0)] = direction;
			}

			// * This can be refactored, we can write the direction directly into vMaze
			// * This has only visual consequences, the program logic won't be affected by this
			switch (direction)
			{
				case 1:
					if(vMaze[Position(x, y)] == NOT_SET)
					{
						vMaze[Position(x, y)] = SOUTH;
					}
				break;

				case 2:
					if (vMaze[Position(x, y)] == NOT_SET)
					{
						vMaze[Position(x, y)] = EAST;
					}
				break;

				case 3:
					if (vMaze[Position(x, y)] == NOT_SET)
					{
						vMaze[Position(x, y)] = NORTH;
					}
				break;

				case 4:
					if (vMaze[Position(x, y)] == NOT_SET)
					{
						vMaze[Position(x, y)] = WEST;
					}
				break;
			}

			// ? why do we only push the coordinates onto the stack?
			iStack.push(std::make_pair(iStack.top().first + x, iStack.top().second + y));
		}

		// Returns the position of a cell in vMaze (basically converts a pair to a single integer)
		int Position(int x, int y)
		{
			return (iStack.top().second + y) * iMazeWidth + (iStack.top().first + x);
		}
};

int main()
{
	MazeGenerator instance;
	if (instance.Construct(200, 200, 4, 4))
		instance.Start();
	return 0;
}
