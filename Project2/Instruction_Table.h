#include<string>
#include<stack>
#include <iostream>
#include<fstream>

using namespace std;

const int WIDTH_SIZE = 15;

struct instruction
{
	int address;
	string op;
	int oprnd;
};

class Instruction_Table
{
public:
	Instruction_Table();
	void push_address();
	void back_patch(int jump_address);
	void gen_instr(string op, int opperand);
	int current_address();
	void write();

private:
	int _current_addr;
	int _current_pos;
	instruction _instr_table[300];
	stack<int> _jumpstack;
};

Instruction_Table::Instruction_Table()
{
	_current_addr = 1;
	_current_pos = 0;
}

void Instruction_Table::gen_instr(string op, int opperand)
{
	_instr_table[_current_pos].address = _current_addr++;
	_instr_table[_current_pos].op = op;
	_instr_table[_current_pos++].oprnd = opperand;
}

void Instruction_Table::back_patch(int jump_address)
{
	int address = _jumpstack.top();
	_jumpstack.pop();
	_instr_table[address - 1].oprnd = jump_address;
}

int Instruction_Table::current_address()
{
	return _current_addr;
}

void Instruction_Table::push_address()
{
	_jumpstack.push(_current_addr);
}

void Instruction_Table::write()
{
	ofstream ostream;
	ostream.open("Instruction_Table.txt");
	ostream << setw(WIDTH_SIZE) << "Address" << setw(WIDTH_SIZE) << "Operation" << setw(WIDTH_SIZE) << "Operand" << endl;
	
	for (int i = 0; i < _current_pos; i++)
	{
		ostream << setw(WIDTH_SIZE) << _instr_table[i].address << setw(WIDTH_SIZE) << _instr_table[i].op << setw(WIDTH_SIZE) << _instr_table[i].oprnd << endl;
	}
}