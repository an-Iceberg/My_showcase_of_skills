#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <stack>

class Example : public olc::PixelGameEngine
{
public:
	Example()
	{
		sAppName = "Maze generator";
	}

private:
	int iMazeWidth;
	int iMazeHeight;
	int iPathWidth;
	std::vector<int> iMaze;

	int iVisitedCells;

	std::stack<std::pair<int, int>> iStack;

public:
	bool OnUserCreate() override
	{
		srand(time(nullptr));

		iMazeHeight = 50;
		iMazeWidth = 50;

		for (int i = 0; i < iMazeWidth * iMazeHeight; i++) {
			iMaze.push_back(0);
		}

		iStack.push(std::make_pair(0, 0));
		iVisitedCells = 1;

		iPathWidth = 3;

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// === Updating the maze ===
		if (iVisitedCells < iMazeWidth * iMazeHeight) {
			std::vector<int> iNeighbours;

			CheckNeighbours(iNeighbours); // checks if neighbour exists and if it has been visited

			if (!iNeighbours.empty()) { // If there are unvisited neighbours
				int iNextCellDirection = iNeighbours[rand() % iNeighbours.size()]; // Choosing a random neighbour

				switch (iNextCellDirection) {
				case 1: ChangeValueAndPush(0, -1, 1); break; // North
				case 2: ChangeValueAndPush(-1, 0, 2); break; // West
				case 3: ChangeValueAndPush(0, 1, 3); break; // South
				case 4: ChangeValueAndPush(1, 0, 4); break; // East
				}

				iVisitedCells++;
			}
			else {
				iStack.pop();
			}
		}

		// === Drawing the maze ===
		Clear(olc::VERY_DARK_BLUE);

		for (int x = 0; x < iMazeWidth; x++) { // Draws cell positions
			for (int y = 0; y < iMazeHeight; y++) {

				for(int py = 0; py < iPathWidth; py++) // Fills in cell positions with cell interior and walls
					for (int px = 0; px < iPathWidth; px++){

						if (iMaze[y * iMazeWidth + x] != 0) { // Cell has been visited
							Draw(x * (iPathWidth + 1) + px, y * (iPathWidth + 1) + py);
						}
						else { // Cell has not yet been visited
							Draw(x * (iPathWidth + 1) + px, y * (iPathWidth + 1) + py, olc::BLUE);
						}
					}

				for (int p = 0; p < iPathWidth; p++) { // Connects cells by overriding the walls
					if (iMaze[y * iMazeWidth + x] == 3)
						Draw(x * (iPathWidth + 1) + p, y * (iPathWidth + 1) + iPathWidth);

					if (iMaze[y * iMazeWidth + x] == 4)
						Draw(x * (iPathWidth + 1) + iPathWidth, y * (iPathWidth + 1) + p);

					if (iMaze[y * iMazeWidth + x] == 2)
						Draw(x * (iPathWidth + 1) - 1, y * (iPathWidth + 1) + p);

					if (iMaze[y * iMazeWidth + x] == 1)
						Draw(x * (iPathWidth + 1) + p, y * (iPathWidth + 1) - 1);
				}
			}
		}

		// === Drawing the top of the stack ===
		for (int py = 0; py < iPathWidth; py++)
			for (int px = 0; px < iPathWidth; px++)
				Draw(iStack.top().first * (iPathWidth + 1) + px, iStack.top().second * (iPathWidth + 1) + py, olc::GREEN);

		return true;
	}

	// === Functions ===
	void CheckNeighbours(std::vector<int>& neighbour) {
		if (iStack.top().second > 0) // North
			CheckNeighbour(0, -1, 1, neighbour);

		if (iStack.top().first > 0) // West
			CheckNeighbour(-1, 0, 2, neighbour);

		if (iStack.top().second < iMazeWidth - 1) // South
			CheckNeighbour(0, 1, 3, neighbour);

		if (iStack.top().first < iMazeHeight - 1) // East
			CheckNeighbour(1, 0, 4, neighbour);
	}

	void CheckNeighbour(int x, int y, int direction, std::vector<int>& vector) {
		if (iMaze[Position(x, y)] == 0)
				vector.push_back(direction);
	}

	void ChangeValueAndPush(int x, int y, int direction) {
		if(iMaze[Position(0, 0)] == 0)
			iMaze[Position(0, 0)] = direction;

		switch (direction) {
		case 1:
			if(iMaze[Position(x, y)] == 0)
				iMaze[Position(x, y)] = 3;
			break;
		case 2:
			if (iMaze[Position(x, y)] == 0)
				iMaze[Position(x, y)] = 4;
			break;
		case 3:
			if (iMaze[Position(x, y)] == 0)
				iMaze[Position(x, y)] = 1;
			break;
		case 4:
			if (iMaze[Position(x, y)] == 0)
				iMaze[Position(x, y)] = 2;
			break;
		}
		iStack.push(std::make_pair(iStack.top().first + x, iStack.top().second + y));
	}

	int Position(int x, int y) {
		return (iStack.top().second + y) * iMazeWidth + (iStack.top().first + x);
	}
};

int main()
{
	Example demo;
	if (demo.Construct(200, 200, 4, 4))
		demo.Start();
	return 0;
}
