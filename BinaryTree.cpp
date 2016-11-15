#include <iostream>
#include <cassert>
#include <string.h>
#include <fstream>
#include <string>
#include <iomanip>
#include <stack>

using namespace std;

template <class T>
using mapFn = T(*) (const T&);

template <class T>
using mapFn2 = bool(*) (const T&);

template <class T>
struct Node
{
	T data;
	Node<T>* left;
	Node<T>* right;

	Node(const T& data, Node<T>* left, Node<T>* right)
	{
		this->data = data;
		this->left = left;
		this->right = right;
		makeID();
	}
	Node()
	{
		this->left = nullptr;
		this->right = nullptr;
		makeID();
	}
	int getID() const
	{
		return id;
	}
private:
	int id;

	void makeID()
	{
		static int maxID = 0;
		maxID++;
		id = maxID;
	}
};

template <class T>
class BTree
{
private:
	Node<T>* root;
	void simplePrint(Node<T>* subTreeRoot) const;
	bool add(const T& data, const char* trace, Node<T>* & subTreeRoot);
	void deleteAll(Node<T>* subTreeRoot);
	bool member(const T& data, Node<T>* subTreeRoot) const;
	void map(mapFn<T> f, Node<T>* subTreeRoot);
	int count(Node<T>* subTreeRoot);
	int countEvens(Node<T>* subTreeRoot);
	int searchCount(mapFn2<T> f, Node<T>* subTreeRoot);
	int height(Node<T>* subTreeRoot);
	int countLeaves(Node<T>* subTreeRoot);
	T& maxLeaf(T max, Node<T>* subTreeRoot);
	//T& getElement(const char* trace, Node<T>* subTreeRoot);
	void dottyPrint(Node <T>* subTreeRoot, ostream& out) const;
	void serialize(Node<T>* subTreeRoot, ostream& out) const;
	Node<T>* parseTree(istream &in);
	void serializeScheme(Node<T>* subTreeRoot, ostream& out) const;
	Node<T>* parseTreeScheme(istream& in);
	//T& operator[](Node<T>* subTreeRoot, int i);
	void prettyPrint(Node<T>* subTreeRoot, int space);
	void findTrace(Node<T>* subTreeRoot, const T& x, string trace);
	static Node<T>* copyTree(const Node<T>* subTreeRoot);
	void insertBOT(Node<T>* &subTreeRoot, const T& x);
	Node<T>* insertedBOT(Node<T>* subTreeRoot, const T& x);
	T minElement(Node<T>* subTreeRoot) const;
	void deleteElement(Node<T>*& subTreeRoot, const T& x);
	int calculateTree(Node<T>*& subTreeRoot);
	void inOrderPrint(Node<T>* subTreeRoot) const;
	void preOrderPrint(Node<T>* subTreeRoot) const;

	//при обхождането на дървото със стек,
	//в стека се поставят два вида "чакащи операции":
	//"извеждане" на елемента, сочен от указателя,
	//или обхождане на поддървото, сочено от указателя
	using waitingStep = pair<int, Node<T>*>;

	//всеки указател, попаднал в стека, е
	//"анотиран" с едната от двете операции
	#define STEP_EXTRACT_ROOT 0
	#define STEP_TRAVERSE_SUBTREE 1

public:
	BTree();
	void simplePrint() const;
	BTree<T>& add(const T& data, const char* trace);
	~BTree();
	bool member(const T& data) const;
	void map(mapFn<T> f);
	int count();
	int countEvens();
	int searchCount(mapFn2<T> f);
	int height();
	int countLeaves();
	T& maxLeaf();
	//T& getElement(const char* trace);
	void dottyPrint(ostream& out) const;
	void serialize(ostream& out) const;
	void deserialize(istream& in);
	void serializeScheme(ostream& out) const;
	void deserializeScheme(istream& in);
	//T& operator[](int i)
	void prettyPrint();
	string findTrace(const T& x);
	BTree(const BTree<T>& other);
	BTree<T>& operator= (const BTree<T>& other);
	BTree<T>& insertBOT(const T& x);
	BTree<T> insertedBOT(const T& x);
	T minElement() const;
	void deleteElement(const T& x);
	int calculateTree();
	void inOrderPrint() const;
	void preOrderPrint() const;

	//итератор, реализиращ
	//обхождане ЛКД на дървото
	class LeftRootRightIterator
	{
	public:
		LeftRootRightIterator(Node* root);
		T& operator * ();
		LeftRootRightIterator& operator++ ();
		bool operator != (const LeftRootRightIterator& other);

	private:
		stack<waitingStep> operations;
		void unwind();
	};

	LeftRootRightIterator end();
	LeftRootRightIterator begin();

};

template<class T>
BTree<T>::BTree()
{
	this->root = nullptr;
}

template <class T>
void BTree<T>::simplePrint(Node<T>* subTreeRoot) const
{
	if (subTreeRoot == nullptr)
	{
		return;
	}
	cout << subTreeRoot->data << " ";
	simplePrint(subTreeRoot->left);
	simplePrint(subTreeRoot->right);
}

template <class T>
void BTree<T>::simplePrint() const
{
	simplePrint(root);
}

template <class T>
bool BTree<T>::add(const T& x, const char* trace, Node<T>* & subTreeRoot)
{
	if (subTreeRoot == nullptr)
	{
		assert(strlen(trace) == 0);
		subTreeRoot = new Node<T>(x, nullptr, nullptr);
		return true;
	}
	assert(strlen(trace) > 0);
	if (trace[0] == 'L')
	{
		return add(x, trace + 1, subTreeRoot->left);
	}
	assert(trace[0] == 'R');
	return add(x, trace + 1, subTreeRoot->right);
	return true;
}

template <class T>
BTree<T>& BTree<T>::add(const T& x, const char* trace)
{
	add(x, trace, root);
	return *this;
}

template <class T>
void BTree<T>::deleteAll(Node<T>* subTreeRoot)
{
	if (subTreeRoot == nullptr)
	{
		return;
	}
	delete subTreeRoot;
	deleteAll(subTreeRoot->left);
	deleteAll(subTreeRoot->right);
}

template <class T>
BTree<T>::~BTree()
{
	deleteAll(root);
}

template <class T>
bool BTree<T>::member(const T& x, Node<T>* subTreeRoot) const
{
	if (subTreeRoot == nullptr)
	{
		return false;
	}
	return subTreeRoot->data == x || member(x, subTreeRoot->left) || member(x, subTreeRoot->right);
}

template <class T>
bool BTree<T>::member(const T& x) const
{
	return member(x, root);
}

template <class T>
void BTree<T>::map(mapFn<T> f, Node<T>* subTreeRoot)
{
	if (subTreeRoot == nullptr)
	{
		return;
	}
	subTreeRoot->data = f(subTreeRoot->data);
	map(f, subTreeRoot->left);
	map(f, subTreeRoot->right);
}

template <class T>
void BTree<T>::map(mapFn<T> f)
{
	map(f, root);
}

int plusOne(const int &i)
{
	return i + 1;
}

template <class T>
int BTree<T>::count(Node<T>* subTreeRoot)
{
	if (subTreeRoot == nullptr)
	{
		return 0;
	}
	return 1 + count(subTreeRoot->left) + count(subTreeRoot->right);

}

template <class T>
int BTree<T>::count()
{
	return count(root);
}

template <class T>
int BTree<T>::countEvens(Node<T>* subTreeRoot)
{
	if (subTreeRoot == nullptr)
	{
		return 0;
	}
	int count = 0;
	if (subTreeRoot->data % 2 == 0) {
		count++;
	}

	return count + countEvens(subTreeRoot->left) + countEvens(subTreeRoot->right);
}

template <class T>
int BTree<T>::countEvens()
{
	return countEvens(root);
}

template <class T>
int BTree<T>::searchCount(mapFn2<T> f, Node<T>* subTreeRoot)
{
	if (subTreeRoot == nullptr)
	{
		return 0;
	}
	if (f(subTreeRoot->data)) {
		return 1 + searchCount(f, subTreeRoot->left) + searchCount(f, subTreeRoot->right);
	}
	else
	{
		return searchCount(f, subTreeRoot->left) + searchCount(f, subTreeRoot->right);
	}
}

template <class T>
int BTree<T>::searchCount(mapFn2<T> f)
{
	return searchCount(f, root);
}

bool oddNumber(const int& num)
{
	return (num % 2 == 1) ? 1 : 0;
}

int max(int a, int b)
{
	return (a > b) ? a : b;
}

template <class T>
int BTree<T>::height(Node<T>* subTreeRoot)
{
	if (subTreeRoot == nullptr) {
		return 0;
	}
	return max(height(subTreeRoot->left), height(subTreeRoot->right)) + 1;
}

template <class T>
int BTree<T>::height()
{
	return height(root);
}

template <class T>
int BTree<T>::countLeaves(Node<T>* subTreeRoot)
{
	if (subTreeRoot == nullptr)
	{
		return 0;
	}
	if (subTreeRoot->left == nullptr&&subTreeRoot->right == nullptr)
	{
		return 1;
	}
	else return countLeaves(subTreeRoot->left) + countLeaves(subTreeRoot->right);
}

template <class T>
int BTree<T>::countLeaves()
{
	return countLeaves(root);
}

template <class T>
T& BTree<T>::maxLeaf(T max, Node<T>* subTreeRoot)
{
	if (subTreeRoot == nullptr)
		return max;

	if (subTreeRoot->left == nullptr && subTreeRoot->right == nullptr)
	{
		if (max < subTreeRoot->data)
		{
			max = subTreeRoot->data;
		}
	}

	T maxLeft = maxLeaf(max, subTreeRoot->left);
	T maxRight = maxLeaf(max, subTreeRoot->right);
	if (maxLeft > maxRight)
	{
		return maxLeft;
	}
	else return maxRight;
}

template <class T>
T& BTree<T>::maxLeaf()
{
	return maxLeaf(0, root);
}

template <class T>
void BTree<T>::dottyPrint(Node<T>* subTreeRoot, ostream& out) const
{

	if (subTreeRoot == nullptr)
	{
		return;
	}
	out << "    " << subTreeRoot->getID() << "[label=\"" << subTreeRoot->data << "\"];" << endl;
	if (subTreeRoot->left != nullptr)
	{

		out << "        " << subTreeRoot->getID() << "->" << subTreeRoot->left->getID() << endl;
	}
	if (subTreeRoot->right != nullptr)
	{

		out << "        " << subTreeRoot->getID() << "->" << subTreeRoot->right->getID() << endl;
	}

	dottyPrint(subTreeRoot->left, out);
	dottyPrint(subTreeRoot->right, out);
}

template <class T>
void BTree<T>::dottyPrint(ostream& out) const
{
	out << "digraph G {" << endl;
	dottyPrint(root, out);
	out << "}" << endl;
}

template <class T>
void BTree<T>::serialize(Node<T>* subTreeRoot, ostream& out) const
{
	if (subTreeRoot == nullptr)
	{
		out << "null ";
		return;
	}
	out << subTreeRoot->data << " ";

	serialize(subTreeRoot->left, out);
	serialize(subTreeRoot->right, out);
}

template <class T>
void BTree<T>::serialize(ostream& out) const
{
	serialize(root, out);
	cout << endl;
}

void removeWhite(istream& in)
{
	while (in.peek() <= 32)
	{
		in.get();
	}
}

template <class T>
Node<T>* BTree<T>::parseTree(istream& in)
{
	removeWhite(in);

	if (in.peek() == 'n')
	{
		string dummy;
		in >> dummy;
		assert(dummy == "null");
		return nullptr;
	}
	T data;
	in >> data;
	return new Node<T>(data, parseTree(in), parseTree(in));
}

template <class T>
void BTree<T>::deserialize(istream& in)
{
	deleteAll(root);
	root = parseTree(in);
}

template <class T>
void BTree<T>::serializeScheme(Node<T>* subTreeRoot, ostream& out) const
{
	if (subTreeRoot == nullptr)
	{
		out << "() ";
		return;
	}
	out << "(" << subTreeRoot->data << " ";
	serializeScheme(subTreeRoot->left, out);

	serializeScheme(subTreeRoot->right, out);
	out << ") ";
}

template <class T>
void BTree<T>::serializeScheme(ostream& out) const
{
	serializeScheme(root, out);
	cout << endl;
}

template <class T>
Node<T>* BTree<T>::parseTreeScheme(istream& in)
{
	removeWhite(in);
	if (in.peek() == '(')
	{
		string dummy;
		in >> dummy;
		assert(dummy == "()");
		return nullptr;
	}
	T data;
	in >> data;
	return new Node<T>(data, parseTree(in), parseTreeScheme(in));
}

template <class T>
void BTree<T>::deserializeScheme(istream& in)
{
	deleteAll(root);
	root = parseTree(in);
}

/*template <class T>
T& BTree<T>::operator[](Node<T>* subTreeRoot, int i)
{
if (subTreeRoot == nullptr)
{
return 0;
}
}*/

template <class T>
void BTree<T>::prettyPrint(Node<T>* subTreeRoot, int space)
{
	if (subTreeRoot == nullptr)
	{
		return;
	}
	if (subTreeRoot->right != nullptr)
	{
		prettyPrint(subTreeRoot->right, space + 5);
	}
	if (space != 0)
	{
		cout << setw(space) << " ";
	}
	if (subTreeRoot->right != nullptr)
	{
		cout << endl << setw(space) << " ";
	}
	cout << subTreeRoot->data;
	if (subTreeRoot->left != nullptr)
	{
		cout << setw(space) << " " << endl;
		prettyPrint(subTreeRoot->left, space + 5);
	}
}

template <class T>
void BTree<T>::prettyPrint()
{
	prettyPrint(root, 0);
}

template <class T>
void BTree<T>::findTrace(Node<T>* subTreeRoot, const T& x, string trace)
{
	if (subTreeRoot == nullptr)
	{
		return;
	}
	if (subTreeRoot->data == x)
	{
		cout << trace;
	}
	findTrace(subTreeRoot->left, x, trace + "L");
	findTrace(subTreeRoot->right, x, trace + "R");
}

template <class T>
string BTree<T>::findTrace(const T& x)
{
	string trace = "";
	findTrace(root, x, trace);
	return trace;
}

template <class T>
Node<T>* BTree<T>::copyTree(const Node<T>* subTreeRoot)
{
	if (subTreeRoot == nullptr)
		return nullptr;
	return new Node<T>(subTreeRoot->data, copyTree(subTreeRoot->left), copyTree(subTreeRoot->right));
}

template <class T>
BTree<T>::BTree(const BTree<T>& other)
{
	root = copyTree(other.root);
}

template <class T>
BTree<T>& BTree<T>::operator= (const BTree<T>& other)
{
	if (this == &other)
	{
		return *this;
	}
	deleteAll(root);
	root = copyTree(other.root);
	return *this;
}

template <class T>
void BTree<T>::insertBOT(Node<T>* &subTreeRoot, const T& x)
{
	if (subTreeRoot == nullptr)
	{
		subTreeRoot = new Node<T>(x, nullptr, nullptr);
		return;
	}
	if (x <= subTreeRoot->data)
	{
		insertBOT(subTreeRoot->left, x);
	}
	else
	{
		insertBOT(subTreeRoot->right, x);
	}
}

template <class T>
BTree<T>& BTree<T>::insertBOT(const T& x)
{
	insertBOT(root, x);
	return *this;
}

template <class T>
Node<T>* BTree<T>::insertedBOT(Node<T>* subTreeRoot, const T& x)
{
	if (subTreeRoot == nullptr)
	{
		return new Node<T>(x, nullptr, nullptr);
	}
	if (x > subTreeRoot->data)
	{
		return new Node<T>(subTreeRoot->data,
			copyTree(subTreeRoot->left),
			insertedBOT(subTreeRoot->right, x));
	}
	return new Node<T>(subTreeRoot->data,
		insertedBOT(subTreeRoot->left, x),
		copyTree(subTreeRoot->right));
}

template <class T>
BTree<T> BTree<T>::insertedBOT(const T& x)
{
	BTree<T> result;
	result.root = insertedBOT(root, x);
	return result;
}

template <class T>
T BTree<T>::minElement(Node<T>* subTreeRoot) const
{
	assert(subTreeRoot != nullptr);
	Node<T>* current = subTreeRoot;

	while (current->left != nullptr)
	{
		current = current->left;
	}
	return current->data;
}

template <class T>
T BTree<T>::minElement() const
{
	return minElement(root);
}

template <class T>
void BTree<T>::deleteElement(Node<T>*& subTreeRoot, const T& x)
{
	//триене от празното дърво
	if (subTreeRoot == nullptr)
	{
		return;
	}

	//триене от листо
	if (subTreeRoot->data == x&&
		subTreeRoot->left == nullptr&&
		subTreeRoot->right == nullptr)
	{
		delete subTreeRoot;
		subTreeRoot = nullptr;
		return;
	}

	//триене от лявото поддърво
	if (x < subTreeRoot->data)
	{
		deleteElement(subTreeRoot->left, x);
		return;
	}

	//триене от дясното поддърво
	if (x > subTreeRoot->data)
	{
		deleteElement(subTreeRoot->right, x);
		return;
	}

	//вече сме сигурни, че трием корена и че той има ПОНЕ ЕДИН наследник

	//триене на корен със само един наследник
	if (subTreeRoot->right == nullptr)
	{
		Node<T>* temp = subTreeRoot;
		subTreeRoot = subTreeRoot->left;
		delete temp;
		return;
	}

	//триене на корен със само един наследник
	//този случай може да не се разглежда
	if (subTreeRoot->left == nullptr)
	{
		Node<T>* temp = subTreeRoot;
		subTreeRoot = subTreeRoot->right;
		delete temp;
		return;
	}

	//вече сме сигурни, че коренът има точно два наследника
	//триене на кореда

	T minEl = minElement(subTreeRoot->right);
	subTreeRoot->data = minEl;
	deleteElement(subTreeRoot->right, minEl);
}

template <class T>
void BTree<T>::deleteElement(const T& x)
{
	deleteElement(root, x);
}

template <class T>
int BTree<T>::calculateTree(Node<T>*& subTreeRoot)
{
	if (subTreeRoot->left == nullptr && subTreeRoot->right == nullptr)
	{
		return subTreeRoot->data - 48;
	}
	assert(subTreeRoot->left != nullptr&&subTreeRoot->right != nullptr);
	switch (subTreeRoot->data)
	{
		case '+':
			return calculateTree(subTreeRoot->left) + calculateTree(subTreeRoot->right);
			break;
		case '-':
			return calculateTree(subTreeRoot->left) - calculateTree(subTreeRoot->right);
			break;
		case '*':
			return calculateTree(subTreeRoot->left) * calculateTree(subTreeRoot->right);
			break;
		case '/':
			return calculateTree(subTreeRoot->left) / calculateTree(subTreeRoot->right);
			break;
	}
}

template <class T>
int BTree<T>::calculateTree()
{
	return calculateTree(root);
}

//ITERATOR

/*template<class T>
BTree<T>::LeftRootRightIterator::LeftRootRightIterator(Node<T>* root)
{
	if (root != nullptr)
	{
		operations.push(BTree<T>::waitingStep(STEP_TRAVERSE_SUBTREE, root));
		//unwind осигурява, че на върха на стека
		//има операция за извличане или стекът е празен. Други
		//състояния на стека не са допустими.
		unwind();
	}
}*/

template <class T>
void BTree<T>::LeftRootRightIterator::unwind()
{
	//този метод извършва обхождането на чакащите за
	//обхождане указатели към поддървета до момента, в който
	//на върха на стека попадне операция за извличане на елемент. Т.е.
	//обхождането се "развива" до достигане на следващя чакащ
	//за "отпечатване" връх

	if (operations.empty())
	{
		return;
	}
	waitingStep topOperation = operations.top();
	Node* topNode = topOperation.second;

	while (!operations.empty() && topOperation.first != STEP_EXTRACT_ROOT)
	{
		operations.pop();

		if (topNode->right != nullptr)
		{
			operations.push(BTree<T>::waitingStep(STEP_TRAVERSE_SUBTREE, topNode->right));
		}

		operations.push(BTree<T>::waitingStep(STEP_EXTRACT_ROOT, topNode));

		if (topNode->left != nullptr)
		{
			operations.push(BTree<T>::waitingStep(STEP_TRAVERSE_SUBTREE, topNode->left));
		}

		topOperation = operations.top();
		topNode = topOperation.second;
	}

}

template <class T>
T& BTree<T>::LeftRootRightIterator::operator *()
{
	//всички операции с итератора осигуряват, че на върха му
	//има операция за извличане или стекът е празен. Други
	//състояния на стека не са допустими.
	//Възелът на върха на стека е текущо посетения възел
	//при обхождането

	assert(!operations.empty());
	assert(operations.top().first == STEP_EXTRACT_ROOT);
	assert(operations.top().second != nullptr);

	return operations.top().second->data;
}

template <class T>
typename BTree<T>::LeftRootRightIterator& BTree<T>::LeftRootRightIterator::operator++()
{
	//всички операции с итератора осигуряват, че на върха му
	//има операция за извличане или стекът е празен. Други
	//състояния на стека не са допустими.

	assert(!operations.empty());

	//премахва се операцията за извличане, която е на върха на стека.
	//това довежда до продължаване на обхождането към следващите "части"
	//да дървото, или "активиране" на ичакващите под върха на стека операции

	operations.pop();

	//стекът се "доразвива" до достигане на следваща
	//операция за извличане

	unwind();
	return *this;
}

template <class T>
bool BTree<T>::LeftRootRightIterator::operator!= (const LeftRootRightIterator& other)
{
	if (operations.empty())
	{
		return !other.operations.empty();
	}
	if (other.operations.empty())
	{
		return !operations.epmty();
	}

	//и двете са непразни
	
	return operations.top() != other.operations.top();
}

template <class T>
typename BTree<T>::LeftRootRightIterator BTree<T>::end()
{
	return LeftRootRightIterator(nullptr);
}

template <class T>
typename BTree<T>::LeftRootRightIterator BTree<T>::begin()
{
	return LeftRootRightIterator(root);
}

//DONE WITH THE ITERATOR

template <class T>
void BTree<T>::inOrderPrint(Node<T>* subTreeRoot) const
{
	if (subTreeRoot == nullptr)
	{
		return;
	}
	inOrderPrint(subTreeRoot->left);
	cout << subTreeRoot->data << " ";
	inOrderPrint(subTreeRoot->right);
}

template <class T>
void BTree<T>::inOrderPrint() const
{
	inOrderPrint(root);
}

template <class T>
void BTree<T>::preOrderPrint(Node<T>* subTreeRoot) const
{
	if (subTreeRoot == nullptr)
	{
		return;
	}
	preOrderPrint(subTreeRoot->left);
	preOrderPrint(subTreeRoot->right);
	cout << subTreeRoot->data << " ";
}

template <class T>
void BTree<T>::preOrderPrint() const
{
	preOrderPrint(root);
}

int main()
{
	BTree<int> t;
	t.add(2, "");
	t.add(3, "L");
	t.add(342, "R");
	t.add(9, "LL");
	t.add(26, "LR");
	t.add(18, "RL");
	t.add(1, "RR");
	t.add(12, "LLR");
	t.add(14, "LRL");
	t.add(16, "LRR");

	cout << "ORIGINAL TREE: " << endl;
	t.simplePrint();
	cout << endl << "-----------------" << endl;
	cout << endl << "IS 2 IN THE TREE?   " << t.member(2) << endl;
	cout << "IS 43 IS THE TREE?   " << t.member(43) << endl;
	t.map(plusOne);
	cout << endl << "PlusOne function applied: ";
	t.simplePrint();
	cout << endl;
	cout << endl << "COUNT: " << t.count() << endl;
	cout << endl << "COUNT EVENS: " << t.countEvens() << endl;

	cout << endl << "ELEMENTS SATISFYING THE FUNCTION: " << t.searchCount(oddNumber) << endl;
	cout << endl << "HEIGHT: " << t.height() << endl;
	cout << endl << "LEAVES: " << t.countLeaves() << endl;
	cout << endl << "MAX LEAF: " << t.maxLeaf() << endl << endl;

	t.dottyPrint(cerr);

	cout << endl << "-----SERIALIZATION-----" << endl;
	t.serialize(cout);

	/*BTree<int> test;
	ifstream in("data.txt");

	test.deserialize(in);
	cout << "-----DESERIALIZED TREE-----" << endl;
	test.serialize(cout);
	test.dottyPrint(cerr);*/

	cout << endl << "-----SCHEME SERIALIZATION-----" << endl;
	t.serializeScheme(cout);

	/*BTree<int> test;
	ifstream in("data.txt");

	cout << "-----DESERALIZED SCHEME TREE----" << endl;
	test.deserializeScheme(in);
	test.serialize(cout);
	test.dottyPrint(cerr);*/

	cout << endl << "PRETTY PRINT: " << endl;
	t.prettyPrint();
	cout << endl << endl << "-----TRACE TO 15-----" << endl;

	t.findTrace(15);
	cout << endl << "-----COPY CONTRUCTOR-----" << endl;

	BTree<int> t1 = t;
	t1.simplePrint();

	cout << endl << "-----INSERT BOT-----" << endl;
	BTree<int> tree;
	tree.insertBOT(34).
		insertBOT(56).
		insertBOT(9).
		insertBOT(64).
		insertBOT(23).
		insertBOT(17).
		insertBOT(18).
		insertBOT(10).
		insertBOT(6).
		insertBOT(3);
	tree.prettyPrint();

	/*cout << endl<<"-----INSERTING 23-----"<<endl;

	BTree<int> inserted=tree.insertedBOT(23);
	inserted.prettyPrint();*/

	cout << endl << "-----MINIMAL ELEMENT-----" << endl;
	cout << tree.minElement();

	cout << endl << "-----DELETING 34-----" << endl;
	tree.deleteElement(34);
	tree.prettyPrint();
	cout << endl;

	cout  <<"-----PRETTY PRINT OF TREE TO CALCULATE-----"<< endl;
	BTree<char> calc;
	calc.add('+', "").add('*', "L").add('+', "R").add('2', "LL").add('3', "LR").add('9', "RL").add('0', "RR");
	calc.prettyPrint();

	cout << endl << "-----CALCULATING TREE-----" << endl << calc.calculateTree() << endl <<endl<< "-----IN ORDER PRINT-----" << endl;
	calc.inOrderPrint();
	cout << endl;
	tree.inOrderPrint();

	cout << endl << endl << "-----PRE ORDER PRINT" << endl;
	calc.preOrderPrint();
	cout << endl;
	tree.preOrderPrint();

	cout << endl << endl << "-----POSR ORDER PRINT-----" << endl;
	calc.simplePrint();
	cout << endl;
	tree.simplePrint();
	cout << endl;

	system("pause");
	return 0;
}