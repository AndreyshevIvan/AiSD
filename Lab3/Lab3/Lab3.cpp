#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

const int ARGUMENTS_COUNT = 2;

struct Tree
{
	string key;
	Tree* left;
	Tree* right;
	Tree* backTag = nullptr;
	Tree* father;
	int level = 0;
	bool isTag = false;
};

struct Stack
{
	Tree* top;
	Stack* next;
};

void AddToStack(Stack** stack, Tree* vertex);
void DeleteTop(Stack** stack);

void ReadFromFile(ifstream & inputFile, Tree** tree);
string ReadKey(string const& line, size_t const& startPos);
Tree* ReadVertex(string &line);

void PrintTree(Tree* tree);
void PrintVertexInfo(Tree* tree);

Tree* ThreadingTree(Tree* tree);
Tree* SearchLastLeaf(Tree* tree);
Tree* SearchTree(Tree* tree, string const& searchKey);
void DeleteSubTree(Tree* tree, Tree* subTree);
void DeleteTree(Tree* tree);

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "rus");

	if (argc != ARGUMENTS_COUNT)
		return 1;

	ifstream inputFile;
	inputFile.open(argv[1]);
	if (!inputFile.is_open())
	{
		std::cout << "Невозможно открыть файл для чтения" << endl;
		return 1;
	}

	Tree* root = new Tree;

	ReadFromFile(inputFile, &root);
	ThreadingTree(root);
	cout << "Прошитое дерево:" "\n";
	PrintTree(root);

	cout << "Введите ключ удаляемой вершины или команду завершения: ";
	string searchKey;
	getline(cin, searchKey);

	while (searchKey != "stop")
	{
		cout << "Ключ удаляемой вершины: " << searchKey << "\n";

		Tree* searchTree = SearchTree(root, searchKey);
		if (!searchTree)
		{
			cout << "В дереве не содержится такой элемент";
			return 1;
		}

		cout << "Дерево после удаления вершины:" "\n";
		DeleteSubTree(root, searchTree);

		if (root->key == "")
		{
			cout << "Корень дерева был удалён" << "\n";
			return 0;
		}
		else
		{
			PrintTree(root);
		}

		cout << "Введите ключ удаляемой вершины или команду завершения: ";
		getline(cin, searchKey);
	}

	if (searchKey == "stop")
	{
		cout << "Работа программы была завершена. Досвидания." "\n";
	}

	return 0;
}

void AddToStack(Stack** stack, Tree* vertex)
{
	Stack* newTop = new Stack;
	newTop->next = *stack;
	newTop->top = vertex;
	*stack = newTop;
}

void DeleteTop(Stack** stack)
{
	Stack* oldTop;
	oldTop = *stack;
	*stack = oldTop->next;
	delete oldTop;
}

string ReadKey(string const& line, size_t const& startPos)
{
	string name = "";
	size_t pos = startPos;

	while ((line[pos] != '\0') && (line[pos] != ' '))
	{
		name += line[pos];
		pos++;
	}

	return name;
}

Tree* ReadVertex(string &line)
{
	if (line.empty())
	{
		return nullptr;
	}

	size_t level = 0;
	while (line[level] == '.') level++;

	Tree *newVertex = new Tree;
	size_t readPos = level;
	newVertex->key = ReadKey(line, readPos);
	line.clear();

	newVertex->level = level;
	newVertex->left = nullptr;
	newVertex->right = nullptr;

	return newVertex;
}

void ReadFromFile(ifstream & inputFile, Tree** tree)
{
	Stack* stack;
	Tree* currTree;
	Tree* prevTree;
	string currentLine;

	currTree = prevTree = nullptr;

	stack = new Stack;
	stack->next = nullptr;

	while (getline(inputFile, currentLine))
	{
		currTree = ReadVertex(currentLine);

		if (currTree->level == 0)
		{
			currTree->father = nullptr;
			*tree = currTree;
			prevTree = *tree;
			AddToStack(&stack, prevTree);
			continue;
		}
		if (currTree->level > prevTree->level)
		{
			currTree->father = prevTree;
			if (currTree->key != "&")
			{
				stack->top->left = currTree;
			}
		}
		else if (currTree->level == prevTree->level)
		{
			DeleteTop(&stack);
			currTree->father = prevTree->father;
			stack->top->right = currTree;
		}
		else
		{
			while (stack->top->level >= currTree->level)
			{
				DeleteTop(&stack);
			}

			stack->top->right = currTree;
			currTree->father = stack->top;
		}
		prevTree = currTree;
		AddToStack(&stack, prevTree);
	}
}

void PrintTree(Tree* tree)
{
	Tree *currTree = tree;

	if (currTree)
	{
		PrintVertexInfo(currTree);
		if (!currTree->left)
		{
			currTree = currTree->right;
		}
		else
		{
			currTree = currTree->left;
		}
		PrintTree(currTree);
	}
}

void PrintVertexInfo(Tree* tree)
{
	string levelStr = "";
	for (size_t i = 0; i < tree->level; i++)
	{
		levelStr += '.';
	}

	cout << levelStr << tree->key;

	if (tree->isTag && tree->right)
	{
		cout << " (" + tree->right->key << ')';
	}

	cout << "\n";
}

Tree* ThreadingTree(Tree* tree)
{
	Tree* topTree = new Tree;
	topTree->left = tree;
	topTree->level = -1;

	Stack* stack = new Stack;
	stack->top = topTree;
	stack->next = nullptr;

	Tree* currTree = tree;

	while (currTree->level != -1)
	{
		if (currTree->left != nullptr)
		{
			if (currTree->right != nullptr)
			{
				AddToStack(&stack, currTree->right);
			}
			currTree = currTree->left;
		}
		else if (currTree->right != nullptr)
		{
			currTree = currTree->right;
		}
		else
		{
			currTree->isTag = true;
			currTree->right = stack->top;
			stack->top->backTag = currTree;
			currTree = stack->top;
			DeleteTop(&stack);
		}
	}

	SearchLastLeaf(tree)->isTag = false;
	SearchLastLeaf(tree)->left = nullptr;
	SearchLastLeaf(tree)->right = nullptr;

	return tree;
}

Tree* SearchLastLeaf(Tree* tree)
{
	Tree *current = tree;

	while (current)
	{
		while (current->left)
		{
			current = current->left;
		}
		if (current->right)
		{
			if (current->right->level <= tree->level)
			{
				return current;
			}
		}
		if (current->right == nullptr)
		{
			return current;
		}
		current = current->right;

	}
	return current;
}

Tree* SearchTree(Tree* tree, string const& searchKey)
{
	Tree* current = tree;

	while ((current != nullptr) && (current->level >= tree->level) && (current->key != searchKey))
	{
		while (current->left)
		{
			current = current->left;
			if (current->key == searchKey)
			{
				return current;
			}
		}
		current = current->right;
	}

	if (current != nullptr)
	{
		if (current->level < 0)
		{
			return nullptr;
		}
	}

	return current;
}

void DeleteSubTree(Tree* tree, Tree* subTree)
{
	Tree* current = subTree;

	if (tree == subTree)
	{
		tree->key = "";
		DeleteTree(subTree->left);
		DeleteTree(subTree->right);
	}
	else
	{
		while (current->right && !current->isTag)
		{
			current = current->right;
		}

		if (subTree->backTag)
		{
			subTree->backTag->right = current->right;
			subTree->backTag->isTag = current->isTag;

			if (subTree->father->left == subTree)
			{
				subTree->father->left = nullptr;
			}
			else if (subTree->father->right == subTree)
			{
				subTree->father->right = nullptr;
			}
		}
		else
		{
			if (subTree->father->left == subTree)
			{
				subTree->father->left = nullptr;
			}
			else if (subTree->father->right == subTree)
			{
				subTree->father->right = nullptr;
			}

			if (subTree->father->right == nullptr)
			{
				subTree->father->right = current->right;
				subTree->father->isTag = subTree->isTag;
			}
		}
		DeleteTree(subTree);
	}
}

void DeleteTree(Tree* tree)
{
	if (tree != nullptr && !tree->isTag)
	{
		DeleteTree(tree->left);
		DeleteTree(tree->right);
		delete(tree);
	}
}