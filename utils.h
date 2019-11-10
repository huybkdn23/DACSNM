#ifndef _utils_h_
#define _utils_h_

#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include "Stack.h"
// char operators[13][7] = {"+", "-", "*", "/", "(", ")", "~", "sin", "cos", "tan", "arcsin", "sqrt", "log"};
const int MAX = 100;

char * readFile(const char * filename) {
  const int MAX_LENGTH = 63;
  char * data = malloc(sizeof(char) * MAX_LENGTH);
  int i = 0;
  char ch;
  FILE * fin = fopen(filename, "r");

  if (fin == NULL) {
    perror("Error while opening the file.\n");
    exit(0);
  }

  while((ch = fgetc(fin)) != EOF) {
    data[i] = ch;
    data[i + 1] = '\n';
    i += 1; 
  }
  fclose(fin);
  return data;
}

void writeFile(char* s ,const char * filename) {
  FILE * fout = fopen(filename, "w+");
  fputs(s, fout);
  fclose(fout);
}

char *trim(char *s) {
  while (isspace((unsigned char) *s)) s++;
  if (*s) {
    char *p = s;
    while (*p) p++;
    while (isspace((unsigned char) *(--p)));
    p[1] = '\0';
  }
  return s;
}

bool is_valid(char* s, const int STRING_LENGTH) {
  //Check if the ')' is in front of the string
		bool check = false;
    /* number of elements in `array` = size of `array` in bytes / size of `array[0]` in bytes*/
		for (int i = 0; i < STRING_LENGTH; i++) {
			if (*(s+i) == ')' && check == false) {
				printf("Invalid syntax!");
				return false;
			}
			if (*(s+i) == '(') check = true;
		}
		
		return true;
}

bool is_number(char* s) {
  int operators = 0;
  int dot = 0;
  const int STRING_LENGTH = strlen(s);
  for(int i = 1; i < STRING_LENGTH; i++) {
    if (s[i] == '(' || s[i] == ')') return false;
    if (s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/') operators++;
    else if (s[i] == '.') dot++;
    else if (s[i] >= '0' && s[i] <= '9') continue;
    if (dot == 2 || operators >= 2 || (operators == 1 && s[0] != '-')) return false;
  }
  return true;
}

double stringToDouble(char* s) {
  return atof(s);
}

char* doubleToString(double s) {
  const int NUMBER_DIGITS_AFTER_POINT = 15;
  char* result = malloc(MAX * sizeof(char));
  gcvt(s, NUMBER_DIGITS_AFTER_POINT, result);
  return result;
}

char* concat(char* str1, char* str2) {
  const int LENGTH1 = strlen(str1);
  const int LENGTH2 = strlen(str2);
  const int NEW_LENGTH = LENGTH1 + LENGTH2;
  char* newStr = malloc(NEW_LENGTH * sizeof(char));
  for(int i = 0; i < NEW_LENGTH; i++) {
    if (i < LENGTH1) *(newStr + i) = *(str1 + i);
    else *(newStr + i) = *(str2 + i - LENGTH1);
  }
  return newStr;
}

char* appendCharacter(char* str, char c) {
  const int STRING_LENGTH = strlen(str);
  char* newStr = malloc((STRING_LENGTH + 1) * sizeof(char));
  strcpy(newStr, str);
  *(newStr + STRING_LENGTH) = c;
  return newStr;
}

bool is_operator(char c) {
		return (c == '+' || c == '-' ||c == '*' ||c == '/');
}

char* subString(char* str, int start, int end) {
  const int STRING_LENGTH = strlen(str);
  char* sub = malloc(STRING_LENGTH * sizeof(char));
  strncpy(sub, str + start, end - start);
  return sub;
}

char* removeCharacters(char* str) {
  int STRING_LENGTH = strlen(str);
  for (int i = 0; i < STRING_LENGTH; i++) {
    if ((*(str + i) >= 'A' && *(str + i) <= 'Z') || 
    *(str + i) >= 'a' && *(str + i) <= 'z') {
      str = concat(subString(str, 0, i), subString(str, i+1, STRING_LENGTH));
      STRING_LENGTH = strlen(str);
    }
  }
  return str;
}

//-----------------------------------------------------------------------------------

#endif