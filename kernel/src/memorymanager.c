#include "ram.h"
#include "pcb.h"
#include "memorymanager.h"
#include "kernel.h"
// global BackingStore page counter.
int INCREMENT = -1;

int countTotalPages(FILE *f) {
  char lineBuffer[1024];
  int lineCounter = 0;
  int numPages = 0;
  while (!feof(f)) {
    if (lineCounter % 4 == 0) {
      numPages++;
    }
    if (fgets(lineBuffer, 1023, f) != NULL) {
      lineCounter++;
    }
  }

  return numPages;
}

void loadPage(int pageNumber, FILE *f, int frameNumber) {
	char lineBuffer[1024];

  for (int i = 0; i < 4; i++) {
   	if (fgets(lineBuffer, 1023, f)) {
			int cell = (4 * frameNumber) + i;
			memSet(cell, lineBuffer);
		}
	}
	
	fclose(f);
}

int findFrame() {
  for (int i = 0; i < 40; i += 4) {
    if (memGet(i) == NULL) {
      return i / 4;
    }
  }

  return -1;
}

int frameInPCB(int frameNo, struct PCB *p) {
	int inPCB = 0;
	for (int i = 0; i < 10; i++) {
		// Check if the victim frame is owned by the PCB
		if (p->pageTable[i] == frameNo) {
			inPCB = 1;
		} 
	}

	return inPCB;
}

// if findFrame returns -1
// then we must find some random frame 
// and 
int findVictim(struct PCB *p) {
	// init random number generator
	time_t t;
	srand((unsigned) time(&t));
	
	int frameNo = rand() % 10;
	int counter = 0;

		while(frameInPCB(frameNo, p)) {
			frameNo++;
			counter++;
			// All of the frames are full...
			if (counter == 10) {
				printf("Memory is full.");
				return -1;
			}
	
			if (frameNo > 9) {
				frameNo = 0;
			}		
		}

	return frameNo;
}

int updatePageTable(struct PCB *p, int pageNumber, int frameNumber, int victimFrame) {
	p->pageTable[pageNumber] = frameNumber;
	return 0;
}

int launcher(FILE *p) {
  int BUFFER_SIZE = 1024;
  int linecounter = 0;

  FILE *page = NULL;

  char linebuffer[1024];

	int startPage = INCREMENT + 1;

	while (fgets(linebuffer, sizeof(linebuffer), p)) {
    if (linecounter % 4 == 0) {

      INCREMENT++;

      if (page != NULL) fclose(page);

      char buffer1[64] = "BackingStore/";
      char buffer2[32];

      sprintf(buffer2, "PAGE_%d", INCREMENT);

      char* filename = strcat(buffer1, buffer2);

      page = fopen(filename, "w");
    }
    
    // read a line from the source file.
    // write the same line to the bs file.
    fprintf(page, linebuffer, BUFFER_SIZE - 1);
    
    linecounter++;
  }

	int endPage = INCREMENT;
  // this will make sure new programs start on a fresh page.

  // close the source file for others to read.
  fclose(p);
	fclose(page);

	int n;

	if (startPage == endPage) {
		n = 1;
	} else {
		n = 2;
	}

	int numPages = endPage - startPage + 1;
	struct PCB *pcb = makePCB(startPage, numPages);

	// load the first 2 pages into RAM

	char filename[64];

	for (int i = 0; i < n; i++) {
		int pageNumber = startPage + i;
		sprintf(filename, "BackingStore/PAGE_%d", pageNumber);

		FILE *f = fopen(filename, "r");

		int frame = findFrame();
		int victim = -1;
		if (frame >= 0) {
			loadPage(pageNumber, f, frame);
		} else {
			victim = findVictim(pcb);
			frame = victim;
			loadPage(pageNumber, f, frame);
		}

		updatePageTable(pcb, i, frame, victim);
	}

	// The PC should not be 0, this would mean frame 0.
	// In fact it should be the first cell in the first frame that holds a page for this program.

	pcb->PC = pcb->pageTable[0] * 4;

	enQueue(pcb);

	return 0;
}

void freeFrames(int frame) {
	if (frame < 0) return;
	for (int i = 0; i < 4; i++) {
		int cellNumber = frame * 4 + i;
		memSet(cellNumber, NULL);
	}	
}
