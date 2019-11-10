#ifndef _pipelib_h_
#define _pipelib_h_

#include "utils.h"
#include "balan.h"
#include "Stack.h"

const int PIPE_READER_CODE = 0;
const int PIPE_WRITER_CODE = 1;
const int INPUT_LENGTH = 63;
const char DATA_INPUT_FILE[25] = "input.txt";
const char DATA_OUTPUT_FILE[25] = "output.txt";

void parentProcessing(int* fds) {
  char* inputString = malloc(sizeof(char) * INPUT_LENGTH);
  char outputString[INPUT_LENGTH];

  inputString = readFile(DATA_INPUT_FILE);
  // send
  write(*(fds + PIPE_WRITER_CODE), inputString, INPUT_LENGTH);
  wait(NULL);
  read(*(fds + PIPE_READER_CODE), outputString, INPUT_LENGTH);

  close(*(fds + PIPE_READER_CODE));
  close(*(fds + PIPE_WRITER_CODE));

  writeFile(outputString, DATA_OUTPUT_FILE);
  exit(0);
}

void childProcessing(int* fds) {
  char* pipeDataInput = malloc(INPUT_LENGTH * sizeof(char));
  char* pipeDataOutput = malloc(INPUT_LENGTH * sizeof(char));

  read(*(fds + PIPE_READER_CODE), pipeDataInput, INPUT_LENGTH);
  pipeDataInput = trim(pipeDataInput);
  // Calculate the expression
  // char* s = "((23424-424324*2+3)+(4*(24-3)-(4-7))-23423/(-34))";
  // char* s = "(23424-424324*2+(3+(4*(24-3)-(4-7)))-23423/(-34))";
  // char* s = "23424-424*2-(12-3)";
  // char* s = "(1-(1+0.1*2)+1)";

  double result = expression(pipeDataInput);
  pipeDataOutput = doubleToString(result);
  printf("\n%f\n", result);
  write(*(fds + PIPE_WRITER_CODE), pipeDataOutput, INPUT_LENGTH);

  close(*(fds + PIPE_READER_CODE));
  close(*(fds + PIPE_WRITER_CODE));
  exit(0);
}


#endif
