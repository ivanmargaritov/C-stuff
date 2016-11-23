#include <iostream>
#include <stack>

using namespace std;

#define MAX_SIZE 4

int labirinth[MAX_SIZE][MAX_SIZE] = { 0,0,0,0,
									  1,1,1,0,
									  0,1,1,0,
									  0,0,0,0 };

void printLabirinth()
{
	for (int r = 0; r < MAX_SIZE; r++)
	{
		for (int c = 0; c < MAX_SIZE; c++)
		{
			cout << labirinth[r][c] << " ";
		}
		cout << endl;
	}
}

bool possibleMov(int x, int y)
{
	return x >= 0 &&
		   y >= 0 &&
		   x < MAX_SIZE&&
		   y < MAX_SIZE&&
		   labirinth[x][y] == 0;
}

bool downstairs(int sx, int sy, int tx, int ty)
{
	if (sx == tx&&sy == ty)
	{
		return true;
	}
	if (possibleMov(sx + 1, sy))
	{
		return downstairs(sx + 1, sy, tx, ty);
	}
	if (possibleMov(sx, sy + 1))
	{
		return downstairs(sx, sy + 1, tx, ty);
	}
	return false;
}

struct Operation
{
	int x, y;
	Operation(int _x, int _y) :x(_x), y(_y) {}
};

bool downstairsStack(int sx, int sy, int tx, int ty)
{
	stack<Operation> operations;
	operations.push(Operation(sx, sy));

	while (!operations.empty())
	{
		Operation topOperation = operations.top();
		operations.pop();

		if (topOperation.x = tx&&topOperation.y == ty)
		{
			return 1;
		}
		else if (possibleMov(topOperation.x + 1, topOperation.y))
		{
			operations.push(Operation(topOperation.x + 1, topOperation.y));
		}
		else if (possibleMov(topOperation.x, topOperation.y + 1))
		{
			operations.push(Operation(topOperation.x, topOperation.y + 1));
		}
	}
	if (operations.empty())
		return false;
}

int main()
{
	cout << downstairs(0,2,3,3) << endl;
	cout << downstairs(3, 1, 0, 0) << endl;

	cout << downstairsStack(0, 2, 3, 3) << endl;
	cout << downstairsStack(3, 1, 0, 0) << endl;

	system("pause");
	return 0;
}