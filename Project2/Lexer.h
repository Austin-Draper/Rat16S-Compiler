#pragma once
#include<iostream>
#include <fstream>
#include <string>
#include<algorithm>

using namespace std;

//creates a new file without comments, comments are denoted by [* and *]
void remove_comments(ifstream &original_file);
//decides whether to call functions to check if a set of characters is a seperator, operator, unknown, real, digit, identifier, keyword, or unknown
void decide_action(char c, ifstream &istream, ofstream& outstream);
//checks if $ is followed by another $ and if : if followed by a =, also assigns the operator :=
void seperator(char c, ifstream &istream, ofstream& outstream);
//checks if operator is a multi-character operator and writes down operator to oustream
void op(char c, ifstream&istream, ofstream& outstream);
//main function. creates the streams and files and loops decide_action till the end of the file is reached
void Lexer(std::ifstream &original_file);
//declare a character as an unknown
void unknown(char c, ifstream&istream, ofstream& outstream);
//declare a string as an unknown
void unkown(string str, ifstream&istream, ofstream& outstream);
//determines if a set of characters is a digit or real. Calls upon the real_state_machine
void isRealDigit(char c, ifstream&istream, ofstream& outstream);
//tests if a set of characters is an ID and if there is an ID test if that ID is a keyword
void isID(char c, ifstream&istream, ofstream& outstream);
//called by the isID function
bool isKeyWord(string id);
short real_state_machine(short state, char c);
short id_state_machine(short state, char c);


void remove_comments(std::ifstream &original_file)
{
	ofstream no_c_stream;
	no_c_stream.open("No_Comments.txt");
	char c = original_file.get();
	//checks if file starts with a comment
	if (c == '[')
	{
		original_file.get(c);
		if (c == '*')
		{
			while (!original_file.eof())
			{
				original_file.get(c);
				if (c == '*')
				{
					original_file.get(c);
					if (c == ']')
					{
						original_file.get(c);
						break;
					}
				}
			}
		}
	}
	//checks if rest of file contains comments
	while (original_file)
	{
		no_c_stream << c;
		original_file.get(c);
		if (c == '[')
		{
			original_file.get(c);
			if (c == '*')
			{
				while (!original_file.eof())
				{
					original_file.get(c);
					if (c == '*')
					{
						original_file.get(c);
						if (c == ']')
						{
							original_file.get(c);
							break;
						}
					}
				}
			}
		}
	}
}

void decide_action(char c, ifstream&istream, ofstream& outstream)
{
	if (c == '$' || c== '(' || c== ')' || c== ',' || c==':' || c=='{' || c=='}' || c== ';')
		seperator(c,istream, outstream);
	else if (c == '=' || c == '/' || c == '>' || c == '<' || c == '+' || c == '-' || c == '*')
		op(c, istream, outstream);
	else if (c == '.' || isdigit(c))
		isRealDigit(c, istream, outstream);
	else if (isalpha(c))
		isID(c, istream, outstream);
	else if (isspace(c));
	else
		unknown(c, istream, outstream);
}

//checks if $ is followed by another $ and if : if followed by a =, also assigns the operator :=
void seperator(char c, ifstream &istream, ofstream& outstream)
{
	if (c == '$')
	{
		if (!istream.eof() && istream.get() == '$')
		{
			outstream << "Seperator $$\n";
		}
		else
		{
			unknown(c, istream, outstream);
			istream.unget();
		}
	}
	else if (c == ':')
	{
		if (istream.get() != '=')
		{
			outstream << "Seperator :\n";
			istream.unget();
		}
		else
		{
			outstream << "Operator :=\n";
		}
	}
	else
	{
		outstream << "Seperator " << c << endl;
	}
}

//checks if operator is a multi-character operator and writes down operator to oustream
void op(char c, ifstream&istream, ofstream& outstream)
{
	if (c == '=')
	{
		if (istream.get() == '>')
		{
			outstream << "Operator =>\n";
		}
		else
		{
			outstream << "Operator =\n";
			istream.unget();
		}
	}
	else if (c == '/')
	{
		if (istream.get() == '=')
		{
			outstream << "Operator /=\n";
		}
		else
		{
			outstream << "Operator /\n";
			istream.unget();
		}
	}
	else if (c == '<')
	{
		if (istream.get() == '=')
		{
			outstream << "Operator <=\n";
		}
		else
		{
			outstream << "Operator <\n";
			istream.unget();
		}
	}
	else
	{
		outstream << "Operator " << c << endl;
	}
}

//determines if a set of characters is a digit or real. Calls upon the real_state_machine
//if we call upon this function we shouldn't stay in the 0 state of real_state_machine
void isRealDigit(char c, ifstream&istream, ofstream& outstream)
{
	short state = 0;
	string realDigit; 
	state = real_state_machine(state,c);
	realDigit += c;
	//loops infinately until we reach a break. A break occurs at the end of file, istream encounters a character that isn't a digit or . or the state machine returns 4
	while (1)
	{
		istream.get(c);
		if (istream.eof())
			break;
		if (c == '.' || isdigit(c) || isalpha(c) || c == '_')
		{
			if (isspace(c))
			{
				istream.unget();
				break;
			}
			realDigit += c;
			state = real_state_machine(state, c);
			if (state == 4)
				break;
		}
		else
		{
			istream.unget();
			break;
		}
	}
	if (state == 1)
		outstream << "Integer " << realDigit << endl;
	else if (state == 3)
		outstream << "Real " << realDigit << endl;
	else if (state == 4 || state == 2 || state == 0)
		outstream << "Unknown " << realDigit << endl;


}

//returns the state the machine is in, if 4 is returned we're in the empty state and an unknown was found, if 5 was returned an error occured.
//state 1 and 3 are accepting states. If we end at 1 a digit was found. If we end at 3 a real was found.
//states can be between 0 and 4
short real_state_machine(short state, char c)
{
	short finite_SMachine[5][2] = { { 1,2 },{ 1, 2 },{ 3,4 },{ 3,4 },{ 4,4 } };
	if (isdigit(c))
	{
		return finite_SMachine[state][0];
	}
	else if (c == '.')
	{
		return finite_SMachine[state][1];
	}
	else
		return 4;
}

//tests if a set of characters is an ID and if there is an ID test if that ID is a keyword
//should pass state 0 immediatly as we only call this function if c is a letter
void isID(char c, ifstream&istream, ofstream& outstream)
{
	short state = 0;
	string id;
	state = id_state_machine(state, c);
	id += c;
	//loops infinately until we reach a break. A break occurs at the end of file, istream encounters a character that isn't a digit or . or the state machine returns 4
	while (1)
	{
		istream.get(c);
		if (istream.eof())
			break;
		if(c == '.' || isdigit(c) || isalpha(c) || c == '_')
		{
			if (isspace(c))
			{
				istream.unget();
				break;
			}

			id += c;
			state = id_state_machine(state, c);
		}
		else
		{
			istream.unget();
			break;
		}
	}
	if (state == 1 || state == 2)
	{
		if (isKeyWord(id))
			outstream << "Keyword " << id << endl;
		else
			outstream << "Identifier " << id << endl;
	}
	else if (state == 0 || state == 3 || state == 4)
		outstream << "Unknown " << id << endl;

}

//state 1 and 2 are accepting states
short id_state_machine(short state, char c)
{
	short finite_SMachine[5][2] = { { 1,4 },{ 2, 3 },{ 2,3 },{ 2,3 },{ 4,4 } };
	if (isalpha(c))
	{
		return finite_SMachine[state][0];
	}
	else if (c == '_')
	{
		return finite_SMachine[state][1];
	}
	else
		return 4;
}

//returns true if an ID is a keyword, or false if it is not
bool isKeyWord(string id)
{
	transform(id.begin(), id.end(), id.begin(), ::tolower);
	//"function" is a character, so it needs to be set as string first, then compared with the string id
	string func = string("function");
	//id and temp being equal to 0 means they are both equal strings
	if (id.compare(func) == 0)
		return true;
	string integ = string("integer");
	if (id.compare(integ) == 0)
		return true;
	string boo = string("boolean");
	if (id.compare(boo) == 0)
		return true;
	string real = string("real");
	if (id.compare(real) == 0)
		return true;
	string if_ = string("if");
	if (id.compare(if_) == 0)
		return true;
	string endif_ = string("endif");
	if (id.compare(endif_) == 0)
		return true;
	string els = string("else");
	if (id.compare(els) == 0)
		return true;
	string retu = string("return");
	if (id.compare(retu) == 0)
		return true;
	string prt = string("printf");
	if (id.compare(prt) == 0)
		return true;
	string scn = string("scanf");
	if (id.compare(scn) == 0)
		return true;
	string whl = string("while");
	if (id.compare(whl) == 0)
		return true;
	string tru = string("true");
	if (id.compare(tru) == 0)
		return true;
	string fals = string("false");
	if (id.compare(fals) == 0)
		return true;
	else
		return false;
}

//declare a character as an unknown
void unknown(char c, ifstream&istream, ofstream& outstream)
{
	outstream << "Unknown " << c << endl;
}

//declare a string as an unknown
void unkown(string str, ifstream&istream, ofstream& outstream)
{
	outstream << "Unknown " << str << endl;
}

void Lexer(std::ifstream &original_file)
{
	remove_comments(original_file);
	original_file.close();
	ifstream istream;
	istream.open("No_Comments.txt");
	char c = istream.get();
	ofstream outstream;
	outstream.open("Lexer.txt");
	while (istream)
	{
		decide_action(c, istream, outstream);
		istream.get(c);
	}
}
