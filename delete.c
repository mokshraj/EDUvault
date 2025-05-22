#include "csv_module.h"
#define MAIN_FILE
#include "main.c"

int main()
{
    buffer a;
    csvhead(&a,CSV_FILE);
    char *str=NULL;
    char *choice;
    int temp;

    compile_and_run("display.c");
    printf("\n");

    //goto label regid
    regid:
    printf("Enter Reg ID to delete : ");
    getsinput(&str,11);

    int line = csvfind(&a,str,1); // defined in global to find for RegID

    if(sscanf(str, "%d", &temp)){}
    else
    {
        printf("Invalid Input try again !\n\n");
        goto regid;
    }

    if(line){}
    else
    {
        printf("\nReg ID does not exist!\n\n");
        return 0;
    }

    //goto label choice
    choice:
    printf("Are you sure? (y/n): ");
    getsinput(&choice,2);

    if(strcmp(choice,"Y") == 0 || strcmp(choice,"y") == 0)
        csvdelete(&a,line);

    else if(strcmp(choice,"N") == 0 || strcmp(choice,"n") == 0)
        return 0;

    else
    {
        printf("Invalid Input try again !\n\n");
        goto choice;
    }

    free(str);
    free(choice);
    csvheadfree(&a);

    remove("temp.csv"); // delete temp csv

    printf("----UPDATED----");
    compile_and_run("display.c");

    return 0;
}
