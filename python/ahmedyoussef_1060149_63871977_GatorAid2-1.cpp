#include <string>
#include <sstream>
#include <queue>
#include <algorithm>
#include <math.h>
#include <iostream>
using namespace std;

class Account
{
private:
	string name;
	int id;
	int height;
	int balance;
	Account* rightRotate(Account* node)
	{
		Account* node1 = node->left;
		Account* temp = node1->right;

		node1->right = node;
		node->left = temp;
		node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
		node1->height = max(getHeight(node1->left), getHeight(node1->right)) + 1;
		return node1;
		//		Account* x = node->getLeft();
		//		Account* T2;
		//		if (x != nullptr)
		//		{
		//			T2 = x->right;
		//		}

		//		else
		//		{
		//			T2 = nullptr;
		//		}

		//
		//		if (x != nullptr)
		//		{
		//			x->right = this;
		//		}

		//		else {}

		//
		//        node->left = T2;
		//
		//        node->height = max(getHeight(node->left),
		//			getHeight(node->right)) + 1;
		//		if (x != nullptr)
		//		{
		//			x->height = max(getHeight(x->left),
		//				getHeight(x->right)) + 1;
		//		}

		//
		//		if (x == nullptr)
		//		{
		//			return node->right;
		//		}

		//		else
		//		{
		//			return x;
		//		}
	}

	Account* leftRotate(Account* node)
	{
		Account* node1 = node->right;
		Account* temp = node1->left;

		node1->left = node;
		node->right = temp;
		node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
		node1->height = max(getHeight(node1->left), getHeight(node1->right)) + 1;
		return node1;
		//		Account* y = node->getRight();
		//		Account* T2;
		//		if (y != nullptr)
		//		{
		//			T2 = y->left;
		//		}

		//		else
		//		{
		//			T2 = nullptr;
		//		}

		//
		//		if (y != nullptr)
		//		{
		//			y->left = this;
		//		}

		//		else {}

		//
		//        node->right = T2;
		//
		//        node->height = max(getHeight(node->left),
		//			getHeight(node->right)) + 1;
		//		if (y != nullptr)
		//		{
		//			y->height = max(getHeight(y->left),
		//				getHeight(y->right)) + 1;
		//		}

		//
		//		if (y == nullptr)
		//		{
		//			return node->left;
		//		}

		//		else
		//		{
		//			return y;
		//		}
	}

	Account* adjust(Account*& node)
	{
		if (node->balance > 1 && node->id > node->left->id)
		{
			return rightRotate(node);
		}

		if (node->balance < -1 && node->id < node->right->id)
		{
			return leftRotate(node);
		}

		if (node->balance > 1 && node->id < node->left->id)
		{
			node->left = leftRotate(node->left);
			return rightRotate(node);
		}

		if (node->balance < -1 && node->id > node->right->id)
		{
			node->right = rightRotate(node->right);
			return leftRotate(node);
		}
	}

public:
	~Account()
	{
		right = nullptr;
		left = nullptr;
	}

	Account* left;
	Account* right;
	Account(string _name, int _id)
	{
		left = nullptr;
		right = nullptr;
		name = _name;
		_id = id;
		height = 1;
		balance = 0;
	}

	void setHeight(Account*& node)
	{
		if (node == nullptr)
		{
			return;
		}

		node->height = 1 + max(getHeight(node->left),
			getHeight(node->right));

	}

	//	int getHeight()
	//	{
	//		if (this == nullptr)
	//		{
	//			return 0;
	//		}

	//		else
	//		{
	//			return height;
	//		}

	//	}

	int getHeight(Account* account)
	{
		if (account == nullptr)
			return 0;
		else
			return account->height;

	}

	string getName()
	{
		return name;
	}

	void setName(string _name)
	{
		name = _name;
	}

	int getId()
	{
		return id;
	}

	bool addLeft(Account*& newAccount, Account*& head, Account*& node)
	{
		if ((node != nullptr) && (newAccount->getId() == node->getId()))
		{
			return false;
		}
		else
			if (node == nullptr)
			{
				node = newAccount;
				setHeight(node);
				setBalance(head, node);
				return true;
			}
			else if (newAccount->getId() < node->getId())
			{
				bool
					var = node->addLeft(newAccount, head, node->left);
				setHeight(node);
				setBalance(head, node);
				return var;
			}
			else
			{
				bool
					var = node->addRight(newAccount, head, node->right);
				setHeight(node);
				setBalance(head, node);
				return var;
			}
	}

	bool addRight(Account*& newAccount, Account*& head, Account*& node)
	{
		if ((node != nullptr) && (newAccount->getId() == node->getId()))
		{
			return false;
		}
		else
			if (node == nullptr)
			{
				node = newAccount;
				setHeight(node);
				setBalance(head, node);
				return true;
			}
			else if (newAccount->getId() > node->getId())
			{
				bool
					var = node->addRight(newAccount, head, node->right);
				setHeight(node);
				setBalance(head, node);
				return var;
			}
			else
			{
				bool
					var = node->addLeft(newAccount, head, node->left);
				setHeight(node);
				setBalance(head, node);
				return var;
			}
	}

	Account* getLeft()
	{
		if (left != nullptr)
		{
			return left;
		}
		else
		{
			return nullptr;
		}
	}

	Account* getRight()
	{
		if (right != nullptr)
		{
			return right;
		}
		else
		{
			return nullptr;
		}
	}

	void setId(int _id)
	{
		id = _id;
	}

	void setBalance(Account*& head, Account*& node)
	{
		if (node == nullptr)
			node->balance = 0;
		node->balance = getHeight(node->left) -
			getHeight(node->right);

		if (abs(node->balance) > 1)
		{
			if (node == head)
			{
				head = adjust(node);
			}
			else
			{
				node = adjust(node);
			}
		}
	}
};
bool searchID(int id, Account* head)
{
	if (to_string(id).length() != 8)
	{
		return false;
	}

	if (head == nullptr)
	{
		return false;
	}
	else if (head->getId() == id)
	{
		cout << head->getName() << endl;
		return true;
	}
	else if (id < head->getId())
	{
		return (searchID(id, head->getLeft()));
	}
	else
	{
		return (searchID(id, head->getRight()));
	}
}

bool checkDuplicate(int id, Account* head)
{
	if (to_string(id).length() != 8)
	{
		return false;
	}

	if (head == nullptr)
	{
		return false;
	}
	else if (head->getId() == id)
	{
		return true;
	}
	else if (id < head->getId())
	{
		return (checkDuplicate(id, head->getLeft()));
	}
	else
	{
		return (checkDuplicate(id, head->getRight()));
	}
}

bool checkName(string name)
{
	int flag = 0;
	for (int i = 0; i < name.length(); i++)
	{
		if ((name[i] >= 'a' && name[i] <= 'z') || (name[i] >= 'A' && name[i] <= 'Z') || (name[i] == ' '))
			flag = 0;
		else
		{
			flag = 1;
			break;
		}
	}

	if (flag == 0)
		return false;
	else
		return true;
}

bool insertAccount(string _name, int _id, Account*& head)
{
	if (to_string(_id).length() != 8)
	{
		return false;
	}

	if (checkName(_name))
	{
		return false;
	}

	if (checkDuplicate(_id, head))
	{
		return false;
	}

	Account* newAccount = new Account(_name, _id);
	newAccount->setId(_id);
	newAccount->setHeight(head);
	bool result = false;
	if (head != nullptr && newAccount->getId() == head->getId())
	{
		return false;
	}
	else if (head != nullptr)
	{
		if (head->getId() < newAccount->getId())
		{
			result = head->addRight(newAccount, head, head->right);
		}
		else if (head->getId() > newAccount->getId())
		{
			result = head->addLeft(newAccount, head, head->left);
		}
	}
	else
	{
		head = newAccount;
		result = true;
	}

	head->setHeight(head);
	head->setBalance(head, head);
	return result;
}

bool searchName(string name, Account* head)
{
	std::size_t found = name.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz ");
	if (checkName(name))
	{
		return false;
	}

	if (head == nullptr)
	{
		return false;
	}
	else if (head->getName() == name)
	{
		searchName(name, head->getLeft());
		cout << head->getId() << endl;
		searchName(name, head->getRight());
		return true;
	}
	else
	{
		bool check1 = searchName(name, head->getLeft());
		bool check2 = searchName(name, head->getRight());
		return (check1 || check2);
	}

	return true;
}

void clearPrintQueue(queue<string>& printNamesQueue)
{
	while (!printNamesQueue.empty())
	{
		printNamesQueue.pop();
	}
}

void addToQueueInOrder(Account* head, queue<string>& printNamesQueue)
{
	if (head != nullptr)
	{
		addToQueueInOrder(head->getLeft(), printNamesQueue);
		printNamesQueue.push(head->getName());
		addToQueueInOrder(head->getRight(), printNamesQueue);
	}
}

void addToQueuePreOrder(Account* head, queue<string>& printNamesQueue)
{
	if (head != nullptr)
	{
		printNamesQueue.push(head->getName());
		addToQueuePreOrder(head->getLeft(), printNamesQueue);
		addToQueuePreOrder(head->getRight(), printNamesQueue);
	}
}

void addToQueuePostOrder(Account* head, queue<string>& printNamesQueue)
{
	if (head != nullptr)
	{
		addToQueuePostOrder(head->getLeft(), printNamesQueue);
		addToQueuePostOrder(head->getRight(), printNamesQueue);
		printNamesQueue.push(head->getName());
	}
}

void print(queue<string>& printNamesQueue)
{
	while (!printNamesQueue.empty())
	{
		cout << printNamesQueue.front();
		printNamesQueue.pop();
		if (!printNamesQueue.empty())
		{
			cout << ", ";
		}
		else
		{
			cout << endl;
		}
	}
}

void printInOrder(Account* head, queue<string>& printNamesQueue)
{
	clearPrintQueue(printNamesQueue);
	addToQueueInOrder(head, printNamesQueue);
	print(printNamesQueue);
}

void printPreOrder(Account* head, queue<string>& printNamesQueue)
{
	clearPrintQueue(printNamesQueue);
	addToQueuePreOrder(head, printNamesQueue);
	print(printNamesQueue);
}

void printPostOrder(Account* head, queue<string>& printNamesQueue)
{
	clearPrintQueue(printNamesQueue);
	addToQueuePostOrder(head, printNamesQueue);
	print(printNamesQueue);
}

void printLevelCount(Account* head)
{
	if (head == nullptr)
	{
		cout << 0 << endl;
	}
	else
	{
		cout << head->getHeight(head) << endl;
	}
}

Account* min(Account*& left, Account*& right)
{
	return left->getId() > right->getId() ? left : right;
}

Account* getSmallest(Account*& head)
{
	if (head->getLeft() == nullptr)
	{
		return head;
	}
	else
	{
		return getSmallest(head->left);
	}
}

bool searchByID(int id, Account* head)
{
	if (to_string(id).length() != 8)
	{
		return false;
	}

	if (head == nullptr)
	{
		return false;
	}
	else if (head->getId() == id)
	{
		return true;
	}
	else if (id < head->getId())
	{
		return (searchByID(id, head->getLeft()));
	}
	else
	{
		return (searchByID(id, head->getRight()));
	}
}

Account* minimumValue(Account* node)
{
	Account* current = node;
	while (current->left != nullptr)
		current = current->left;
	return current;
}

Account* deleteNode(Account* node, int key)
{
	if (node == nullptr)
		return node;
	if (key < node->getId())
		node->left = deleteNode(node->left, key);
	else if (key > node->getId())
		node->right = deleteNode(node->right, key);
	else
	{
		if (node->left == nullptr || node->right == nullptr)
		{
			Account* temp = nullptr;
			if (temp == node->left)
				temp = node->right;
			else
				temp = node->left;
			if (temp == nullptr)
			{
				node = nullptr;
			}
			else
				node = temp;
		}
		else
		{
			Account* temp = minimumValue(node->right);
			node->setId(temp->getId());
			node->setName(temp->getName());
			node->right = deleteNode(node->right, temp->getId());
		}
	}

	return node;
}

bool deleteId(int id, Account* node, Account*& root)
{
	if (node == nullptr || !searchByID(id, root))
		return false;

	Account* temp = deleteNode(node, id);
	if (temp == nullptr)
		return false;
	else
		return true;
	//    if(node== nullptr)
	//        return node;
	//
	//    if(id < node->getId())
	//        node->left = deleteId(id, node->left, root);

}

void addInorder(Account* node, queue<Account*>& que)
{
	if (node == nullptr)
		return;
	addInorder(node->left, que);
	que.push(node);
	addInorder(node->right, que);

}

bool removeInorder(int n, int& currentLevel, Account*& head)
{
	if (head == nullptr)
		return false;
	queue<Account*> que;
	addInorder(head, que);
	int count = 0;
	n = n + 1;
	Account* temp = head;
	while (!que.empty() && count != n)
	{
		temp = que.front();
		que.pop();
		count += 1;
	}

	if (que.empty() && count != n)
		return false;
	return deleteId(temp->getId(), head, head);
}

int main()
{
	int commandCount = 0;
	cin >> commandCount;
	string input = "";
	queue<string> printNamesQueue;
	string command = "";
	string name = "";
	string idString = "";
	int id = -1;
	int idCount = 0;
	string temp;
	getline(cin, temp);
	Account* head = nullptr;
	if (commandCount >= 0 && commandCount <= 200000)
	{
		for (unsigned int test = 0; test < commandCount; test++)
		{
			id = -1;
			getline(cin, input);
			stringstream ss(input);
			ss >> command;
			if (command == "insert")
			{
				int flag = 0;
				int i;
				name = "";
				idString = "";
				for (i = 0; i < input.length(); i++)
				{
					if (input[i] == '\"' && flag == 1)
					{
						break;
					}

					if (flag == 1)
					{
						name += input[i];
					}

					if (input[i] == '\"' && flag == 0)
					{
						flag = 1;
					}
				}

				for (i = i + 2; i < input.length(); i++)
				{
					idString += input[i];
				}

				if (idCount <= 100000 && !name.empty())
				{
					try
					{
						for (unsigned int i = 0; i < idString.length(); i++)
						{
							if (!isdigit(idString.at(i)))
							{
								i = idString.length();
								cout << "unsuccessful" << endl;
								continue;
							}
						}

						if (idString.length() != 8)
						{
							cout << "unsuccessful" << endl;
							continue;
						}

						id = stoi(idString);
						if (insertAccount(name, id, head))
						{
							cout << "successful" << endl;
							idCount++;
						}
						else
						{
							cout << "unsuccessful" << endl;
						}
					}

					catch (int e)
					{
						cout << "unsuccessful" << endl;
					}
				}
				else
				{
					cout << "unsuccessful" << endl;
				}
			}
			else if (command == "search")
			{
				ss >> name;
				if (!name.empty() && name.at(0) == '\"')
				{
					try
					{
						name.erase(remove(name.begin(), name.end(), '\"'), name.end());
						if (!searchName(name, head))
						{
							cout << "unsuccessful" << endl;
						}
						else {}
					}

					catch (int e)
					{
						cout << "unsuccessful" << endl;
					}
				}
				else if (!name.empty())
				{
					try
					{
						name.erase(remove(name.begin(), name.end(), '\"'), name.end());
						for (unsigned int i = 0; i < name.length(); i++)
						{
							if (!isdigit(name.at(i)))
							{
								i = name.length();
								throw (0);
							}
						}

						id = stoi(name);
						if (!searchID(id, head))
						{
							cout << "unsuccessful" << endl;
						}
						else {}
					}

					catch (int e)
					{
						cout << "unsuccessful" << endl;
					}
				}
			}
			else if (command == "printInorder")
			{
				printInOrder(head, printNamesQueue);
			}
			else if (command == "printPostorder")
			{
				printPostOrder(head, printNamesQueue);
			}
			else if (command == "printPreorder")
			{
				printPreOrder(head, printNamesQueue);
			}
			else if (command == "printLevelCount")
			{
				printLevelCount(head);
			}
			else if (command == "remove")
			{
				try
				{
					ss >> id;
					if (deleteId(id, head, head))
					{
						cout << "successful" << endl;
					}
					else
					{
						cout << "unsuccessful" << endl;
					}
				}

				catch (int e)
				{
					cout << "unsuccessful" << endl;
				}
			}
			else if (command == "removeInorder")
			{
				try
				{
					ss >> id;
					int level = -1;
					if (removeInorder(id, level, head))
					{
						cout << "successful" << endl;
					}
					else
					{
						cout << "unsuccessful" << endl;
					}
				}

				catch (int e)
				{
					cout << "unsuccessful" << endl;
				}
			}
		}
	}
	else
	{
		cout << "unsuccessful" << endl;
	}
}