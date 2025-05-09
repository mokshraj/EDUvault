#ifndef CSV_MODULE_H
#define CSV_MODULE_H

#define MAX_NAME 101
#define MAX_STUDENTS 100
#define MAX_COURSE 51
#define MAX_CONTACT 11
#define MAX_EMAIL 151
#define MAX_CITY 51
#define MAX_GENDER 11

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
    char name[MAX_NAME], gender[MAX_GENDER], course[MAX_COURSE], contact[MAX_CONTACT], email[MAX_EMAIL], city[MAX_CITY];
    float percent;
} Record;

// csv operations
int read_csv(const char *filename, Record records[], int *count);
void display_all(const Record records[], int count);
int csvfind(buffer *a,char *str,int columb);
// function declarations
void csvfindexact(buffer *a, char *str, int columb, int **arr);
void csvfindlike(buffer *a,char *str,int columb, int **arr);
void getsinput(char **str, int maxsize);
void csvhead(buffer *a, char *filename);
void csvheadfree(buffer *a);
void clearbuffer(buffer *a);
void csvseek(buffer *a, int offset, int x);
void csvreadline(buffer *a);
void csvreadlineat(buffer *a,int line);
void csvwriteline(buffer *a, char *str, int line);
void csvwriteat(buffer *a,char *str,int line,int column);
void csvdelete(buffer *a,int line);
void csvsize(buffer *a);
void csvlines(buffer *a);

#endif