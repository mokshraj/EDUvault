#ifndef CSV_FUNC_H
#define CSV_FUNC_H

#define MAX_NAME 100
#define MAX_STUDENTS 100
#define MAX_COURSE 30
#define MAX_CONTACT 15
#define MAX_EMAIL 50
#define MAX_CITY 40

#define CSV_FILE "records.csv"

//RegNo,Name,Gender,Age,Course,Sem,Marks,Contact,Email,Address
typedef struct
{
    int regid, age, sem;
    char name[MAX_NAME], gender[10], course[MAX_COURSE], contact[MAX_CONTACT], email[MAX_EMAIL], city[MAX_CITY];;
    float percent;
} Record;

// csv operations
int read_csv(const char *filename, Record records[], int *count);
// int write_csv(const char *filename, Record records[], int count);
// int append_csv(const char *filename, Record newrecord);
// int update_student(const char *filename, int rollToUpdate, Record updatedStudent);
// int delete_student(const char *filename, int rollToDelete);

// for display/printing
void display_all(const Record records[], int count);

// CSV-safe string functions (opt.)
//void strip_newline(char *str);

#endif
