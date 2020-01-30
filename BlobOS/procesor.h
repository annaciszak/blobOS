#pragma once
#include "process.h"
#include <list>
#include <vector>
#include <memory>

using namespace std;

class Planist{
    private:
    //proces_state state;     //stan procesu
    //array<int, 5> register_copy{0, 0, 0, 0, 0};     // kopia rejestru
    //static bool compare_PCB(const shared_ptr<PCB>& first, const shared_ptr<PCB>& second);

    public:
	// Lista procesow gotowych do wykonania
    vector<shared_ptr<PCB>> ReadyPCB;
	// Lista procesow oczekujacych
	vector<shared_ptr<PCB>> WaitPCB;
	// Czasu do podzia³u (w liczbie rozkazów)
	int time;
	// Suma przydzielonego czasu - jeœli 0 - odpalamy menager 
	int time_sum;
	// Kwant czasu
	int time_per_one;
	// Liczba procesów w kolejce ready
	int amount_process;
	
	Planist() {
		this->time = 10;
		this->time_per_one = 0;
		this->time_sum = 0;
		this->amount_process = 0;
	}
    ~Planist() = default;
	
	void init();											// BARTEK: ¿eby go dzia³a³o przy starcie systemu
    void add_process(const shared_ptr<PCB>& process);		// dodanie procesu do kolejek
    void remove_process(const shared_ptr<PCB>& process);	// usuniêcie procesu z kolejek
	void manager();											// przydzielenie kwantu czasu 
	void first_to_end();									// przeniesenie pierszego na ostatni
	void sort_list();										// ustawienie pierwszego na run
	void check();											// sprawdzenie stanów procesu i posortowanie w kolejkach
    void display_PCB_list();								// wyœwietlenie stanu obu kolejek
};

extern Planist planist;

