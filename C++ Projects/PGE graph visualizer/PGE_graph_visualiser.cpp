// TODO: better comments/documentation of functions and everything
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <set>
#include <stack>
#include <vector>
#include <climits>
#include <queue>

enum Mode
{
	MOVE,
	VERTEX,
	EDGE,
	DIJKSTRA
};

struct s_Vertex
{
	float positionX;
	float positionY;
	int id;

	// Constructor
	s_Vertex(float x, float y, int _id)
	{
		positionX = x;
		positionY = y;
		id = _id;
	}
};

struct s_Edge
{
	// Id of the source vertex
	int source;

	// Id of the target vertex
	int target;

	int length;

	// Constructor
	s_Edge(int _source, int _target, int _length)
	{
		source = _source;
		target = _target;
		length = _length;
	}
};

struct s_Point
{
	int id;
	int parent;
	int distanceToParent;
	bool visited;

	s_Point(int _id, int _parent, int distance_to_parent)
	{
		id = _id;
		parent = _parent;
		distanceToParent = distance_to_parent;
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
	std::vector<s_Vertex> v_Vertices;

	// Contains the data for the edges
	std::vector<s_Edge> v_Edges;

	std::set<int> s_Indices;

	// Contains the ids of the vertices that form the shortest path from i_Start to i_End
	std::vector<int> v_Path;

	s_Vertex *p_SelectedVertex = nullptr;

	int i_Radius = 15;
	int i_SelectedVertex = -1;
	int i_EdgeLength = 1;
	Mode e_Mode = MOVE;

	// Id of the selected starting vertex
	int i_Start = -1;

	// Id of the selected ending vertex
	int i_End = -1;

	bool b_ChangeHasOccurred = false;

public:
	bool OnUserCreate() override
	{
		// Debug data, remove when deploying to production
		v_Vertices = {
			s_Vertex(258, 467, 0),
			s_Vertex(474, 627, 1),
			s_Vertex(53, 663, 2),
			s_Vertex(461, 353, 3),
			s_Vertex(325, 146, 4),
			s_Vertex(232, 311, 5),
			s_Vertex(53, 461, 6),
			s_Vertex(568, 488, 7),
			s_Vertex(744, 441, 8),
			s_Vertex(564, 241, 9),
			s_Vertex(454, 63, 10),
			s_Vertex(830, 91, 11),
			s_Vertex(880, 317, 12),
			s_Vertex(1211, 59, 13),
			s_Vertex(1176, 462, 14),
			s_Vertex(855, 608, 15),
			s_Vertex(1216, 182, 16)
		};

		v_Edges = {
			s_Edge(13, 16, 1),
			s_Edge(16, 14, 1),
			s_Edge(14, 15, 1),
			s_Edge(12, 11, 1),
			s_Edge(11, 10, 1),
			s_Edge(9, 8, 1),
			s_Edge(8, 7, 1),
			s_Edge(7, 3, 1),
			s_Edge(4, 5, 1),
			s_Edge(5, 0, 1),
			s_Edge(0, 1, 1),
			s_Edge(6, 2, 1),
			s_Edge(1, 6, 2),
			s_Edge(10, 9, 2),
			s_Edge(3, 4, 2),
			s_Edge(15, 12, 2),
			s_Edge(14, 12, 4),
			s_Edge(12, 10, 4),
			s_Edge(12, 10, 4),
			s_Edge(9, 3, 4),
			s_Edge(3, 5, 4),
			s_Edge(0, 6, 4),
			s_Edge(1, 2, 4),
			s_Edge(16, 12, 5),
			s_Edge(8, 3, 5),
			s_Edge(14, 8, 5),
			s_Edge(5, 6, 6),
			s_Edge(16, 11, 7),
			s_Edge(12, 8, 7),
			s_Edge(10, 4, 9),
			s_Edge(7, 1, 8),
			s_Edge(15, 1, 10),
			s_Edge(15, 8, 9),
			s_Edge(3, 0, 7),
			s_Edge(7, 0, 8),
			s_Edge(13, 11, 7)
		};

		s_Indices = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

		i_Start = 12;
		i_End = 2;
		e_Mode = DIJKSTRA;

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		UserInput();
		VertexCollision();
		DrawingRoutine();
		return true;
	}

	// Handles user input
	void UserInput()
	{
		// If any substantial changes have occured to the graph, the shortesst path may no longer be valid
		// This flag is for detecting such changes and removing the shortest path data in that case
		b_ChangeHasOccurred = false;

		// TODO: refactor this
		// Changing modes via arrow keys
		if (e_Mode == MOVE && GetKey(olc::RIGHT).bPressed)
		{
			e_Mode = VERTEX;
		}
		else if (e_Mode == VERTEX && GetKey(olc::RIGHT).bPressed)
		{
			e_Mode = EDGE;
		}
		else if (e_Mode == EDGE && GetKey(olc::RIGHT).bPressed)
		{
			e_Mode = DIJKSTRA;
		}
		else if (e_Mode == DIJKSTRA && GetKey(olc::LEFT).bPressed)
		{
			e_Mode = EDGE;
		}
		else if (e_Mode == EDGE && GetKey(olc::LEFT).bPressed)
		{
			e_Mode = VERTEX;
		}
		else if (e_Mode == VERTEX && GetKey(olc::LEFT).bPressed)
		{
			e_Mode = MOVE;
		}

		// Different modes handle user input differently
		switch (e_Mode)
		{
			// Allows the user to move vertices around via mouse input
			case MOVE:
				// Select vertex by pressing left mouse
				if (GetMouse(0).bPressed)
				{
					SelectVertex();
				}

				// Move vertex around by holding left mouse
				if (GetMouse(0).bHeld)
				{
					MoveVertex();
				}

				if (GetMouse(0).bReleased)
				{
					p_SelectedVertex = nullptr;
				}
			break;

			// Allows user to create/delete vertices
			case VERTEX:
				// The maximum number of vertices allowed is 99
				// Create vertex with left mouse
				if (v_Vertices.size() < 100 && GetMouse(0).bPressed)
				{
					CreateNewVertex();
				}

				// Delete vertex with right mouse
				if (GetMouse(1).bPressed)
				{
					DeleteVertex();
				}
			break;

			// Allows user to create/delete edges
			case EDGE:
				// Create new edge with left mouse
				if (GetMouse(0).bPressed)
				{
					CreateNewEdge();
				}

				// Delete edge with right mouse
				if (GetMouse(1).bPressed)
				{
					DeleteEdge();
				}
			break;

			// Allows user to set a start and end point and let the program find the shortest path between the two
			case DIJKSTRA:
				// Left mouse button sets start vertex
				if (GetMouse(0).bPressed)
				{
					SetStart();
				}

				// Right mouse button sets end vertex
				if (GetMouse(1).bPressed)
				{
					SetEnd();
				}

				// Enter key finds the shortest path between start and end vertex
				if (GetKey(olc::ENTER).bReleased)
				{
					FindShortestPathUsingDijkstra();
				}
			break;
		}

		// Pressing A key increases edge length
		if (i_EdgeLength > 1 && GetKey(olc::A).bPressed)
		{
			i_EdgeLength--;
		}

		// Pressing D key reduces edge length
		if (i_EdgeLength < 20 && GetKey(olc::D).bPressed)
		{
			i_EdgeLength++;
		}

		// Pressing UP key increases vertex radius
		if (i_Radius < 20 && GetKey(olc::UP).bPressed)
		{
			i_Radius++;
		}

		// Pressing DOWN key reduces vertex radius
		if (i_Radius > 12 && GetKey(olc::DOWN).bPressed)
		{
			i_Radius--;
		}

		// User can clear all graph data by pressing backspace
		if (!v_Vertices.empty() && GetKey(olc::BACK).bPressed)
		{
			v_Vertices.clear();
			v_Edges.clear();
			v_Path.clear();
			i_Start = -1;
			i_End = -1;
			return;
		}

		// If any substantial change has occured to the graph, the shortest path data is cleared
		if (!v_Path.empty() && b_ChangeHasOccurred)
		{
			v_Path.clear();
		}
	}

	// TODO: Better comments for this function
	// Detects ovelap between two vertices and applies froce pushing them away from each other
	void VertexCollision()
	{
		for (auto &vertex : v_Vertices)
		{
			for (auto &target : v_Vertices)
			{
				if (vertex.id != target.id)
				{
					if (DoCirclesOverlap(vertex.positionX, vertex.positionY, i_Radius, target.positionX, target.positionY, i_Radius))
					{
						float fDistance = sqrtf((vertex.positionX - target.positionX) * (vertex.positionX - target.positionX) + (vertex.positionY - target.positionY) * (vertex.positionY - target.positionY));
						float fOverlap = 0.5f * (fDistance - (i_Radius * 2));

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
		switch (e_Mode)
		{
		case MOVE: mode = "  move >"; break;
		case VERTEX: mode = "< vertex >"; break;
		case EDGE: mode = "< edge >"; break;
		case DIJKSTRA: mode = "< Dijkstra's shortest path"; break;
		}

		// Drawing the edges with length and direction
		for (auto const &edge : v_Edges)
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
			float x1 = targetX + (directionX * (i_Radius / length));
			float y1 = targetY + (directionY * (i_Radius / length));

			// x2/y2 are on the edge
			float x2 = targetX + (directionX * ((i_Radius + 15.0f) / length));
			float y2 = targetY + (directionY * ((i_Radius + 15.0f) / length));

			// Helper positions to calculate position of floading triangel corners
			float xh = targetX + (directionX * ((i_Radius + 20.0f) / length));
			float yh = targetY + (directionY * ((i_Radius + 20.0f) / length));

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
			if (e_Mode == DIJKSTRA && EdgeIsInPath(edge.source, edge.target))
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
		for (auto const &vertex : v_Vertices)
		{
			// Drawing the vertices (color depends on mode and whether it's in the path)
			if (e_Mode == DIJKSTRA && VertexIsInPath(vertex.id))
			{
				FillCircle(vertex.positionX, vertex.positionY, i_Radius, olc::GREEN);
			}
			else
			{
				FillCircle(vertex.positionX, vertex.positionY, i_Radius, olc::Pixel(255, 128, 0));
			}

			// The selected circle is highlighted magenta
			if (vertex.id == i_SelectedVertex)
			{
				FillCircle(vertex.positionX, vertex.positionY, i_Radius, olc::MAGENTA);
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
		if (e_Mode == DIJKSTRA)
		{
			for (auto const &vertex : v_Vertices)
			{
				if (vertex.id == i_Start)
				{
					DrawString(vertex.positionX - 40, vertex.positionY - 32.0f, "Start", olc::CYAN, 2);
				}

				if (vertex.id == i_End)
				{
					DrawString(vertex.positionX - 24, vertex.positionY - 32.0f, "End", olc::CYAN, 2);
				}
			}
		}

		// TODO: Draw radius size
		// Drawing mode and edge length information in the top left corner
		DrawString(5.0f, 5.0f,  "Edge length: "+std::to_string(i_EdgeLength), olc::MAGENTA, 2);
		DrawString(5.0f, 25.0f, "Mode: "+mode, olc::MAGENTA, 2);
	}

	// Returns true, if the edge is part of the shortest path
	bool EdgeIsInPath(int const &sourceId, int const &targetId)
	{
		if (v_Path.empty())
		{
			return false;
		}

		for (int i = 0; i < v_Path.size() - 1; i++)
		{
			// If the current path element is the source and the next the target, return true
			if (sourceId == v_Path[i] && targetId == v_Path[i + 1])
			{
				return true;
			}
		}

		return false;
	}

	// Returns true, if a vertex is part of the shortest path
	bool VertexIsInPath(int const &id)
	{
		for (auto const &point : v_Path)
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

		for (auto const &vertex : v_Vertices)
		{
			if (Is_PointInCircle(vertex.positionX, vertex.positionY, i_Radius, mouseX, mouseY))
			{
				i_Start = vertex.id;
				b_ChangeHasOccurred = true;
				return;
			}
		}

		b_ChangeHasOccurred = false;
	}

	// The user right-clicks on a vertex, setting it as the ending point of dijkstra's shortest path
	void SetEnd()
	{
		float mouseX = GetMouseX();
		float mouseY = GetMouseY();

		for (auto const &vertex : v_Vertices)
		{
			if (Is_PointInCircle(vertex.positionX, vertex.positionY, i_Radius, mouseX, mouseY))
			{
				i_End = vertex.id;
				b_ChangeHasOccurred = true;
				return;
			}
		}

		b_ChangeHasOccurred = false;
	}

	// TODO: Refactor a lot of this; there are too many for loops in place
	// Puts the shortest path between the selected start and end point into v_Path
	void FindShortestPathUsingDijkstra()
	{
		// If either the beginning or end are not set, return
		if (i_Start == -1 || i_End == -1)
		{
			return;
		}

		std::vector<s_Point> v_Dijkstra;

		// The starting node gets pushed onto v_Dijkstra first
		v_Dijkstra.push_back(s_Point(i_Start, i_Start, 0));

		// Pushes each vertex as a point onto v_Dijkstra
		for (auto const &vertex : v_Vertices)
		{
			// Skip the starting node
			if (vertex.id == i_Start)
			{
				continue;
			}

			v_Dijkstra.push_back(s_Point(vertex.id, -1, 10000));
		}

		// The immediate children who's distance needs to be updated
		std::stack<int> s_Children;

		// The next points to visit
		std::queue<int> q_NextPoints;

		int i_CurrentVertex = i_Start;

		// TODO: not all points are marked as visited
		// TODO: the queue is not being emptied properly
		// BUG: for paths that are too long the path is not being shown
		// BUG: all points in v_Dijkstra seem to be unvisited
		// TODO {OPTIONAL}: maybe implement this using recursion (does appear to be easier to implement)
		// Dijkstra's shortest path algorithm implementation
		for (int i = 0; i < v_Dijkstra.size(); i++)
		{
			// Set the current point to visited
			for (auto &point : v_Dijkstra)
			{
				if (point.id == i_CurrentVertex)
				{
					point.visited == true;
				}
			}

			// Find all the children of the current point
			for (auto const &edge : v_Edges)
			{
				if (edge.source == i_CurrentVertex)
				{
					s_Children.push(edge.target);

					// Only enqueuing the children if they have not already been visited
					for (auto const &point : v_Dijkstra)
					{
						if (point.id == edge.target && point.visited == false)
						{
							q_NextPoints.push(point.id);
						}
					}
				}
			}

			// Update the distance and parent of each child
			while (!s_Children.empty())
			{
				// Find the current child
				for (auto &point : v_Dijkstra)
				{
					if (point.id == s_Children.top())
					{
						int *distanceOfCurrent;
						int *distanceToChild;
						int *distanceOfChild;

						// Distance of the current point
						for (auto &point : v_Dijkstra)
						{
							if (point.id == i_CurrentVertex)
							{
								distanceOfCurrent = &point.distanceToParent;
							}
						}

						// Distance to the child
						for (auto &edge : v_Edges)
						{
							if (edge.source == i_CurrentVertex && edge.target == s_Children.top())
							{
								distanceToChild = &edge.length;
							}
						}

						// Distance of the child
						for (auto &point : v_Dijkstra)
						{
							if (point.id == s_Children.top())
							{
								distanceOfChild = &point.distanceToParent;
							}
						}

						// Update the distance of the child
						if (*distanceOfCurrent + *distanceToChild < *distanceOfChild)
						{
							*distanceOfChild = *distanceOfCurrent + *distanceToChild;

							// Update the parent of the child
							for (auto &point : v_Dijkstra)
							{
								if (point.id == s_Children.top())
								{
									point.parent = i_CurrentVertex;
								}
							}
						}
					}
				}

				s_Children.pop();
			}

			// If there are no more children to visit, abort the algorithm
			if (q_NextPoints.empty())
			{
				break;
			}

			// Moving on to the next point and removing it from the queue
			i_CurrentVertex = q_NextPoints.front();
			q_NextPoints.pop();
		}

		// Since we need to follow the path backwards through v_Dijkstra, we need to inverse the entire path before we can insert it into v_Path
		// Reusing the previous variables
		i_CurrentVertex = i_End;

		std::stack<int> s_Path;

		s_Path.push(i_CurrentVertex);

		// Pushing all points onto the stack
		for (int i = 0; i < v_Dijkstra.size(); i++)
		{
			// Finding the parent of i_CurrentVertex
			for (auto const &point : v_Dijkstra)
			{
				// i_Start has itself as its parent
				// if (point.parent == i_Start && point.id == i_Start)
				// {
				// 	break;
				// }

				// Setting the parent id to be the current point
				if (point.id == i_CurrentVertex)
				{
					i_CurrentVertex = point.parent;

					// The parent was found, no need to search any further
					break;
				}
			}

			s_Path.push(i_CurrentVertex);
		}

		// DEBUGINFO: s_Path is full of -1s when the path is not shown

		// Copying the path onto v_Path
		while (!s_Path.empty())
		{
			v_Path.push_back(s_Path.top());

			s_Path.pop();
		}

		b_ChangeHasOccurred = false;
	}

	// TODO: account for void return value
	// Returns the position of a vertex in the x axis
	float GetX(int const &id)
	{
		for (auto const &vertex : v_Vertices)
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
		for (auto const &vertex : v_Vertices)
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
		p_SelectedVertex = nullptr;

		float mouseX = GetMouseX();
		float mouseY = GetMouseY();

		for (auto &vertex : v_Vertices)
		{
			if (Is_PointInCircle(vertex.positionX, vertex.positionY, i_Radius, mouseX, mouseY))
			{
				p_SelectedVertex = &vertex;
				break;
			}
		}
	}

	// The selected vertex follows the position of the mouse cursor
	void MoveVertex()
	{
		if (p_SelectedVertex != nullptr)
		{
			p_SelectedVertex->positionX = GetMouseX();
			p_SelectedVertex->positionY = GetMouseY();
		}
	}

	// TODO: refactor this mess
	// Creates a new vertex
	void CreateNewVertex()
	{
		p_SelectedVertex = nullptr;
		bool bGoodMousePosition = false;

		float mouseX = GetMouseX();
		float mouseY = GetMouseY();

		if (v_Vertices.size() == 0)
		{
			v_Vertices.push_back(s_Vertex(mouseX, mouseY, 0));
			s_Indices.insert(0);
			return;
		}

		// Checks if the mouse is inside a vertex
		for (auto &vertex : v_Vertices)
		{
			if (!Is_PointInCircle(vertex.positionX, vertex.positionY, i_Radius, mouseX, mouseY))
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
		while (s_Indices.count(id) > 0)
		{
			id++;
		}

		if (bGoodMousePosition)
		{
			v_Vertices.push_back(s_Vertex(mouseX, mouseY, id));
			s_Indices.insert(id);
		}

		bGoodMousePosition = false;
		b_ChangeHasOccurred = true;
	}

	// Deletes a vertex
	void DeleteVertex()
	{
		p_SelectedVertex = nullptr;
		int _id = -1;

		float mouseX = GetMouseX();
		float mouseY = GetMouseY();

		for (int i = 0; i < v_Vertices.size(); i++)
		{
			if (Is_PointInCircle(v_Vertices[i].positionX, v_Vertices[i].positionY, i_Radius, mouseX, mouseY))
			{
				_id = v_Vertices[i].id;
				v_Vertices.erase(v_Vertices.begin() + i);
				s_Indices.erase(_id);
			}
		}

		for (int i = 0; i < v_Edges.size(); i++)
		{
			if (v_Edges[i].source == _id || v_Edges[i].target == _id)
			{
				v_Edges.erase(v_Edges.begin() + i);
				i--;
			}
		}

		b_ChangeHasOccurred = true;
	}

	// TODO: refactor this mess
	// Creates a new edge
	void CreateNewEdge()
	{
		float mouseX = GetMouseX();
		float mouseY = GetMouseY();

		// If no vertex has been selected yet
		if (i_SelectedVertex == -1)
		{

			for (auto &vertex : v_Vertices)
			{
				if (Is_PointInCircle(vertex.positionX, vertex.positionY, i_Radius, mouseX, mouseY))
				{
					i_SelectedVertex = vertex.id;
				}
			}
		}
		else
		{
			for (auto &vertex : v_Vertices)
			{
				// Don't create an edge from the vertex to itself
				if (vertex.id != i_SelectedVertex)
					if (Is_PointInCircle(vertex.positionX, vertex.positionY, i_Radius, mouseX, mouseY))
					{
						for (auto const &edge : v_Edges)
						{
							// Don't create a duplicate edge
							if (edge.source == i_SelectedVertex && edge.target == vertex.id)
							{
								i_SelectedVertex = -1;
								return;
							}
							// Don't create a bidirectional edge
							if (edge.target == i_SelectedVertex && edge.source == vertex.id)
							{
								i_SelectedVertex = -1;
								return;
							}
						}

						v_Edges.push_back(s_Edge(i_SelectedVertex, vertex.id, i_EdgeLength));
					}
			}

			i_SelectedVertex = -1;
		}

		b_ChangeHasOccurred = true;
	}

	// Deletes an edge
	void DeleteEdge()
	{
		float mouseX = GetMouseX();
		float mouseY = GetMouseY();

		for (auto const &vertex : v_Vertices)
		{
			if (Is_PointInCircle(vertex.positionX, vertex.positionY, i_Radius, mouseX, mouseY))
			{
				for (int i = 0; i < v_Edges.size(); i++)
				{
					if (v_Edges[i].source == i_SelectedVertex && v_Edges[i].target == vertex.id)
					{
						v_Edges.erase(v_Edges.begin() + i);
					}
				}
			}
		}

		i_SelectedVertex = -1;
		b_ChangeHasOccurred = true;
	}

	// Returns whether two given circles overlap
	bool DoCirclesOverlap(float const &x1, float const &y1, float const &r1, float const &x2, float const &y2, float const &r2)
	{
		return fabs((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)) <= (r1 + r2) * (r1 + r2);
	}

	// Returns whether a given point is within a circle
	bool Is_PointInCircle(float const &circleX, float const &circleY, float const &radius, float const &pointX, float const &pointY)
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
