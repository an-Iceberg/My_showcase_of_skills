#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <set>
#include <stack>
#include <vector>
#include <limits>

enum Mode
{
	MOVE,
	VERTEX,
	EDGE,
	DIJKSTRA
};

struct sVertex
{
	float positionX;
	float positionY;
	int id;

	sVertex(float x, float y, int _id)
	{
		positionX = x;
		positionY = y;
		id = _id;
	}
};

struct sEdge
{
	int source; // Id of the source vertex
	int target; // Id of the target vertex
	int length;

	sEdge(int _source, int _target, int _length)
	{
		source = _source;
		target = _target;
		length = _length;
	}
};

// This might no longer be necessary
struct sPoint
{
	int id;
	int parent;
	int distance;
	bool visited;

	sPoint(int _id, int _parent, int _distance)
	{
		id = _id;
		parent = _parent;
		distance = _distance;
		visited = false;
	}
};

class GraphingTool : public olc::PixelGameEngine
{
public:
	GraphingTool()
	{
		sAppName = "Graphing Tool";
	}

private:
	// Contains the data for the vertices
	std::vector<sVertex> vVertices;

	// Contains the data for the edges
	std::vector<sEdge> vEdges;

	std::set<int> sIndices;

	// Contains the ids of the vertices that form the shortest path from iStart to iEnd
	std::vector<int> vPath;

	sVertex *pSelectedVertex = nullptr;

	int iRadius = 15;
	int iSelectedVertex = -1;
	int iEdgeLength = 1;
	Mode eMode = MOVE;

	// Id of the selected starting vertex
	int iStart = -1;

	// Id of the selected ending vertex
	int iEnd = -1;

	bool bChangeHasOccured = false;

public:
	bool OnUserCreate() override
	{
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		UserInput();
		Collision();
		DrawingRoutine();
		return true;
	}

	// Handles user input
	void UserInput()
	{
		// If any substantial changes have occured to the graph, the shortesst path may no longer be valid
		// This flag is for detecting such changes and removing the shortest path data in that case
		bChangeHasOccured = false;

		// Changing modes via arrow keys
		if (eMode == MOVE && GetKey(olc::RIGHT).bPressed)
		{
			eMode = VERTEX;
		}
		else if (eMode == VERTEX && GetKey(olc::RIGHT).bPressed)
		{
			eMode = EDGE;
		}
		else if (eMode == EDGE && GetKey(olc::RIGHT).bPressed)
		{
			eMode = DIJKSTRA;
		}
		else if (eMode == DIJKSTRA && GetKey(olc::LEFT).bPressed)
		{
			eMode = EDGE;
		}
		else if (eMode == EDGE && GetKey(olc::LEFT).bPressed)
		{
			eMode = VERTEX;
		}
		else if (eMode == VERTEX && GetKey(olc::LEFT).bPressed)
		{
			eMode = MOVE;
		}

		// Different modes handle user input differently
		switch (eMode)
		{
		// Move mode
		// Allows the user to move vertices around via mouse input
		case MOVE:
			if (GetMouse(0).bPressed)
			{
				SelectVertex();
			}

			if (GetMouse(0).bHeld)
			{
				MoveVertex();
			}

			if (GetMouse(0).bReleased)
			{
				pSelectedVertex = nullptr;
			}

			break;

		// Vertex mode
		// Allows user to create/delete vertices
		case VERTEX:
			// The maximum number of vertices we allow is 99
			if (vVertices.size() < 100 && GetMouse(0).bPressed)
			{
				CreateNewVertex();
			}

			if (GetMouse(1).bPressed)
			{
				DeleteVertex();
			}

			break;

		// Edge mode
		// Allows user to create/delete edges
		case EDGE:
			if (GetMouse(0).bPressed)
			{
				CreateNewEdge();
			}

			if (GetMouse(1).bPressed)
			{
				DeleteEdge();
			}

			break;

		// Dijkstra mode
		// Allows user to set a start and end point and let the program find the shortest path between the two
		case DIJKSTRA:
			if (GetMouse(0).bPressed)
			{
				SetStart();
			}

			if (GetMouse(1).bPressed)
			{
				SetEnd();
			}

			if (GetKey(olc::ENTER).bReleased)
			{
				Dijkstra();
			}

			break;
		}

		// User can change the lenth of an edge by pressing A or D
		if (iEdgeLength > 1 && GetKey(olc::A).bPressed)
		{
			iEdgeLength--;
		}

		if (iEdgeLength < 20 && GetKey(olc::D).bPressed)
		{
			iEdgeLength++;
		}

		// User can change the radius of the vertices displayed by pressign up or down
		if (iRadius < 20 && GetKey(olc::UP).bPressed)
		{
			iRadius++;
		}

		if (iRadius > 12 && GetKey(olc::DOWN).bPressed)
		{
			iRadius--;
		}

		// User can clear all graph data by pressing backspace
		if (!vVertices.empty() && GetKey(olc::BACK).bPressed)
		{
			vVertices.clear();
			vEdges.clear();
			vPath.clear();
			iStart = -1;
			iEnd = -1;
			return;
		}

		// If any substantial change has occured to the graph, the shortest path data is cleared
		if (!vPath.empty() && bChangeHasOccured)
		{
			vPath.clear();
		}
	}

	// TODO: Better comments for this function
	// Detects ovelap between two vertices and applies froce pushing them away from each other
	void Collision()
	{
		for (auto &vertex : vVertices)
		{
			for (auto &target : vVertices)
			{
				if (vertex.id != target.id)
				{
					if (DoCirclesOverlap(vertex.positionX, vertex.positionY, iRadius, target.positionX, target.positionY, iRadius))
					{
						float fDistance = sqrtf((vertex.positionX - target.positionX) * (vertex.positionX - target.positionX) + (vertex.positionY - target.positionY) * (vertex.positionY - target.positionY));
						float fOverlap = 0.5f * (fDistance - (iRadius * 2));

						vertex.positionX -= fOverlap * (vertex.positionX - target.positionX) / fDistance;
						vertex.positionY -= fOverlap * (vertex.positionY - target.positionY) / fDistance;

						target.positionX += fOverlap * (vertex.positionX - target.positionX) / fDistance;
						target.positionY += fOverlap * (vertex.positionY - target.positionY) / fDistance;
					}
				}
			}
		}
	}

	// Draws the graph to the screen
	void DrawingRoutine()
	{
		// Clears the screen
		Clear(olc::DARK_BLUE);

		std::string mode;

		// Selects the text to be drawn based upon which mode is active
		switch (eMode)
		{
		case MOVE: mode = "  move >"; break;
		case VERTEX: mode = "< vertex >"; break;
		case EDGE: mode = "< edge >"; break;
		case DIJKSTRA: mode = "< Dijkstra's shortest path"; break;
		}

		// Drawing the edges with length and direction
		for (auto const &edge : vEdges)
		{
			// The coorditates of the source edge
			float sourceX = GetX(edge.source);
			float sourceY = GetY(edge.source);

			// The coordinates of the target edge
			float targetX = GetX(edge.target);
			float targetY = GetY(edge.target);

			// Distance between source and targen in x direction (also indicates direction by the sign (+/-))
			float directionX = sourceX - targetX;

			// Distance between source and targen in y direction (also indicates direction by the sign (+/-))
			float directionY = sourceY - targetY;

			// Distance between source and target (by using pythagoras' theorem)
			float length = sqrtf((directionX) * (directionX) + (directionY) * (directionY));

			// Calculating the positions of the triangle corners
			// positionTriangleCorner = position + (direction * (radius / length))
			// x1/y1 are touching the circle
			float x1 = targetX + (directionX * (iRadius / length));
			float y1 = targetY + (directionY * (iRadius / length));

			// x2/y2 are on the edge
			float x2 = targetX + (directionX * ((iRadius + 15.0f) / length));
			float y2 = targetY + (directionY * ((iRadius + 15.0f) / length));

			// Helper positions to calculate position of floading triangel corners
			float xh = targetX + (directionX * ((iRadius + 20.0f) / length));
			float yh = targetY + (directionY * ((iRadius + 20.0f) / length));

			float directionHX = xh - sourceX;
			float directionHY = yh - sourceY;

			// positionTriangleCornerFloating = phelper position +- (direction / length), aka tangent
			// These positions are floating to the left/right of the edge
			float x3 = xh - (directionHY / length) * 10.0f;
			float y3 = yh + (directionHX / length) * 10.0f;
			float x4 = xh + (directionHY / length) * 10.0f;
			float y4 = yh - (directionHX / length) * 10.0f;

			// TODO: refactor these drawing routines to be more concise and efficient; we are using too many if statements
			// Drawing the edges (color depends on mode and whether it's in the path)
			if (eMode == DIJKSTRA && EdgeIsInPath(edge.source, edge.target))
			{
				DrawLine(sourceX, sourceY, targetX, targetY, olc::GREEN);
			}
			else
			{
				DrawLine(sourceX, sourceY, targetX, targetY, olc::MAGENTA);
			}

			FillTriangle(x1, y1, x2, y2, x3, y3, olc::MAGENTA);
			FillTriangle(x1, y1, x2, y2, x4, y4, olc::MAGENTA);

			// Draws the length of the edge in the centre of the edge
			DrawString((sourceX + targetX) / 2.0f - 8.0f, (sourceY + targetY) / 2.0f - 8.0f, std::to_string(edge.length), olc::CYAN, 2);
		}

		// Drawing the vertices
		for (auto const &vertex : vVertices)
		{
			// Drawing the vertices (color depends on mode and whether it's in the path)
			if (eMode == DIJKSTRA && VertexIsInPath(vertex.id))
			{
				FillCircle(vertex.positionX, vertex.positionY, iRadius, olc::GREEN);
			}
			else
			{
				FillCircle(vertex.positionX, vertex.positionY, iRadius, olc::Pixel(255, 128, 0));
			}

			// The selected circle is highlighted magenta
			if (vertex.id == iSelectedVertex)
			{
				FillCircle(vertex.positionX, vertex.positionY, iRadius, olc::MAGENTA);
			}

			// Adjusting text position so it always looks centerd in the vertex
			if (vertex.id > 9)
			{
				DrawString(vertex.positionX - 15.0f, vertex.positionY - 7.0f, std::to_string(vertex.id), olc::BLACK, 2);
			}
			else
			{
				DrawString(vertex.positionX - 7.0f, vertex.positionY - 7.0f, std::to_string(vertex.id), olc::BLACK, 2);
			}
		}

		// Drawing the 'Start' and 'End' labels if mode is dijkstra
		if (eMode == DIJKSTRA)
		{
			for (auto const &vertex : vVertices)
			{
				if (vertex.id == iStart)
				{
					DrawString(vertex.positionX - 40, vertex.positionY - 32.0f, "Start", olc::CYAN, 2);
				}

				if (vertex.id == iEnd)
				{
					DrawString(vertex.positionX - 24, vertex.positionY - 32.0f, "End", olc::CYAN, 2);
				}
			}
		}

		// TODO: Draw radius size
		// Drawing mode and edge length information in the top left corner
		DrawString(5.0f, 5.0f,  "Edge length: "+std::to_string(iEdgeLength), olc::MAGENTA, 2);
		DrawString(5.0f, 25.0f, "Mode: "+mode, olc::MAGENTA, 2);
	}

	// Returns true, if the edge is part of the shortest path
	bool EdgeIsInPath(int const &sourceId, int const &targetId)
	{
		if (vPath.empty())
		{
			return false;
		}

		for (int i = 0; i < vPath.size() - 1; i++)
		{
			// If the current path element is the source and the next the target, return true
			if (sourceId == vPath[i] && targetId == vPath[i + 1])
			{
				return true;
			}
		}

		return false;
	}

	// Returns true, if a vertex is part of the shortest path
	bool VertexIsInPath(int const &id)
	{
		for (auto const &point : vPath)
		{
			if (point == id)
			{
				return true;
			}
		}

		return false;
	}

	// The user left-clicks on a vertex, setting it as the starting point of dijkstra's shortest path
	void SetStart()
	{
		float mouseX = GetMouseX();
		float mouseY = GetMouseY();

		for (auto const &vertex : vVertices)
		{
			if (IsPointInCircle(vertex.positionX, vertex.positionY, iRadius, mouseX, mouseY))
			{
				iStart = vertex.id;
			}
		}

		bChangeHasOccured = true;
	}

	// The user right-clicks on a vertex, setting it as the ending point of dijkstra's shortest path
	void SetEnd()
	{
		float mouseX = GetMouseX();
		float mouseY = GetMouseY();

		for (auto const &vertex : vVertices)
		{
			if (IsPointInCircle(vertex.positionX, vertex.positionY, iRadius, mouseX, mouseY))
			{
				iEnd = vertex.id;
			}
		}

		bChangeHasOccured = true;
	}

	// Returns true, if the list contains the point
	bool containsPoint(int const &_node, std::list<sPoint> const &nodes)
	{
		for (auto const &node : nodes)
		{
			if (node.id == _node)
			{
				return true;
			}
		}

		return false;
	}

	// Returns true, if the vector contains the node
	bool containsNode(int const &_node, std::vector<int> const &nodes)
	{
		for (auto const &node : nodes)
		{
			if (_node == node)
			{
				return true;
			}
		}

		return false;
	}

	// Puts the shortest path between the selected start and end point into vPath
	void Dijkstra()
	{
		if (iStart == -1 || iEnd == -1)
		{
			return;
		}

		// 	TODO: implement dijkstra's shortest path

		bChangeHasOccured = false;
	}

	// TODO: account for void return value
	// Returns the position of a vertex in the x axis
	float GetX(int const &id)
	{
		for (auto const &vertex : vVertices)
		{
			if (vertex.id == id)
			{
				return vertex.positionX;
			}
		}

		// ! This is not a good solution
		return 0.0f;
	}

	// TODO: account for void return value
	// Returns the position of a vertex in the y axis
	float GetY(int const &id)
	{
		for (auto const &vertex : vVertices)
		{
			if (vertex.id == id)
			{
				return vertex.positionY;
			}
		}

		// ! This is not a good solution
		return 0.0f;
	}

	// The vertex clicked on by the user is marked as selected
	void SelectVertex()
	{
		pSelectedVertex = nullptr;

		float mouseX = GetMouseX();
		float mouseY = GetMouseY();

		for (auto &vertex : vVertices)
		{
			if (IsPointInCircle(vertex.positionX, vertex.positionY, iRadius, mouseX, mouseY))
			{
				pSelectedVertex = &vertex;
				break;
			}
		}
	}

	// The selected vertex follows the position of the mouse cursor
	void MoveVertex()
	{
		if (pSelectedVertex != nullptr)
		{
			pSelectedVertex->positionX = GetMouseX();
			pSelectedVertex->positionY = GetMouseY();
		}
	}

	// TODO: refactor this mess
	// Creates a new vertex
	void CreateNewVertex()
	{
		pSelectedVertex = nullptr;
		bool bGoodMousePosition = false;

		float mouseX = GetMouseX();
		float mouseY = GetMouseY();

		if (vVertices.size() == 0)
		{
			vVertices.push_back(sVertex(mouseX, mouseY, 0));
			sIndices.insert(0);
			return;
		}

		// Checks if the mouse is inside a vertex
		for (auto &vertex : vVertices)
		{
			if (!IsPointInCircle(vertex.positionX, vertex.positionY, iRadius, mouseX, mouseY))
			{
				bGoodMousePosition = true;
			}

			else
			{
				bGoodMousePosition = false;
				break;
			}
		}

		int id = 0;

		// Creates appropriate id
		while (sIndices.count(id) > 0)
		{
			id++;
		}

		if (bGoodMousePosition)
		{
			vVertices.push_back(sVertex(mouseX, mouseY, id));
			sIndices.insert(id);
		}

		bGoodMousePosition = false;
		bChangeHasOccured = true;
	}

	// Deletes a vertex
	void DeleteVertex()
	{
		pSelectedVertex = nullptr;
		int _id = -1;

		float mouseX = GetMouseX();
		float mouseY = GetMouseY();

		for (int i = 0; i < vVertices.size(); i++)
		{
			if (IsPointInCircle(vVertices[i].positionX, vVertices[i].positionY, iRadius, mouseX, mouseY))
			{
				_id = vVertices[i].id;
				vVertices.erase(vVertices.begin() + i);
				sIndices.erase(_id);
			}
		}

		for (int i = 0; i < vEdges.size(); i++)
		{
			if (vEdges[i].source == _id || vEdges[i].target == _id)
			{
				vEdges.erase(vEdges.begin() + i);
				i--;
			}
		}

		bChangeHasOccured = true;
	}

	// TODO: refactor this mess
	// Creates a new edge
	void CreateNewEdge()
	{
		float mouseX = GetMouseX();
		float mouseY = GetMouseY();

		// If no vertex has been selected yet
		if (iSelectedVertex == -1)
		{

			for (auto &vertex : vVertices)
			{
				if (IsPointInCircle(vertex.positionX, vertex.positionY, iRadius, mouseX, mouseY))
				{
					iSelectedVertex = vertex.id;
				}
			}
		}
		else
		{
			for (auto &vertex : vVertices)
			{
				// Don't create an edge from the vertex to itself
				if (vertex.id != iSelectedVertex)
					if (IsPointInCircle(vertex.positionX, vertex.positionY, iRadius, mouseX, mouseY))
					{
						for (auto const &edge : vEdges)
						{
							// Don't create a duplicate edge
							if (edge.source == iSelectedVertex && edge.target == vertex.id)
							{
								iSelectedVertex = -1;
								return;
							}
							// Don't create a bidirectional edge
							if (edge.target == iSelectedVertex && edge.source == vertex.id)
							{
								iSelectedVertex = -1;
								return;
							}
						}

						vEdges.push_back(sEdge(iSelectedVertex, vertex.id, iEdgeLength));
					}
			}

			iSelectedVertex = -1;
		}

		bChangeHasOccured = true;
	}

	// Deletes an edge
	void DeleteEdge()
	{
		float mouseX = GetMouseX();
		float mouseY = GetMouseY();

		for (auto const &vertex : vVertices)
		{
			if (IsPointInCircle(vertex.positionX, vertex.positionY, iRadius, mouseX, mouseY))
			{
				for (int i = 0; i < vEdges.size(); i++)
				{
					if (vEdges[i].source == iSelectedVertex && vEdges[i].target == vertex.id)
					{
						vEdges.erase(vEdges.begin() + i);
					}
				}
			}
		}

		iSelectedVertex = -1;
		bChangeHasOccured = true;
	}

	// Returns whether two given circles overlap
	bool DoCirclesOverlap(float const &x1, float const &y1, float const &r1, float const &x2, float const &y2, float const &r2)
	{
		return fabs((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)) <= (r1 + r2) * (r1 + r2);
	}

	// Returns whether a given point is within a circle
	bool IsPointInCircle(float const &circleX, float const &circleY, float const &radius, float const &pointX, float const &pointY)
	{
		return fabs((circleX - pointX) * (circleX - pointX) + (circleY - pointY) * (circleY - pointY)) < (radius * radius);
	}
};

int main()
{
	GraphingTool demo;

	if (demo.Construct(1280, 720, 1, 1))
	{
		demo.Start();
	}

	return 0;
}
