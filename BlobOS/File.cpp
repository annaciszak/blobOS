
#include"File.h"

struct inode
{
	int size_int_byte;


	//pierwsze dwa to nr bloki danych 
	//nast�pny to nr bloku indeksowego
	vector<int> number;

	inode() {
		size_int_byte = 0;
	}

	void change_size(string name) {
		/*fstream plik(name.c_str(), ios::app);

		if (plik)
		{

			plik.seekg(0, plik.end);
			int length = plik.tellg();
			plik.seekg(0, plik.beg);

			cout << "Pom " << length << endl;
			size_int_byte = length;
		}


		plik.close();*/
	}

};



FileManager FM;

//Tworzy plik o podanej nazwie, umieszcza go w katalogu
void FileManager::create_file(string name) {

	inode file_node;
	pair<string, inode> file(name, file_node);

	cataloge.push_back(file);

	//zamek???

}

//Wyszukuje pusty blok pami�ci
int FileManager::free_block() {
	for (int i = 0; i < 32; i++) {
		if (disc[i].free == 0)return i;
	}
	cout << "Brak wolnego bloku" << endl;
	return -1;
}

//Odszukuje plik o danej nazwie w katalogu i zwraca jego numer
int FileManager::find_file(string name) {

	int pom = -1;
	for (int i = 0; i < cataloge.size(); i++) {
		if (cataloge[i].first == name) {
			pom = i;
			//cout << "ZNALEZIONO plik" << endl;
		}

	}
	//if (pom == -1)cout << "Nie znaleziono pliku" << endl;
	return pom;
}

//Zapisuje tekst do pami�ci dla podanego pliku 
//Dzia�a do 32 bajt�w
void FileManager::save_data_to_file(string name, string text) {
	int pom = find_file(name);
	unsigned int length = text.size();

	if (length <= 32) {
		int nr = FileManager::free_block();
		cataloge[pom].second.number.push_back(nr);
		//zapis danych
		for (int i = 0; i < length; i++) {
			disc[cataloge[pom].second.number[0]].block[i] = text[i];
		}
		cataloge[pom].second.size_int_byte = length;
		disc[nr].free = 1;
	}

	if (length <= 64 || length > 32) {
		int nr1 = free_block();
		int nr2 = free_block();
		cataloge[pom].second.number.push_back(nr1);
		cataloge[pom].second.number.push_back(nr2);
		//zapis danych


		disc[nr1].free = 1;
		disc[nr2].free = 1;
	}
	//if z blokiem indeksowym
}

//W sumie teraz to chyba nic nie robi, zobacz� potem czy nie b�d� tego potrzebowa�a do czego� innego
void FileManager::edit_file(string name, string text) {
	//edytor tekstu od Ani

	save_data_to_file(name, text);

	
}


// Po podaniu nazwy pliku zwraca stringa z jego zawarto�ci�
string FileManager::show_file(string name) {
	string text;
	int pom = -1;
	for (int i = 0; i < cataloge.size(); i++) {
		if (cataloge[i].first == name) {
			pom = i;
			cout << "ZNALEZIONO plik" << endl;
		}

	}
	if (pom == -1)cout << "Nie znaleziono pliku" << endl;
	int length = cataloge[pom].second.size_int_byte;
	if (length<= 32) {
		
		
		for (int i = 0; i < length; i++) {
			text.push_back(disc[cataloge[pom].second.number[0]].block[i]);
		}

	}
	cout << "??? " << text << endl;

	return text;
}


//Jeszcze nie dzia�a
void FileManager::add_to_file() {
	string name;
	string text;
	cout << "Podaj nazw� pliku ";
	cin >> name;
	name += ".txt";

	int pom = -1;
	for (int i = 0; i < cataloge.size(); i++) {
		if (cataloge[i].first == name) {
			pom = i;
		}
	}



	//czy ma pokazywa� co juz jest w plkiu?
	ofstream plik(name.c_str(), ios::app);
	if (plik) {
		unsigned char znak;
		do
		{
			getline(cin, text);
			plik << text;
			/*if ((int)text.back == 13) {

				plik << endl;
			}
			else {
				plik << text;
			}*/
			znak = _getch();
			cout << znak;
			if (znak != 27)plik << znak;
			static_cast <int>(znak);
			while (_kbhit())
			{
				getline(cin, text);
				plik << text;
				znak = _getch();
				cout << znak;
				if (znak != 27)plik << znak;
				static_cast <int>(znak);
			}
			//std::cout << std::endl;


		} while (znak != 27); //ESC

	}
	else
	{
		cout << "BLAD" << endl;
	}
	plik.close();

	cataloge[pom].second.change_size(cataloge[pom].first);

	if (pom != -1)
		cout << "3=" << cataloge[pom].second.size_int_byte << endl;
	else {
		cout << "Nie ma takiego pliku";
	}
	//dodawanie czego� na ko�cu pliku
}


//Jeszcze nie usuwa, b�dzie mie� nazw� pliku jako argument
void FileManager::delete_file() {
	/*
	vector<pair<string, inode>>::iterator i = FileManager::cataloge.begin();
	string name;
	cout << "Podaj nazw� pliku ";
	cin >> name;
	name += ".txt";
	for (auto e : cataloge) {
		if (e.first == name) {
			break;
		}
		i++;
	}

	FileManager::cataloge.erase(i);
	//remove(name.c_str());
	if (remove(name.c_str()) == 0)
		printf("Usunieto pomyslnie plik.");
	else
		printf("Nie udalo sie skasowac pliku.");

		*/
	//usuni�cie fizycznego pliku
}



