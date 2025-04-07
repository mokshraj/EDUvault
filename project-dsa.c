#include<stdio.h>
#include<stdlib.h>
#include<string.h>

FILE *rec;

typedef struct {
    int line;
    FILE *csv;
    char **buffer;
    int size;
    int arrsize;
} buffer;
void csvhead(buffer * a, char *filename) {
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
    for(int i = 0; i < a->size; i++){
        free(a->buffer[i]);
    }
    free(a->buffer);
    fclose(a->csv);
    a->buffer = NULL;
    a->size = 0;
}
void clearbuffer(buffer *a){
    for(int i = 0; i < a->size; i++){
        free(a->buffer[i]);
    }
    free(a->buffer);
    a->buffer = NULL;
    a->size = 0;
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
    a->arrsize=0;
    int size = 1;
    char *buffer = (char*)malloc(size);
    if (!buffer) {
        perror("Error assigning memory");
        return;
    }

    char ch = fgetc(a->csv);
    while(ch != '\n' && ch != EOF) {
        buffer = (char *)realloc(buffer, size + 1);
        if (!buffer) {
            perror("Error reassigning memory");
            return;
        }
        buffer[size-1] = ch;
        size++;
        ch = fgetc(a->csv);
    }
    buffer[size-1] = '\0';

    // Free previous buffer contents
    clearbuffer(a);

    char *str = strtok(buffer, ",");
    while(str != NULL) {
        a->buffer = (char **)realloc(a->buffer, (a->size + 1) * sizeof(char*));
        if (!a->buffer) {
            perror("Error reallocating memory");
            free(buffer);
            return;
        }
        a->buffer[a->size] = strdup(str);
        if (!a->buffer[a->size]) {
            perror("Error duplicating string");
            free(buffer);
            return;
        }
        a->arrsize++;
        a->size++;
        str = strtok(NULL, ",");
    }
    a->line++;
    free(str);
    free(buffer);
}
void csvreadlineat(buffer *a,int line){
    int i;
    csvseek(a,0,1);
    for(i = 0;i<line;i++){
        csvreadline(a);
    }
}
void csvwriteline(buffer *a,char * str,int line){
    int i,prevline=a->line;
    csvseek(a,0,1);
    for(i = 0;i<line-1;i++){
        csvreadline(a);
    }
    fprintf(a->csv,"%s\n",str);
    csvseek(a,0,1);
    for(i = 0;i<prevline-1;i++){
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
int main() {
    buffer a;
    csvhead(&a,"rec.csv");
    printf("%d\n",a.line);

    csvreadline(&a);
    for(int i = 0; i < a.size; i++) {
        printf("%s ", a.buffer[i]);
    }
    printf("\n");
    printf("%d\n",a.line);

    csvreadline(&a);
    for(int i = 0; i < a.size; i++) {
        printf("%s ", a.buffer[i]);
    }
    printf("\n");
    printf("%d\n",a.line);

    csvwriteline(&a,"hello,world",4);

    csvreadline(&a);
    for(int i = 0; i < a.size; i++) {
        printf("%s ", a.buffer[i]);
    }
    printf("\n");
    printf("%d\n",a.line);

    csvreadline(&a);
    for(int i = 0; i < a.size; i++) {
        printf("%s ", a.buffer[i]);
    }
    printf("\n");
    printf("%d\n",a.line);

    csvreadlineat(&a,1);
    for(int i = 0; i < a.size; i++) {
        printf("%s ", a.buffer[i]);
    }
    printf("\n");
    printf("%d\n",a.line);

    csvwriteat(&a,"hello",1,2);

    csvreadlineat(&a,1);
    for(int i = 0; i < a.size; i++) {
        printf("%s ", a.buffer[i]);
    }
    printf("\n");
    printf("%d\n",a.line);
    csvheadfree(&a);
    return 0;
}