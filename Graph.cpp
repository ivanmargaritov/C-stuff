#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <queue>

using namespace std;

struct Vertex {
	//typedef pair<int, Vertex*> Edges;
	vector<pair<int, Vertex*>> adj;
	string name;
	Vertex(string s) : name(s) {}
};
typedef pair<int, Vertex*> Edges;
//cost of edge, destination vertex
typedef unordered_map<string, Vertex*> GraphMap;

class Graph
{
public:
	GraphMap graph;
	void addVertex(const string&);
	void addEdge(const string& from, const string& to, int cost);
	void printNeighbours(Vertex n);
	void print();
	void BFS(string name);
};

void Graph::addVertex(const string &name)
{
	GraphMap::iterator itr = graph.find(name);
	if (itr == graph.end())
	{
		Vertex* v;
		v = new Vertex(name);
		graph[name] = v;
		return;
	}
}

void Graph::addEdge(const string& from, const string& to, int cost)
{
	Vertex* f = (graph.find(from)->second);
	Vertex* t = (graph.find(to)->second);
	pair<int, Vertex*> edge = make_pair(cost, t);
	f->adj.push_back(edge);
}

void Graph::printNeighbours(Vertex n)
{
	vector<Edges>::iterator itr = n.adj.begin();
	while (itr != n.adj.end())
	{
		cout << (*itr).second->name << ", ";
		itr++;
	}
	cout << endl;
}

void Graph::print() 
{
	GraphMap::iterator itr = graph.begin();
	while (itr != graph.end())
	{
		cout << itr->first << endl;
		printNeighbours(*(itr->second));
		itr++;
	}
}

void Graph::BFS(string name)
{
	unordered_map<string, bool> visited;
	queue<Vertex> q;
	q.push(*graph[name]);
	while (!q.empty())
	{
		Vertex current = q.pop();
		visited[current.name] = true;
		vector<Edges>::iterator itr = current.adj.begin();
		while (itr != current.adj.end())
		{
			name = itr->second.name;
			if (visited[name])
			{
				q.push(itr->second);
			}
			itr++;
		}
	}

}

int main() {

	Graph test;
	test.addVertex("A");
	test.addVertex("B");
	test.addVertex("C");

	test.addEdge("A", "B", 1);
	test.print();

	system("pause");
	return 0;
}