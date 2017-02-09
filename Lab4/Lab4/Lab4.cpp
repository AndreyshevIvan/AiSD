#include "stdafx.h"

const int ARGUMENTS_COUNT = 2;

using namespace std;

bool ReadGraph(vector<vector<int>>& matrix, ifstream& input);
vector<int> ReadNumbersFromStr(string const& str);
void PrintMatrix(vector<vector<int>> const& matrix);

bool dfs(int v);

vector<vector<int>> currMatrix;
vector<char> IsVisited;
int cycle_status;
int cycle_end;

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "rus");

	if (argc < ARGUMENTS_COUNT)
	{
		cout << "Аргументы введены не правильно!" "\n"
			<< "Используйте: Lab4.exe <input file>" "\n";
		return 1;
	}

	ifstream input(argv[1]);
	string countOfNodesStr;

	int countOfNodes;
	cout << "Укажите количество вершин: ";
	cin >> countOfNodes;
	cout << "\n";
	if (countOfNodes < 1)
	{
		cout << "Количество вершин не может быть меньше 1." "\n";
		return 1;
	}
	vector<vector<int>> matrix(countOfNodes, vector<int>(countOfNodes, 0));

	if (!ReadGraph(matrix, input))
	{
		cout << "Ошибка ввода матрицы. Используйте: <Вершина> <Путь> <Вес>!" "\n";
		return 1;
	}

	PrintMatrix(matrix);

	currMatrix = matrix;
	IsVisited.assign(countOfNodes, 0);
	cycle_status = -1;

	for (int i = 0; i < countOfNodes; i++)
	{
		if (dfs(i))
		{
			break;
		}
	}

	if (cycle_status == -1)
	{
		puts("Acyclic");
	}
	else 
	{
		puts("Cyclic");
	}

	return 0;
}

bool ReadGraph(vector<vector<int>>& matrix, ifstream& input)
{
	string str;
	vector<int> numbers;

	while (getline(input, str))
	{
		numbers = ReadNumbersFromStr(str);
		if (numbers[0] < 0 || numbers[0] > matrix.size())
		{
			return false;
		}
		if (numbers[1] < 0 || numbers[1] > matrix.size())
		{
			return false;
		}

		matrix[numbers[0] - 1][numbers[1] - 1] = numbers[2];
	}

	return true;
}

vector<int> ReadNumbersFromStr(string const& str)
{
	if (str.empty())
	{
		return std::vector<int>();
	}

	std::vector<int> numbers;

	std::string buffer;

	for (size_t i = 0; i < str.length(); ++i)
	{
		char ch = static_cast<char>(str[i]);

		if (isdigit(ch))
		{
			buffer += ch;
		}
		else if (!isspace(ch))
		{
			numbers.push_back(-1);
		}
		else if (!buffer.empty())
		{
			numbers.push_back(std::stoi(buffer));
			buffer.clear();
		}
	}

	if (!buffer.empty())
	{
		numbers.push_back(std::stoi(buffer));
	}

	return numbers;
}

void PrintMatrix(vector<vector<int>> const& matrix)
{
	for (size_t i = 0; i < matrix.size(); i++)
	{
		for (size_t j = 0; j < matrix[i].size(); j++)
		{
			cout << matrix[i][j] << " ";
		}
		cout << "\n";
	}
}

bool dfs(int currentNode)
{
	IsVisited[currentNode] = 1;

	for (size_t nextNode = 0; nextNode < currMatrix[currentNode].size(); nextNode++)
	{
		int nextNodeSize = currMatrix[currentNode][nextNode];

		if (nextNodeSize != 0 && IsVisited[nextNode] == 0)
		{
			if (nextNode != currentNode && dfs(nextNode))
			{
				return true;
			}
		}
		else if (nextNodeSize != 0 && IsVisited[nextNode] == 1)
		{
			cycle_end = currentNode;
			cycle_status = nextNode;
			return true;
		}
	}

	IsVisited[currentNode] = 2;
	return false;
}