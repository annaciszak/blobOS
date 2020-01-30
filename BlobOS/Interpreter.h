#pragma once
#include <array>
#include <string>
#include <vector>
using namespace std;

class PCB;
class Planist;

class Interpreter {
private:
	int A = 0, B = 0, C = 0, D = 0;
	unsigned int instruction_counter;
	std::string instruction;
	array<string, 4> exec_instruction;
	
	string get_instruction(unsigned int& instruction_counter, const shared_ptr<PCB>& running_proc);
	void take_from_proc(const shared_ptr<PCB>& running_proc);
	void update_proc(const shared_ptr<PCB>& running_proc);
	array<string, 4> instruction_separate(const std::string & instruction);
	int execute_instruction(std::string& instruction, shared_ptr<PCB>& running_proc);
	void display_registers();
public:
	//Wykonuje pojedynczy rozkaz asemblerowy
	int execute_line();
};