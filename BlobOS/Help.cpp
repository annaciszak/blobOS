#include "Help.h"
#include"Shell.h"
#include <iostream>

void Help::cp() {
	set_color(white);
	std::cout << "cp [PROC NAME] [FILENAME] [PARENT PID]" << std::endl;
}

void Help::dp() {
	set_color(white);
	std::cout << "dp -n [PROC NAME]" << std::endl;
	std::cout << "dp -p [PID]" << std::endl;
}

void Help::show() {
	//set_color(white);
	printf(R"EOF(
Usage: show [OPTION]
Show the information about OPTION.

	-pcb [PROC NAME] [FILENAME] [PARENT PID]  display information about the process with this PID
	-pcblist	display the contents of WAITING and RUNNING list
	-pagetable [PROC PID]	display the contents of page table
	-frame [NO FRAME]	display the content of frame
	-pages [PROC PID]	display the content of pages
	-tree		print the tree of processes
	-ram		display the contents of RAM
	-queue		display the contents of FIFO queue
	-pagefile	display the contents of page file
	-lock		display the contents of lock queue
	-catalogue	display the contents of catalogue
	-block [BLOCK]	display the contents of block

Example:
	show -pcb p1 file1 1	Information about the process p1. 

)EOF");
}


void Help::touch() {
	set_color(white);
	printf(R"EOF(
Usage: touch [FILENAME]

A FILE argument that does not exist is created empty. 

	--help	display this help and exit

)EOF");
}

void Help::open() {
	set_color(white);
	printf(R"EOF(
Usage: open [FILENAME]

Open the FILE.

	--help	display this help and exit

)EOF");

}

void Help::close() {
	set_color(white);
	printf(R"EOF(
Usage: close [FILENAME]

Close the FILE.

	--help	display this help and exit

)EOF");

}

void Help::rm() {
	set_color(white);
	printf(R"EOF(
Usage: rm [FILENAME]

Remove the FILE.

	--help	display this help and exit

)EOF");
}

void Help::edit() {
	set_color(white);
	printf(R"EOF(
Usage: edit [FILENAME]

Display the contain of FILE and edit the FILE in text editor.

	--help	display this help and exit

)EOF");
}

void Help::write() {
	set_color(white);
	printf(R"EOF(
Usage: write [FILENAME] [line to add to FILE]

Add the line at the end ???

	--help	display this help and exit

)EOF");
}

void Help::copy() {
	set_color(white);
	printf(R"EOF(
Usage: copy [SOURCE FILENAME1] [DEST FILENAME2]

Copy the contain of SOURCE to DEST.

	--help	display this help and exit

)EOF");
}

void Help::cat() {
	set_color(white);
	printf(R"EOF(
Usage: cat [FILENAME]...

Concatenate FILE(s) to standard output.

With no FILE read standard input. // Mo¿e zrobie

	--help	display this help and exit

)EOF");
}

void Help::fileinfo() {
	set_color(white);
	printf(R"EOF(
Usage: fileinfo [OPTION] [FILENAME]

List information about FILE.

	--all	list information about all FILEs in the directory

	--help	display this help and exit

)EOF");
}

void Help::go() {
	set_color(white);
	std::cout << "Musze pomyslec jak tutaj napisac";
}