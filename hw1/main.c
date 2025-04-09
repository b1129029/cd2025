#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TOKEN_LEN 100

typedef struct Node {
    char data;
    int count;
    struct Node *next;
} Node;

Node *head = NULL;

void updateNode(char c) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (!new_node) {
        return;
    }
    new_node->data = c;
    new_node->next = NULL;

    if (head == NULL) {
        head = new_node;
    } else {
        Node *temp = head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_node;
    }
}

void freeList() {
    Node *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

const char *keywords[] = {"int", "main", "if", "else", "while"};
const char *keyword_tokens[] = {"TYPE_TOKEN", "MAIN_TOKEN", "IF_TOKEN", "ELSE_TOKEN", "WHILE_TOKEN"};

int is_keyword(const char *str) {
    for (int i = 0; i < 5; i++) {
        if (strcmp(str, keywords[i]) == 0) return i;
    }
    return -1;
}

int is_letter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

int is_digit(char c) {
    return c >= '0' && c <= '9';
}

void turn_token() {
    Node *curr = head;
    while (curr != NULL) {
        char c = curr->data;

        if (c == ' ' || c == '\n' || c == '\t') {
            curr = curr->next;
            continue;
        }

        if (is_letter(c)) {
            char buffer[MAX_TOKEN_LEN] = {0};
            int idx = 0;
            buffer[idx++] = c; 
            curr = curr->next;

            while (curr != NULL && (is_letter(curr->data) || is_digit(curr->data))) {
                buffer[idx++] = curr->data;
                curr = curr->next;
            }
            buffer[idx] = '\0';
            int kw = is_keyword(buffer);
            if (kw >= 0) {
                printf("%s: %s\n", buffer, keyword_tokens[kw]);
            } else {
                printf("%s: ID_TOKEN\n", buffer);
            }

        } else if (is_digit(c)) {
            char buffer[MAX_TOKEN_LEN] = {0};
            int idx = 0;
            buffer[idx++] = c;       
            curr = curr->next;

            while (curr != NULL && is_digit(curr->data)) {
                buffer[idx++] = curr->data;
                curr = curr->next;
            }
            buffer[idx] = '\0';
            printf("%s: LITERAL_TOKEN\n", buffer);

        } else {
            char next = (curr->next != NULL) ? curr->next->data : '\0';
            switch (c) {
                case '=':
                    if (next == '=') {
                        printf("==: EQUAL_TOKEN\n");
                        curr = curr->next;
                    } else {
                        printf("=: ASSIGN_TOKEN\n");
                    }
                    break;
                case '>':
                    if (next == '=') {
                        printf(">=: GREATEREQUAL_TOKEN\n");
                        curr = curr->next;
                    } else {
                        printf(">: GREATER_TOKEN\n");
                    }
                    break;
                case '<':
                    if (next == '=') {
                        printf("<=: LESSEQUAL_TOKEN\n");
                        curr = curr->next;
                    } else {
                        printf("<: LESS_TOKEN\n");
                    }
                    break;
                case '+': printf("+: PLUS_TOKEN\n"); break;
                case '-': printf("-: MINUS_TOKEN\n"); break;
                case ';': printf(";: SEMICOLON_TOKEN\n"); break;
                case '(': printf("(: LEFTPAREN_TOKEN\n"); break;
                case ')': printf("): REFTPAREN_TOKEN\n"); break;
                case '{': printf("{: LEFTBRACE_TOKEN\n"); break;
                case '}': printf("}: REFTBRACE_TOKEN\n"); break;
                default: break;
            }
            curr = curr->next;
        }
    }
}

int main() {
    FILE *fp = fopen("cd2025.c", "r");
    if (fp == NULL) {
        return 1;
    }

    char ch;
    while ((ch = fgetc(fp)) != EOF) {
        updateNode(ch);
    }
    fclose(fp);

    turn_token();
    freeList();
    getchar();
    return 0;
}