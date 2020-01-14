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
    vector<shared_ptr<PCB>> ReadyPCB;     // lista procesow gotowych do wykonania
	vector<shared_ptr<PCB>> WaitPCB;		// lista procesow oczekujacych
	int time;							// czas do podzia�u
	int time_sum;						// suma przydzielonego czasu - je�li 0 - odpalamy menager 
	int time_per_one;					// kwant czasu
	int amount_process;					// liczba proces�w w kolejce ready
	

	Planist() {
		this->time = 10;
		this->time_per_one = 0;
		this->time_sum = 0;
		this->amount_process = 0;
	}
    ~Planist() = default;

    //BARTEK: �eby go dzia�a�o przy starcie systemu
    void init();

    void add_process(const shared_ptr<PCB>& process); // dodanie procesu do kolejek
    void remove_process(const shared_ptr<PCB>& process); // usuni�cie procesu z kolejek
	void manager(); // przydzielenie kwantu czasu 
	void first_to_end(); // przeniesenie pierszego na ostatni
	void sort_list(); // ustawienie pierwszego na run

    void check(); // sprawdzenie stan�w procesu i posortowanie w kolejkach
    void display_PCB_list(); // wy�wietlenie stanu obu kolejek
};

extern Planist planist;

