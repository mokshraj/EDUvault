#include "csv_module.h"

int main()
{
    buffer a;
    csvhead(&a,CSV_FILE);
    char *str;
    int i,temp,column;
    int *arr;

    //goto label choice
    choice:
    printf("\nChoose search method : \n1.Search by Other Details.\n2.Search by Reg ID\n3.exit\n");
    getsinput(&str,2);

    if(sscanf(str, "%d", &temp)){}
    else
    {
        printf("Invalid Input try again !\n\n");
        goto choice;
    }

    if (temp == 1)
    {
        printf("\nChoose field to Search in : \n1. Name \n2. Gender \n3. Age \n4. Course \n5. Sem \n6. Contact No. \n7. Email ID \n8. City\n");
        getsinput(&str,2);

        if(sscanf(str, "%d", &temp)){}
        else
        {
            printf("Invalid Input try again !\n\n");
            goto choice;
        }

        if (temp >0 && temp<9){}
        else
        {
            printf("Invalid Input try again !\n\n");
            goto choice;
        }

        switch (temp)
        {
        case 1:
            printf("Enter Name : ");
            getsinput(&str,MAX_NAME);

            column = 2;
            break;
        case 2:
            printf("Enter Gender : ");
            getsinput(&str,MAX_GENDER);

            column = 3;
            break;
        case 4:
            printf("Enter Course : ");
            getsinput(&str,MAX_COURSE);

            column = 5;
            break;
        case 6:
            printf("Enter Contact no. : ");
            getsinput(&str,MAX_CONTACT);

            column = 8;
            break;
        case 7:
            printf("Enter Email ID : ");
            getsinput(&str,MAX_EMAIL);

            column = 9;
            break;
        case 8:
            printf("Enter City : ");
            getsinput(&str,MAX_CITY);

            column = 10;
            break;
        case 3:
            printf("Enter age : ");
            getsinput(&str,3);

            if(sscanf(str, "%d", &temp)){}
            else
            {
                printf("Invalid Input try again !\n\n");
                goto choice;
            }

            column = 4;
            goto findexact;
            break;
        case 5:
            printf("Enter semester : ");
            getsinput(&str,3);

            if(sscanf(str, "%d", &temp)){}
            else
            {
                printf("Invalid Input try again !\n\n");
                goto choice;
            }

            column = 6;
            goto findexact;
            break;
        default:
            printf("Invalid Input try again !\n\n");
            goto choice;
            break;
        }

        csvfindlike(&a,str,column,&arr);

        printf("%-6s %-15s %-8s %-4s %-10s %-4s %-10s %-15s %-25s %-15s\n",
        "RegID", "Name", "Gender", "Age", "Course", "Sem", "Percentage",
        "Contact No.", "Email ID", "City");
        printf("------ --------------- -------- ---- ---------- ---- ---------- --------------- ------------------------- ---------------\n");

        csvlines(&a);

        i = 0;

        while(arr[i]!=-1)
        {
            csvreadlineat(&a,arr[i]);
            printf("%-6s %-15s %-8s %-4s %-10s %-4s %-10s %-15s %-25s %-15s\n",
                a.buffer[0], a.buffer[1],
                a.buffer[2], a.buffer[3],
                a.buffer[4], a.buffer[5],
                a.buffer[6], a.buffer[7],
                a.buffer[8], a.buffer[9]);
            i++;
        }
    }

    else if(temp == 2)
    {
        printf("Enter Reg ID : ");
        getsinput(&str,11);
        column = 1;
        findexact:
        csvfindexact(&a,str,column,&arr);
        printf("%-6s %-15s %-8s %-4s %-10s %-4s %-10s %-15s %-25s %-15s\n",
        "RegID", "Name", "Gender", "Age", "Course", "Sem", "Percentage",
        "Contact No.", "Email ID", "City");
        printf("------ --------------- -------- ---- ---------- ---- ---------- --------------- ------------------------- ---------------\n");
        csvlines(&a);
        i = 0;

        while(arr[i]!=-1)
        {
            csvreadlineat(&a,arr[i]);
            printf("%-6s %-15s %-8s %-4s %-10s %-4s %-10s %-15s %-25s %-15s\n",
                a.buffer[0], a.buffer[1],
                a.buffer[2], a.buffer[3],
                a.buffer[4], a.buffer[5],
                a.buffer[6], a.buffer[7],
                a.buffer[8], a.buffer[9]);

            i++;
        }
    }

    else if(temp == 3)
    {
        printf("\nexiting to menu ....");
        return 0;
    }

    else
    {
        printf("Invalid Input try again !\n\n");
        goto choice;
    }

    free(str);
    free(arr);
    csvheadfree(&a);

    return 0;
}
