#include "csv_module.h"

// read_csv function here
int read_csv(const char *filename, Record rec[], int *count)
{
    FILE *fp = fopen(filename, "r");
    if (!fp)
    {
        perror("Error opening file");
        return -1;
    }

    char line[1024];
    *count = 0;

    // Skip header line
    fgets(line, sizeof(line), fp);

    while (fgets(line, sizeof(line), fp))
    {
        line[strcspn(line, "\r\n")] = 0; // remove newline char

        // updated sscanf format string to trim spaces
        sscanf(line, "%d,%[^,],%[^,],%d,%[^,],%d,%f,%[^,],%[^,],%[^\n]",
               &rec[*count].regid, rec[*count].name,
               rec[*count].gender, &rec[*count].age,
               rec[*count].course, &rec[*count].sem,
               &rec[*count].percent, rec[*count].contact,
               rec[*count].email, rec[*count].city);

        (*count)++;
    }

    fclose(fp);
    return 0;
}

// display_all function
void display_all(const Record rec[], int count)
{
    printf("%-6s %-15s %-8s %-4s %-10s %-4s %-10s %-15s %-25s %-15s\n",
           "RegID", "Name", "Gender", "Age", "Course", "Sem", "Percentage",
           "Contact No.", "Email ID", "City");

    printf("------ --------------- -------- ---- ---------- ---- ---------- --------------- ------------------------- ---------------\n");

    for (int i = 0; i < count; i++)
    {
        printf("%-6d %-15.15s %-8s %-4d %-10s %-4d %-10.2f %-15s %-25s %-15s\n",
               rec[i].regid, rec[i].name,
               rec[i].gender, rec[i].age,
               rec[i].course, rec[i].sem,
               rec[i].percent, rec[i].contact,
               rec[i].email, rec[i].city);
    }
}

int main()
{
    Record rec[MAX_STUDENTS];
    int count = 0;

    if (read_csv(CSV_FILE, rec, &count) != 0)
    {
        printf("Failed to read from %s\n", CSV_FILE);
        return 1;
    }

    if (count == 0)
    {
        printf("No student records found.\n");
        return 0;
    }

    printf("\n--- Student Records ---\n\n");
    display_all(rec, count);

    return 0;
}
