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

#include<stdio.h>  // for standard input/output
#include<stdlib.h> // for using dynamic array struct
#include<string.h> // for string functions
#include <ctype.h>

// structure
typedef struct
{
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
    float percent;
    char name[MAX_NAME], gender[MAX_GENDER];
    char course[MAX_COURSE], contact[MAX_CONTACT];
    char email[MAX_EMAIL], city[MAX_CITY];
} Record;



// csv operations
int read_csv(const char *filename, Record rec[], int *count);
void display_all(const Record rec[], int count);
int csvfind(buffer *a,char *str,int column);




// function declarations

// to clear buffer array from memory
void clearbuffer(buffer *a)
{
    for(int i = 0; i < a->arrsize; i++)
        free(a->buffer[i]);

    free(a->buffer);
    a->buffer = NULL;
}


// to initialize file and values to default
void csvhead(buffer * a, char *filename)
{
    a->filename = strdup(filename);
    a->csv = fopen(filename, "r+");

    if (!a->csv)
        perror("Error opening file");

    a->buffer = NULL;
    a->size = 0;
    a->line = 1;
    a->arrsize = 0;
}


// to close file and free all dynamically initialized value
void csvheadfree(buffer *a)
{
    for(int i = 0; i < a->arrsize; i++)
        free(a->buffer[i]);

    free(a->buffer);
    fclose(a->csv);
    a->buffer = NULL;
    a->size = 0;
    free(a->filename);
}


// to make use of fseek easier
void csvseek(buffer *a ,int offset, int x)
{
    if (x == 1)
    {
        fseek(a->csv, offset, SEEK_SET);
        a->line = 1;
    }

    else if(x == 0)
        fseek(a->csv, offset, SEEK_CUR);

    else if(x == 2)
        fseek(a->csv, offset, SEEK_END);

    else
    {
        perror("Invalid seek option");
        return;
    }
}



// to read current line in csv file
void csvreadline(buffer *a)
{
    char ch = fgetc(a->csv);

    if (ch == EOF)
        return;

    a->arrsize=0;
    a->size = 1;

    char *buffer = (char*)malloc(a->size);

    if (!buffer)
    {
        perror("Error assigning memory");
        return;
    }

    while(ch != '\n' && ch != EOF && ch != '\r')
    {
        buffer = (char *)realloc(buffer, a->size + 1);

        if (!buffer)
        {
            perror("Error reassigning memory");
            return;
        }

        buffer[a->size-1] = ch;
        a->size++;
        ch = fgetc(a->csv);
    }

    buffer[a->size-1] = '\0';

    if (ch == '\r') {
        ch = fgetc(a->csv);  // Read and discard \n
    }

    // Free previous buffer contents
    clearbuffer(a);

    char *str = strtok(buffer, ",");

    while(str != NULL)
    {
        a->buffer = (char **)realloc(a->buffer, (a->arrsize + 1) * sizeof(char*));

        if (!a->buffer)
        {
            perror("Error reallocating memory");
            free(buffer);
            return;
        }

        a->buffer[a->arrsize] = strdup(str);

        if (!a->buffer[a->arrsize])
        {
            perror("Error duplicating string");
            free(buffer);
            return;
        }

        a->arrsize++;
        str = strtok(NULL, ",");
    }

    a->line++;
    free(str);
    free(buffer);
}


// to read specified line in csv
void csvreadlineat(buffer *a,int line)
{
    int i;
    a->line = 1;
    csvseek(a,0,1);

    for(i = 0;i<line;i++)
        csvreadline(a);
}


// to write at specified line
void csvwriteline(buffer *a, char *str, int line)
{
    int i, prevline = a->line;

    csvseek(a, 0, 1);

    // Open a temporary file for writing
    FILE *temp = fopen("temp.csv", "w+");

    if (!temp)
    {
        perror("Error opening temporary file");
        return;
    }

    // Read and copy lines up to the target line
    char ch;

    for (i = 0; i < line - 1; i++)
    {
        while ((ch = fgetc(a->csv)) != '\n' && ch != EOF)
            fputc(ch, temp);

        if (ch == '\n')
            fputc('\n', temp);
    }

    // Write the new line
    fprintf(temp, "%s\n", str);

    // Skip the line being replaced in the original file
    while ((ch = fgetc(a->csv)) != '\n' && ch != EOF);

    // Copy the remaining lines from the original file to the temporary file
    while ((ch = fgetc(a->csv)) != EOF)
        fputc(ch, temp);

    // Close the original file and replace its contents with the temporary file
    fclose(a->csv);
    a->csv = fopen(a->filename, "w+");

    if (!a->csv)
    {
        perror("Error reopening original file");
        fclose(temp);
        return;
    }

    rewind(temp);

    while ((ch = fgetc(temp)) != EOF)
    {
        fputc(ch, a->csv);
    }

    fclose(temp);

    // Reopen the original file in read/write mode
    a->csv = freopen(a->filename, "r+", a->csv);

    if (!a->csv)
    {
        perror("Error reopening file");
        return;
    }

    // Restore the previous line position
    csvseek(a, 0, 1);

    for (i = 0; i < prevline - 1; i++)
        csvreadline(a);
}


// to write at specified column of specified line
void csvwriteat(buffer *a,char *str,int line, int column)
{
    if (line < 1 || column < 1)
    {
        perror("Invalid line or column number");
        return;
    }

    int i,total_size = 1;
    csvreadlineat(a,line);

    if (column-1 < a->arrsize && a->buffer[column-1])
        free(a->buffer[column-1]);

    a->buffer[column-1] = strdup(str);

    if (!a->buffer[column-1])
    {
        perror("Error duplicating string");
        return;
    }

    char *arr = (char *)malloc(1);

    if (!arr)
    {
        perror("Memory allocation failed");
        return;

    }

    arr[0] = '\0';

    for (i = 0; i < a->arrsize; i++)
    {
        total_size += strlen(a->buffer[i]) + 1;
        arr = (char *)realloc(arr, total_size);

        if (!arr)
        {
            perror("Memory reallocation failed");
            return;
        }

        strcat(arr, a->buffer[i]);

        if (i < a->arrsize - 1) strcat(arr, ",");
    }

    csvwriteline(a, strdup(arr), line);
    free(arr);
}


// to delete specified line
void csvdelete(buffer *a,int line)
{
    int i, prevline = a->line;
    csvseek(a, 0, 1);

    // Open a temporary file for writing
    FILE *temp = fopen("temp.csv", "w+");

    if (!temp)
    {
        perror("Error opening temporary file");
        return;
    }

    // Read and copy lines up to the target line
    char ch;

    for (i = 0; i < line - 1; i++) {
        while ((ch = fgetc(a->csv)) != '\n' && ch != EOF)
            fputc(ch, temp);

        if (ch == '\n')
            fputc('\n', temp);
    }

    // Skip the line being replaced in the original file
    while ((ch = fgetc(a->csv)) != '\n' && ch != EOF);

    // Copy the remaining lines from the original file to the temporary file
    while ((ch = fgetc(a->csv)) != EOF)
        fputc(ch, temp);

    // Close the original file and replace its contents with the temporary file
    fclose(a->csv);
    a->csv = fopen(a->filename, "w+");

    if (!a->csv)
    {
        perror("Error reopening original file");
        fclose(temp);
        return;
    }

    rewind(temp);

    while ((ch = fgetc(temp)) != EOF)
        fputc(ch, a->csv);

    fclose(temp);

    // Reopen the original file in read/write mode
    a->csv = freopen(a->filename, "r+", a->csv);

    if (!a->csv)
    {
        perror("Error reopening file");
        return;
    }

    // Restore the previous line position
    csvseek(a, 0, 1);

    for (i = 0; i < prevline - 1; i++)
        csvreadline(a);
}


// to find the total lines of csv file
void csvlines(buffer *a)
{
    a->total_lines = 0;
    char ch;
    csvseek(a,0,1);
    while ((ch = fgetc(a->csv)) != EOF)
    {
        if (ch == '\n')
            a->total_lines++;
    }

    csvreadlineat(a,a->line+1);
}


// to given string in specified column
int csvfind(buffer *a,char *str,int column)
{
    csvlines(a);
    int line=0, i=0;

    csvseek(a,0,1);

    for(i = 0;i<a->total_lines;i++)
    {
        csvreadline(a);
        if(strcmp(a->buffer[column-1],str)==0)
        {
            line = a->line-1;
            break;
        }
    }
    return line;
}

//lowercase function for csvfindlike
void lowercase(char *a){
    if((int)(*a)<91 && (int)(*a)>64){
        *a = *a + 32;
    }
}

// to find string in specified column
void csvfindlike(buffer *a, char *str, int column, int **arr)
{
    int arrsize = 0;
    *arr = NULL;  // Initialize to NULL so realloc works properly

    csvlines(a);

    char *bufcpy = NULL;

    csvseek(a, 0, 1);

    for (int i = 0; i < a->total_lines; i++)
    {
        csvreadline(a);

        bufcpy = strdup(a->buffer[column - 1]);

        for(int i = 0;bufcpy[i]!='\0';i++){ // covert to lowercase
            lowercase(&bufcpy[i]);
        }

        for(int i = 0;str[i]!='\0';i++){ // covert to lowercase
            lowercase(&str[i]);
        }

        if (strncmp(bufcpy, str, strlen(str)) == 0)
        {
            arrsize++;
            *arr = (int *)realloc(*arr, arrsize * sizeof(int));
            (*arr)[arrsize - 1] = a->line - 1;
        }
    }

    free(bufcpy);

    // Add sentinel (zero) at end
    arrsize++;
    *arr = (int *)realloc(*arr, arrsize * sizeof(int));
    (*arr)[arrsize - 1] = -1;
}


// to find exact string in specified column
void csvfindexact(buffer *a, char *str, int column, int **arr)
{
    int arrsize = 0;
    *arr = NULL;  // Initialize to NULL so realloc works properly

    csvlines(a);

    csvseek(a, 0, 1);

    for (int i = 0; i < a->total_lines; i++)
    {
        csvreadline(a);

        if (strcmp(a->buffer[column - 1], str) == 0)
        {
            arrsize++;
            *arr = (int *)realloc(*arr, arrsize * sizeof(int));
            (*arr)[arrsize - 1] = a->line - 1;
        }
    }

    // Add sentinel (zero) at end
    arrsize++;
    *arr = (int *)realloc(*arr, arrsize * sizeof(int));
    (*arr)[arrsize - 1] = -1;
}


// to find the size of csv
void csvsize(buffer *a)
{
    csvseek(a,0,1);
    a->total_size = 0;
    char ch;

    while ((ch = fgetc(a->csv)) != EOF)
        a->total_size++;

    csvreadlineat(a,a->line+1);
}

// to get input from user and it excludes new line or end of file i.e \n
void getsinput(char **str, int maxsize)
{
    char *a, ch = fgetc(stdin);
    int size = 1;

    a = (char*)malloc(size);

    if (!a)
        perror("Error assigning memory");

    while(ch != '\n' && ch != EOF && ch != '\r')
    {
        a = (char *)realloc(a, size + 1);

        if (!a)
            perror("Error reassigning memory");

        a[size-1] = ch;
        size++;
        ch = fgetc(stdin);
    }

    if(size > maxsize)
        a[maxsize] = '\0';

    else
        a[size-1] = '\0';

    *str = strdup(a);
    free(a);
}

#endif
