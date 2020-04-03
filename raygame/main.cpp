/*******************************************************************************************
*
*   raylib [core] example - basic window
*
*   This example has been created using raylib 1.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2013-2016 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include <vector>
#include <algorithm>
#include <iostream>

struct Node;

struct Edge {
	Node* target;
	float cost;
};

struct Node {
	Vector2 position;

	float gScore;
	Node* previous;

	std::vector<Edge> connections;
};

std::vector<Node*> dijkstrasSearch(Node* startNode, Node* endNode) {
	//Validate the input
	if (startNode == nullptr || endNode == nullptr) {
		return std::vector<Node*>();
	}

	if (startNode == endNode) {
		std::vector<Node*> singleNodePath;
		singleNodePath.push_back(startNode);
		return singleNodePath;
	}

	//Initialize the starting node
	startNode->gScore = 0;
	startNode->previous = nullptr;

	//Create our temporary lists for storing nodes
	std::vector<Node*> openList;
	std::vector<Node*> closedList;

	//Add the starting node to openList
	openList.push_back(startNode);

	while (!openList.empty()) {
		//Set the current node to the first node in the openList
		Node* currentNode = openList.front();
		//Remove currentNode from openList
		openList.erase(openList.begin());
		//Add currentNode to closedList
		closedList.push_back(currentNode);

		//If the destination node was added to the closed list,
		//the shortest path has been found
		if (currentNode == endNode) {
			break;
		}

		//For each Edge e in currentNode's connections
		for (Edge e : currentNode->connections) {
			//If the target node is in the closedList, ignore it
			if (std::find(closedList.begin(), closedList.end(), e.target) != closedList.end()) {
				continue;
			}
			//If the target node is not in the openList, update it
			if (std::find(openList.begin(), openList.end(), e.target) == openList.end()) {
				//Calculate the target node's G Score
				e.target->gScore = currentNode->gScore + e.cost;
				//Set the target node's previous to currentNode
				e.target->previous = currentNode;
				//Find the earliest point we should insert the node
				//to the list to keep it sorted
				auto insertionPos = openList.end();
				for (auto i = openList.begin(); i != openList.end(); i++) {
					if (e.target->gScore < (*i)->gScore) {
						insertionPos = i;
						break;
					}
				}
				//Insert the node at the appropriate position
				openList.insert(insertionPos, e.target);
			}
			//Otherwise the target node IS in the open list
			else {
				//Compare the new G Score to the old one before updating
				if (currentNode->gScore + e.cost < e.target->gScore) {
					//Calculate the target node's G Score
					e.target->gScore = currentNode->gScore + e.cost;
					//Set the target node's previous to currentNode
					e.target->previous = currentNode;
				}
			}
		}
	}

	//Create path in reverse from endNode to startNode
	std::vector<Node*> path;
	Node* currentNode = endNode;

	while (currentNode != nullptr) {
		//Add the current node to the beginning of the path
		path.insert(path.begin(), currentNode);
		//Go to the previous node
		currentNode = currentNode->previous;
	}

	return path;
}

void drawNode(Node* node, bool selected = false) {
	static char buffer[10];
	sprintf_s(buffer, "%.0f", node->gScore);

	//Draw the circle for the outline
	DrawCircle(node->position.x, node->position.y, 50, YELLOW);
	//Draw the inner circle
	if (selected)
	{
		DrawCircle(node->position.x, node->position.y, 45, BROWN);
	}
	else
	{
		DrawCircle(node->position.x, node->position.y, 45, BLACK);
	}
	//Draw the text
	DrawText(buffer, node->position.x - 20, node->position.y - 20, 30, WHITE);
}

void drawGraph(Node* node, std::vector<Node*>* drawnList) {

	drawNode(node);
	drawnList->push_back(node);

	//For each Edge in this node's connections
	for (Edge e : node->connections) {
		//Draw the Edge
		DrawLine(node->position.x, node->position.y, e.target->position.x, e.target->position.y, WHITE);
		//Draw the cost
		Vector2 costPos = { (node->position.x + e.target->position.x) / 2, (node->position.y + e.target->position.y) / 2 };
		static char buffer[10];
		sprintf_s(buffer, "%.0f", e.cost);
		DrawText(buffer, costPos.x, costPos.y, 30, WHITE);
		//Draw the target node
		if (std::find(drawnList->begin(), drawnList->end(), e.target) == drawnList->end()) {
			drawGraph(e.target, drawnList);
		}
	}
}

int main()
{
	// Initialization
	//--------------------------------------------------------------------------------------
	int screenWidth = 1600;
	int screenHeight = 800;

	InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

	SetTargetFPS(60);

	//Nodes
	Node* a = new Node();
	a->position = Vector2{ 250.0f, 150.0f };
	Node* b = new Node();
	b->position = Vector2{ 500.0f, 150.0f };
	Node* c = new Node();
	c->position = Vector2{ 500.0f, 300.0f };
	Node* d = new Node();
	d->position = Vector2{ 500.0f, 450.0f };
	Node* e = new Node();
	e->position = Vector2{ 375.0f, 600.0f };
	Node* f = new Node();
	f->position = Vector2{ 250.0f, 450.0f };
	//Edges
	a->connections.push_back(Edge{ b, 2 });
	a->connections.push_back(Edge{ f, 5 });
	b->connections.push_back(Edge{ c, 3 });
	c->connections.push_back(Edge{ a, 3 });
	c->connections.push_back(Edge{ d, 1 });
	d->connections.push_back(Edge{ e, 4 });
	d->connections.push_back(Edge{ f, 4 });
	f->connections.push_back(Edge{ e, 6 });

	std::vector<Node*> shortestPath = dijkstrasSearch(a, e);

	for (Node* node : shortestPath) {
		std::cout << node->gScore << std::endl;
	}
	//--------------------------------------------------------------------------------------

	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		// Update
		//----------------------------------------------------------------------------------
		// TODO: Update your variables here
		//----------------------------------------------------------------------------------

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(BLACK);

		//Draw the graph
		std::vector<Node*>* drawnList = new std::vector<Node*>();
		drawGraph(a, drawnList);
		delete drawnList;

		//Draw the shortest path
		for (Node* node : shortestPath) {
			drawNode(node, true);
		}

		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------   
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}