#include"File.h"

FileManager FM;

void FileManager::create_file(string name) {
	//Sprawdzenie czy nie ma już takiego pliku
	int sec_name = find_file(name);
	if (sec_name == -1) {
		inode file_node; //Tworzy i-węzeł
		pair<string, inode> file(name, file_node); //Tworzy ,,plik"

		cataloge.push_back(file); //Dodaje plik do katalogu
		std::cout << "File " << name << " created" << std::endl;
	}
	else cout << "File exists" << endl;
}

int FileManager::free_block() {
	for (int i = 0; i < 32; i++) {
		if (disc[i].free == 0)return i;
	}
	cout << "There are no free blocks" << endl;
	return -1;
}

int FileManager::find_file(string name) {
	int pom = -1;
	for (int i = 0; i < cataloge.size(); i++) {
		if (cataloge[i].first == name) {
			pom = i;
		}
	}
	return pom;
}

void FileManager::clean_file_data(string name) {
	//Szuka pliku
	int pom = find_file(name);
	if (pom != -1) {
		unsigned int length = cataloge[pom].second.size_int_byte;
		if (length > 0) {
			if (length <= 32) {
				//Czyści dane zapisane w bloku pamięci
				disc[cataloge[pom].second.number[0]].block.clear();
				disc[cataloge[pom].second.number[0]].free = 0;
				disc[cataloge[pom].second.number[0]].index= 0;
				//Usuwa blok pamięci z i-węzła
				cataloge[pom].second.number.clear();
				cataloge[pom].second.size_int_byte = 0;
			}

			if (length > 32 && length <= 64) {
				//Czyści dane zapisane w blokach pamięci
				disc[cataloge[pom].second.number[0]].block.clear();
				disc[cataloge[pom].second.number[0]].free = 0;
				disc[cataloge[pom].second.number[0]].index = 0;
				disc[cataloge[pom].second.number[1]].block.clear();
				disc[cataloge[pom].second.number[1]].free = 0;
				disc[cataloge[pom].second.number[1]].index = 0;
				//Usuwa bloki pamięci z i-węzła
				cataloge[pom].second.number.clear();
				cataloge[pom].second.size_int_byte = 0;
			}

			if (length > 64) {
				//Czyści dane zapisane w blokach pamięci
				disc[cataloge[pom].second.number[0]].free = 0;
				disc[cataloge[pom].second.number[0]].block.clear();
				disc[cataloge[pom].second.number[0]].index = 0;
				disc[cataloge[pom].second.number[1]].free = 0;
				disc[cataloge[pom].second.number[1]].block.clear();
				disc[cataloge[pom].second.number[1]].index = 0;

				int k = 0, len = cataloge[pom].second.number[2];
				int ilosc = disc[len].block.size();
				while (k < ilosc) {
					//Czyści bloki pamięci zapisane w bloku indeksowym
					int help = int(disc[cataloge[pom].second.number[2]].block[k]) - 48;
					disc[help].block.clear();
					disc[help].free = 0;
					disc[help].index = 0;
					k++;
				}
				//Czyści blok indeksowy
				disc[cataloge[pom].second.number[2]].free = 0;
				disc[cataloge[pom].second.number[2]].block.clear();
				disc[cataloge[pom].second.number[2]].index = 0;
				cataloge[pom].second.size_int_byte = 0;
				//Usuwa bloki pamięci z i-węzła
				cataloge[pom].second.number.clear();
			}
		}
	}
}

void FileManager::show_disc() {
	for (int j = 0; j < 32; j++) {
		cout << j << ": ";
		for (auto e : disc[j].block) {
			if (disc[j].index == 1) {
				if(e>57&&e<68)cout << "[1" <<e-58 << "] ";
				else if (e > 67 && e < 78)cout << "[2" << e - 68 << "] ";
				else if (e > 77 && e < 88)cout << "[3" << e - 78 << "] ";
				else cout << "[" << e << "] ";
			}
			else cout << "[" << e << "] ";
		}
		cout << endl;
	}
}

void FileManager::show_lock_queue(const string &filename)
{
	int pom = find_file(filename);
	if (pom != -1) {
		std::deque<std::shared_ptr<PCB>> queue = FM.cataloge[pom].second.mutex.copy_queue();
		int counter = 0;
		std::cout << "Queue of processes waiting for the lock of '" << filename << "' file:\n";
		for (auto process : queue) {
			std::cout << counter << ". " << process->name << " " << process->pid << "\n";
			counter++;
		}
		std::cout << "\n";
	}
}

void FileManager::save_data_to_file(string name, string text) {
	//Wyszukuje plik
	int pom = find_file(name);
	bool save = 0;
	if (pom != -1) {
		string kopia = show_file(name);		//Kopia danych na wypadek gdyby nie zmie�ci�y si� w pami�ci i nie mo�na by�o zapisa� pliku
		unsigned int length = text.size();
		clean_file_data(name); 				//Czyści aktualne dane pliku
		if (length <= 32) {					//Zapis dla plików o maksymalnie 32 bajtach
			int nr = free_block();			//Wyszukuje pusty blok pamięci
			if (nr != -1) {
				cataloge[pom].second.number.push_back(nr);
				//Zapis danych
				for (int i = 0; i < length; i++) {
					disc[cataloge[pom].second.number[0]].block.push_back(text[i]);
				}
				cataloge[pom].second.size_int_byte = length; //Zmiana długości pliku
				disc[nr].free = 1;
			}
			else {
				clean_file_data(name);
				save = 1;
			}
		}

		if (length > 32 && length <= 64) {	//Zapis dla plików o maksymalnie 64 bajtach
			int nr1 = free_block();			//Wyszukuje pusty blok pami�ci
			if (nr1 != -1) {
				disc[nr1].free = 1;
				int nr2 = free_block();		//Wyszukuje pusty blok pami�ci
				if (nr2 != -1) {
					disc[nr2].free = 1;
					cataloge[pom].second.number.push_back(nr1);
					int i = 0;
					//Zapis danych
					for (i; i < 32; i++)
						disc[cataloge[pom].second.number[0]].block.push_back(text[i]);

					cataloge[pom].second.number.push_back(nr2);
					for (i; i < length; i++) 
						disc[cataloge[pom].second.number[1]].block.push_back(text[i]);

					cataloge[pom].second.size_int_byte = length;
				}
				else {
					clean_file_data(name);
					save = 1;
				}
			}
		}

		if (length > 64) {				//Zapis dla wi�kszych plik�w 
			int i = 0;
			int nr1 = free_block();		//Wyszukuje pusty blok pami�ci
			if (nr1 != -1) {
				disc[nr1].free = 1;
				int nr2 = free_block(); //Wyszukuje pusty blok pami�ci
				if (nr2 != -1) {
					disc[nr2].free = 1;
					cataloge[pom].second.number.push_back(nr1);

					//Zapis danych
					for (i; i < 32; i++) {
						disc[cataloge[pom].second.number[0]].block.push_back(text[i]);
					}
					cataloge[pom].second.number.push_back(nr2);

					//Zapis danych
					for (i; i < 64; i++) {
						disc[cataloge[pom].second.number[1]].block.push_back(text[i]);
					}

					int index = free_block(); //Wyszukuje pusty blok pamięci
					//Blok indeksowy
					if (index != -1) {
						disc[index].free = 1;
						cataloge[pom].second.number.push_back(index);
						disc[index].index = 1;
						cataloge[pom].second.size_int_byte = length;
						int k = 0, len = length - 64;
						int j = 0, dl = 0;

						//Sprawdzanie czy do bloku ma wpisać 32 bajty czy mniej
						while (i < length) {
							if (len > 32)dl = 32;
							else dl = len;
							nr1 = free_block(); //Wyszukuje pusty blok pami�ci
							if (nr1 != -1) {
								disc[nr1].free = 1;
								//Wpisywanie numeru bloku (ASCII)
								string a = to_string(nr1);
								if (nr1 > 9)a[0] += nr1 - 1;
								if (nr1 > 19)a[0] -= 1;
								if (nr1 > 29)a[0] -= 1;
								disc[index].block.push_back(a[0]);
								//Zapis danych
								for (j = 0; j < dl; j++) {
									int help = int(disc[index].block[k]) - 48;
									disc[help].block.push_back(text[i]);
									i++;
								}
								k++;
								len -= 32;
							}
							else {
								clean_file_data(name);
								save = 1;
								break;
							}
						}
					}
					else {
						clean_file_data(name);
						save = 1;
					}
				}
				else {
					clean_file_data(name);
					save = 1;
				}
			}
			else {
				clean_file_data(name);
				save = 1;
			}
		}
		//Zapis poprzedniego pliku jeżeli, nowe dane się nie mieszczą
		if (save == 1) {
			save_data_to_file(name, kopia);
		}
	}
}

void FileManager::edit_file(string name, string text) {
	int pom = find_file(name);
	if (pom != -1){
		if (cataloge[pom].second.mutex.get_owner_id() == planist.ReadyPCB[0]->pid) {
			save_data_to_file(name, text);
		}
	}
	else cout << "File does not exist" << endl;
}

void FileManager::edit_file_editor(string name, string text) {
	int pom = find_file(name);
	if (pom != -1) {
		if ( cataloge[pom].second.mutex.lock_for_editor()) {
			save_data_to_file(name, text);
		}
	}
	else cout << "File does not exist" << endl;
}

string FileManager::show_file(string name) {
	string text;
	int pom = find_file(name);
	if (pom == -1) return "File does not exist";
	int length = cataloge[pom].second.size_int_byte;

	//Dla plików nie większych niż 32 bajty
	if (length <= 32) {
		for (int i = 0; i < length; i++)
			text.push_back(disc[cataloge[pom].second.number[0]].block[i]);
	}

	//Dla plików nie większych niż 64 bajty
	if (length > 32 && length <= 64) {
		int i = 0, j = 0;
		for (i; i < 32; i++)
			text.push_back(disc[cataloge[pom].second.number[0]].block[i]);
		for (i; i < length; i++) {
			text.push_back(disc[cataloge[pom].second.number[1]].block[j]);
			j++;
		}
	}

	//Dla plików większych niż 64 bajty
	if (length > 64) {
		int i = 0, j = 0;
		for (i; i < 32; i++)
			text.push_back(disc[cataloge[pom].second.number[0]].block[i]);
		for (i; i < 64; i++) {
			text.push_back(disc[cataloge[pom].second.number[1]].block[j]);
			j++;
		}
		int k = 0, len = length - 64;
		while (i < length) {
			while (len > 32) {
				for (j = 0; j < 32; j++) {
					int help = int(disc[cataloge[pom].second.number[2]].block[k]) - 48;
					text.push_back(disc[help].block[j]);
					i++;
				}
				k++;
				len -= 32;
			}
			if (len <= 32) {
				for (j = 0; j < len; j++) {
					int help = int(disc[cataloge[pom].second.number[2]].block[k]) - 48;
					text.push_back(disc[help].block[j]);
					i++;
				}
			}
		}
	}
	return text;
}

void FileManager::add_to_file(string name, string text) {
	//szuka pliku
	int pom = find_file(name);
	if (pom != -1) {
		//Zczytuje dane już zapisane i dopisuje na ich końcu nowo dodane dane
		if (cataloge[pom].second.mutex.get_owner_id() == planist.ReadyPCB[0]->pid){
			string plik = show_file(name);
			plik += text;
			edit_file(name, plik);
		}
	}
	else cout << "File does not exist" << endl;
}

void FileManager::copy_file(string name1, string name2) {
	int pom1 = find_file(name1);
	int pom2 = find_file(name2);
	if (pom1 != -1) {
		if (pom2 != -1) {
			string tekst1 = show_file(name1);
			edit_file(name2, tekst1);

		}
		else cout << "File does not exist" << endl;
	}
	else cout << "File does not exist" << endl;
}

void FileManager::delete_file(string name) {
	//Wyszukuje plik
	int pom = find_file(name);
	if (pom == -1) {
		cout<< "File does not exist";
	}
	else {
		//Czy�ci dane pliku i usuwa go z katalogu
		clean_file_data(name);
		cataloge.erase(cataloge.begin() + pom);
		std::cout << "File " << name << " deleted" << std::endl;
	}
}

void FileManager::open_file(string name) {
	//Szuka pliku
	int pom = find_file(name);
	if (pom != -1) {
		//Probuje dostac zamek, jesli sie nie uda zmienia stan procesu na waiting i wywoluje planiste
		cataloge[pom].second.mutex.lock(planist.ReadyPCB[0]);
	}
	else cout << "File does not exist" << endl;
}

void FileManager::close_file(string name) {
	//Szuka pliku
	int pom = find_file(name);
	if (pom != -1){
		//Odblokowuje zamek
		cataloge[pom].second.mutex.unlock(planist.ReadyPCB[0]);
	}
	else cout << "File does not exist" << endl;
}

void FileManager::file_info(string name) {
	//szuka pliku
	int pom = find_file(name);
	if (pom != -1) {
		cout << "File name: " << name << endl;
		cout << "File size (in bytes): " << cataloge[pom].second.size_int_byte << endl;
		cout << "Blocks of disc used by this file: ";
		int j = cataloge[pom].second.number.size();
		for (int i = 0; i < j; i++) {
			cout << "[" << cataloge[pom].second.number[i] << "] ";
			if (i == 2) {
				for (auto e: disc[cataloge[pom].second.number[2]].block) {
					if (disc[cataloge[pom].second.number[2]].index == 1) {
							if (e > 57 && e < 68)cout << "[1" << e - 58 << "] ";
							else if (e > 67 && e < 78)cout << "[2" << e - 68 << "] ";
							else if (e > 77 && e < 88)cout << "[3" << e - 78 << "] ";
							else cout << "[" << e << "] ";
						}
					else cout << "[" << e << "] ";
				}
			}
		}
		cout << endl;
	}
	else cout << "File does not exist" << endl;
}

void FileManager::show_cataloge() {
	if (cataloge.size() > 0) {
		cout << "File list:" << endl;
		for (int i = 0; i < cataloge.size(); i++) {
			cout << "- " << cataloge[i].first << endl;
		}
	}
	else cout << "There is no file in cataloge" << endl;
}

void FileManager::show_block(int nr) {
	cout << "Content of block " << nr << ": " << endl;
	for (auto e : disc[nr].block) {
		cout << "|" << e << "| ";
	}
	cout << endl;
}