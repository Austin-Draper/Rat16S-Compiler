#include<iostream>
#include<fstream>
#include <sstream>
#include"Lexer.h"
#include "syntaxparser.h"

using namespace std;

int main()
{

	ifstream original_file;
	original_file.open("text.txt");
	Lexer(original_file);
	original_file.close();
	//ifstream fin("Lexer.txt");
	//string temp;
	// Note! The '\n' at the end of each file line is NOT in the string in 'temp'.
	Parser parse(1);
	parse.Rat16S();


	system("pause");
}