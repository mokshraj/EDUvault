
void getsinput(char **str, int maxsize){
    char *a;
    char ch = fgetc(stdin);
    int size = 1;
    a = (char*)malloc(size);
    if (!a) {
        perror("Error assigning memory");
    }
    while(ch != '\n' && ch != EOF && ch != '\r') {
        a = (char *)realloc(a, size + 1);
        if (!a) {
            perror("Error reassigning memory");
        }
        a[size-1] = ch;
        size++;
        ch = fgetc(stdin);
    }
    if(size > maxsize){
        a[maxsize] = '\0';
    }
    else{
        a[size-1] = '\0';
    }
    *str = strdup(a);
    free(a);
}