#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void csvhead(buffer * a, char *filename) {
    a->filename = strdup(filename);
    a->csv = fopen(filename, "r+");
    if (!a->csv) {
        perror("Error opening file");
    }
    a->buffer = NULL;
    a->size = 0;
    a->line = 1;
    a->arrsize = 0;
}
void csvheadfree(buffer *a){
    for(int i = 0; i < a->arrsize; i++){
        free(a->buffer[i]);
    }
    free(a->buffer);
    fclose(a->csv);
    a->buffer = NULL;
    a->size = 0;
    free(a->filename);
}
void clearbuffer(buffer *a){
    for(int i = 0; i < a->arrsize; i++){
        free(a->buffer[i]);
    }
    free(a->buffer);
    a->buffer = NULL;
}
void csvseek(buffer *a ,int offset, int x){
    if (x == 1){
        fseek(a->csv, offset, SEEK_SET);
        a->line = 1;
    }
    else if(x == 0){
        fseek(a->csv, offset, SEEK_CUR);
    }
    else if(x == 2){
        fseek(a->csv, offset, SEEK_END);
    }
    else{
        perror("Invalid seek option");
        return;
    }
}
void csvreadline(buffer *a) {
    char ch = fgetc(a->csv);
    if (ch == EOF) {
        return;
        }
    a->arrsize=0;
    a->size = 1;
    char *buffer = (char*)malloc(a->size);
    if (!buffer) {
        perror("Error assigning memory");
        return;
    }
    while(ch != '\n' && ch != EOF && ch != '\r') {
        buffer = (char *)realloc(buffer, a->size + 1);
        if (!buffer) {
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
    while(str != NULL) {
        a->buffer = (char **)realloc(a->buffer, (a->arrsize + 1) * sizeof(char*));
        if (!a->buffer) {
            perror("Error reallocating memory");
            free(buffer);
            return;
        }
        a->buffer[a->arrsize] = strdup(str);
        if (!a->buffer[a->arrsize]) {
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
void csvreadlineat(buffer *a,int line){
    int i;
    a->line = 1;
    csvseek(a,0,1);
    for(i = 0;i<line;i++){
        csvreadline(a);
    }
}
void csvwriteline(buffer *a, char *str, int line) {
    int i, prevline = a->line;
    csvseek(a, 0, 1);
    // Open a temporary file for writing
    FILE *temp = fopen("temp.csv", "w+");
    if (!temp) {
        perror("Error opening temporary file");
        return;
    }
    // Read and copy lines up to the target line
    char ch;
    for (i = 0; i < line - 1; i++) {
        while ((ch = fgetc(a->csv)) != '\n' && ch != EOF) {
            fputc(ch, temp);
        }
        if (ch == '\n') {
            fputc('\n', temp);
        }
    }
    // Write the new line
    fprintf(temp, "%s\n", str);
    // Skip the line being replaced in the original file
    while ((ch = fgetc(a->csv)) != '\n' && ch != EOF);
    // Copy the remaining lines from the original file to the temporary file
    while ((ch = fgetc(a->csv)) != EOF) {
        fputc(ch, temp);
    }
    // Close the original file and replace its contents with the temporary file
    fclose(a->csv);
    a->csv = fopen(a->filename, "w+");
    if (!a->csv) {
        perror("Error reopening original file");
        fclose(temp);
        return;
    }
    rewind(temp);
    while ((ch = fgetc(temp)) != EOF) {
        fputc(ch, a->csv);
    }
    fclose(temp);
    // Reopen the original file in read/write mode
    a->csv = freopen(a->filename, "r+", a->csv);
    if (!a->csv) {
        perror("Error reopening file");
        return;
    }
    // Restore the previous line position
    csvseek(a, 0, 1);
    for (i = 0; i < prevline - 1; i++) {
        csvreadline(a);
    }
}
void csvwriteat(buffer *a,char *str,int line,int column){
    if (line < 1 || column < 1) {
        perror("Invalid line or column number");
        return;
    }
    int i,total_size = 1;
    csvreadlineat(a,line);

    if (column-1 < a->arrsize && a->buffer[column-1]) {
        free(a->buffer[column-1]);
    }
    
    a->buffer[column-1] = strdup(str);
    if (!a->buffer[column-1]) {
        perror("Error duplicating string");
        return;
    }
    char *arr = (char *)malloc(1);
    if (!arr) {
        perror("Memory allocation failed");
        return;
        
    }
    arr[0] = '\0';
    for (i = 0; i < a->arrsize; i++) {
        total_size += strlen(a->buffer[i]) + 1;
        arr = (char *)realloc(arr, total_size);
        if (!arr) {
            perror("Memory reallocation failed");
            return;
        }
        strcat(arr, a->buffer[i]);
        if (i < a->arrsize - 1) strcat(arr, ",");
    }

    csvwriteline(a, strdup(arr), line);
    free(arr);
}
void csvsize(buffer *a){
    csvseek(a,0,1);
    a->total_size = 0;
    char ch;
    while ((ch = fgetc(a->csv)) != EOF) {
        a->total_size++;
    }
    csvreadlineat(a,a->line+1);
}
void csvlines(buffer *a){
    a->total_lines = 1;
    char ch;
    csvseek(a,0,1);
    while ((ch = fgetc(a->csv)) != EOF) {
        if (ch == '\n') {
            a->total_lines++;
        }
    }
    csvreadlineat(a,a->line+1);
}