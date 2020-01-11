﻿#pragma once

#include <array>
#include <vector>
#include <queue>
#include <map>

#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <algorithm>
#include <sstream>
#include <string>

/*Struktura z informacją o tym, czy dana strona znajduje się w RAM i jaką ramkę zajmuje. Wykorzystywana do tablicy stronic w PCB.*/
class PageInfo {
public:
	int frame; //Numer ramki, którą zajmuje dana stronica
	bool bit;  //Wartość mówiąca, czy stronica znajduje się w RAM
	PageInfo();
	PageInfo(int frame, bool bit);
};

class Page {
public:
	std::array<char, 16> data;
	Page();
	Page(std::string s);
};

class Memory{
public:
	Memory();
	
	/*---------RAM---------*/

	//Zmienne
	std::array<char, 256> RAM = { ' ' };

	// Mapa do wyszukiwania stronnic procesów
	//      RAMKA           PID  STRONA
	std::map<int, std::pair<int, int>> Frames;

	//Funkcje - nie jestem pewny, które się przydadzą

	/*Zapisuje liczbę we wskazane miejsce w RAMie.
	nr - numer komórki RAM
	data - zmienna do zapisania*/
	void insert_to_ram(int nr, int data, int PID);

	/*Pobranie danej ramki z RAMu*/
	std::array<char, 16> get_frame(int nr);

	/*Pobranie liczby o podanej ilości cyfr z RAMu.
	nr - numer komórki RAM
	size - ilość cyfr*/
	int get_data(int nr, int size, int PID);

	/*Ustawianie zawartości komórki*/
	void set(int address, int val, int PID);

	/*Pobieranie zawartości komórki*/
	char get(int address, int PID);

	/*Wyświetla daną ramkę.*/
	void show_frame(int nr);

	/*Wyświetla cały RAM.*/
	void show_ram();

	/*---------Virtual---------*/

	// Kolejka FIFO dla algorytmu wymiany stronnic
	std::queue<int> FIFO;

	// Plik wymiany
	std::map<int, std::vector<Page>> PageFile;

	// Obsługa ładowania stron do RAM
	void PageHandler(int address, int PID);

	// Ładowanie programu do pliku stronnicowania
	// Na razie do testowania std::string ale to zależy od syetmu plików
	void LoadProgram(std::string kod, int PID);

	// Funkcja ładująca program Init
	void SetupInitProcess();

	/*Tworzy wskaźnik do tablicy stronic procesu znajdującej się w PCB.
	Używana przy tworzeniu nowego procesu.
	pid  - ID procesu*/
	void CreatePageTable(int PID);

	/*Wyświetla tablicę stron danego procesu.*/
	void ShowPageTable(int PID);

	/*Wyświetla strony danego procesu.*/
	void ShowPages(int PID);

	/*Wyświetla strony w pliku wymiany.*/
	void ShowPageFile();

	/*Wyświetla kolejkę FIFO.*/
	void ShowQueue();
};