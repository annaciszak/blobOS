#pragma once

#include <string>
#include <vector>
#include <array>
#include <iostream>
#include <memory>
#include "Memory.h"
//#include "procesor.h"

extern Memory memory;

using namespace std;
static int pid_pcb = 1;

enum process_state
{
    ready,
    run,
    wait,
    terminated
}; // terminated?

class PCB
{

public:
    string name;
    int pid;
    int parent_pid;                             // pid rodzica
    int time_run;                               //przydzielony kwant czasu do wykonania
    process_state state;                        // stan procesu
    array<int, 5> register_copy{0, 0, 0, 0, 0}; // kopia rejestru
    shared_ptr<PCB> parent_pcb = nullptr;       // wskaźnik na rodzica
    vector<shared_ptr<PCB>> children_vector;    // wektor dzieci
    vector<PageInfo> page_table;                // coś tam co Bartek chce i mi truł że mo być - to jest. :D
    int executionTimeLeft = 0;                  // ilość cykli do końca 
	bool bool_kill = 0;							// zmiena pomocnicza do kill pid

    PCB()
    { // konstruktor INIT
        this->name = "init";
        this->pid = 0;							// poprawione na 0 :) 
        this->parent_pid = 0;
        this->state = ready;
		this->time_run = 0;
		this->executionTimeLeft = 0;
    }

    PCB(string &name, int &parent_pid)
    {
        this->name = name;
        this->pid = pid_pcb;
        pid_pcb++;
        this->parent_pid = parent_pid;
   		this->state = ready;
		this->time_run = 0;
		this->executionTimeLeft = 0;
    }

    shared_ptr<PCB> get_kid_pid(int &find_pid);                         // przeszukiwanie dzieci po pid
	shared_ptr<PCB> get_kid_name(string &find_name);                    // przeszukiwanie dzieci po pid
    void copy_register(array<int, 5> &cpu_register);					//kopiowanie zawartości rejestru do PCB
    void restore_register(array<int, 5> &cpu_register);					// przywracanie zawartości rejestru
    void change_state(process_state &new_state);						// zmiana stanu procesu
	bool null_vector_child();											// sparwdza czy są dzieci 
    //void kill();														//zabicie procesu
	void kill_kid(int &kill_pid);										// usunięcie dziecka z vektora dzieci
	//void show_vector_child();  // wypisanie potomków
	void show_info(); // wyświetlenie informacji o procesie 
};

//void create_process_size(string &name, int &size, int &parent_pid);


class ProcTree {

public:

    shared_ptr<PCB> init_proc;

    ProcTree() {};

    //BARTEK: żeby go działało przy starcie systemu
    void init(shared_ptr<PCB> init_proc);

    int create_process_file(string &name, string &file_name, int parent_pid); // zakładająć że nie mamy podfolderów i ścieżka będzię jedynie nazwą pliku
    shared_ptr<PCB> find_pid(shared_ptr<PCB> pcb_child, int pid_proc); // przeszukiwanie drzewa po pid
	shared_ptr<PCB> find_pid_recu(shared_ptr<PCB> pcb_child, int pid_proc);

	shared_ptr<PCB> find_name(shared_ptr<PCB> pcb_child, string &name); // przeszukiwanie drzewa po name 
	shared_ptr<PCB> find_name_recu(shared_ptr<PCB> pcb_child, string &name);
	//shared_ptr<PCB> find_pid_child_vect(shared_ptr<PCB> pcb_child, int pid_proc); // szukanie dzieci po pid 
	//shared_ptr<PCB> find_name_child_vect(shared_ptr<PCB> pcb_child, string &name); // szukanie dzieci po name



	void show_vector_child(shared_ptr<PCB> proc_show, bool end);

    void kill_pid(int pid); // zabicie procesu po PID
    void kill_name(string name); // zabicie procesu po nazwie

    void display_tree(); // wyświetlenie całego drzewa procesów

};