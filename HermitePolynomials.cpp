#include <iostream>
#include <stack>
using namespace std;

int HermitePol(int x, int n)
{
	if (n == 0)
		return 1;
	else if (n == 1)
		return 2 * x;
	else return 2 * x*HermitePol(x, n - 1) + 2 * (n - 1)*HermitePol(x, n - 2);
}


void print(stack<int> &s)
{
	if (s.empty())
	{
		cout << endl;
		return;
	}
	int x = s.top();
	s.pop();
	print(s);
	s.push(x);
	cout << x << " ";
}

int HermitePolStack(int x, int n)
{
	if (n == 0)
		return 1;
	if (n == 1)
		return 2 * x;

	stack<int> s;
	s.push(1);
	s.push(2 * x);

	while (n > 1)
	{
		int t = s.top();
		s.pop();
		int y = s.top();
		s.pop();
		s.push(t);
		s.push(2 * x*t + 2 * (n - 1)*y);
		n--;
	}
	print(s);
	cout << endl;
	return s.top();

}

int main()
{
	cout <<"result: "<< HermitePol(3, 4) << endl;
	cout << "result with stack funtion: " << HermitePolStack(3, 4) << endl;

	system("pause");
	return 0;
}