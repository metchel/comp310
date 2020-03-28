#include "ram.h"
#include "pcb.h"
#include "memorymanager.h"
#include "logging.h"

// global BackingStore page counter.
int INCREMENT = 0;

int launcher(FILE *p) {
  writeLog("Launching...");
  
  int BUFFER_SIZE = 1024;
  int linecounter = 0;

  FILE *page = NULL;

  char linebuffer[1024];

  while (!feof(p)) {
    if (linecounter % 4 == 0) {

      INCREMENT++;

      if (page != NULL) fclose(page);

      char buffer1[64] = "BackingStore/";
      char buffer2[32];

      sprintf(buffer2, "PAGE_%d", INCREMENT - 1);

      char* filename = strcat(buffer1, buffer2);

      page = fopen(filename, "w");
    }
    
    // read a line from the source file.
    if (fgets(linebuffer, BUFFER_SIZE - 1, p) != NULL) {
      // write the same line to the bs file.
      fprintf(page, linebuffer, BUFFER_SIZE - 1);
    }
    
    linecounter++;
  }

  // don't put code from different programs in the same page.
  // this will make sure new programs start on a fresh page.
  INCREMENT++;

  // close the source file for others to read.
  fclose(p);

  return 0;
}

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
  char filename[64];
  sprintf(filename, "BackingStore/PAGE_%d", pageNumber);

  f = fopen(filename, "r");
  char lineBuffer[1024];

  for (int i = 0; i < 4; i++) {
    if (fgets(lineBuffer, 1023, f) != NULL) {
      memSet(4*frameNumber + i, lineBuffer);
    }
  }
}

int findFrame() {
  for (int i = 0; i < 1000; i += 4) {
    if (memGet(i) != NULL) {
      return i;
    }
  }

  return -1;
}

// if findFrame returns -1
// then we must find some random frame 
// and 
int findVictim(struct PCB *p) {
  return 0;
}

int updatePageTable(struct PCB *p, int pageNumber, int frameNumber, int victimFrame) {
  return 0;
}
