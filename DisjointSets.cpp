#include <iostream>
#include <set>
#include <unordered_map>
using namespace std;

int main()
{
	set<int> set1;
	int numOfElements1;
	cout << "Enter num of elements" << endl;
	cin >> numOfElements1;
	cout << "enter elements of first set:" << endl;
	for (int i = 0; i < numOfElements1; i++)
	{
		int k;
		cin >> k;
		set1.insert(k);
	}

	unordered_map<int, bool> map1;
	for (set<int>::iterator it = set1.begin(); it != set1.end(); it++) 
	{
		map1[*it] = false;
	}

	set<int> set2;
	int numOfElements2;
	cout << "Enter num of elements" << endl;
	cin >> numOfElements2;
	cout << "enter elements of second set:" << endl;
	for (int i = 0; i < numOfElements2; i++)
	{
		int k;
		cin >> k;
		set2.insert(k);
	}

	set<int>::iterator it = set2.begin();
	while (it != set2.end())
	{
		if (map1.find(*it) != map1.end())
		{
			cout << "The sets are not disjoint" << endl;
			it++;
			break;
		}
		cout<<"The sets are disjoint"<<endl;
		break;
	}

	system("pause");
	return 0;
}