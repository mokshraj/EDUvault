#include "csv_module.h"
// Include the implementation file directly
#pragma message("Compiling csv_module.c along with test.c")
#include "csv_module.c"
int main() {
    buffer a;
    csvhead(&a,"rec.csv");
    printf("%d\n",a.line);

    csvreadline(&a);
    for(int i = 0; i < a.arrsize; i++) {
        printf("%s ", a.buffer[i]);
    }
    printf("\n");
    printf("%d\n",a.line);

    csvreadline(&a);
    for(int i = 0; i < a.arrsize; i++) {
        printf("%s ", a.buffer[i]);
    }
    printf("\n");
    printf("%d\n",a.line);

    csvwriteline(&a,"hello,world",4);

    csvreadline(&a);
    for(int i = 0; i < a.arrsize; i++) {
        printf("%s ", a.buffer[i]);
    }
    printf("\n");
    printf("%d\n",a.line);

    csvreadline(&a);
    for(int i = 0; i < a.arrsize; i++) {
        printf("%s ", a.buffer[i]);
    }
    printf("\n");
    printf("%d\n",a.line);

    csvreadlineat(&a,1);
    for(int i = 0; i < a.arrsize; i++) {
        printf("%s ", a.buffer[i]);
    }
    printf("\n");
    printf("%d\n",a.line);

    csvwriteat(&a,"hello",1,2);

    csvreadlineat(&a,1);
    for(int i = 0; i < a.arrsize; i++) {
        printf("%s ", a.buffer[i]);
    }
    printf("\n");
    printf("%d\n",a.line);
    printf(("%s\n",a.filename));
    csvheadfree(&a);
    return 0;
}