#include <iostream>
#include <stack>
using namespace std;

#define MAX_SIZE 8

bool board[MAX_SIZE][MAX_SIZE] = { false };

void printBoard(int size)
{
	cout << "+++++++++++++++++++\n";
	for (int r = 0; r < size; r++)
	{
		for (int c = 0; c < size; c++)
		{
			if (board[r][c])
				cout << "H ";
			else
				cout << "_ ";
		}
		cout << endl;
	}
	cout << "+++++++++++++++++++\n";
}

bool hasHorse(int r, int c, int size)
{
	return r >= 0 &&
		   c >= 0 &&
		   r < size&&
		   c < size&&
		   board[r][c];
}

bool canPlaceHorse(int r, int c, int size)
{
	return !hasHorse(r - 2, c - 1, size) &&
		   !hasHorse(r - 2, c + 1, size) &&
		   !hasHorse(r - 1, c - 2, size) &&
		   !hasHorse(r - 1, c + 2, size) &&
		   !hasHorse(r + 1, c - 2, size) &&
		   !hasHorse(r + 1, c + 2, size) &&
		   !hasHorse(r + 2, c - 1, size) &&
		   !hasHorse(r + 2, c + 1, size) &&
	   	   !hasHorse(r , c , size);
}   

void config(int k, int size)
{
	if (k == 0)
	{
		printBoard(size);
		return;
	}

	for (int r = 0; r < size; r++)
	{
		for (int c = 0; c < size; c++)
		{
			if (canPlaceHorse(r, c, size))
			{
				board[r][c] = true;
				config(k - 1, size);
				board[r][c] = false;
			}
		}
	}
}

struct Operation
{
	int operType;
	int numKnights;
	int boardSize;
	int row;
	int column;

	Operation(int type, int num, int size)
		:operType(type), numKnights(num), boardSize(size) {}

	Operation(int type, int num, int size, int row, int col) :
		operType(type), numKnights(num), boardSize(size), row(row), column(col) {}

};

#define OPER_PLACE_HORSE 0
#define OPER_REMOVE_HORSE 1
#define OPER_FIND_SOLUTION 2

void configStack(int k, int size)
{
	stack<Operation> operations;

	operations.push(Operation(OPER_FIND_SOLUTION, k, size));

	while (!operations.empty())
	{
		Operation topOperation = operations.top();
		int k = topOperation.numKnights;
		int size = topOperation.boardSize;
		operations.pop();

		if (k == 0)
		{
			printBoard(size);
		}
		else if (topOperation.operType == OPER_PLACE_HORSE)
		{
			board[topOperation.row][topOperation.column] = true;
		}
		else if (topOperation.operType == OPER_REMOVE_HORSE)
		{
			board[topOperation.row][topOperation.column] = false;
		}
		else
		{
			for (int r = 0; r < size; r++)
			{
				for (int c = 0; c < size; c++)
				{
					if (canPlaceHorse(r, c, size))
					{
						operations.push(Operation(OPER_REMOVE_HORSE, 1, size, r, c));
						operations.push(Operation(OPER_FIND_SOLUTION, k - 1, size));
						operations.push(Operation(OPER_PLACE_HORSE, 1, size, r, c));
					}
				}
			}
		}
	}
}

int main()
{

	configStack(1, 8);

	system("pause");
	return 0;
}