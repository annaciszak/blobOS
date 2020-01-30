#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<array>
#include<fstream>
#include <conio.h>
#include "procesor.h"
#include "mutex.h"
using namespace std;
//WAŻNE - plik jest reprezentowany przez std::pair<string, inode> - nazwa + i-węzeł

struct  disc_area
{
	bool free;
	vector<char> block;
	bool index;

	disc_area() {
		free = 0;
		index = 0;
	}
};

//i-węzeł
struct inode {
	//rozmiar danych pliku
	int size_int_byte; 
	//pierwsze dwa to nr bezpośrednich bloków danych, następny to numer bloku indeksowego
	vector<int> number; 
	mutex mutex;

	inode() {size_int_byte = 0;}
};

class FileManager {
	public:
	array<disc_area, 32> disc;
	vector < pair < string, inode >> cataloge;

	void create_file(string name);						//Tworzy plik o podanej nazwie, umieszcza go w katalogu
	int free_block();									//Wyszukuje pusty blok pamięci i zwraca jego 
	int find_file(string name);							//Odszukuje plik o danej nazwie w katalogu i zwraca jego numer
	void clean_file_data(string name);					//Czyści plik z danych
	void show_disc();									//Pokazuje dysk
	void show_lock_queue(const string &filename);		//Pokazuje kolejkę zamka
	void save_data_to_file(string name, string text);	//Zapisuje tekst do pamięci dla podanego pliku 
	void edit_file(string name, string text);			//Sprawdza zamek i zapisuje dane do pliku
	void edit_file_editor(string name, string text);	//Sprawdza zamek dla edytora i zapisuje dane do pliku
	string show_file(string name);						//Zwraca stringa z zawartością pliku
	void add_to_file(string name,string text);			//Dopisuje na końcu pliku
	void copy_file(string name1, string name2);			//Kopiuje zawartość pierwszego pliku do drugiego
	void delete_file(string name);						//Usuwa plik
	void open_file(string name);						//Otwiera plik z procesu
	void close_file(string name);						//Zamyka plik z procesu
	void file_info(string name);						//Wyświetla informacje o pliku
	void show_cataloge();								//Pokazuje wpisy katalowgowe
	void show_block(int nr);							//Pokazuje zawartość bloku pamięci
};
extern Planist planist;