#include "csv_module.h"
#define MAIN_FILE
#include "main.c"

#define LINE_LEN 1024
#define MAX_RECORDS 1000
#define MAX_FIELDS 10

char headers[][20] = {
    "RegID", "Name", "Gender", "Age", "Course",
    "Sem", "Percentage", "Contact", "Email", "City"};

void get_field_value(const char *line, int field_index, char *output)
{
    char sorted[LINE_LEN];
    strcpy(sorted, line);
    char *token = strtok(sorted, ",");

    for (int i = 0; i < field_index && token != NULL; i++)
        token = strtok(NULL, ",");

    if (token != NULL)
    {
        // trim leading/trailing spaces // moves the trimmed string to the beginning
        while (isspace((unsigned char)*token)) token++;
        char *end = token + strlen(token) - 1;
        while (end > token && isspace((unsigned char)*end)) end--;
        *(end + 1) = '\0';

        strcpy(output, token);
    }

    else
        output[0] = '\0';
}

void merge(char arr[][LINE_LEN], int l, int m, int r, int field_index, int ascending)
{
    int n1 = m - l + 1;
    int n2 = r - m;

    char L[n1][LINE_LEN], R[n2][LINE_LEN];

    for (int i = 0; i < n1; i++)
        strcpy(L[i], arr[l + i]);

    for (int j = 0; j < n2; j++)
        strcpy(R[j], arr[m + 1 + j]);

    int i = 0, j = 0, k = l;

    while (i < n1 && j < n2)
    {
        char field1[100], field2[100];
        get_field_value(L[i], field_index, field1);
        get_field_value(R[j], field_index, field2);

        int cmp;
        if (field_index == 0 || field_index == 3 || field_index == 5 || field_index == 6)
        {
            double d1 = atof(field1), d2 = atof(field2);
            cmp = (d1 > d2) - (d1 < d2);
        }

        else
            cmp = strcmp(field1, field2);

        if ((ascending && cmp <= 0) || (!ascending && cmp >= 0))
            strcpy(arr[k++], L[i++]);

        else
            strcpy(arr[k++], R[j++]);
    }

    while (i < n1) strcpy(arr[k++], L[i++]);
    while (j < n2) strcpy(arr[k++], R[j++]);
}

void merge_sort(char arr[][LINE_LEN], int l, int r, int field_index, int ascending)
{
    if (l < r)
    {
        int m = l + (r - l) / 2;
        merge_sort(arr, l, m, field_index, ascending);
        merge_sort(arr, m + 1, r, field_index, ascending);
        merge(arr, l, m, r, field_index, ascending);
    }
}

int main()
{
    FILE *fp = fopen(CSV_FILE, "r");
    if (!fp)
    {
        perror("Error opening file");
        return -1;
    }

    FILE *sorted = fopen("sorted.csv", "w+");
    if (!sorted)
    {
        printf("Error creating sorted.csv\n");
        fclose(fp);
        return 1;
    }

    char header_line[LINE_LEN];
    fgets(header_line, sizeof(header_line), fp);
    fprintf(sorted, "%s", header_line);  // write headers to sorted.csv

    char records[MAX_RECORDS][LINE_LEN];
    int count = 0;

    while (fgets(records[count], sizeof(records[count]), fp))
    {
        records[count][strcspn(records[count], "\n")] = '\0'; // removes newline
        count++;
    }
    fclose(fp);

    printf("Select field to sort by:\n");
    for (int i = 0; i < MAX_FIELDS; i++)
        printf("%d. %s\n", i + 1, headers[i]);

    int choice;
    printf("Enter your choice: ");
    scanf("%d", &choice);

    if (choice < 1 || choice > MAX_FIELDS)
    {
        printf("Invalid choice.\n");
        return 1;
    }

    int ascending = 1;
    if (choice == 1)
    {
        // RegID already in ascending order
        printf("RegID is already in ascending order. Sorting in descending.\n");
        ascending = 0;
    }
    else
    {
        int order;
        printf("Choose sorting order:");
        printf("\n1. Ascending");
        printf("\n2. Descending");
        printf("\nEnter Choice: ");
        scanf("%d", &order);

        if (order == 2) //(strcmp(order, "desc") == 0)
            ascending = 0;
    }

    merge_sort(records, 0, count - 1, choice - 1, ascending);

    for (int i = 0; i < count; i++)
        fprintf(sorted, "%s\n", records[i]);

    fclose(sorted);

    rename("records.csv", "org.csv");     // backup of org csv
    //remove("records.csv");                // delete org csv
    rename("sorted.csv", "records.csv");  // rename to org csv

    // Display sorted CSV
    compile_and_run("display.c");

    // Replace original file
    remove("records.csv");                // delete sorted csv
    rename("org.csv", "records.csv");     // rename to org csv
    remove("temp.csv");                   // delete temp csv

    return 0;
}
