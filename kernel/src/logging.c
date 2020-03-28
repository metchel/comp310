#include <stdio.h>

int LOGGING_MODE = 1;

void writeLog(char* message) {
  if (LOGGING_MODE == 2) {
    printf("%s", message);
  }
}

void setLoggingMode(int mode) {
  LOGGING_MODE = mode;
}
