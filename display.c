#include "csv_module.h"

// read_csv function here
int read_csv(const char *filename, Record records[], int *count)
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
        line[strcspn(line, "\r\n")] = 0; // Remove newline character

        // Updated sscanf format string to trim spaces
        sscanf(line, "%d,%[^,],%[^,],%d,%[^,],%d,%f,%[^,],%[^,],%[^\n]",
               &records[*count].regid, records[*count].name,
               records[*count].gender, &records[*count].age,
               records[*count].course, &records[*count].sem,
               &records[*count].percent, records[*count].contact,
               records[*count].email, records[*count].city);

        (*count)++;
    }

    fclose(fp);
    return 0;
}

// display_all function
void display_all(const Record records[], int count)
{
    printf("%-6s %-15s %-8s %-4s %-10s %-4s %-10s %-15s %-25s %-15s\n",
           "RegID", "Name", "Gender", "Age", "Course", "Sem", "Percentage",
           "Contact No.", "Email ID", "City");

    printf("------ --------------- -------- ---- ---------- ---- ---------- --------------- ------------------------- ---------------\n");

    for (int i = 0; i < count; i++)
    {
        printf("%-6d %-15s %-8s %-4d %-10s %-4d %-10.2f %-15s %-25s %-15s\n",
               records[i].regid, records[i].name,
               records[i].gender, records[i].age,
               records[i].course, records[i].sem,
               records[i].percent, records[i].contact,
               records[i].email, records[i].city);
    }
}

int main()
{
    Record records[MAX_STUDENTS];
    int count = 0;

    if (read_csv(CSV_FILE, records, &count) != 0)
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
    display_all(records, count);

    return 0;
}
