#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include "Lexer.h"
#include "SymbolTable.h"
#include "stack"
#include "Instruction_Table.h"

using namespace std;



//syntaxparser(string lexfilename, string productionfile, string assemblyfile);

//Pass in the token and the lexeme to be used for all these functions.
class Parser {
public:
	void Rat16S(); //will start the parser and call upon all the private functions
	Parser();	//default to using "Lexer.h" and asking user if they wish grammar to be shown
	Parser(string);	//string is the name of the file the parser should read. Defaults to asking the user if they wish grammar to be shown
	Parser(int); //For the integer enter 1 so compiler displays grammar in the consule, 0 so it does not. Defaults to file "Lexer.h" as the input file
	Parser(int debug, string s); //For the integer enter 1 so compiler displays grammar in the consule, 0 so it does not. string is the name of the input file
	~Parser(); //closes input and output files
private:
	bool OptFunctionDefinitions();
	bool OptDeclarationList();
	bool StatementList();
	bool FunctionDefinitions();
	bool DeclarationList();
	bool Declaration();
	bool Statement();
	bool Function();
	bool OptParameterList();
	bool ParameterList();
	bool Parameter();
	bool Body(string);
	bool Qualifier();
	bool IDs();
	bool Compound();
	bool Assign();
	bool If();
	bool Return();
	bool Write();
	bool Read();
	bool While();
	bool Condition();
	bool Relop();
	bool Factor();
	bool Primary();
	bool Expression();
	bool ExpressionPrime();
	bool Term();
	bool TermPrime();
	bool Empty();	//returns true if file is empty
	void load_tokens(); //reads the next token and lexeme in the input file
	void show_grammar(); //optional function that makes how Parser is declared redundant.
	void print();


	//private variables
	int _debug,id_location=0;
	string token, lexeme, _type, identifier[50],assembly_token;
	ifstream fin;
	ofstream fout;
	SymbolTable _table;
	Instruction_Table _instr_table;
};

Parser::Parser()
{
	fin.open("Lexer.txt");
	fout.open("Parsedfile.txt");
	show_grammar();
}

Parser::Parser(string s)
{
	fin.open(s);
	fout.open("Parsedfile.txt");
	show_grammar();
}

//For the integer enter 1 so compiler displays grammar in the consule, 0 so it does not. Defaults to file "Lexer.h" as the input file
Parser::Parser(int debug)
{
	fin.open("Lexer.txt");
	fout.open("Parsedfile.txt");
	_debug = debug;
}

//For the integer enter 1 so compiler displays grammar in the consule, 0 so it does not. string is the name of the input file
Parser::Parser(int debug, string s)
{
	fin.open(s);
	fout.open("Parsedfile.txt");
	_debug = debug;
}

Parser::~Parser()
{
	fin.close();
	fout.close();
}
//optional ask the user if they wish to show_grammer or not
void Parser::show_grammar()
{
	bool check = true;
	char choice;
	while (check)
	{
		cout << "Would you like to display the production rules? (y/n) : ";
		cin >> choice;
		switch (choice)
		{
		case 'Y':
			_debug = 1;
			check = false;
			break;
		case 'y':
			_debug = 1;
			check = false;
			break;
		case 'N':
			_debug = 0;
			check = false;
			break;
		case 'n':
			_debug = 0;
			check = false;
			break;
		default:
			cout << "Please type a valid answer (y/n): " << endl;
			break;
		}
	}
}
// Purpose of print method: prints the current token and lexeme values
void Parser::print()
{
	fout << endl << left << "Token: " << setw(15) << token << "Lexeme: " << setw(10) << lexeme << endl;
	if (_debug != 0)
		cout << endl << left << "Token: " << setw(15) << token << "Lexeme: " << setw(10) << lexeme << endl;
}

//loads the next token from the input file into the parser
void Parser::load_tokens()
{
	string word;
	fin >> word;
	//cout << word << '\n';
	token = word;
	fin >> word;
	//cout << word << endl;
	lexeme = word;
	if (token == "Unknown")
	{
		cout << "Unknown token found. Lexeme: " << lexeme << endl;
		system("pause");
		exit(1);
	}
}

//calls OptFunctionDefinitions(), OptDeclarationList(), and StatementList(), also checks for $$
void Parser::Rat16S()
{
	OptFunctionDefinitions();
	//print();
	if (lexeme == "$$")
	{
		//load_tokens();
		OptDeclarationList();
		print();
		if (lexeme == "$$")
		{
			load_tokens();
			StatementList();
			if (lexeme == "$$")
			{
				cout << "Parser Was Successful!\n";

			}
			else {
				load_tokens();
				print();
				if (lexeme == "$$")
				{
					cout << "Parser Was Successful!" << endl;
				}
				else if (lexeme != "$$")
				{
					cout << "Missing Finishing $$" << endl;
					system("pause");
					exit(1);
				}
			}
		}
		else if (lexeme != "$$")
		{
			cout << "Missing $$ after Function definitions" << endl;
			system("pause");
			exit(1);
		}
	}
	else if (lexeme != "$$")
	{
		cout << "Missing $$" << endl;
		system("pause");
		exit(1);
	}
	_table.print_table();
	_instr_table.write();
}

//checks if optional functions were declared will call either function or function and itself
bool Parser::OptFunctionDefinitions()
{
	bool _OptFunctionDefinitions = false;
	load_tokens();
	print();
	//check if next lexeme is a function
	if (lexeme == "function")
	{
		if (_debug)
		{
			cout << "<Opt Function Definitions> ::= <FunctionDefinitions>" << endl;
		}
		fout << "<Opt Function Definitions> ::= <FunctionDefinitions>" << endl;
		_OptFunctionDefinitions = true;
		FunctionDefinitions();
	}
	else
	{ //if no function, then it's empty
		if (_debug)
		{
			cout << "<Opt Function Definitions> ::= <Empty>" << endl;
		}
		fout << "<Opt Function Definitions> ::= <Empty>" << endl;
		Empty();
		_OptFunctionDefinitions = true;
	}
	return _OptFunctionDefinitions;
}

//check if any optinal declarations were made
bool Parser::OptDeclarationList()
{
	bool _OptDeclarationList = false;
	print();
	load_tokens();
	//check if next lexeme is qualifier
	if (lexeme == "Integer" || lexeme == "integer" || lexeme == "Boolean" || lexeme == "boolean" || lexeme == "Real" || lexeme == "real")
	{
		//print();//remove
		if (_debug)
		{
			cout << endl << "<OptDeclarationList> ::= <DeclarationList>" << endl;
		}
		fout << endl << "<OptDeclarationList> ::= <DeclarationList>" << endl;
		_OptDeclarationList = true;
		DeclarationList();
	}
	else
	{
		if (_debug)
		{
			cout << "<OptDeclarationList> ::= <Empty>" << endl;
		}
		fout << "<OptDeclarationList> ::= <Empty>" << endl;
		_OptDeclarationList = true;
		Empty();
	}
	return _OptDeclarationList;
}

//begins check to see if we are looking at a statement
bool Parser::StatementList()
{
	bool _StatementList = false;
	if (_debug)
	{
		cout << "<Statement List> ::= <Statement> <Statement List>" << endl;
	}
	fout << "<Statement List> ::= <Statement> <Statement List>" << endl;
	if (Statement())
	{
		_StatementList = true;
	}
	else
	{
		cout << "Missing: Statement" << endl;
		system("pause");
		exit(1);
	}
	if (token == "Keyword" || token == "keyword" || token == "Identifier" || token == "identifier")
	{
		_StatementList = true;
		StatementList();
	}
	return _StatementList;
}

//begins check to see if we are looking at a function
bool Parser::FunctionDefinitions()
{
	bool _FunctionDefinitions = false;
	if (_debug)
	{
		cout << "<Function Definitions> ::= <Function><Function Definitions>" << endl;
	}
	fout << "<Function Definitions> ::= <Function><Function Definitions>" << endl;
	if (Function())
	{
		if (FunctionDefinitions())
		{
			_FunctionDefinitions = true;
		}
		_FunctionDefinitions = true;
		if (_debug)
		{
			cout << "<Function Definitions> ::= <Function>" << endl;
		}
		fout << "<Function Definitions> ::= <Function>" << endl;
	}
	return _FunctionDefinitions;
}
/*for DeclarationList, can be <Declaration>;<DeclarationList> OR just <Declaration>
Right now it's only set to do the first one above. Rework it to do either*/
bool Parser::DeclarationList()
{
	bool _DeclarationList = false;
	if (_debug)
	{
		cout << endl << "<DeclarationList> ::= <Declaration>;<DeclarationList>" << endl;
	}
	fout << endl << "<DeclarationList> ::= <Declaration>;<DeclarationList>" << endl;
	Declaration();
	//checks if next lexeme is a qualifier
	if (lexeme == ";")
	{
		print();
		load_tokens();
		if (lexeme == "integer" || lexeme == "Integer" || lexeme == "boolean" || lexeme == "Boolean" || lexeme == "real" || lexeme == "Real")
		{
			load_tokens();
			//function call for rest of declarations
			DeclarationList();
		}
	}
	else
	{
		cout << "Missing: ;" << endl;
		system("pause");
		exit(1);
	}
	_DeclarationList = true;
	return _DeclarationList;
}

//call qualifier to and id to check if a declaration
bool Parser::Declaration()
{
	bool _Declaration = false;
	if (_debug)
	{
		cout << "<Declaration> ::= <Qualifier> <IDs>" << endl;
	}
	fout << "<Declaration> ::= <Qualifier> <IDs>" << endl;
	Qualifier();
	load_tokens();
	IDs();
	for (int i = 0; i < id_location; i++)
	{
		_table.add(identifier[i], _type);
	}
	id_location = 0;
	_Declaration = true;
	return _Declaration;
}

//checks if a statement calls compound, assign if an ID, if, return, write, read, or while
bool Parser::Statement()
{
	bool _Statement = false;
	if (lexeme == "{")
	{
		_Statement = true;
		if (_debug)
		{
			cout << "<Statement> ::= <Compound>" << endl;
		}
		fout << "<Statement> ::= <Compound>" << endl;
		Compound();
	}
	else if (token == "Identifier")
	{
		_Statement = true;
		if (_debug)
		{
			cout << "<Statement> ::= <Assign>" << endl;
		}
		fout << "<Statement> ::= <Assign>" << endl;
		Assign();
	}
	else if (lexeme == "if" || lexeme == "If" || lexeme == "IF")
	{
		_Statement = true;
		if (_debug)
		{
			cout << "<Statement> ::= <If>" << endl;
		}
		fout << "<Statement> ::= <If>" << endl;
		If();
	}
	else if (lexeme == "return")
	{
		_Statement = true;
		if (_debug)
		{
			cout << "<Statement> ::= <Return>" << endl;
		}
		fout << "<Statement> ::= <Return>" << endl;
		Return();
	}
	else if (lexeme == "printf" || lexeme == "Printf")
	{
		_Statement = true;
		if (_debug)
		{
			cout << "<Statement> ::= <Write>" << endl;
		}
		fout << "<Statement> ::= <Write>" << endl;
		Write();
	}
	else if (lexeme == "scanf")
	{
		_Statement = true;
		if (_debug)
		{
			cout << "<Statement> ::= <Read>" << endl;
		}
		fout << "<Statement> ::= <Read>" << endl;
		Read();
	}
	else if (lexeme == "while")
	{
		_Statement = true;
		if (_debug)
		{
			cout << "<Statement> ::= <While>" << endl;
		}
		fout << "<Statement> ::= <While>" << endl;
		While();
	}
	return _Statement;
}

//check if a function checks if we begin witht he string "function" and then looks for an identifer, followed by an optional parameter list, optional declaratin list, and finally a body
bool Parser::Function()
{
	bool _Function = false;
	if (_debug)
	{
		cout << "<Function> ::= function <identifier> ( <Opt Parameter List> ) <Opt Declaration List> <Body>" << endl;
	}
	fout << "<Function> ::= function <identifier> ( <Opt Parameter List> ) <Opt Declaration List> <Body>" << endl;
	if (lexeme == "function")
	{
		load_tokens();
		print();
		if (token == "Identifier" || token == "identifier")
		{
			load_tokens();
			print();
			if (lexeme == "(")
			{
				load_tokens();
				if (OptParameterList())
				{
					//load_tokens();
					if (lexeme == ")")
					{
						print();
						load_tokens();
						string temp;
						temp = lexeme;
						if (OptDeclarationList())
						{
							if (Body(temp))
							{
								_Function = true;
							}
						}
					}
					else
					{
						cout << "Missing: )" << endl;
						system("pause");
						exit(1);
					}
				}
			}
			else
			{
				cout << "Missing: (" << endl;
				system("pause");
				exit(1);
			}
		}
		else
		{
			cout << "Missing: identifier" << endl;
			system("pause");
			exit(1);
		}
	}
	return _Function;
}
//will call parameter or parameter and itself
bool Parser::OptParameterList()
{
	bool _OptParameterList = false;
	//check to see if the optional parameter list is empty
	if (lexeme != ")")
	{
		if (_debug)
		{
			cout << "<Opt Parameter List> ::= <Parameter List>" << endl;
		}
		fout << "<Opt Parameter List> ::= <Parameter List>" << endl;
		ParameterList();
		_OptParameterList = true;
	}
	else if (lexeme == ")")
	{
		if (_debug)
		{
			cout << "<Opt Parameter List> ::= <empty>" << endl;
		}
		fout << "<Opt Parameter List> ::= <empty>" << endl;
		Empty();
		_OptParameterList = true;
	}
	return _OptParameterList;
}
bool Parser::ParameterList()
{
	bool _ParameterList = false;
	if (_debug)
	{
		cout << "<Parameter List> ::= <Parameter> , <Parameter List>" << endl;
	}
	fout << "<Parameter List> ::= <Parameter> , <Parameter List>" << endl;
	Parameter();
	load_tokens();
	if (lexeme == ",")
	{
		load_tokens();
		print();
		//if we have , then recursively call for more parameters
		ParameterList();
	}
	
	/*if (lexeme == ":")
	{
		load_tokens();
		print();
		//if we have colon, then recursively call for more parameters
		ParameterList();
	}*/
	_ParameterList = true;
	return _ParameterList;
}
//checks if there is an ID followed by a : followed by a qualifier
bool Parser::Parameter()
{
	bool _Parameter = false;
	if (_debug)
	{
		cout << "<Parameter> ::= <IDS> : <Qualifier>" << endl;
	}
	fout << "<Parameter> ::= <IDS> : <Qualifier>" << endl;
	IDs();
	
	if (lexeme == ":")
	{
		print();
		load_tokens();
		Qualifier();
		for (int i = 0; i <  id_location; i++)
		{
			_table.add(identifier[i], _type);
		}
		id_location = 0;
		_Parameter = true;
	}
	else
	{
		cout << "Missing: :" << endl;
		system("pause");
		exit(1);
	}
	return _Parameter;
}
//checks if there is a statement list
bool Parser::Body(string temp)
{
	bool _body = false;
	if (temp == "{" || lexeme == "{")
	{
		print();
		if (_debug)
		{
			cout << endl << "<Body> ::= { <Statement List> }" << endl;
		}
		fout << endl << "<Body> ::= { <Statement List> }" << endl;
		StatementList();
		//print();
		if (lexeme == "}")
		{
			print();
			load_tokens();
			_body = true;
		}
		else
		{
			cout << "Missing: }" << endl;
			cout << "got here" << endl;
			system("pause");
			exit(1);
		}
	}
	else
	{
		cout << "Missing: {" << endl;
		system("pause");
		exit(1);
	}
	return _body;
}
//a qualifier is an integer, boolean, or real
bool Parser::Qualifier()
{
	bool _Qualifier = false;
	if (_debug)
	{
		cout << "<Qualifier> ::= " << lexeme << endl;
	}
	fout << endl << "<Qualifier> ::= " << lexeme << endl << endl;
	if (lexeme == "Integer" || lexeme == "integer" || lexeme == "Boolean" || lexeme == "boolean" || lexeme == "Real" || lexeme == "real")
	{
		_type = lexeme;
		print();
		_Qualifier = true;
	}
	else
	{
		cout << "Error! Expected Integer, Boolean, or Real." << endl;
		system("pause");
		exit(1);
	}
	return _Qualifier;
}

//identifier calls  or identifier + itself
bool Parser::IDs()
{
	bool _IDs = false;
	if (token == "Identifier" || token == "identifier")
	{
		identifier[id_location++] = lexeme;
		print();
		int _memory = _table.get_memory(lexeme, _type);
		_instr_table.gen_instr("PUSHM", _memory);
		load_tokens();
		if (_debug)
		{
			cout << "<IDs> ::= <Identifier>" << endl;
		}
		fout << "<IDs> ::= <Identifier>" << endl;
		if (lexeme == ",")
		{
			if (_debug)
			{
				cout << "<IDs> ::= <Identifier>, <IDs>" << endl;
			}
			fout << "<IDs> ::= <Identifier>, <IDs>" << endl;
			print();//print the comma
			load_tokens();//get the next token to see if ID
			IDs();
			_IDs = true;
		}
		_IDs = true;
	}
	else
	{
		cout << "Missing Identifier" << endl;
		system("pause");
		exit(1);
	}
	return _IDs;
}

//calls statement lsit
bool Parser::Compound()
{
	bool _Compound = false;
	if (lexeme == "{")
	{
		load_tokens();
		print();
		if (_debug)
		{
			cout << "<compound> ::= { <Statement List> }" << endl;
		}
		fout << "<compound> ::= { <Statement List> }" << endl;
		StatementList();
		if (lexeme == "}")
		{
			print();
			cout << endl;
			_Compound = true;
			//load_tokens(); this caused one too many tokens at the end of body.
		}
		else
		{
			cout << "Missing: }" << endl;
			system("pause");
			exit(1);
		}
	}
	return _Compound;
}
//calls ID and then checks for a :=, then calls expression, and then finally checks if expression is followed by a ;
bool Parser::Assign()
{
	bool _Assign = false;
	if (token == "Identifier")
	{
		string save_type = _type;
		string save_lexeme = lexeme;
		print();
		cout << endl;
		load_tokens();
		if (_debug)
		{
			cout << "<Assign> ::= <Identifier> := <Expression>;" << endl;
		}
		fout << "<Assign> ::= <Identifier> := <Expression>;" << endl;
		if (lexeme == ":=")
		{
			print();
			cout << endl;
			load_tokens();
			Expression();
			int _memory = _table.get_memory(save_lexeme, save_type);
			_instr_table.gen_instr("POPM", _memory);
			if (lexeme == ";")
			{
				print();
				cout << endl;
				load_tokens();
				_Assign = true;
			}
			else
			{
				cout << "Missing: ;" << endl;
				system("pause");
				exit(1);
			}
		}
	}
	return _Assign;
}
//checks if there is an "if (", followed by a condition, ")", calls statement, followed by an end if
//or //checks if there is an "if (", followed by a condition, ")", calls statement, an "else, calls statement, followed by an end if
bool Parser::If()
{
	bool _If = false;
	if (_debug)
	{
		cout << "<If> ::= if ( <Condition> ) <Statement> endif" << endl;
	}
	fout << "<If> ::= if ( <Condition> ) <Statement> endif" << endl;
	if (lexeme == "if")
	{
		print();
		int _memory = _instr_table.current_address();
		cout << endl;
		load_tokens();
		if (lexeme == "(")
		{
			print();
			cout << endl;
			load_tokens();
			Condition();
			if (lexeme == ")")
			{
				print();
				cout << endl;
				load_tokens();
				print();
				Statement();
				if (lexeme == "else")
				{
					print();
					cout << endl;
					load_tokens();
					print();
					_If = true;
					Statement();
					_instr_table.back_patch(_memory);
					if (lexeme == "endif")
					{
						print();
						cout << endl;
						load_tokens();
						print();
						_If = true;
					}
				}
				else
				if (lexeme == "endif")
				{
					print();
					cout << endl;
					load_tokens();
					_If = true;
				}
				else
				{
					cout << "Missing: endif" << endl;
					system("pause");
					exit(1);
				}
			}
			else
			{
				cout << "Missing: )" << endl;
				system("pause");
				exit(1);
			}
		}
		else
		{
			cout << "Missing: (" << endl;
			system("pause");
			exit(1);
		}
	}
	return _If;
}
//checks if there is a "return ;" or if there is a return followed by an expression followed by a ;
bool Parser::Return()
{
	bool _Return = false;
	if (lexeme == "return")
	{
		load_tokens();
		if (_debug)
		{
			cout << "<Return> ::= return;" << endl;
		}
		fout << "<Return> ::= return;" << endl;
		if (lexeme == ";")
		{
			print();
			cout << endl;
			load_tokens();
			_Return = true;
		}
		else {
			Expression();
			if (lexeme == ";")
			{
				print();
				cout << endl;
				load_tokens();
				_Return = true;
				if (_debug)
				{
					cout << "<Return> ::= return <Expression>;" << endl;
				}
				fout << "<Return> ::= return <Expression>;" << endl;
			}
			else
			{
				cout << "Missing: ;" << endl;
				system("pause");
				exit(1);
			}
		}
	}
	return _Return;
}
// checks if there is a printf( and then calls expression followed by a ); will call assign or give an error if not
bool Parser::Write()
{
	bool _Write = false;
	if (lexeme == "printf" || lexeme == "Printf")
	{
		print();
		load_tokens();
		if (lexeme == "(")
		{
			if (_debug)
			{
				cout << "<Write> ::= printf ( <Expression> );" << endl;
			}
			fout << "<Write> ::= printf ( <Expression> );" << endl;
			print();
			cout << endl;
			load_tokens();
			Expression();
			if (lexeme == ")")
			{
				print();
				cout << endl;
				load_tokens();
				if (lexeme == ";")
				{
					print();
					cout << endl;
					load_tokens();
					_Write = true;
				}
				else
				{
					cout << "Missing: ;" << endl;
					system("pause");
					exit(1);
				}
			}
			else
			{
				cout << "Missing: )" << endl;
				system("pause");
				exit(1);
			}
		}
		else
		{
			cout << "Expected ( after printf\n";
			system("pause");
			exit(1);
		}
	}
	return _Write;
}
//checks if there is a "scanf (" followed by calling ID and then checks if there is a ");", if not call an eror or Assign()
bool Parser::Read()
{
	bool _Read = false;
	if (lexeme == "scanf")
	{
		print();
		cout << endl;
		load_tokens();

		if (lexeme == "(")
		{
			if (_debug)
			{
				cout << "<Read> ::= scanf ( <IDs> );" << endl;
			}
			fout << "<Read> ::= scanf ( <IDs> );" << endl;
			print();
			cout << endl;
			load_tokens();
			IDs();

			if (lexeme == ")")
			{
				print();
				cout << endl;
				load_tokens();
				if (lexeme == ";")
				{
					print();
					cout << endl;
					load_tokens();
					_Read = true;
				}
				else
				{
					cout << "Missing: ;" << endl;
					system("pause");
					exit(1);
				}
			}
			else
			{
				cout << "Missing: )" << endl;
				system("pause");
				exit(1);
			}
		}
		else
		{
			cout << "Expected ( after scanf\n";
			system("pause");
			exit(1);
		}
	}
	return _Read;
}
//checks if there is a while "while(" and then calls Condition(), checks if there is a ")" followed by calling Statement()
bool Parser::While()
{
	bool _While = false;
	if (lexeme == "while")
	{
		int _memory = _table.get_memory(lexeme, _type);
		_instr_table.gen_instr("LABEL", -999);
		print();
		cout << endl;
		load_tokens();
		if (_debug)
		{
			cout << "<While> ::=( <Condition> ) <Statement>" << endl;
		}
		fout << "<While> ::=( <Condition> ) <Statement>" << endl;
		if (lexeme == "(")
		{
			print();
			cout << endl;
			load_tokens();
			Condition();
			if (lexeme == ")")
			{
				print();
				cout << endl;
				load_tokens();
				print();
				Statement();
				_instr_table.gen_instr("JUMP", _memory);
				_instr_table.back_patch(_instr_table.current_address());
				_While = true;
			}
			else
			{
				cout << "Missing: )" << endl;
				system("pause");
				exit(1);
			}
		}
	}
	return _While;
}
//calls Expression() followed by Relop(), followed by Expression()
bool Parser::Condition()
{
	bool _Condition = false;
	if (_debug)
	{
		cout << "<Condition> :== <Expression> <Relop> <Expression>" << endl;
	}
	fout << "<Condition> :== <Expression> <Relop> <Expression>" << endl;
	Expression();
	Relop();
	Expression();

	_Condition = true;
	return _Condition;
}
// checks if there is  = |  /=  |   >   | <   |  =>   | <=    
bool Parser::Relop()
{
	cout << endl;
	bool _Relop = false;
	if (lexeme == "=")
	{
		print();
		load_tokens();
		_instr_table.gen_instr("EQU", -999);
		_instr_table.push_address();
		_instr_table.gen_instr("JUMPZ", -999);
		_Relop = true;
		if (_debug)
		{
			cout << "<Relop> ::= =" << endl;
		}
		fout << "<Relop> ::= =" << endl;
	}
	else if (lexeme == "/=")
	{
		print();
		load_tokens();
		_instr_table.gen_instr("NEQ", -999);
		_instr_table.push_address();
		_instr_table.gen_instr("JUMPZ", -999);
		_Relop = true;
		if (_debug)
		{
			cout << "<Relop> ::= /=" << endl;
		}
		fout << "<Relop> ::= /=" << endl;
	}
	else if (lexeme == ">")
	{
		print();
		load_tokens();
		_instr_table.gen_instr("GRT", -999);
		_instr_table.push_address();
		_instr_table.gen_instr("JUMPZ", -999);
		_Relop = true;
		if (_debug)
		{
			cout << "<Relop> ::= >" << endl;
		}
		fout << "<Relop> ::= >" << endl;
	}
	else if (lexeme == "<")
	{
		print();
		_instr_table.gen_instr("LES", -999);
		_instr_table.push_address();
		_instr_table.gen_instr("JUMPZ", -999);
		load_tokens();
		_Relop = true;
		if (_debug)
		{
			cout << "<Relop> ::= <" << endl;
		}
		fout << "<Relop> ::= <" << endl;
	}
	else if (lexeme == "=>")
	{
		print();
		load_tokens();
		_instr_table.gen_instr("GEQ", -999);
		_instr_table.push_address();
		_instr_table.gen_instr("JUMPZ", -999);
		_Relop = true;
		if (_debug)
		{
			cout << "<Relop> ::= =>" << endl;
		}
		fout << "<Relop> ::= =>" << endl;
	}
	else if (lexeme == "<=")
	{
		print();
		_instr_table.gen_instr("LEQ", -999);
		_instr_table.push_address();
		_instr_table.gen_instr("JUMPZ", -999);
		load_tokens();
		_Relop = true;
		if (_debug)
		{
			cout << "<Relop> ::= <=" << endl;
		}
		fout << "<Relop> ::= <=" << endl;
	}
	else
	{
		cout << "Missing: Relop" << endl;
		system("pause");
		exit(1);
	}
	return _Relop;
}
//checks if there is a "-" plus a primary() or a primary()
bool Parser::Factor()
{
	bool _Factor = false;
	if (lexeme == "-")
	{
		print();
		cout << endl;
		load_tokens();
		if (_debug)
		{
			cout << "<Factor> ::= -<Primary>" << endl;
		}
		fout << "<Factor> ::= -<Primary>" << endl;
		Primary();
		_Factor = true;
	}
	else
	{
		if (_debug)
		{
			cout << "<Factor> ::= <Primary>" << endl;
		}
		fout << "<Factor> ::= <Primary>" << endl;
		Primary();
		_Factor = true;
	}
	return _Factor;
}
//<Identifier> | <Integer> | <Identifier> (<IDs>)  | (<Expression>) | <Real> | true | false

bool Parser::Primary()
{
	bool _Primary = false, _flag = true;
	if (token == "identifier" || token == "Identifier")
	{
		print();
		int _memory = _table.get_memory(lexeme, _type);
		_instr_table.gen_instr("PUSHM", _memory);
		load_tokens();
		if (lexeme == "(")
		{
			print();
			cout << endl;
			load_tokens();
			if (_debug)
			{
				cout << "<Primary> ::= <identifier> (<IDs>)" << endl;
			}
			fout << "<Primary> ::= <identifier> (<IDs>)" << endl;
			IDs();
			if (lexeme == ")")
			{
				print();
				cout << endl;
				load_tokens();
				_Primary = true;
				_flag = false;
			}
			else
			{
				cout << "Missing: )" << endl;
				system("pause");
				exit(1);
			}
		}
		if (_flag == true)
		{
			_Primary = true;
			if (_debug)
			{
				cout << "<Primary> ::= <identifier>" << endl;
			}
			fout << "<Primary> ::= <identifier>" << endl;
		}
	}
	else if (token == "integer" || token == "Integer")
	{
		print();
		cout << endl;
		load_tokens();
		_Primary = true;

		if (_debug)
		{
			cout << "<Primary> ::= <integer>" << endl;
		}
		fout << "<Primary> ::= <integer>" << endl;
	}
	else if (lexeme == "(")
	{
		print();
		cout << endl;
		load_tokens();
		if (_debug)
		{
			cout << "<Primary> ::= ( <Expression> )" << endl;
		}
		fout << "<Primary> ::= ( <Expression> )" << endl;

		Expression();
		if (lexeme == ")")
		{
			print();
			cout << endl;
			load_tokens();
			_Primary = true;
		}
		else
		{
			cout << "Missing: )" << endl;
			system("pause");
			exit(1);
		}
	}//start commented out zone
	else if (token == "real" || token == "Real")
	{
		print();
		cout << endl;
		load_tokens();
		_Primary = true;
		if (_debug)
		{
			cout << "<Primary> ::= <real>" << endl;
		}
		fout << "<Primary> ::= <real>" << endl;
	}//end commented out zone
	else if (lexeme == "true" || lexeme == "True")
	{
		print();
		cout << endl;
		load_tokens();
		_Primary = true;
		if (_debug)
		{
			cout << "<Primary> ::= <true>" << endl;
		}
		fout << "<Primary> ::= <true>" << endl;
	}
	else if (lexeme == "false" || lexeme == "False")
	{
		print();
		cout << endl;
		load_tokens();
		_Primary = true;
		if (_debug)
		{
			cout << "<Primary> ::= <false>" << endl;
		}
		fout << "<Primary> ::= <false>" << endl;
	}
	else
	{
		cout << "Missing: Primary" << endl;
		system("pause");
		exit(1);
	}
	return _Primary;
}
//<Expression> + <Term>  | <Expression>  - <Term>  | <Term>
bool Parser::Expression()
{
	bool _Expression = false;
	if (_debug)
	{
		cout << "<Expression> ::= <Term><ExpressionPrime>" << endl;
	}
	fout << "<Expression> ::= <Term><ExpressionPrime>" << endl;
	Term();
	ExpressionPrime();
	_Expression = true;
	return _Expression;
}
//used to prevent left recursion
bool Parser::ExpressionPrime()
{
	bool _ExpressionPrime = false;
	if (lexeme == "+")
	{
		print();
		cout << endl;
		load_tokens();

		if (_debug)
		{
			cout << "<ExpressionPrime> ::= +<Term><ExpressionPrime>" << endl;
		}
		fout << "<ExpressionPrime> ::= +<Term><ExpressionPrime>" << endl;
		_ExpressionPrime = true;
		Term();
		_instr_table.gen_instr("ADD", -999);
		ExpressionPrime();
	}
	else if (lexeme == "-")
	{
		print();
		cout << endl;
		load_tokens();

		if (_debug)
		{
			cout << "<ExpressionPrime> ::= -<Term><ExpressionPrime>" << endl;
		}
		fout << "<ExpressionPrime> ::= -<Term><ExpressionPrime>" << endl;
		_ExpressionPrime = true;
		Term();
		_instr_table.gen_instr("SUB", -999);
		ExpressionPrime();
	}
	else
	{
		_ExpressionPrime = true;
		if (_debug)
		{
			cout << "<ExpressionPrime> ::= epsilon" << endl;
		}
		fout << "<ExpressionPrime> ::= epsilon" << endl;
	}
	return _ExpressionPrime;
}
//<Term> * <Factor>  | <Term> / <Factor> |  <Factor>
bool Parser::Term()
{
	bool _Term = false;
	if (_debug)
	{
		cout << "<Term> ::= <Factor><TermPrime>" << endl;
	}
	fout << "<Term> ::= <Factor><TermPrime>" << endl;
	Factor();
	TermPrime();
	_Term = true;
	return _Term;
}
//used to prevent left recursion in term
bool Parser::TermPrime()
{
	bool _TermPrime = false;
	if (lexeme == "*")
	{
		print();
		cout << endl;
		load_tokens();
		if (_debug)
		{
			cout << "<TermPrime> ::= *<Factor><TermPrime>" << endl;
		}
		fout << "<TermPrime> ::= *<Factor><TermPrime>" << endl;
		Factor();
		_instr_table.gen_instr("MUL", -999);
		TermPrime();
		_TermPrime = true;
	}
	else if (lexeme == "/")
	{
		print();
		cout << endl;
		load_tokens();
		if (_debug)
		{
			cout << "<TermPrime> ::= /<Term><FactorPrime>" << endl;
		}
		fout << "<TermPrime> ::= /<Term><FactorPrime>" << endl;
		_TermPrime = true;
		Factor();
		_instr_table.gen_instr("DIV", -999);
		TermPrime();
	}
	else
	{
		_TermPrime = true;
		if (_debug)
		{
			cout << "<TermPrime> ::= epsilon" << endl;
		}
		fout << "<TermPrime> ::= epsilon" << endl;
	}
	return _TermPrime;
}

//checks if the next lexeme is a $$ which should mean empty
bool Parser::Empty()
{
	if (lexeme == "$$")
	{
		if (_debug == 1)
		{
			cout << "<Empty> ::= epsilon" << endl;
		}
		fout << "<Empty> ::= epsilon" << endl;
		return true;
	}
	else
		return false;
}
//Everything returns a true or false. If false, try another function that fits the description
//As soon as it is true, print the result and then return to main to run lexer and parser again
//Until end of file.