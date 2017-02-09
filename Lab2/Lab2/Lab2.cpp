// � ���������� ������ �������� ��������� �� ���� �
// �������� � ����������� �����(���� �������� ����� ���������).
// ��������� ������������ ������ � ��������� ��������� �
// ���������(�������) ����� �� ��������. ��������, ���������
// (a+b)*c-d*e ������������ � ����������� ����� ��� ab+c*de*-.
// ��������� ������ ��� � ����(((a+b)*c)-(d*e)) (7).

// 1. ���� ��������� ������ ������� ������ - �����, �� ������ ��� � ����. 
// 2. ���� ��������� ������ - ���� ��������, �� ��������� �� ����� ��� ������� �����,
//    ���������� �� � �������� ��������� ��� ���� ��������, ����� ������ ��������� ������� � ����.
// ����� ��� ������� ������ ����� ��������� � ����� ������ �������� ���� �����, ������� � ����� ����������� ������� ���������.

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
		cout << "ERROR: �������� ������ - ������" "\n";
		return 1;
	}

	if (!IsCharactersValid(rpnStr, wasError))
	{
		cout << "ERROR: ����������� ������ �� ������� ������" "\n";
		return 1;
	}

	string result = Convert(rpnStr, wasError);

	if (!wasError)
	{
		cout << "��������� � ����������� �������: " << rpnStr << "\n"
			<< "��������� � ��������� �������: " << result << "\n";
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
					return "ERROR: � ������� " + to_string(i + 1) + " ������ �������� " + currSymbol + "\n";
				}
			}
			i++;
		}
	}
	if (stack.getTop() != 1)
	{
		wasError = true;
		return "ERROR: � ��������� " + rpnStr + " ������������ ����������" "\n";
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