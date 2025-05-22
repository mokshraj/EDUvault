#include "csv_module.h"

int diglen(int a)
{
    int len=0;
    while(a!=0)
    {
        a/=10;
        len++;
    }
    return len;
}


void printRecord(buffer *a)
{
    csvlines(a);
    csvreadlineat(a,a->total_lines);
    printf("\n===== Student Record =====\n");
    printf("Registration ID : %s\n", a->buffer[0]);
    printf("Name            : %s\n", a->buffer[1]);
    printf("Gender          : %s\n", a->buffer[2]);
    printf("Age             : %s\n", a->buffer[3]);
    printf("Semester        : %s\n", a->buffer[4]);
    printf("Course          : %s\n", a->buffer[5]);
    printf("Percentage      : %s\n", a->buffer[6]);
    printf("Contact         : %s\n", a->buffer[7]);
    printf("Email           : %s\n", a->buffer[8]);
    printf("City            : %s\n", a->buffer[9]);
    printf("===========================\n");
}


void add(buffer *a,Record rec)
{
    fclose(a->csv);
    a->csv = fopen(CSV_FILE,"r+");
    int ssize=1,asize=1,temp;
    char *arr;
    char *str;

    csvlines(a);
    csvreadlineat(a,a->total_lines);

    sscanf(a->buffer[0],"%d",&temp);

    arr = (char *)malloc(asize);
    arr[0] = '\0';
    str = (char *)malloc(ssize);

    temp++;

    str = (char *)realloc(str,diglen(temp));
    sprintf(str,"%d",temp);
    asize = asize+strlen(str)+1;
    arr = (char *)realloc(arr,asize);
    strcat(arr,str);
    strcat(arr,",");

    ssize = 1 + strlen(rec.name);
    str = (char *)realloc(str,ssize);
    str = strdup(rec.name);
    asize = asize+strlen(str)+1;
    arr = (char *)realloc(arr,asize);
    strcat(arr,str);
    strcat(arr,",");

    ssize = 1 + strlen(rec.gender);
    str = (char *)realloc(str,ssize);
    str = strdup(rec.gender);
    asize = asize+strlen(str)+1;
    arr = (char *)realloc(arr,asize);
    strcat(arr,str);
    strcat(arr,",");

    str = (char *)realloc(str,diglen(rec.age));
    sprintf(str,"%d",rec.age);
    asize = asize+strlen(str)+1;
    arr = (char *)realloc(arr,asize);
    strcat(arr,str);
    strcat(arr,",");

    ssize = 1 + strlen(rec.course);
    str = (char *)realloc(str,ssize);
    str = strdup(rec.course);
    asize = asize+strlen(str)+1;
    arr = (char *)realloc(arr,asize);
    strcat(arr,str);
    strcat(arr,",");

    str = (char *)realloc(str,diglen(rec.sem));
    sprintf(str,"%d",rec.sem);
    asize = asize+strlen(str)+1;
    arr = (char *)realloc(arr,asize);
    strcat(arr,str);
    strcat(arr,",");

    str = (char *)realloc(str,diglen(rec.percent)+2);
    sprintf(str,"%0.2f",rec.percent);
    asize = asize+strlen(str)+1;
    arr = (char *)realloc(arr,asize);
    strcat(arr,str);
    strcat(arr,",");

    ssize = 1 + strlen(rec.contact);
    str = strdup(rec.contact);
    asize = asize + strlen(str) + 1;
    arr = (char *)realloc(arr, asize);
    strcat(arr, str);
    strcat(arr, ",");

    ssize = 1 + strlen(rec.email);
    str = strdup(rec.email);
    asize = asize + strlen(str) + 1;
    arr = (char *)realloc(arr, asize);
    strcat(arr, str);
    strcat(arr, ",");

    //write to csv file
    ssize = 1 + strlen(rec.city);
    str = strdup(rec.city);
    asize = asize + strlen(str) + 1;
    arr = (char *)realloc(arr, asize);
    strcat(arr, str);
    free(str);

    //write to csv
    csvwriteline(a, arr, a->total_lines+1);
    free(arr);
}


int main()
{
    buffer a;
    csvhead(&a,CSV_FILE);
    Record rec;
    char *str = NULL;

    printf("Enter Name: ");
    getsinput(&str,MAX_NAME);
    strncpy(rec.name, str, sizeof(rec.name) - 1);
    rec.name[sizeof(rec.name) - 1] = '\0';

    printf("Enter Gender (M/F): ");
    getsinput(&str,MAX_GENDER);
    strncpy(rec.gender, str, sizeof(rec.gender) - 1);
    rec.gender[sizeof(rec.gender) - 1] = '\0';

    //goto point age
    age:
    printf("Enter Age: ");
    getsinput(&str,3);
    if(sscanf(str, "%d", &rec.age)){}
    else
    {
        printf("Invalid Input try again !\n\n");
        goto age;
    }

    printf("Enter Course: ");
    getsinput(&str,MAX_COURSE);
    strncpy(rec.course, str, sizeof(rec.course) - 1);
    rec.course[sizeof(rec.course) - 1] = '\0';

    //goto point sem
    sem:
    printf("Enter Semester: ");
    getsinput(&str,3);

    if(sscanf(str, "%d", &rec.sem)){}
    else
    {
        printf("Invalid Input try again !\n\n");
        goto sem;
    }

    //goto point percent
    percent:
    printf("Enter Percentage: ");
    getsinput(&str,5);
    if(sscanf(str, "%f", &rec.percent)){}
    else
    {
        printf("Invalid Input try again !\n\n");
        goto percent;
    }

    printf("Enter Contact Number: ");
    getsinput(&str,MAX_CONTACT);
    strncpy(rec.contact, str, sizeof(rec.contact) - 1);
    rec.contact[sizeof(rec.contact) - 1] = '\0';

    printf("Enter Email: ");
    getsinput(&str,MAX_EMAIL);
    strncpy(rec.email, str, sizeof(rec.email) - 1);
    rec.email[sizeof(rec.email) - 1] = '\0';

    printf("Enter City: ");
    getsinput(&str,MAX_CITY);
    strncpy(rec.city, str, sizeof(rec.city) - 1);
    rec.city[sizeof(rec.city) - 1] = '\0';

    add(&a,rec);

    printf("\nDisplaying entered record:\n");
    printRecord(&a);
    csvheadfree(&a);

    remove("temp.csv"); // delete temp csv

    return 0;
}
