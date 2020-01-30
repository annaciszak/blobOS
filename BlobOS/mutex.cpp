#include "mutex.h"

mutex::mutex()
{
	LOCKED = false;
	OWNER_ID = 0;
}

void mutex::lock(std::shared_ptr<PCB>process)
{
	set_color(white);
	if (WAITING_PCB.empty())
	{
		//utworzono proces i otwarto z niego zamek przy pustej kolejce
		if (process->pid != 0)
		{
			//dodaj
			OWNER_ID = process->pid;
			LOCKED = true;
			WAITING_PCB.push_back(process);
			std::cout << "lock: " << process->name << ": file has been locked.\n";
		}
		//dodaj init
		else
		{
			OWNER_ID = process->pid;
			WAITING_PCB.push_back(process);
			std::cout << "lock: " << process->name << ": file has been locked.\n";
		}
	}
	else
	{
		if (process->pid == OWNER_ID && LOCKED)
		{
			std::cout << "The process already is the owner, processing.\n";
		}
		else
		{
			if (process->pid != OWNER_ID && LOCKED)
			{
				std::cout << "lock: " << process->name << ": file locked by: " << WAITING_PCB.front()->name << ", calling planist.\n";
				//dodaje oczekujacy proces do kolejki zamka
				WAITING_PCB.push_back(process);
				wait(process);
			}
			else if (process->pid != OWNER_ID && !LOCKED)
			{
				//usuwanie inita z poczatku kolejki
				if (WAITING_PCB.front()->name == "init")
				{
					WAITING_PCB.pop_front();
				}
				//ustawia ID procesu-wlasciciela
				OWNER_ID = process->pid;
				//zamyka
				LOCKED = true;
				WAITING_PCB.push_back(process);
				std::cout << "lock: " << process->name << ": file has been locked.\n";
			}
			else
			{
				//start systemu
				if (WAITING_PCB.empty())
				{
					PCB init;
					std::shared_ptr<PCB>init_ptr = make_shared<PCB>(init);
					WAITING_PCB.push_back(init_ptr);
					std::cout << "lock(): lock's queue is empty, adding init.\n";
				}
				else
				{
					//zamyka
					if (WAITING_PCB.front()->name != "init")
					{
						LOCKED = true;
						std::cout << "lock(): " << process->name << ": file has been locked.\n";
					}
				}

			}
		}
	}
}

void mutex::unlock(std::shared_ptr<PCB>process)
{
	set_color(white);
	//jesli proces nie jest wlascicielem
	if (process->pid != OWNER_ID)
		std::cout << "unlock(): " << process->name << ": the process is not the lock's owner, cannot unlock.\n";
	else if (!LOCKED) std::cout << "unlock(): " << process->name << ": the lock is open, cannot unlock.\n";
	//jesli proces jest wlascicielem
	else
	{
		//sprawdza stan kolejki i usuwa z niej wykonany proces 
		if (!WAITING_PCB.empty())
			WAITING_PCB.pop_front();
		LOCKED = false;
		cout << "File closed, lock has been unlocked.\n";
		notify();
	}
}

void mutex::wait(std::shared_ptr<PCB>process)
{
	//zmienia stan procesu: running->waiting
	process_state state = process_state::wait;//ing
	process->change_state(state);
	//wywoluje planiste - przekazanie sterowania
	planist.check();
}

void mutex::notify()

{		set_color(white);
		if (!WAITING_PCB.empty())
		{
			//zmienia stan procesu kolejnego w kolejce: waiting->ready
			process_state state = ready;
			WAITING_PCB.front()->change_state(state);
			OWNER_ID = WAITING_PCB.front()->pid;
			//wywoluje planiste - przekazanie sterowania
			planist.check();
		}
		else 
		{
			OWNER_ID = 0;
			std::cout << "notify(): the lock's queue is empty.\n";
		}
		//badz wolny pliku		
}

bool mutex::lock_for_editor()
{
	if (LOCKED)
	{
		set_color(white);
		cout << "The file is being edited by another process, cannot save. Use '$ go' and try again.\n";
		return false;
	}
	else return true;
}

const int mutex::get_owner_id() const
{
	return OWNER_ID;
}

std::deque<std::shared_ptr<PCB>> mutex::copy_queue()
{
	std::deque<std::shared_ptr<PCB>> copied;
	for (auto process : WAITING_PCB)
	{
		copied.push_back(process);
	}
	return copied;
}