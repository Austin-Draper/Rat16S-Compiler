#include <iostream>
#include <string>
#include <fstream>

using namespace std;

const int ARRAY_SIZE = 300;
const int SPACE_SIZE = 20;



struct Symbol
{
	string id;
	int memory;
	string type;
};


class SymbolTable
{
public:
	SymbolTable();
	void add(string id,string type); //adds a lexeme to the symbol table, duplicate lexemes leads to an error
	bool exists(string id, string type); //returns true if a particular lexeme exists in the symbol table
	void print_table(); //prints symbol table
	int get_memory(string id, string type);//returns the memory address of the id or -1 if the id is not found;
	~SymbolTable();
private:
	ofstream _ostream;
	Symbol _array[ARRAY_SIZE];
	int _arr_location = 0;
};

SymbolTable::SymbolTable(){
	_ostream.open("Symbol_Table.txt");
	_ostream << setw(SPACE_SIZE) << left << "Identification" << setw(SPACE_SIZE) << left << "Memory" << setw(SPACE_SIZE) << left << "Type" << endl;
}
SymbolTable::~SymbolTable(){
	_ostream.close();
}

void SymbolTable::add(string id, string type)
{
	for (int i = 0; i < _arr_location; i++)
	{
		if (_array[i].id == id)
		{
			if (_array[i].type == type)
			{
				cout << "Error! Multiple occasions of: " << type << " " << id << endl;
				system("pause");
				exit(1);
			}
		}
	}
	_array[_arr_location].id = id;
	_array[_arr_location].memory = 5000 + _arr_location;
	_array[_arr_location].type = type;
	_ostream << setw(SPACE_SIZE) << left << _array[_arr_location].id << setw(SPACE_SIZE) << left << _array[_arr_location].memory << setw(SPACE_SIZE) << left << _array[_arr_location].type << endl;
	_arr_location++;
}

bool SymbolTable::exists(string id, string type)
{
	for (int i = 0; i < _arr_location; i++)
	{
		if (_array[i].id == id)
		{
			if (_array[i].type == type)
			{
				return true;
			}
		}
	}
		return false;
}

void SymbolTable::print_table()
{
	cout << "\nSymbol Table\n----------------------------------------------------------";
	cout << endl << setw(SPACE_SIZE) << left << "Identification" << setw(SPACE_SIZE) << left << "Memory" << setw(SPACE_SIZE) << left << "Type" << endl;
	cout << setw(SPACE_SIZE) << left << "------------------" << setw(SPACE_SIZE) << left << "------------------" << setw(SPACE_SIZE) << left << "------------------" << endl;
	for (int i = 0; i < _arr_location; i++)
	{

		cout << setw(SPACE_SIZE) << left << _array[i].id << setw(SPACE_SIZE) << left << _array[i].memory << setw(SPACE_SIZE) << left << _array[i].type << endl;
	}
}

int SymbolTable::get_memory(string id, string type)
{
	for (int i = 0; i < _arr_location; i++)
	{
		if (_array[i].id == id)
		{
			if (_array[i].type == type)
			{
				return _array[i].memory;
			}
		}
	}
	return -1;
}