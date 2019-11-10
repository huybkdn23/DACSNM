#ifndef _Stack_h_
#define _Stack_h_

struct Stack {
    char* str;
    int STRING_LENGTH;
    struct Stack* next;
};

struct Stack* create() {
    struct Stack *stack = NULL;
    return stack;
}

void pushIntoStack(struct Stack **first, char* str) {
    struct Stack *p = (struct Stack*) malloc(sizeof(struct Stack));
    p->str = strdup(str);
    p->STRING_LENGTH = strlen(str);
    p->next = *first;
    *first = p;
}

struct Stack* popFromStack(struct Stack **first) {
    struct Stack *p = *first;
    *first = p->next;
    p->next = NULL;
    return p;
}

bool isEmpty(struct Stack *first) {
    return first == NULL;
}

void showStack(struct Stack* first) {
    struct Stack *p = first;
    while(p != NULL) {
        printf("%s\n", p->str);
        p = p->next;
    }
}
#endif