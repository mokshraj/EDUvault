#include "csv_module.h"
#include "csv_module.c"
#include "getsinput.c"
#define MAIN_FILE
#include "main.c"
int main(){
    buffer a;
    csvhead(&a,CSV_FILE);
    char *str;
    compile_and_run("display.c");
    printf("\n\n");
    printf("Enter the Reg ID of record to be edited : ");
    getsinput(&str,11);
    int check,line=csvfind(&a,str,1);
    printf("\n%d\n",line);
    if(line == 0){
        printf("Reg Id not Found!\nexiting to menu .....\n");
        return 0;
    }
    csvreadlineat(&a,line);
    choice:
    printf("----: Choose Field to be Updated :----");
    printf("\n");
    int choice;
    printf("\n1.Name\n2.Gender\n3.Age\n4.Course\n5.Sem\n6.Percentage\n7.Contact\n8.Email\n9.City\n0.exit\n\nEnter choice no. here : ");
    getsinput(&str,2);
    if(sscanf(str,"%d",&choice)){
        if(choice){}
        else{
            printf("existing to menu ......\n");
            return 0;
        }
    }
    else{
        printf("Invalid input!\n");
        goto choice;
    }
    printf("\nOriginal Value : %s\n",a.buffer[choice]);
    printf("\nInput value to be updated : ");
    switch (choice)
    {
    case 1:
        getsinput(&str,MAX_NAME);
        break;
    case 2:
        getsinput(&str,MAX_GENDER);
        break;
    case 3:
        getsinput(&str,4);
        if(sscanf(str,"%d",&check)){}
        else{
            printf("Invalid input!\n");
            goto choice;
        }
        break;
    case 4:
        getsinput(&str,MAX_COURSE);
        break;
    case 5:
        getsinput(&str,4);
        if(sscanf(str,"%d",&check)){}
        else{
            printf("Invalid input!\n");
            goto choice;
        }
        break;
    case 6:
        getsinput(&str,5);
        if(sscanf(str,"%f",&check)){}
        else{
            printf("Invalid input!\n");
            goto choice;
        }
        break;
    case 7:
        getsinput(&str,MAX_CONTACT);
        break;
    case 8:
        getsinput(&str,MAX_EMAIL);
        break;
    case 9:
        getsinput(&str,MAX_CITY);
        break;
    default:
        printf("Invalid Input!\n");
        goto choice;
        break;
    }
    char *choice2;
    choice2:
    printf("are you sure ? (Y/n): ");
    getsinput(&choice2,2);
    if(strcmp(choice2,"y") == 0 || strcmp(choice2,"Y") == 0){
        csvwriteat(&a,str,line,choice+1);
    }
    else if(strcmp(choice2,"n") == 0 || strcmp(choice2,"N") == 0){
        goto choice;
    }
    else{
        printf("Invalid Input try again !\n\n");
        goto choice2;
    }
    printf("\n----updated----");
    compile_and_run("display.c");
    free(str);
    return 0;
}