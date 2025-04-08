#ifndef CSV_MODULE_H
#define CSV_MODULE_H

#define MAX_NAME 100
#define MAX_STUDENTS 100
#define MAX_COURSE 50
#define MAX_CONTACT 15
#define MAX_EMAIL 150
#define MAX_CITY 50

#define CSV_FILE "records.csv"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// Structure definition
typedef struct {
    char *filename;
    int line;
    FILE *csv;
    char **buffer;
    int total_size;
    int total_lines;
    int size;
    int arrsize;
} buffer;
//RegNo,Name,Gender,Age,Course,Sem,Marks,Contact,Email,City
typedef struct
{
    int regid, age, sem;
    char name[MAX_NAME], gender[10], course[MAX_COURSE], contact[MAX_CONTACT], email[MAX_EMAIL], city[MAX_CITY];;
    float percent;
} Record;

// csv operations
int read_csv(const char *filename, Record records[], int *count);

// function declarations
void display_all(const Record records[], int count);
void csvhead(buffer *a, char *filename);
void csvheadfree(buffer *a);
void clearbuffer(buffer *a);
void csvseek(buffer *a, int offset, int x);
void csvreadline(buffer *a);
void csvreadlineat(buffer *a,int line);
void csvwriteline(buffer *a, char *str, int line);
void csvwriteat(buffer *a,char *str,int line,int column);
void csvsize(buffer *a);
void csvlines(buffer *a);

#endif