#pragma once
#include<vector>
#include<string>
#include "process.h"
#include "procesor.h"
#include "Memory.h"
#include "Interpreter.h"
#include<conio.h>

typedef enum color {
	purple = 5,
	white = 7,
	gray = 8,
	lightGreen = 10,
	lightAqua = 11
};
void set_color(int col);

class Shell {
protected:
	bool status;
	std::string line;
	std::vector<std::string>parsed;
	
	void read_line();
	void parse();
	void execute();
	void loop();
	void not_recognized();
	void help();
	void clear();
	void exit();
	void logo();

	//procesy i proces

	void cp();
	void dp();
	void show();		//pcb, pcblist, ram, pagefile, root, lock
	void showpcb();
	void showpcblist();
	void showtree();
	
	//pamięć

	void showram();
	void showpagefile();
	void showpagetable();
	void showqueue();
	void showframe();
	void showpages();

	//pliki i katalogi

	void touch();
	void open();
	void close();
	void rm();
	void edit();
	void copy();
	void cat();
	void fileinfo();
	void showdisc();
	void showlock();
	void editor(std::string filename);
	void write();
	void showblock();
	void showcatalogue();

	//interpreter

	void go();


public :
	void boot();
	Shell();
	~Shell() = default;
};