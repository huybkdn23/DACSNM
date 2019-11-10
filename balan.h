#ifndef _balan_h_
#define _balan_h_
#include "utils.h"
#include "Stack.h"

bool is_matched_expression_with_operators_and_operations(char* s) {
  int count1 = 0, count2 = 0;
  int STRING_LENGTH = strlen(s);
  for (int i = 0; i < STRING_LENGTH; i++) {
    char c = *(s + i);
    if (is_operator(c)) {
      count2++;
      if (is_number(subString(s, 0, i))) count1++;
      s = subString(s, i+1, STRING_LENGTH);
      STRING_LENGTH = strlen(s);
      if (is_number(s)) {
        if(is_operator( *(s + 0) )) count2++;
        count1++; break;
      }
      i = -1;
    }
  }
  if (count2 >= count1) return false;
  return true;
}

int compare_parenth(char* s, const int STRING_LENGTH) {
  //Return 0: ( = )
  //Return 1: ( not found )
  //Return -1: ( < or > )
  //Check number of '(' is equal with ')'
  int count1 = 0, count2 = 0;
  for (int i = 0; i < STRING_LENGTH; i++) {
    if (s[i] == '(') count1++;
    else if (s[i] == ')') count2++;
  }
  if (count1 == count2) {
    if (count1 != 0) return 0; //equal but not raw
    else return 1; //raw
  }
  return -1;
}

double calculate_raw(char* a, char* operator, char* b) {
  double parseA = stringToDouble(a), parseB = stringToDouble(b);
  switch (*operator) {
  case '+':
    return parseA + parseB;
  case '-':
    return parseA - parseB;
  case '*':
    return parseA * parseB;
  case '/':
    return parseA / parseB;
  default:
    printf("Invalid syntax!");
    return 0;
  }
}

char* standardized(char* s) {
  int STRING_LENGTH = strlen(s);
  //duyet tu trai sang de dua ve dang chuan
		for (int i = 0; i < STRING_LENGTH - 1; i++) {
			char c[1] = {*(s + i)};
			char n[1] = {*(s + i + 1)};
      			
			switch (*c) {
			case '+':
				if (*n == '+' || *n == '-') {
          s = concat( concat( subString(s, 0, i), n ), subString(s, i+2, STRING_LENGTH));
          STRING_LENGTH = strlen(s);
					// s = s.substring(0, i) + n + s.substring(i+2, s.length());
					i--;
				}
				else if (*n >= '0' && *n <= '9') continue;
				else return "Invalid syntax! STD";
				break;
			case '-': 
				if (*n == '+') {
          // s = subString(s, 0, i) + '-' + subString(s, i+2, s.length()); 
          s = concat( concat( subString(s, 0, i), c ), subString(s, i+2, STRING_LENGTH));
          STRING_LENGTH = strlen(s);
          i--;
        }
				else if (*n == '-') {
          // s = s.substring(0, i) + '+' + s.substring(i+2, s.length()); 
          s = concat( concat( subString(s, 0, i), "+" ), subString(s, i+2, STRING_LENGTH));
          STRING_LENGTH = strlen(s);
          i--;
        }
				else if (*n >= '0' && *n <= '9') continue;
				else return "Invalid syntax!";
				break;
			case '*': 
			case '/':
				if (*n >= '0' && *n <= '9') continue;
        //quay lai tim phep tinh truoc do de tinh * hoac /, vi du 5+3/-2 => tinh 3/-2
				else if (*n == '-' || *n == '+') {
          //i la vi tri toan tu hien tai
          //j la vi tri toan tu truoc do
          //k la vi tri toan tu tiep theo
					for (int j = i-1; j >= 0; j--) {
						char tmp = *(s + j);
						if (is_operator(tmp)) {
							for (int k = i+2; k < STRING_LENGTH; k++) {
								tmp = *(s + k);
								if (is_operator(tmp)) {
                  s = concat( concat( subString(s, 0, j+1), doubleToString(calculate_raw( subString(s, j+1, i), c, subString(s, i, k) ))), 
                  subString(s, k, STRING_LENGTH)
                  );
                  STRING_LENGTH = strlen(s);
									i = j;
									break;
								}
								else if (k == STRING_LENGTH - 1) {
                  s = concat( subString(s, 0, j+1), doubleToString(calculate_raw( subString(s, j+1, i), c, subString(s, i + 1, STRING_LENGTH) )));
                  STRING_LENGTH = strlen(s);
									i = j;
									break;
								}
							}
							break;
						}
					}
				}
			default:
				break;
			}
		}
		return s;
	}

double expression_raw(char* s) {
  s = removeCharacters(s);
  while(!is_matched_expression_with_operators_and_operations(s)) s = standardized(s);
  // printf("Expression_raw: %s\n", s);
  int STRING_LENGTH = strlen(s);
  //Assume the string is validated
  struct Stack *operations = create();
  struct Stack *operators = create();
  int end = STRING_LENGTH;
  double total = 0;
  char *currentOperator, *nextOperator, *currentOperation, *nextOperation;
  for (int i = STRING_LENGTH - 1; i >= 0; i--) {
    char operator[1] = {*(s + i)};
    if (*operator == '+' || 
      *operator == '-' || 
      *operator == '*' || 
      *operator == '/' ) {
      pushIntoStack(&operations, subString(s, i + 1, end));
      pushIntoStack(&operators, operator);
      end = i;
    }
    if (i == 0) pushIntoStack(&operations, subString(s, i, end));
  }
  // System.out.println(operations + " " + operators);
  currentOperator = popFromStack(&operators)->str;
  while (!isEmpty(operators)) {
    //Check if the string only exist one operator
    nextOperator = popFromStack(&operators)->str;
    //*/
    if (strcmp(currentOperator, "*") == 0 || strcmp(currentOperator, "/") == 0 ) {
      char* operation1 = popFromStack(&operations)->str;
      char* operation2 = popFromStack(&operations)->str;
      total = calculate_raw(operation1, currentOperator, operation2);
      pushIntoStack(&operations, doubleToString(total));
      // strcpy(currentOperator, nextOperator);
      currentOperator = strdup(nextOperator);
    }
    //+- va sau do la */
    else if (strcmp(nextOperator, "*") == 0 || strcmp(nextOperator, "/") == 0) {
      currentOperation = popFromStack(&operations)->str;
      nextOperation = popFromStack(&operations)->str;
      char *nextNextOperation = popFromStack(&operations)->str;
      double prioritize = calculate_raw(nextOperation, nextOperator, nextNextOperation);
      // printf("\nPUSH %s %s %s", doubleToString(prioritize), currentOperation, currentOperator);
      pushIntoStack(&operations, doubleToString(prioritize));
      pushIntoStack(&operations, currentOperation);
      pushIntoStack(&operators, currentOperator);
      if (isEmpty(operators)) {
        break;
      }
      nextOperator = popFromStack(&operators)->str;
    }
    //+- va sau do la +-
    else {
      // printf("\n-----------STACK BEFORE-----------\n");
      // showStack(operations);
      // showStack(operators);
      // printf("\n-----------STACK BEFORE-----------\n");
      char* operation1 = popFromStack(&operations)->str;
      char* operation2 = popFromStack(&operations)->str;
      total = calculate_raw(operation1, currentOperator, operation2);
      pushIntoStack(&operations, doubleToString(total));
      // printf("\n-----------STACK AFTER-----------\n");
      // showStack(operations);
      // showStack(operators);
      // printf("\n-----------STACK AFTER-----------\n");
      // strcpy(currentOperator, nextOperator);
      currentOperator = strdup(nextOperator);
    }
  }
  if (isEmpty(operators)) {
    char* operation1 = popFromStack(&operations)->str;
    char* operation2 = popFromStack(&operations)->str;
    total = calculate_raw(operation1, currentOperator, operation2);
    }
  return total;
}

double expression(char* s) {
  s = trim(s);
  int STRING_LENGTH = strlen(s);
  if (is_number(s)) return stringToDouble(s);
  // printf("\nSTRING = %s", s);
  //Check if the ')' is in front of the string
  if (!is_valid(s, STRING_LENGTH)) {
    perror("Invalid syntax expression.\n");
    exit(0);
  }
  //Check the first character is a number
  int check = compare_parenth(s, STRING_LENGTH);
  // printf("#CHECK = %d", check);
  if (check == 1) return expression_raw(s);
  else if (check == -1) {perror("Invalid syntax expression.\n"); exit(0);};
  //---------------------------------------------
  //---------------------------------------------
  int count1 = 0, count2 = 0;
  char* expressionString = malloc(MAX * sizeof(char));
  char* subExpression = malloc(STRING_LENGTH * sizeof(char));
  //duyet tu trai sang de tinh toan bieu thuc trong ngoac tra ve mot so
  for (int i = 0; i < STRING_LENGTH; i++) {
    if (s[i] == '(') {
      count1++;
      for (int j = i+1; j < STRING_LENGTH; j++) {
        if (s[j] == '(') count1++;
        else if(s[j] == ')') count2++;
        if(count1 == count2) {
          // strncpy(subExpression, s+i+1, j - (i+1));
          subExpression = subString(s, i+1, j);
          double subTotal = expression(subExpression);
          // printf("\nSUB = %s\tTOTAL = %f", subExpression, subTotal);
          char* tmp = doubleToString(subTotal);
          expressionString = concat(expressionString, tmp);
          count1 = 0;
          count2 = 0;
          i = j;
          break;
        }
      }
    }
    else  expressionString = appendCharacter(expressionString, s[i]);
  }
  return expression(expressionString);
}
#endif