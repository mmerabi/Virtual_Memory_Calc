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
int framenum;
FILE *test;

//Declaring a struct for the virtual memory similar to the cache assignment
struct ptable {
	int frame;
	int vpage;

}*mainmem;


//Function for displaying the parameter properties

void parameterdisplay() {
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
void parameters() {
	//taking in input
	// policy 0=LRU, 1=FIFO
	fscanf(test,"%d", &mainmemsize);
	fscanf(test,"%d", &pagesize);
	fscanf(test,"%d", &policy);
	framenum = 0;
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

void shiftTable(int index){
	int i;
	for (i = index; i >0; i--){
		mainmem[i].vpage = mainmem[i-1].vpage;
    	mainmem[i].frame = mainmem[i-1].frame;
    }
}

//Function for mapping the addresses
void mapadd() {
	int vmadd;
	int physadd;
	int vpage;
	int offset;
	int h;

	printf("Enter virtual memory address to access: ");
	fscanf(test,"%d", &vmadd);

	//Offset and virtual page calculation

	offset = vmadd % pagesize;	
	vpage = vmadd / pagesize;

	int i = 0;
	while(i<pages) {
		//if there is a hit (page in memory)
		if (mainmem[i].vpage == vpage) {
			physadd = (mainmem[i].frame * pagesize) + offset;
			h = mainmem[i].frame;
			//LRU
			if (policy == 0) {
				shiftTable(i);
				mainmem[0].frame = h;
				mainmem[0].vpage = vpage;
			}
			printf("\n***Page Hit\n");
			printf("***Virtual Address: %d maps to physical address %d\n", vmadd, physadd);
			i = pages - 1;
		}
		//if theres a blank space
		else if (mainmem[i].vpage == -1) {
			shiftTable(i);
			//setting virtual page 
			mainmem[0].vpage = vpage;
			//keeping track of framenum
			mainmem[0].frame = framenum;

			printf("\n\n***Page fault!\n");
			physadd = (mainmem[0].frame * pagesize) + offset;
			printf("***Virtual Address: %d maps to physical address %d\n", vmadd, physadd);
			//set index to last element
			i = pages - 1;
			if (framenum != pages){
				framenum++;
			}
		}
		//If statement for the last index of page
		else if (i == pages - 1) {
			h = mainmem[i].frame;
			shiftTable(i);
			mainmem[0].vpage = vpage;
			mainmem[0].frame = h;
			//For loop for shifting the addresses

			printf("\n***Page Fault! \n");
			physadd = (mainmem[0].frame * pagesize) + offset;
			printf("***Virtual Address: %d maps to physical address %d\n", vmadd, physadd);

		}
		i++;
	}

	//For loop for printing out the current addresses
	for (i = framenum-1; i >=0; i--) {
		if (mainmem[i].frame != -1 && mainmem[i].vpage != -1) {
			printf("***VP %d ----> ", mainmem[i].vpage);
			printf("PF %d ", mainmem[i].frame);
			printf("\n");
		}
	}
}

//MAIN FUNCTION
int main(int argc, char *argv[]){
	framenum = 0;
	test = fopen(argv[1], "r");
	int choice = 0;

	printf("NAME: Michael Merabi\nVirtual Memory\nCOMP222 Fall 2018\n8:00AM-9:30");
	printf("\n-----------------------------------------\n");
	printf("1)Enter Parameters\n2)Map Virtual Address\n3)Quit\n");

	while (choice != 3) {
		printf("\n");

		//Choice input
		fscanf(test,"%d", &choice);
		if (choice == 1) {
			parameters();
			memallocate();
			parameterdisplay();
		}
		else if (choice == 2){
			mapadd();
		}
		else if (choice == 3){
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