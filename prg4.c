/*
Project #4: - Virtual Memory Assignment
Michael Merabi
CS222 - Fall 2018
Meeting time: T/Th 8-9:15am
*/

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>


//Global Declarations
int mainmemsize=0;
int policy = 2;
int pagesize = 0;
int pages = 0;

//Declaring a struct for the virtual memory similar to the cache assignment
struct ptable {
	int frame;
	int vpage;

}*mainmem;

//function for displaying introduction, looks cleaner
void intro() {
	//Prompt for input
	printf("NAME: Michael Merabi\nVirtual Memory\nCOMP222 Fall 2018\n8:00AM-9:30");
	printf("\n-----------------------------------------\n");
	printf("1)Enter Parameters\n2)Map Virtual Address\n3)Quit\n");
}


//Function for displaying the parameter properties

void parameterdisplay() {
	printf("\n\nNAME: Michael Merabi\n");
	printf("Virtual Address Mapping\n");
	printf("--------------------------------------\n");
	printf("NEW CONFIGURATION\n");
	printf("*** Main Memory Size: %i", mainmemsize);
	printf("\n*** Page Size: %i", pagesize);
	printf("\n*** Number of pages: %i", pages);
	printf("\n*** Replacement Algorithm: ");
	if (policy == 0) { printf("LRU\n\n"); }
	else if (policy == 1) {
		printf("FIFO\n\n");
	}
	
}


//function for inputting parameters
void parameters(*FILE) {
	//taking in input
	// policy 0=LRU, 1=FIFO
	fscanf(test,"%i", &mainmemsize);
	fscanf(test,"%i", &pagesize);
	fscanf(test,"%i", &policy);

}


//Function for exiting program
void aexit() {
	printf("\n***Program Terminated Normally\n\n");
	system("pause");
	exit(EXIT_FAILURE);
}


//Function to allocate the memory, and annex the numbers
void memallocate() {
	int i = 0;
	pages = (mainmemsize / pagesize);
	mainmem = (struct ptable *)malloc(pages * sizeof(struct ptable));
	//Setting everything to -1, a number we won't be working with to help with logic later
	for (i = 0; i < pages; i++) {
		mainmem[i].frame = -1;
		mainmem[i].vpage = -1;

	}
}
//Function for mapping the addresses
void mapadd(*FILE) {
	int vmadd;
	int physadd;
	int vpage;
	int offset;
	int shiftpframe;
	int shiftvp;
	int g;
	int h;
	int framenum = 0;

	printf("Enter virtual memory address to access: ");
	fscanf(test,"%d", &vmadd);

	//Offset and virtual page calculation

	offset = vmadd % pagesize;	
	vpage = vmadd / pagesize;

	int i;
	for (i = 0; i < pages; i++) {
		//if there is a hit (page in memory)
		if (mainmem[i].vpage == vpage) {
			physadd = (mainmem[i].frame * pagesize) + offset;
			//LRU
			if (policy == 0) {
				//For loop shift page table
				for (g = i; g < pages - 1; g++) {
					
					shiftpframe = mainmem[g].frame;
					shiftvp = mainmem[g].vpage;
				//	mainmem[g] = mainmem[g + 1];
					mainmem[g + 1].frame = shiftpframe;
					mainmem[g + 1].vpage = shiftvp;
				}

			}
			printf("***Page Hit\n");
			printf("***Virtual Address: %d maps to physical address %d\n", vmadd, physadd);
			i = pages - 1;
		}
		//if theres a blank space
		else if (mainmem[i].vpage == -1) {
			//setting virtual page 
			mainmem[i].vpage = vpage;
			//keeping track of framenum
			mainmem[i].frame = framenum;
			mainmem[g + 1].frame = shiftpframe;
			mainmem[g + 1].vpage = shiftvp;
			printf("***Page fault!\n");
			physadd = (mainmem[i].frame * pagesize) + offset;
			printf("***Virtual Address: %d maps to physical address %d\n", vmadd, physadd);
			//set index to last element
			i = pages - 1;
			framenum++;
		}
		//If statement for the last index of page
		else if (i == pages - 1) {
			mainmem[0].vpage = vpage;

			//For loop for shifting the addresses
			for (h = 0; h < pages - 1; h++) {
				
				shiftpframe = mainmem[h].frame;
				shiftvp = mainmem[h].vpage;
				mainmem[h] = mainmem[h + 1];
				mainmem[h + 1].frame = shiftpframe;
				mainmem[h + 1].vpage = shiftvp;
			}
			printf("***Page Fault! \n");
			physadd = (mainmem[i].frame * pagesize) + offset;
			printf("***Virtual Address: %d maps to physical address %d\n", vmadd, physadd);

		}

	}

	//For loop for printing out the current addresses
	for (i = 0; i < pages; i++) {
		if (mainmem[i].frame != -1 && mainmem[i].vpage != -1) {
			printf("***VP %d ----> ", mainmem[i].vpage);
			printf("PF %d ", mainmem[i].frame);
			printf("\n");
		}
	}
}

//MAIN FUNCTION
int main(int argc, char *argv[]){
	FILE *test;
	test = fopen(argv[1], "r");

	for (;;) {
		printf("\n");
		int choice = 0;
		//intro function
		intro();

		//Choice input
		fscanf(test,"%i", &choice);
		switch (choice) {
		case 1:
			parameters(test);
			memallocate(test);
			parameterdisplay();
			break;

		case 2:
			mapadd(test);
			break;

		case 3:
			aexit();
		}
		
	}
}
//Leftover
void intro();
void parameters();
void parameterdisplay();
void aexit();
void memallocate();
void mapadd();