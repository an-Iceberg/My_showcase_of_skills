#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <set>
#include <stack>
#include <vector>
#include <limits>

struct sVertex
{
	float px;
	float py;
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
	int source;
	int target;
	int length;

	sEdge(int _source, int _target, int _length)
	{
		source = _source;
		target = _target;
		length = _length;
	}
};

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
	std::vector<sVertex> vVertices;
	std::vector<sEdge> vEdges; // OPTIONAL: implement vEdges as a set
	std::set<int> sIndices;
	std::vector<int> vPath;

	sVertex *pSelectedVertex = nullptr;

	int iRadius = 15;
	int iSelectedVertex = -1;
	int iEdgeLength = 1;
	int iMode = 1;
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

	void UserInput()
	{
		bChangeHasOccured = false;
		if (iMode < 4 && GetKey(olc::RIGHT).bPressed)
			iMode++;

		if (iMode > 1 && GetKey(olc::LEFT).bPressed)
			iMode--;

		switch (iMode)
		{
		case 1: // Move vertices
			if (GetMouse(0).bPressed)
				SelectVertex();
			if (GetMouse(0).bHeld)
				MoveVertex();
			if (GetMouse(0).bReleased)
				pSelectedVertex = nullptr;
			break;

		case 2:													// Vertex creation/deletion
			if (vVertices.size() < 100 && GetMouse(0).bPressed) // Max number of vertices: 99
				CreateNewVertex();
			if (GetMouse(1).bPressed)
				DeleteVertex();
			break;

		case 3: // Edge creation/deletion
			if (GetMouse(0).bPressed)
				CreateNewEdge();
			if (GetMouse(1).bPressed)
				DeleteEdge();
			break;

		case 4: // Dijkstra's shortest path
			if (GetMouse(0).bPressed)
				SetStart();
			if (GetMouse(1).bPressed)
				SetEnd();
			if (GetKey(olc::ENTER).bReleased)
				Dijkstra();
			break;
		}

		// === User definable edge length ===
		if (iEdgeLength > 1 && GetKey(olc::A).bPressed)
			iEdgeLength--;
		if (iEdgeLength < 20 && GetKey(olc::D).bPressed)
			iEdgeLength++;

		// === Changeable radius ===
		if (iRadius < 20 && GetKey(olc::UP).bPressed)
			iRadius++;
		if (iRadius > 12 && GetKey(olc::DOWN).bPressed)
			iRadius--;

		// === Clear graph ===
		if (!vVertices.empty() && GetKey(olc::BACK).bPressed)
		{
			vVertices.clear();
			vEdges.clear();
		}

		if (!vPath.empty() && bChangeHasOccured)
			vPath.clear();
	}

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

	void DrawingRoutine()
	{
		Clear(olc::DARK_BLUE);

		std::string mode;
		switch (iMode)
		{
		case 1:
			mode = "Mode: move ->";
			break;
		case 2:
			mode = "Mode: <- vertex ->";
			break;
		case 3:
			mode = "Mode: <- edge ->";
			break;
		case 4:
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

			if (iMode == 4)
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
			if (iMode == 4)
			{ // Drawing the path
				if (VertexIsInPath(vertex.id))
					FillCircle(vertex.px, vertex.py, iRadius, olc::GREEN);
				else
					FillCircle(vertex.px, vertex.py, iRadius, olc::Pixel(255, 128, 0));
			}
			else
				FillCircle(vertex.px, vertex.py, iRadius, olc::Pixel(255, 128, 0));

			if (vertex.id == iSelectedVertex)
			{
				FillCircle(vertex.px, vertex.py, iRadius, olc::MAGENTA); // Selected circle is highlighted red
			}
			if (vertex.id > 9)
			{
				DrawString(vertex.px - 15.0f, vertex.py - 7.0f, std::to_string(vertex.id), olc::BLACK, 2);
			}
			else
			{
				DrawString(vertex.px - 7.0f, vertex.py - 7.0f, std::to_string(vertex.id), olc::BLACK, 2);
			}
		}

		if (iMode == 4)
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

	bool EdgeIsInPath(int sid, int tid)
	{
		if (vPath.empty())
			return false;
		for (int i = 0; i < vPath.size() - 1; i++)
			if (sid == vPath[i] && tid == vPath[i + 1])
				return true;
		return false;
	}

	bool VertexIsInPath(int id)
	{
		for (auto const &point : vPath)
			if (point == id)
				return true;
		return false;
	}

	void SetStart()
	{
		for (auto const &vertex : vVertices)
			if (IsPointInCircle(vertex.px, vertex.py, iRadius, GetMouseX(), GetMouseY()))
				iStart = vertex.id;
		bChangeHasOccured = true;
	}

	void SetEnd()
	{
		for (auto const &vertex : vVertices)
			if (IsPointInCircle(vertex.px, vertex.py, iRadius, GetMouseX(), GetMouseY()))
				iEnd = vertex.id;
		bChangeHasOccured = true;
	}

	bool containsPoint(int _node, std::list<sPoint> nodes)
	{
		for (auto const &node : nodes)
			if (node.id == _node)
				return true;
		return false;
	}

	bool containsNode(int _node, std::vector<int> nodes)
	{
		for (auto const &node : nodes)
		{
			if (_node == node)
				return true;
		}
		return false;
	}

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

		// Loads the relevant parts of the graph into a list
		while (!stack.empty())
		{ // TODO: this goddamn algorithm
			// === Debug ===
			std::cout << "size:" + stack.size() << ' ' << "top:" + stack.top() << ' ';
			for (auto const &n : exclusionNodes)
				std::cout << n << ',';
			std::cout << std::endl;
			// === /Debug ===

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
			{
				for (auto const &edge : vEdges)
					if (edge.source == stack.top())
					{
						hasChildren = true;
						stack.push(edge.target);
					}
			}

			// If it has no children and it's not iEnd, pop it without consequence
			if (!hasChildren && stack.top() != iEnd)
			{
				exclusionNodes.push_back(stack.top());
				stack.pop();
			}
		}

		// === Debug ===
		std::cout << "Graph:";
		for (auto const &point : dijkstra)
			std::cout << point.id << ' ';
		std::cout << std::endl;
		// === /Debug ===

		for (auto &point : dijkstra)
		{ // Dijkstra's shortest path construction data
			if (point.visited)
				continue;
			point.visited = true;

			for (auto const &edge : vEdges)
				if (edge.source == point.id) // for all neighbours that are a source
					for (auto &_point : dijkstra)
						if (edge.target == _point.id) // for all neighbours that are a source to this point
							if (_point.distance > point.distance + edge.length)
							{ // Check all sources for distance, if shorter then update them accordingly
								_point.distance = point.distance + edge.length;
								_point.parent = point.id;
							}
		}

		std::list<sPoint>::iterator it = dijkstra.begin();

		while (it->id != iEnd)
			it++;

		while (!stack.empty())
		{ // Clear the stack
			stack.pop();
		}
		int parent;

		while (it->id != iStart)
		{
			if (it->parent == iEnd) // This prevents an infinite loop
				break;
			stack.push(it->id);
			parent = it->parent;
			it = --dijkstra.end();
			while (it->id != parent)
				it--;
		}

		it = dijkstra.begin();
		stack.push(it->id);

		while (!stack.empty())
		{
			vPath.push_back(stack.top());
			stack.pop();
		}

		// === Debug ===
		std::cout << "Path:";
		for (auto const &point : vPath)
			std::cout << point << ' ';
		std::cout << std::endl;
		// === /Debug ===

		bChangeHasOccured = false;
	}

	float GetX(int id)
	{
		for (auto const &vertex : vVertices)
			if (vertex.id == id)
				return vertex.px;
	}

	float GetY(int id)
	{
		for (auto const &vertex : vVertices)
			if (vertex.id == id)
				return vertex.py;
	}

	void SelectVertex()
	{
		pSelectedVertex = nullptr;
		for (auto &vertex : vVertices)
		{
			if (IsPointInCircle(vertex.px, vertex.py, iRadius, GetMouseX(), GetMouseY()))
			{
				pSelectedVertex = &vertex;
				break;
			}
		}
	}

	void MoveVertex()
	{
		if (pSelectedVertex != nullptr)
		{
			pSelectedVertex->px = GetMouseX();
			pSelectedVertex->py = GetMouseY();
		}
	}

	void CreateNewVertex()
	{ // TODO: refactor this mess
		pSelectedVertex = nullptr;
		bool bGoodMousePosition = false;
		if (vVertices.size() == 0)
		{
			vVertices.push_back(sVertex(GetMouseX(), GetMouseY(), 0));
			sIndices.insert(0);
			return;
		}
		for (auto &vertex : vVertices)
		{ // Checks if the mouse is inside a vertex
			if (!IsPointInCircle(vertex.px, vertex.py, iRadius, GetMouseX(), GetMouseY()))
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
		while (sIndices.count(id) > 0)
		{ // Creates appropriate id
			id++;
		}
		if (bGoodMousePosition)
		{
			vVertices.push_back(sVertex(GetMouseX(), GetMouseY(), id));
			sIndices.insert(id);
		}
		bGoodMousePosition = false;
		bChangeHasOccured = true;
	}

	void DeleteVertex()
	{
		pSelectedVertex = nullptr;
		int _id = -1;
		for (int i = 0; i < vVertices.size(); i++)
		{
			if (IsPointInCircle(vVertices[i].px, vVertices[i].py, iRadius, GetMouseX(), GetMouseY()))
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

	void CreateNewEdge()
	{ // TODO: refactor this mess
		if (iSelectedVertex == -1)
		{ // If no vertex has been selected yet
			for (auto &vertex : vVertices)
			{
				if (IsPointInCircle(vertex.px, vertex.py, iRadius, GetMouseX(), GetMouseY()))
				{
					iSelectedVertex = vertex.id;
				}
			}
		}
		else
		{
			for (auto &vertex : vVertices)
			{
				if (vertex.id != iSelectedVertex)
				{ // Don't create an edge from the vertex to itself
					if (IsPointInCircle(vertex.px, vertex.py, iRadius, GetMouseX(), GetMouseY()))
					{
						for (auto const &edge : vEdges)
						{
							if (edge.source == iSelectedVertex && edge.target == vertex.id)
							{ // Don't create a duplicate edge
								iSelectedVertex = -1;
								return;
							}
							if (edge.target == iSelectedVertex && edge.source == vertex.id)
							{ // Don't create a bidirectional edge
								iSelectedVertex = -1;
								return;
							}
						}
						vEdges.push_back(sEdge(iSelectedVertex, vertex.id, iEdgeLength));
					}
				}
			}
			iSelectedVertex = -1;
		}
		bChangeHasOccured = true;
	}

	void DeleteEdge()
	{
		for (auto const &vertex : vVertices)
		{
			if (IsPointInCircle(vertex.px, vertex.py, iRadius, GetMouseX(), GetMouseY()))
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

	bool DoCirclesOverlap(float x1, float y1, float r1, float x2, float y2, float r2)
	{
		return fabs((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)) <= (r1 + r2) * (r1 + r2);
	}

	bool IsPointInCircle(float x1, float y1, float r1, float px, float py)
	{
		return fabs((x1 - px) * (x1 - px) + (y1 - py) * (y1 - py)) < (r1 * r1);
	}
};

int main()
{
	GraphingTool demo;
	if (demo.Construct(1280, 720, 1, 1))
		demo.Start();
	return 0;
}
