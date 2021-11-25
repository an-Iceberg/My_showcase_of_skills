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
	float px; // Position on screen
	float py; // Position on screen
	int id;

	sVertex(float x, float y, int _id)
	{
		px = x;
		py = y;
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
	std::vector<sVertex> vVertices; // This contains the data for the vertices
	std::vector<sEdge> vEdges; // This contains the data for the edges
	std::set<int> sIndices;
	std::vector<int> vPath; // This contains the ids of the vertices that forms dijkstra's shortest path

	sVertex *pSelectedVertex = nullptr;

	int iRadius = 15;
	int iSelectedVertex = -1;
	int iEdgeLength = 1;
	Mode eMode = MOVE;
	int iStart = -1;
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
		bChangeHasOccured = false;

		// Changing modes via arrow keys
		if (eMode == MOVE && GetKey(olc::RIGHT).bPressed)
			eMode = VERTEX;
		else if (eMode == VERTEX && GetKey(olc::RIGHT).bPressed)
			eMode = EDGE;
		else if (eMode == EDGE && GetKey(olc::RIGHT).bPressed)
			eMode = DIJKSTRA;
		else if (eMode == DIJKSTRA && GetKey(olc::LEFT).bPressed)
			eMode = EDGE;
		else if (eMode == EDGE && GetKey(olc::LEFT).bPressed)
			eMode = VERTEX;
		else if (eMode == VERTEX && GetKey(olc::LEFT).bPressed)
			eMode = MOVE;

		switch (eMode)
		{
		case MOVE: // Move vertices
			if (GetMouse(0).bPressed)
				SelectVertex();

			if (GetMouse(0).bHeld)
				MoveVertex();

			if (GetMouse(0).bReleased)
				pSelectedVertex = nullptr;

			break;

		case VERTEX:											// Vertex creation/deletion
			if (vVertices.size() < 100 && GetMouse(0).bPressed) // Max number of vertices: 99
				CreateNewVertex();

			if (GetMouse(1).bPressed)
				DeleteVertex();

			break;

		case EDGE: // Edge creation/deletion
			if (GetMouse(0).bPressed)
				CreateNewEdge();

			if (GetMouse(1).bPressed)
				DeleteEdge();

			break;

		case DIJKSTRA: // Dijkstra's shortest path
			if (GetMouse(0).bPressed)
				SetStart();

			if (GetMouse(1).bPressed)
				SetEnd();

			if (GetKey(olc::ENTER).bReleased)
				Dijkstra();

			break;
		}

		// User definable edge length
		if (iEdgeLength > 1 && GetKey(olc::A).bPressed)
			iEdgeLength--;

		if (iEdgeLength < 20 && GetKey(olc::D).bPressed)
			iEdgeLength++;

		// Changeable radius
		if (iRadius < 20 && GetKey(olc::UP).bPressed)
			iRadius++;

		if (iRadius > 12 && GetKey(olc::DOWN).bPressed)
			iRadius--;

		// Clear graph
		if (!vVertices.empty() && GetKey(olc::BACK).bPressed)
		{
			vVertices.clear();
			vEdges.clear();
		}

		if (!vPath.empty() && bChangeHasOccured)
			vPath.clear();
	}

	// Detects ovelap between two vertices and applies froce pushing them away from each other
	void Collision()
	{
		for (auto &vertex : vVertices)
		{
			for (auto &target : vVertices)
			{
				if (vertex.id != target.id)
				{
					if (DoCirclesOverlap(vertex.px, vertex.py, iRadius, target.px, target.py, iRadius))
					{
						float fDistance = sqrtf((vertex.px - target.px) * (vertex.px - target.px) + (vertex.py - target.py) * (vertex.py - target.py));
						float fOverlap = 0.5f * (fDistance - (iRadius * 2));

						vertex.px -= fOverlap * (vertex.px - target.px) / fDistance;
						vertex.py -= fOverlap * (vertex.py - target.py) / fDistance;

						target.px += fOverlap * (vertex.px - target.px) / fDistance;
						target.py += fOverlap * (vertex.py - target.py) / fDistance;
					}
				}
			}
		}
	}

	// Draws the graph to the screen
	void DrawingRoutine()
	{
		Clear(olc::DARK_BLUE);

		std::string mode;

		switch (eMode)
		{
		case MOVE:
			mode = "Mode: move ->";
			break;
		case VERTEX:
			mode = "Mode: <- vertex ->";
			break;
		case EDGE:
			mode = "Mode: <- edge ->";
			break;
		case DIJKSTRA:
			mode = "Mode: <- Dijkstra's shortest path";
			break;
		}

		std::string length = "Edge length: ";

		// Drawing the edges with length and direction
		for (auto const &edge : vEdges)
		{
			float sx = GetX(edge.source);
			float sy = GetY(edge.source);
			float tx = GetX(edge.target);
			float ty = GetY(edge.target);

			float length = sqrtf((sx - tx) * (sx - tx) + (sy - ty) * (sy - ty));
			float directionX = sx - tx;
			float directionY = sy - ty;

			// position + (direction * (radius / length))
			float x1 = tx + (directionX * (iRadius / length));
			float y1 = ty + (directionY * (iRadius / length));
			float x2 = tx + (directionX * ((iRadius + 15.0f) / length));
			float y2 = ty + (directionY * ((iRadius + 15.0f) / length));
			float xh = tx + (directionX * ((iRadius + 20.0f) / length));
			float yh = ty + (directionY * ((iRadius + 20.0f) / length));

			float directionHX = xh - sx;
			float directionHY = yh - sy;

			// helper position +- (direction / length), aka tangent
			float x3 = xh - (directionHY / length) * 10.0f;
			float y3 = yh + (directionHX / length) * 10.0f;
			float x4 = xh + (directionHY / length) * 10.0f;
			float y4 = yh - (directionHX / length) * 10.0f;

			if (eMode == DIJKSTRA)
			{
				if (EdgeIsInPath(edge.source, edge.target))
					DrawLine(sx, sy, tx, ty, olc::GREEN);
				else
					DrawLine(sx, sy, tx, ty, olc::MAGENTA);
			}
			else
				DrawLine(sx, sy, tx, ty, olc::MAGENTA);

			FillTriangle(x1, y1, x2, y2, x3, y3, olc::MAGENTA);
			FillTriangle(x1, y1, x2, y2, x4, y4, olc::MAGENTA);
			DrawString((sx + tx) / 2.0f - 8.0f, (sy + ty) / 2.0f - 8.0f, std::to_string(edge.length), olc::CYAN, 2);
		}

		// Drawing the vertices with their respevtive indices
		for (auto const &vertex : vVertices)
		{
			// Drawing the path
			if (eMode == DIJKSTRA)
			{
				if (VertexIsInPath(vertex.id))
					FillCircle(vertex.px, vertex.py, iRadius, olc::GREEN);
				else
					FillCircle(vertex.px, vertex.py, iRadius, olc::Pixel(255, 128, 0));
			}
			else
				FillCircle(vertex.px, vertex.py, iRadius, olc::Pixel(255, 128, 0));

			// The selected circle is highlighted magenta
			if (vertex.id == iSelectedVertex)
				FillCircle(vertex.px, vertex.py, iRadius, olc::MAGENTA);

			if (vertex.id > 9)
				DrawString(vertex.px - 15.0f, vertex.py - 7.0f, std::to_string(vertex.id), olc::BLACK, 2);
			else
				DrawString(vertex.px - 7.0f, vertex.py - 7.0f, std::to_string(vertex.id), olc::BLACK, 2);
		}

		if (eMode == DIJKSTRA)
		{
			for (auto const &vertex : vVertices)
			{
				if (vertex.id == iStart)
					DrawString(vertex.px - 40, vertex.py - 32.0f, "Start", olc::CYAN, 2);

				if (vertex.id == iEnd)
					DrawString(vertex.px - 24, vertex.py - 32.0f, "End", olc::CYAN, 2);
			}
		}

		// TODO: Draw radius size
		DrawString(5.0f, 5.0f, length.append(std::to_string(iEdgeLength)), olc::MAGENTA, 2);
		DrawString(5.0f, 25.0f, mode, olc::MAGENTA, 2);
	}

	// Returns whether an edge is part of the selected path
	bool EdgeIsInPath(int sid, int tid)
	{
		if (vPath.empty())
			return false;

		for (int i = 0; i < vPath.size() - 1; i++)
			if (sid == vPath[i] && tid == vPath[i + 1])
				return true;

		return false;
	}

	// Returns whether a vertex is part of the selected path
	bool VertexIsInPath(int id)
	{
		for (auto const &point : vPath)
			if (point == id)
				return true;

		return false;
	}

	// The user left-clicks on a vertex, setting it as the starting point of dijkstra's shortest path
	void SetStart()
	{
		for (auto const &vertex : vVertices)
			if (IsPointInCircle(vertex.px, vertex.py, iRadius, GetMouseX(), GetMouseY()))
				iStart = vertex.id;

		bChangeHasOccured = true;
	}

	// The user right-clicks on a vertex, setting it as the ending point of dijkstra's shortest path
	void SetEnd()
	{
		for (auto const &vertex : vVertices)
			if (IsPointInCircle(vertex.px, vertex.py, iRadius, GetMouseX(), GetMouseY()))
				iEnd = vertex.id;

		bChangeHasOccured = true;
	}

	// Returns whether the list contains the point
	bool containsPoint(int _node, std::list<sPoint> nodes)
	{
		for (auto const &node : nodes)
			if (node.id == _node)
				return true;

		return false;
	}

	// Returns whether the vector contains the node
	bool containsNode(int _node, std::vector<int> nodes)
	{
		for (auto const &node : nodes)
		{
			if (_node == node)
				return true;
		}

		return false;
	}

	// TODO: redesign this mess
	// This provides the data for the shortest path from the start point to the end point
	void Dijkstra()
	{
		if (iStart == -1 || iEnd == -1)
			return;

		std::stack<int> stack;
		std::vector<int> exclusionNodes;
		std::list<sPoint> dijkstra;

		stack.push(iStart);

		bool hasChildren = true;
		bool leadsToEnd = true;

		// TODO: this forsaken algorithm
		// Loads the relevant parts of the graph into a list
		for (int index = 0; index < 100; index++)
		{
			std::cout << "size:" + stack.size() << ' ' << "top:" + stack.top() << ' '; // dbg
			for (auto const &n : exclusionNodes)									   // dbg
				std::cout << n << ',';												   // dbg
			std::cout << std::endl;													   // dbg

			hasChildren = false;

			// If the node is in exclusionNodes, pop it
			if (containsNode(stack.top(), exclusionNodes))
				stack.pop();

			// If one of its children is in dijkstra or is iEnd, move it to dijkstra
			for (auto const &edge : vEdges)

				if (edge.source == stack.top())

					if (containsPoint(edge.target, dijkstra) || edge.target == iEnd)
					{
						dijkstra.push_back(sPoint(stack.top(), iEnd, INT_MAX));
						stack.pop();
						//continue;
					}

			// If the node is in neither lists, push its children onto the stack
			if (!containsNode(stack.top(), exclusionNodes) && !containsPoint(stack.top(), dijkstra))

				for (auto const &edge : vEdges)

					if (edge.source == stack.top())
					{
						hasChildren = true;
						stack.push(edge.target);
					}

			// If it has no children and it's not iEnd, pop it without consequence
			if (!hasChildren && stack.top() != iEnd)
			{
				exclusionNodes.push_back(stack.top());
				stack.pop();
			}
		}

		std::cout << "Graph:";			   // dbg
		for (auto const &point : dijkstra) // dbg
			std::cout << point.id << ' ';  // dbg
		std::cout << std::endl;			   // dbg

		// Dijkstra's shortest path construction data
		for (auto &point : dijkstra)
		{
			if (point.visited)
				continue;

			point.visited = true;

			for (auto const &edge : vEdges)

				// for all neighbours that are a source
				if (edge.source == point.id)

					for (auto &_point : dijkstra)

						// for all neighbours that are a source to this point
						if (edge.target == _point.id)

							// Check all sources for distance, if shorter then update them accordingly
							if (_point.distance > point.distance + edge.length)
							{
								_point.distance = point.distance + edge.length;
								_point.parent = point.id;
							}
		}

		std::list<sPoint>::iterator listIterator = dijkstra.begin();

		while (listIterator->id != iEnd)
			listIterator++;

		// Clear the stack
		while (!stack.empty())
			stack.pop();

		int parent;

		while (listIterator->id != iStart)
		{
			// This prevents an infinite loop
			if (listIterator->parent == iEnd)
				break;

			stack.push(listIterator->id);

			parent = listIterator->parent;

			listIterator = --dijkstra.end();

			while (listIterator->id != parent)
				listIterator--;
		}

		listIterator = dijkstra.begin();
		stack.push(listIterator->id);

		while (!stack.empty())
		{
			vPath.push_back(stack.top());
			stack.pop();
		}

		std::cout << "Path:";			// dbg
		for (auto const &point : vPath) // dbg
			std::cout << point << ' ';	// dbg
		std::cout << std::endl;			// dbg

		bChangeHasOccured = false;
	}

	// TODO: account for void return value
	// Returns the x value of a vertex
	float GetX(int id)
	{
		for (auto const &vertex : vVertices)
			if (vertex.id == id)
				return vertex.px;
	}

	// TODO: account for void return value
	// Returns the y value of a vertex
	float GetY(int id)
	{
		for (auto const &vertex : vVertices)
			if (vertex.id == id)
				return vertex.py;
	}

	// The vertex clicked on by the user is marked as selected
	void SelectVertex()
	{
		pSelectedVertex = nullptr;

		for (auto &vertex : vVertices)

			if (IsPointInCircle(vertex.px, vertex.py, iRadius, GetMouseX(), GetMouseY()))
			{
				pSelectedVertex = &vertex;
				break;
			}
	}

	// The selected vertex follows the position of the mouse cursor
	void MoveVertex()
	{
		if (pSelectedVertex != nullptr)
		{
			pSelectedVertex->px = GetMouseX();
			pSelectedVertex->py = GetMouseY();
		}
	}

	// TODO: refactor this mess
	// Creates a new vertex
	void CreateNewVertex()
	{
		pSelectedVertex = nullptr;
		bool bGoodMousePosition = false;

		if (vVertices.size() == 0)
		{
			vVertices.push_back(sVertex(GetMouseX(), GetMouseY(), 0));
			sIndices.insert(0);
			return;
		}

		// Checks if the mouse is inside a vertex
		for (auto &vertex : vVertices)
		{
			if (!IsPointInCircle(vertex.px, vertex.py, iRadius, GetMouseX(), GetMouseY()))

				bGoodMousePosition = true;

			else
			{
				bGoodMousePosition = false;
				break;
			}
		}

		int id = 0;

		// Creates appropriate id
		while (sIndices.count(id) > 0)
			id++;

		if (bGoodMousePosition)
		{
			vVertices.push_back(sVertex(GetMouseX(), GetMouseY(), id));
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

		for (int i = 0; i < vVertices.size(); i++)

			if (IsPointInCircle(vVertices[i].px, vVertices[i].py, iRadius, GetMouseX(), GetMouseY()))
			{
				_id = vVertices[i].id;
				vVertices.erase(vVertices.begin() + i);
				sIndices.erase(_id);
			}

		for (int i = 0; i < vEdges.size(); i++)

			if (vEdges[i].source == _id || vEdges[i].target == _id)
			{
				vEdges.erase(vEdges.begin() + i);
				i--;
			}

		bChangeHasOccured = true;
	}

	// TODO: refactor this mess
	// Creates a new edge
	void CreateNewEdge()
	{
		// If no vertex has been selected yet
		if (iSelectedVertex == -1)
		{

			for (auto &vertex : vVertices)

				if (IsPointInCircle(vertex.px, vertex.py, iRadius, GetMouseX(), GetMouseY()))
					iSelectedVertex = vertex.id;
		}
		else
		{
			for (auto &vertex : vVertices)

				// Don't create an edge from the vertex to itself
				if (vertex.id != iSelectedVertex)

					if (IsPointInCircle(vertex.px, vertex.py, iRadius, GetMouseX(), GetMouseY()))
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

			iSelectedVertex = -1;
		}

		bChangeHasOccured = true;
	}

	// Deletes an edge
	void DeleteEdge()
	{
		for (auto const &vertex : vVertices)

			if (IsPointInCircle(vertex.px, vertex.py, iRadius, GetMouseX(), GetMouseY()))

				for (int i = 0; i < vEdges.size(); i++)

					if (vEdges[i].source == iSelectedVertex && vEdges[i].target == vertex.id)

						vEdges.erase(vEdges.begin() + i);

		iSelectedVertex = -1;
		bChangeHasOccured = true;
	}

	// Returns whether two given circles overlap
	bool DoCirclesOverlap(float x1, float y1, float r1, float x2, float y2, float r2)
	{
		return fabs((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)) <= (r1 + r2) * (r1 + r2);
	}

	// Returns whether a given point is within a circle
	bool IsPointInCircle(float circleX, float circleY, float radius, float pointX, float pointY)
	{
		return fabs((circleX - pointX) * (circleX - pointX) + (circleY - pointY) * (circleY - pointY)) < (radius * radius);
	}
};

int main()
{
	GraphingTool demo;
	if (demo.Construct(1280, 720, 1, 1))
		demo.Start();
	return 0;
}
