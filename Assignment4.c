#include <stdio.h>
#include <stdlib.h>

/* Define page table as dynamic structure containing virtual page and page frame
   and initialize variable as pointer to structure */
struct node{
  int vp;
  int pf;
}*pt = NULL;

typedef struct node entry;

// Declare global var's
int mm_size, page_size;
int num_entries;
int policy;
FILE *test;


void set_params()
{
  /* Declare local var's */

  /* Prompt for main memory size, page size, and replacement policy */
  fscanf(test,"%d", &mm_size);
  printf("Enter main memory size (words): %d",mm_size);
  fscanf(test,"%d", &page_size);
  printf("Enter page size (words/page): %d",page_size);
  fscanf(test,"%d", &policy);
  printf("Enter replacement policy (0=LRU, 1=FIFO): %d",policy);

  num_entries = mm_size/page_size;

  /* Allocate and initialize page table based on number of entries */
  pt = (entry*)malloc(num_entries * sizeof(entry));
  //initialize each value with for loop, index's are 0-3
  int i;
  for(i = 0; i < 4; i++){
    pt[i].vp = -1;
    pt[i].pf = -1;
  }
	printf("Virtual Address Mapping\n");
	printf("--------------------------------------\n");
	printf("NEW CONFIGURATION\n");
	printf("*** Main Memory Size: %d", mm_size);
	printf("\n*** Page Size: %d", page_size);
	printf("\n*** Number of Frames: %d", num_entries);
	printf("\n*** Replacement Algorithm: ");
	
	if (policy == 0) {
	 printf("LRU\n\n"); 
	}
	else if (policy == 1) {
		printf("FIFO\n\n");
	}

return;
}


void mapping()
{
    int VMAddress;//From user
	int realAddress;//Generated
	int vp;
	int offset;

	fscanf("%d", &VMAddress);
	printf("Enter virtual memory address to access: %d", VMAddress);


	//Calculate offset and virtual page
	offset = VMAddress % page_size;
	vp = VMAddress / page_size;

	int i;
	for (i = 0; i < num_entries; i++) {
		//if there is a hit (page in pt)
		if (pt[i].vp == vp) {
			realAddress = (pt[i].pf * page_size) + offset;
			//LRU
			if (policy == 0) {
				int pfHolder;//placeholder for pf
				int vpHolder;//placeholder for vp
				int g;
				for (g = i; g < num_entries - 1; g++) {
					//Shift page table at index of hit
					pfHolder = pt[g].pf;
					vpHolder = pt[g].vp;
					pt[g] = pt[g + 1];
					pt[g + 1].pf = pfHolder;
					pt[g + 1].vp = vpHolder;
				}
			}
			printf("\nVirtual Address: %d maps to physical address %d\n", VMAddress, realAddress);
			i = num_entries-1;
		}
		//if theres a blank space
		else if (pt[i].vp == -1) {
			//map virtual page to generated virtual page
			pt[i].vp = vp;
			//map pf to current index
			pt[i].pf = i;
			printf("***Page Fault!\n");
			printf("***Virtual Address %d maps to physical address %d", VMAddress,realAddress);
			//set index to last element
			i = num_entries - 1;
		}
		else if (i == num_entries - 1) {//condition for last element
			pt[0].vp = vp;
			int pfHolder;//placeholder for pf
			int vpHolder;//placeholder for vp
			int h;
			for (h = 0; h < num_entries - 1; h++) {
				//shift page table from beginning
				pfHolder = pt[h].pf;
				vpHolder = pt[h].vp;
				pt[h] = pt[h + 1];
				pt[h + 1].pf = pfHolder;
				pt[h + 1].vp = vpHolder;
			}
			printf("***Page Fault! \n");
			printf("***Virtual Address %d maps to physical address %d", VMAddress,realAddress);
		}
	}
	for (i = 0; i < num_entries; i++) {
		if (pt[i].pf != -1 && pt[i].vp != -1) {
			printf("***VP %d ----> ", pt[i].vp);
			printf("PF %d ", pt[i].pf);
			printf("\n");
		}
	}

return;
}

int main(int argc, char *argv[]){

int choice = 0;
/* until program exits, print menu, select choice via switch statement and call appropriate function*/

while(choice != 3){

	printf("\n-----------------------------------------\n");
	printf("1)Enter Parameters\n2)Map Virtual Address\n3)Quit\n");

  scanf("%d", &choice);

  switch(choice){

    case 1: set_params();
    break;
    case 2: mapping();
    break;
    case 3: printf("Quitting... \n");
    break;
    default: printf("Invalid Entry \n");
  }
}
return 1;
}
