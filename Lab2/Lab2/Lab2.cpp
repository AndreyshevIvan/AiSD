// В символьной строке записано выражение из букв и
// операций в постфиксной форме(знак операции после операндов).
// Проверить правильность записи и перевести выражение в
// инфиксную(обычную) форму со скобками. Например, выражение
// (a+b)*c-d*e записывается в постфиксной форме как ab+c*de*-.
// Требуется выдать его в виде(((a+b)*c)-(d*e)) (7).

// 1. Если очередной символ входной строки - число, то кладем его в стек. 
// 2. Если очередной символ - знак операции, то извлекаем из стека два верхних числа,
//    используем их в качестве операндов для этой операции, затем кладем результат обратно в стек.
// Когда вся входная строка будет разобрана в стеке должно остаться одно число, которое и будет результатом данного выражения.

#include "stdafx.h"
#include "stack.h"
#include <map>

using namespace std;

const int ARGUMENTS_COUNT = 2;

bool IsCharactersValid(string const& str, bool& wasError);
string Convert(string & rpnStr, bool& wasError);

int main(int argc, char* argv[])
{
	bool wasError = false;

	if (argc != ARGUMENTS_COUNT)
	{
		cout << "Invalid arguments count" "\n"
			<< "Usage: Lab2.exe <input string>" "\n";
		return 1;
	}

	setlocale(LC_ALL, "rus");
	string rpnStr = argv[1];
	if (rpnStr == "") 
	{
		wasError = true;
		cout << "ERROR: Введённая строка - пустая" "\n";
		return 1;
	}

	if (!IsCharactersValid(rpnStr, wasError))
	{
		cout << "ERROR: Запрещённый символ во входной строке" "\n";
		return 1;
	}

	string result = Convert(rpnStr, wasError);

	if (!wasError)
	{
		cout << "Выражение в постфиксной нотации: " << rpnStr << "\n"
			<< "Выражение в инфиксной нотации: " << result << "\n";
		return 0;
	}

	cout << result;
	return 1;
}

string Convert(string & rpnStr, bool& wasError)
{
	Stack<string> stack(rpnStr.length());
	char currSymbol;
	string resultStr;
	string firstOper;
	size_t i = 0;
	string secondOper; 

	while (i < rpnStr.length() && !wasError)
	{
		{
			currSymbol = rpnStr[i];

			if (isalpha((unsigned char)currSymbol))
			{
				resultStr = currSymbol;
				stack.push(resultStr);
			}
			else if (rpnStr[i] == '*' || rpnStr[i] == '/' || rpnStr[i] == '+' || rpnStr[i] == '-')
			{
				if (stack.getTop() >= 2)
				{
					secondOper = stack.pop();
					firstOper = stack.pop();
					resultStr = "(" + firstOper + currSymbol + secondOper + ")";
					stack.push(resultStr);
				}
				else
				{
					wasError = true;
					return "ERROR: В позиции " + to_string(i + 1) + " лишний оператор " + currSymbol + "\n";
				}
			}
			i++;
		}
	}
	if (stack.getTop() != 1)
	{
		wasError = true;
		return "ERROR: В выражении " + rpnStr + " недостаточно операторов" "\n";
	}

	return stack.pop();
}

bool IsCharactersValid(string const& str, bool& wasError)
{
	size_t i = 0;
	while (i < str.length() && !wasError)
	{
		if (!isalpha((unsigned char)str[i]))
			if (str[i] != '*' && str[i] != '/' && str[i] != '+' && str[i] != '-')
				wasError = true;
		i++;
	}

	return !wasError;
}