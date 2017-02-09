#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <vector>

using namespace std;

const int ARGUMENTS_COUNT = 3;
const char BLANK = ' ';
const size_t MAX_ELEMENTS = 1000;
const std::vector<string> CHECKED_LETTERS = { "который", "которая",  "которое",  "которые" };

void FillingTheCollection(ifstream& inputFile, vector<string>& stringsCollection, int& elementsCount);
void WriteToFile(ofstream& outputFile, vector<string> const& collection, int const& maxNumber);

void AddToCollection(vector<string>& collection, int& stringNumber, string& addedStr);

bool IsLetter(string const& letter);
bool IsSearchLetter(string const& letter);

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "rus");

	if (argc != ARGUMENTS_COUNT)
	{
		cout << "Invalid arguments count" "\n"
			"Usage: program.exe <input file> <output file>" "\n";
		return 1;
	}

	ifstream inputFile(argv[1]);
	ofstream outputFile(argv[2]);

	vector<string> stringsCollection(MAX_ELEMENTS);
	int elementsCount = 0;

	FillingTheCollection(inputFile, stringsCollection, elementsCount);

	for (size_t i = 0; i < elementsCount; i++)
	{
		cout << stringsCollection[i] << "|" "\n";
	}

	WriteToFile(outputFile, stringsCollection, elementsCount);

	return 0;
}

void FillingTheCollection(ifstream& inputFile, vector<string>& stringsCollection, int& elementsCount)
{
	string readingStr;
	size_t charNumber = 0;
	string currentString = "";

	while (getline(inputFile, readingStr))
	{
		while (charNumber < (int)readingStr.size())
		{
			while (readingStr[charNumber] == BLANK)
			{
				charNumber++;
			}

			while (isalpha((unsigned char)readingStr[charNumber]))
			{
				currentString += (char)readingStr[charNumber];
				charNumber++;
			}

			AddToCollection(stringsCollection, elementsCount, currentString);

			if (charNumber < (int)readingStr.size() && readingStr[charNumber] != BLANK && !isalpha((unsigned char)readingStr[charNumber]))
			{
				currentString += (char)readingStr[charNumber];
				charNumber++;
				AddToCollection(stringsCollection, elementsCount, currentString);
			}
		}

		stringsCollection[elementsCount] = "\n";
		elementsCount++;
		charNumber = 0;
	}
}

void AddToCollection(vector<string>& collection, int& stringNumber, string& addedStr)
{
	if (addedStr != "")
	{
		collection[stringNumber] = addedStr;
		stringNumber++;
		addedStr = "";
	}
}

bool IsLetter(string const& letter)
{
	if (isalpha((unsigned char)letter[0]))
	{
		return true;
	}
	
	return false;
}

bool IsSearchLetter(string const& letter)
{
	for (size_t i = 0; i < CHECKED_LETTERS.size(); i++)
		if (letter == CHECKED_LETTERS[i])
		{
			return true;
		}

	return false;
}

void WriteToFile(ofstream& outputFile, vector<string> const& collection, int const& maxNumber)
{
	bool newSentence = true;
	bool afterSymbolLetter = true;
	int emptyStrCount = 0;

	for (int strNumber = 0; strNumber < maxNumber; strNumber++)
	{
		if (IsLetter(collection[strNumber]))
		{
			if (IsSearchLetter(collection[strNumber]) && !afterSymbolLetter && IsLetter(collection[strNumber - 1]))
				outputFile << ",";

			if (!newSentence)
				outputFile << BLANK;

			newSentence = false;
			afterSymbolLetter = false;
		}
		else if ((unsigned char)collection[strNumber][0] != BLANK && collection[strNumber] != "\n")
		{
			afterSymbolLetter = true;
			newSentence = collection[strNumber] == "." || collection[strNumber] == "!" || collection[strNumber] == "?";
		}

		while (collection[strNumber] == "\n")
		{
			emptyStrCount++;
			strNumber++;
		}
		if (IsSearchLetter(collection[strNumber]) && !afterSymbolLetter && emptyStrCount)
		{
			outputFile << ",";
		}
		while (emptyStrCount > 0)
		{
			outputFile << "\n";
			emptyStrCount--;
		}

		outputFile << collection[strNumber];
	}
}
