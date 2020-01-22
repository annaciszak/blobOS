#pragma once

#include <string>
#include <vector>
#include <array>
#include <iostream>
#include "Memory.h"
#include "process.h"
#include "procesor.h"

extern Memory memory;

ProcTree PTree;

using namespace std;

shared_ptr<PCB> PCB_return; // zmienna pomocnicza dla find_pid
bool find_bool = 0; // zmienna pomocnicza dla find_pid 
shared_ptr<PCB> PCB_name_return; // zmienna pomocnicza dla find_name
bool find_bool_name = 0; // zmienna pomocnicza dla find_name 

void PCB::show_info() {

	cout << "NAME: " << this->name << endl;
	cout << "PID: "<< this->pid<< endl;
	cout << "PARENT: " << this->parent_pid << endl;
	cout << "STATE: "; 
	switch (this->state) {
	case 0:
		cout << "READY" << endl;
		break;
	case 1:
		cout << "RUN" << endl;
		break;
	case 2:
		cout << "WAIT" << endl;
		break;
	case 3:
		cout << "TERMINATED" << endl;
		break;
	}
	//cout << "bool kill" << this->bool_kill << endl;

}

void PCB::copy_register(array<int, 5> &cpu_register) //kopiowanie zawartości rejestru do PCB
{ 
    for (int i = 0; i < 5; i++)
    {
        this->register_copy[i] = cpu_register[i];
    }
}

void PCB::restore_register(array<int, 5> &cpu_register) // przywracanie zawartości rejestru
{ 
    for (int i = 0; i < 5; i++)
    {
        cpu_register[i] = this->register_copy[i];
    }
}

/*
void PCB::kill() // zmiana stanu procesu za zakończony
{
    process_state state = terminated;
    this->change_state(state);
}*/

void PCB::change_state(process_state &new_state) // zmiana stanu procesu (planista)
{

	//cout << "change state" << endl;
    this->state = new_state;
	planist.check();
	if (new_state == wait) {
		this->time_run = 0;
	}

	
	// planista - oczekiwanie
	// Planist::manage();
}

bool PCB::null_vector_child() { // zwraca 1 gdy pusty i rodzic może się zakończyć 

	if (this->children_vector.size() == 0) {
		return 1;
	}
	else {
		return 0;
	}
};

shared_ptr<PCB> PCB::get_kid_pid(int &find_pid)
{
    if(this->children_vector.size() == 0){
        return nullptr;
    }

    for(int i=0; i<this->children_vector.size(); i++){ // przeszukujemy wektor dzieci w celu znalezienia szukanego PID
        if(this->children_vector[i]->pid == find_pid){
            return children_vector[i];  // zwracamy wskaźnik na PCB o znalezionym PID
        }
    }

    for(int i=0; i<this->children_vector.size(); i++){ // jeśli nie znajdziemy szukanego PID dla każdego z dzieci wywołujemy funkcję get_kid 
        children_vector[i]->get_kid_pid(find_pid); // rekurencja
    }

    return nullptr; // jeśli nie znajdziemy PID , zwracamy nullptr
}

shared_ptr<PCB> PCB::get_kid_name(string &find_name)
{
	if (this->children_vector.size() == 0) {
		return nullptr;
	}

	for (int i = 0; i < this->children_vector.size(); i++) { // przeszukujemy wektor dzieci w celu znalezienia szukanego name
		if (this->children_vector[i]->name == find_name) {
			return children_vector[i];  // zwracamy wskaźnik na PCB o znalezionym name
		}
	}

	for (int i = 0; i < this->children_vector.size(); i++) { // jeśli nie znajdziemy szukanego PID dla każdego z dzieci wywołujemy funkcję get_kid 
		children_vector[i]->get_kid_name(find_name); // rekurencja
	}

	return nullptr; // jeśli nie znajdziemy name , zwracamy nullptr
}

void PCB::kill_kid(int &kid_pid) {
	
	cout << "the process has been deleted !" << endl;
	for (int i = 0; i < this->children_vector.size(); i++) {
		if (children_vector[i]->pid == kid_pid) {
			//cout << children_vector[i]->name << " - " << children_vector[i]->pid << endl;
			planist.remove_process(children_vector[i]);
			// HAL_START_BARTEK
			memory.PageFile.erase(children_vector[i]->pid);
			// HAL_END_BARTEK
			this->children_vector.erase(this->children_vector.begin() + i); // usunięcie procesu 
			
		}
	}
	
	// przesunięcie pozostałych
}

void ProcTree::init(shared_ptr<PCB> init_proc) {
	this->init_proc = init_proc;
	memory.SetupInitProcess();
}

int ProcTree::create_process_file(string &name, string &file_name, int parent_pid)  // zakładająć że nie mamy podfolderów i ścieżka będzię jedynie nazwą pliku
{
	//cout << "pp "<< parent_pid << endl;
	PCB_return = nullptr;
	find_bool = 0;
	shared_ptr<PCB> proc = ProcTree::find_name(this->init_proc, name);
	if (proc == nullptr) {

		//cout << parent_pid << " " << this->init_proc->pid << "\n";
		if (parent_pid == this->init_proc->pid) {
			proc = make_shared<PCB>(name, parent_pid);  // utworzenie wskaźnika na proces - wywołanie konstruktora procesu
			this->init_proc->children_vector.push_back(proc); // dodanie procesu jako dziecka procesu INIT
			memory.LoadProgram(file_name, proc->pid); // Załaduj kod programu do pamięci wirtualnej
			planist.add_process(proc);
			cout << "process created" << endl;
			proc->show_info();
			planist.check();
			return proc->pid;
		}
		else {
			PCB_return = nullptr;
			find_bool = 0;
			shared_ptr<PCB> parent = ProcTree::find_pid(this->init_proc, parent_pid);
			//this->init_proc->get_kid_pid(parent_pid);
			if (find_bool == 1) {
				parent = PCB_return;
			}

			if (parent == nullptr) {
				cout << "No parent ! " << endl;
				return -1;
			}
			else {
				proc = make_shared<PCB>(name, parent_pid);  // utworzenie wskaźnika na proces - wywołanie konstruktora procesu
				parent->children_vector.push_back(proc); // dodanie procesu jako dziecka odnalezionego procesu rodzica
				memory.LoadProgram(file_name, proc->pid); // Załaduj kod programu do pamięci wirtualnej
				planist.add_process(proc);
				//cout << "process created" << endl;
				proc->show_info();
				planist.check();
				return proc->pid;
			}
		}
		
		
	}
	else {
		cout << "Process already exists !" << endl;
		return -1;
	}
}

shared_ptr<PCB> ProcTree::find_pid_recu(shared_ptr<PCB> pcb_child, int pid_proc) {


	if (PCB_return != nullptr) {
		return PCB_return;
	}
	else {

		if (this->init_proc->pid == pid_proc) {
			return this->init_proc;
		}
		else {

			if (PCB_return != nullptr) {
				return PCB_return;
			}


			for (int i = 0; i < pcb_child->children_vector.size(); i++) {

				if (PCB_return != nullptr) {
					return PCB_return;
				}

				//cout<< pcb_child->children_vector[i]->name << " pid = "<< pcb_child->children_vector[i]->pid << endl;

				if (pcb_child->children_vector[i]->pid == pid_proc) {

				//	cout << "find pid = " << pcb_child->children_vector[i]->pid << endl;
					PCB_return = pcb_child->children_vector[i];
					find_bool = 1;
					return PCB_return;
				}
				else {
					//
					//cout << "rekurencja " << endl;
					if (PCB_return != nullptr) {
						return PCB_return;
					}



					PCB_return = ProcTree::find_pid_recu(pcb_child->children_vector[i], pid_proc);
				}
			}
			if (PCB_return != nullptr) {
				return PCB_return;
			}


			return nullptr;
		}

	}

}

shared_ptr<PCB> ProcTree::find_pid(shared_ptr<PCB> pcb_child, int pid_proc)
{
	//cout << this->init_proc->name << " - " << this->init_proc->pid << endl;
	PCB_return = nullptr;
	shared_ptr<PCB> PCB_re = ProcTree::find_pid_recu(pcb_child, pid_proc);
	return PCB_re;

}

shared_ptr<PCB> ProcTree::find_name_recu(shared_ptr<PCB> pcb_child, string &name) {


	if (PCB_return != nullptr) {
	//	cout << "zwracam PCB 1 " << endl;
		return PCB_return;
	}
	else {

		if (this->init_proc->name == name) {
			return this->init_proc;
		}
		else {

			if (PCB_return != nullptr) {
		//		cout << "zwracam PCB 2 " << endl;
				return PCB_return;
			}


			for (int i = 0; i < pcb_child->children_vector.size(); i++) {

				if (PCB_return != nullptr) {
			//		cout << "zwracam PCB 3 " << endl;
					return PCB_return;
				}

				//cout<< pcb_child->children_vector[i]->name << " pid = "<< pcb_child->children_vector[i]->pid << endl;

				if (pcb_child->children_vector[i]->name == name) {

			//		cout << "find pid = " << pcb_child->children_vector[i]->pid << endl;
					PCB_return = pcb_child->children_vector[i];
					find_bool = 1;
					return PCB_return;
				}
				else {
					//
					//cout << "rekurencja " << endl;
					if (PCB_return != nullptr) {
			//			cout << "zwracam PCB 4 " << endl;
						return PCB_return;
					}



					PCB_return = ProcTree::find_name_recu(pcb_child->children_vector[i], name);
				}
			}
			if (PCB_return != nullptr) {
			//	cout << "zwracam PCB 5" << endl;
				return PCB_return;
			}


			return nullptr;
		}

	}

}

shared_ptr<PCB> ProcTree::find_name(shared_ptr<PCB> pcb_child, string &name)
{
	//cout << this->init_proc->name << " - " << this->init_proc->pid << endl;
	PCB_return = nullptr;
	shared_ptr<PCB> PCB_re = ProcTree::find_name_recu(pcb_child, name);
	if (PCB_re != nullptr) {
		PCB_re->show_info();

	}
	return PCB_re;

}


/*
shared_ptr<PCB> ProcTree::find_name(string &name) {

	shared_ptr<PCB> PCB_return = nullptr;

	for (int i = 0; i < init_proc->children_vector.size(); i++) { // przeszukujemy wektor dzieci w celu znalezienia szukanego PID
		if (init_proc->children_vector[i]->name == name) {
			return init_proc->children_vector[i];  // zwracamy wskaźnik na PCB o znalezionym name
		}
	}

	for (int i = 0; i < init_proc->children_vector.size(); i++) { // jeśli nie znajdziemy szukanego name dla każdego z dzieci wywołujemy funkcję get_kid 
		PCB_return = init_proc->children_vector[i]->get_kid_name(name); // rekurencja
	}

	return PCB_return;

}*/

void ProcTree::kill_pid(int pid) { // zabicie procesu po PID

	shared_ptr<PCB> PCB_proc_kill = ProcTree::find_pid(this->init_proc, pid); 
	shared_ptr<PCB> PCB_parent = nullptr;
	bool vector_child = 0;
	process_state PCB_new_state;

	if (PCB_proc_kill == nullptr) {
		// niedobrze
		cout << "There are no processes with the PID provided" << endl;
	}
	else if (pid == 0) {
		cout << "unable to delete INIT process!" << endl;
	}
	else {
		PCB_parent = ProcTree::find_pid(this->init_proc, PCB_proc_kill->parent_pid);

		vector_child = PCB_proc_kill->null_vector_child();
		if (vector_child == true) {

			
			// kill proces
			PCB_new_state = terminated;
			PCB_proc_kill->change_state(PCB_new_state);  // zmiana stanu na terminated 
			//przesłanie informacji do rodzica o zakończeniu się

			
			// wyświetlenie informacji o zakończonym procesie i jego statusie
			PCB_proc_kill->show_info();

			
			// usuwanie z drzewa
			PCB_parent->kill_kid(pid);


			//usuwanie z kolejek - czeka na zmiany w planiście
//Planist::remove_pcb_from_ready(pid); 
//Planist::remove_pcb_from_wait(pid); 

			if (PCB_parent->bool_kill == true) {
				cout << "parent kill " << endl;
				ProcTree::kill_pid(PCB_parent->pid);
			}

		}
		else {
			cout << "Process has children " << endl;
			PCB_new_state = wait;
			// planista - czeka na funkcję 
			// Planist::manage(); 
			PCB_proc_kill->change_state(PCB_new_state);
			PCB_proc_kill->bool_kill = 1;
			PCB_proc_kill->show_info();

		}

	}

}

void ProcTree::kill_name(string name) { // zabicie procesu po name

	shared_ptr<PCB> PCB_proc_kill = ProcTree::find_name(this->init_proc, name);
	shared_ptr<PCB> PCB_parent = nullptr;
	bool vector_child = 0;
	process_state PCB_new_state;

	if (PCB_proc_kill == nullptr) {
		// niedobrze
		cout << "There are no processes with the NAME provided" << endl;
	}
	else if (PCB_proc_kill->pid == 0) {
		cout << "unable to delete INIT process!" << endl;
	}
	else {
		PCB_parent = ProcTree::find_pid(this->init_proc, PCB_proc_kill->parent_pid);
		vector_child = PCB_proc_kill->null_vector_child();

		if (vector_child == true) {
			// kill proces
			PCB_new_state = terminated;
			PCB_proc_kill->change_state(PCB_new_state);  // zmiana stanu na terminated 

			//PCB_proc_kill->show_info();
			// usuwanie z drzewa
			PCB_parent->kill_kid(PCB_proc_kill->pid);

			//usuwanie z kolejek - czeka na zmiany w planiście
//Planist::remove_pcb_from_ready(pid); 
//Planist::remove_pcb_from_wait(pid); 

			if (PCB_parent->bool_kill == true) {
				cout << "parent kill " << endl;
				ProcTree::kill_pid(PCB_parent->pid);
			}

		}
		else {
			cout << "Process has children " << endl;
			PCB_new_state = wait;
			// planista - czeka na funkcję 
			// Planist::manage(); 
			PCB_proc_kill->change_state(PCB_new_state);
			PCB_proc_kill->bool_kill = 1;
			PCB_proc_kill->show_info();

		}

	}

}

int recu_level = 1;

void ProcTree::show_vector_child(shared_ptr<PCB> proc_show, bool end) {
	std::string tab = "	 ";
	int char_a = 192;
	int char_b = 196;
	int char_c = 195;
	int pion = 179;
	char char_a_s = char_a;
	char char_b_s = char_b;
	char char_c_s = char_c;
	char pion_d = pion;
	for (int i = 0; i < proc_show->children_vector.size(); i++) {

		for (int i = 0; i < recu_level; i++) {
			cout << tab;

			if (i == recu_level - 2 && end == true) {

			}
			else if (i < recu_level - 1) {
				cout << pion_d;
			}

		}

		if (i == proc_show->children_vector.size() - 1) {
			cout << char_a_s << char_b_s << char_b_s;
			end = 1;
		}
		else {
			cout << char_c_s << char_b_s << char_b_s;

		}

		cout << proc_show->children_vector[i]->name << " - " << proc_show->children_vector[i]->pid << " parent - " << proc_show->children_vector[i]->parent_pid << endl;

		recu_level++;
		ProcTree::show_vector_child(proc_show->children_vector[i], end);
		//recu_level--;
	}
	recu_level--;

}

void ProcTree::display_tree()
{
	int char_end_int = 191;
	char char_end = char_end_int;
	cout << this->init_proc->name << " - " << this->init_proc->pid << " " << char_end << endl;

	ProcTree::show_vector_child(this->init_proc, 0);
	recu_level = 1;

	// sposób wyświetlania drzewa do ugadania z Anią :) 
}
