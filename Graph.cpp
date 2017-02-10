#include <iostream>
#include <list>
#include <map>
#include <cassert>
#include <set>
#include <string>
#include <queue>
#include <stack>

using namespace std;

template <class VT, class LT>
class Graph
{
private:
	map<VT, list<pair<VT, LT>>> edges;
	string stringOfLabels(VT start, VT end, const Graph<VT, LT>& g, set<VT>& visited);
public:
	void addVertex(const VT&);
	void addEdge(const VT&, const VT&, const LT&);
	list<pair<VT, LT>> edgesFrom(const VT&) const;
	LT getLabel(const VT&, const VT&) const;
	string stringOfLabels(VT start, VT end, const Graph<VT, LT>& g);
	bool hasPath(VT start, const char* str, const Graph<VT, LT> g);
	bool hasPathBFS(VT start, VT end, const Graph<VT, LT> g);
	bool hasPathDFS(VT start, VT end, const Graph<VT, LT> g);
	bool hasCycle(const Graph<VT, LT> g);
	void print();
	string getPathIter(VT star, VT end, const Graph<VT, LT> g);
};

template <class VT, class LT>
void Graph<VT, LT>::addVertex(const VT& newVertex)
{
	edges[newVertex];
}

template <class VT, class LT>
void Graph<VT, LT>::addEdge(const VT& outV, const VT& inV, const LT& lbl)
{
	addVertex(outV);
	addVertex(inV);

	edges[outV].push_back(pair<VT, LT>(inV, lbl));
}

template <class VT, class LT>
list<pair<VT, LT>> Graph<VT, LT>::edgesFrom(const VT& out) const
{
	return edges.at(out);
}

template <class VT, class LT>
LT Graph<VT, LT>::getLabel(const VT& out, const VT& in) const
{
	assert(edges.count(out) != 0);
	assert(edges.count(in) != 0);

	for (const pair<VT, LT>& edge : edgesFrom(out))
	{
		if (in == edge.first)
		{
			return edge.second;
		}
	}
	assert(false);
}

template <class VT, class LT>
string Graph<VT, LT>::stringOfLabels(VT start, VT end, const Graph<VT, LT>& g, set<VT>& visited)
{
	if (start == end)
	{
		return "";
	}

	visited.insert(start);

	for (const pair<VT, LT>& edge : g.edgesFrom(start))
	{
		if (visited.count(edge.first) == 0)
		{
			string nextString = stringOfLabels(edge.first, end, g, visited);

			if (!nextString.empty())
			{
				return edge.second + nextString;
			}
			if (edge.first == end)
			{
				string result;
				result += edge.second;
				return result;
			}
		}
	}
	return "";
}

template <class VT, class LT>
string Graph<VT, LT>::stringOfLabels(VT start, VT end, const Graph<VT, LT>& g)
{
	set<int> visited;
	return stringOfLabels(start, end, g, visited);
}

template <class VT, class LT>
bool Graph<VT, LT>::hasPath(VT start, const char* str, const Graph<VT, LT> g)
{
	if (*str == 0)
	{
		return true;
	}
	for (const pair<VT, LT>& edge : g.edgesFrom(start))
	{
		if (edge.second == *str && hasPath(edge.first, str + 1, g))
		{
			return true;
		}
	}
	return false;
}

template <class VT,class LT>
bool Graph<VT, LT>::hasPathBFS(VT start, VT end, Graph<VT, LT> g)
{
	set<VT> visited;
	queue<VT> q;

	q.push(start);
	visited.insert(start);

	while (!q.empty() && q.front() != end)
	{
		VT v = q.front();
		q.pop();

		for (const pair<VT, LT>& edge : g.edgesFrom(v))
		{
			const VT& neigh = edge.first;

			if (visited.count(neigh) == 0)
			{
				q.push(neigh);
				visited.insert(neigh);
			}
		}
	}
	return !q.empty();
}

template <class VT,class LT>
bool Graph<VT, LT>::hasPathDFS(VT start, VT end, const Graph<VT, LT> g)
{
	set<VT> visited;
	stack<VT> s;

	s.push(start);
	visited.insert(start);

	while (!s.empty() && s.top() != end)
	{
		VT v = s.top();
		s.pop();

		for (const pair<VT, LT>& edge : g.edgesFrom(v))
		{
			const VT& neigh = edge.first;

			if (visited.count(neigh) == 0)
			{
				s.push(neigh);
				visited.insert(neigh);
			}
		}
	}
	return !s.empty();
}

template <class VT,class LT>
bool Graph<VT, LT>::hasCycle(const Graph<VT, LT> g)
{
	for (const pair<VT, list<pair<VT, LT>>>& elem : edges)
	{
		for (const pair<VT, LT>& edge : elem.second)
		{
			if (hasPathBFS(edge.first, elem.first, g))
			{
				return true;
			}
		}
	}
	return false;
}

template<class VT, class LT>
void Graph<VT, LT>::print()
{
	for (const pair<VT, list<pair<VT, LT>>>& elem: edges)
	{
		for (const pair<VT, LT>& edge : elem.second)
		{
			cout << elem.first << " -> " << "(" << edge.first << "," << edge.second << ")" << endl;
		}
	}
	cout << endl;
}

template <class VT, class LT>
string Graph<VT, LT>::getPathIter(VT start, VT end, Graph<VT, LT> g)
{
	map<VT, VT> history;
	stack<VT> s;

	s.push(start);
	history[start] = start;

	while (!s.empty() && s.top() != end)
	{
		VT v = s.top();
		s.pop();

		for (const pair<VT, LT>& edge : g.edgesFrom(v))
		{
			const VT& neigh = edge.first;

			if (history.count(neigh) == 0)
			{
				s.push(neigh);
				history[neigh] = v;
			}
		}
	}

	if (s.empty())
	{
		return "";
	}

	string result;

	VT current = end;
	while (current != start)
	{
		VT& parent = history[current];
		result += g.getLabel(parent, current);
		current = parent;
	}
	return result;
}

void testGraph()
{
	Graph<int, char> g;
	g.addEdge(0, 1, 'a');
	g.addEdge(2, 0, 'c');
	g.addEdge(1, 4, 'b');
	g.addEdge(1, 2, 'b');
	g.addEdge(1, 3, 'b');
	g.addEdge(4, 3, 'x');
	g.addEdge(3, 4, 'z');

	assert(g.getLabel(0, 1) == 'a');
	assert(g.getLabel(3, 4) == 'z');
	assert(g.getLabel(4, 3) == 'x');
	assert(g.getLabel(1, 3) == 'b');

	cout << g.stringOfLabels(0, 3, g);

	assert(g.hasPath(0, "abc", g));
	assert(!g.hasPath(1, "abc", g));
	assert(g.hasPath(1, "bca", g));
	assert(g.hasPath(0, "abcabcabcabc", g));

	assert(g.hasPathBFS(2, 4, g));
	assert(!g.hasPathBFS(3, 1, g));
	assert(g.hasPathBFS(1, 1, g));
	assert(!g.hasPathBFS(4, 2, g));

	assert(g.hasPathDFS(2, 4, g));
	assert(!g.hasPathDFS(3, 1, g));
	assert(g.hasPathDFS(1, 1, g));
	assert(!g.hasPathDFS(4, 2, g));

	assert(g.hasCycle(g));
	cout << endl;
	g.print();

	cout << g.getPathIter(0, 3, g)<<endl;
}

int main()
{
	testGraph();


	cout  << "GG" << endl;

	system("pause");
	return 0;
}
