#include "csv_module.h"
#include "csv_module.c"
#include "getsinput.c"
#define MAIN_FILE
#include "main.c"

int main(){
    buffer a;
    csvhead(&a,CSV_FILE);
    char *str=NULL;
    char *choice;
    int temp;

    compile_and_run("display.c");
    printf("\n");
    regid:
    printf("Enter Reg ID to delete : ");
    getsinput(&str,11);
    if(sscanf(str, "%d", &temp)){}
    else{
        printf("Invalid Input try again !\n\n");
        goto regid;
    }
    int line = csvfind(&a,str,1);
    if(line){}
    else{
        printf("\nReg ID does not exist!\n\n");
        return 0;
    }
    choice:
    printf("are you sure ? (Y/n): ");
    getsinput(&choice,2);
    if(strcmp(choice,"y") == 0 || strcmp(choice,"Y") == 0){
        csvdelete(&a,line);
    }
    else if(strcmp(choice,"n") == 0 || strcmp(choice,"N") == 0){
        return 0;
    }
    else{
        printf("Invalid Input try again !\n\n");
        goto choice;
    }
    free(str);
    free(choice);
    csvheadfree(&a);
    printf("----updated----");
    compile_and_run("display.c");
    return 0;
}