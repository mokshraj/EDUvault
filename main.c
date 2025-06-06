#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h> // for file existence check
#include "csv_module.h"
#ifdef _WIN32
    #define OS_WINDOWS 1
    #include <io.h>  // Windows-specific file check
    #define access _access
    #define F_OK 0
#else
    #define OS_WINDOWS 0
    #include <unistd.h> // Unix-based file check
#endif
// clear console
void clear(){
    if(OS_WINDOWS){
        system("cls");
    }
    else{
        system("clear");
    }
}
// checks if the csvfile exits or not
void csv_exists(const char *filename)
{
    const char *header = "RegID,Name,Gender,Age,Course,Sem,Percentage,Contact,Email,City\n";
    FILE *fp = fopen(filename, "r");

    // if file does not exist, create one and write header
    if (fp == NULL)
    {
        fp = fopen(filename, "w");

        if (fp == NULL)
        {
            perror("Failed to create file");
            return;
        }

        fputs(header, fp);
        fclose(fp);

        printf("File '%s' created with pre-defined header.\n", filename);

        return;
    }

    // if file exists, check header
    char line[128];
    if (fgets(line, sizeof(line), fp) == NULL || strcmp(line, header) != 0)
    {
        // if header is missing or incorrect, it will rewrite it
        fclose(fp);
        fp = fopen(filename, "w");

        if (fp == NULL)
        {
            perror("Failed to overwrite file");
            return;
        }

        fputs(header, fp);
        fclose(fp);
        printf("File '%s' header was missing/invalid and has been updated with correct header.\n", filename);
    }

    else
        fclose(fp); // file exists with correct header
}

// Check if a file exists
int file_exists(const char *filename)
{
    struct stat buffer;
    return (stat(filename, &buffer) == 0); // if file exists returns 1
}

// to check if source file and executable is missing
int ensure_compiled(const char *sourceFile, const char *exeFile)
{
    printf("\n");
    if (file_exists(exeFile))
        return 1;

    else if (!file_exists(sourceFile))
    {
        printf("Source file '%s' not found.", sourceFile);
        return 0;
    }

    if (!file_exists(exeFile))
    {
        printf("Compiling '%s'...\n", sourceFile);

        char compileCmd[150];
        snprintf(compileCmd, sizeof(compileCmd), "gcc %s -o %s", sourceFile, exeFile);

        if (system(compileCmd) != 0)
        {
            printf("Compilation failed for '%s'.", sourceFile);
            return 0;
        }

        printf("Compiled: %s\n\n", exeFile);
    }
    return 1;
}

// run program based on OS
void run_program(const char *exeFile)
{
    char cmd[100];
    snprintf(cmd, sizeof(cmd), OS_WINDOWS ? "%s" : "./%s", exeFile);
    system(cmd);
}

void pause_menu()
{
    printf("\n\nPress Enter to return to the menu...");
    while(getchar()!= '\n');
}

// to compile and run
void compile_and_run(const char *sourceFile)
{
    char exeFile[100];

    // create executable flie from source file
    snprintf(exeFile, sizeof(exeFile), "%.*s%s",(int)(strrchr(sourceFile, '.') - sourceFile),sourceFile,OS_WINDOWS ? ".exe" : "");

    // compilation check
    if (!ensure_compiled(sourceFile, exeFile))
        return;

    run_program(exeFile);
}

#define MAIN_MENU
#if defined(MAIN_MENU) && !defined(MAIN_FILE)
int main()
{
    int ch;
    char *str;
    while (1)
    {
        // EDUVALT branding
        printf("  ___  __               _           _____   \n");
        printf(" |__  |  \\ |  |   \\  / /_\\  |  | |    |  \n");
        printf(" |___ |__/ |__|    \\/ /   \\ |__| |___ |   \n");
        printf("\nMENU:\n");
        printf("1. Add Record\n");
        printf("2. Edit Record\n");
        printf("3. Delete Record\n");
        printf("4. Find Record\n");
        printf("5. Sort Record\n");
        printf("6. Display All Records\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        getsinput(&str,100);
        if(sscanf(str, "%d", &ch)){}
        else{
            goto invalid;
        }

        switch (ch)
        {
            case 1:
                csv_exists("records.csv");
                compile_and_run("add.c");
                pause_menu();
                clear();
                break;
            case 2:
                csv_exists("records.csv");
                compile_and_run("update.c");
                pause_menu();
                clear();
                break;
            case 3:
                csv_exists("records.csv");
                compile_and_run("delete.c");
                pause_menu();
                clear();
                break;
            case 4:
                csv_exists("records.csv");
                compile_and_run("find.c");
                pause_menu();
                clear();
                break;
            case 5:
                csv_exists("records.csv");
                compile_and_run("sort.c");
                pause_menu();
                clear();
                break;
            case 6:
                csv_exists("records.csv");
                compile_and_run("display.c");
                pause_menu();
                clear();
                break;
            case 0:
                printf("\nExiting Edu Vault...\n");
                exit(0);
            default:
                invalid:
                printf("\nInvalid choice. Try again.");
                pause_menu();
                clear();
                break;
        }
    }
    free(str);
    return 0;
}
#endif
