#include <iostream>
#include <stack>
using namespace std;

struct Operation
{
	static const int OPER_PLACE_HORSE = 0;
	static const int OPER_REMOVE_HORSE = 1;
	static const int OPER_FIND_SOLUTION = 2;

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

class KnightConfigurator
{
private:
	static const int MAX_SIZE = 8;

	bool board[MAX_SIZE][MAX_SIZE];
	stack<Operation> operations;
	bool hasKnight(int row, int column, int size) const;
	bool canPlaceKnight(int row, int column, int size) const;
	bool isValid(int row, int column, int size) const;

public:
	KnightConfigurator(int knights, int size);
	void printCurrentConfig(int size) const;
	//void findNextConfig(int knights, int size);
	void findNextConfig(int numKnights, int size);
	bool hasNextConfig() const;
};

KnightConfigurator::KnightConfigurator(int knights, int size)
{
	for (int r = 0; r < size; r++)
	{
		for (int c = 0; c < size; c++)
		{
			board[r][c] = false;
		}
	}
	operations.push(Operation(Operation::OPER_FIND_SOLUTION, knights, size));
	findNextConfig(knights, size);
}

bool KnightConfigurator::hasKnight(int row, int column, int size) const
{
	return row >= 0 &&
		column >= 0 &&
		row < size &&
		column < size &&
		board[row][column];
}

bool KnightConfigurator::canPlaceKnight(int row, int column, int size) const
{
	return !hasKnight(row - 2, column - 1, size) &&
		!hasKnight(row - 2, column + 1, size) &&
		!hasKnight(row - 1, column - 2, size) &&
		!hasKnight(row - 1, column + 2, size) &&
		!hasKnight(row + 1, column - 2, size) &&
		!hasKnight(row + 1, column + 2, size) &&
		!hasKnight(row + 2, column - 1, size) &&
		!hasKnight(row + 2, column + 1, size) &&
		!hasKnight(row, column, size);
}

bool KnightConfigurator::isValid(int row, int column, int size) const
{
	return row >= 0 &&
		column >= 0 &&
		row < size &&
		column < size;
}

void KnightConfigurator::printCurrentConfig(int size) const
{
	cout << "++++++++++++++++++" << endl;

	for (int r = 0; r < size; r++)
	{
		for (int c = 0; c < size; c++)
		{
			if (hasKnight(r, c, size))
				cout << "H ";
			else
				cout << "_ ";
		}
		cout << endl;
	}
	cout << "++++++++++++++++++" << endl;
}

void KnightConfigurator::findNextConfig(int numKnights, int size)
{
	operations.push(Operation(Operation::OPER_FIND_SOLUTION, numKnights, size));

	while (!operations.empty())
	{
		Operation topOperation = operations.top();
		int numKnights = topOperation.numKnights;
		int size = topOperation.boardSize;
		operations.pop();

		if (numKnights == 0)
		{
			printCurrentConfig(size);
		}
		else if (topOperation.operType == Operation::OPER_PLACE_HORSE)
		{
			board[topOperation.row][topOperation.column] = true;

		}
		else if (topOperation.operType == Operation::OPER_REMOVE_HORSE)
		{
			board[topOperation.row][topOperation.column] = false;

		}
		else
		{
			for (int row = 0; row < size; ++row)
			{
				for (int column = 0; column < size; ++column)
				{
					if (canPlaceKnight(row, column, size))
					{
						operations.push(Operation(Operation::OPER_REMOVE_HORSE, 1, size, row, column));
						operations.push(Operation(Operation::OPER_FIND_SOLUTION, numKnights - 1, size));
						operations.push(Operation(Operation::OPER_PLACE_HORSE, 1, size, row, column));
					}
				}
			}
		}
	}
}

bool KnightConfigurator::hasNextConfig() const
{
	stack<Operation> copyStack = operations;

	while (!copyStack.empty())
	{
		Operation nextOperation = copyStack.top();
		copyStack.pop();

		if (nextOperation.operType != Operation::OPER_REMOVE_HORSE)
			return true;
	}
	return false;
}

int main()
{
	KnightConfigurator ex(4, 4);
	//ex.printCurrentConfig(4);

	/*while (ex.hasNextConfig())
	{
	ex.findNextConfig(4, 4);
	}*/

	while (ex.hasNextConfig())
	{
		ex.findNextConfig(4, 4);
	}

	cout<<ex.hasNextConfig()<<endl;

	system("pause");
	return 0;
}