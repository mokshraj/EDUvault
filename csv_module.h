#ifndef CSV_MODULE_H
#define CSV_MODULE_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
// Structure definition
typedef struct {
    char *filename;
    int line;
    FILE *csv;
    char **buffer;
    int size;
    int arrsize;
} buffer;

// Function declarations
void csvhead(buffer *a, char *filename);
void csvheadfree(buffer *a);
void clearbuffer(buffer *a);
void csvseek(buffer *a, int offset, int x);
void csvreadline(buffer *a);
void csvreadlineat(buffer *a,int line);
void csvwriteline(buffer *a, char *str, int line);
void csvwriteat(buffer *a,char *str,int line,int column);

#endif // CSV_MODULE_H