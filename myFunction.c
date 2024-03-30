#include "myFunction.h"
#include <aio.h>
#include <sys/utsname.h>
#include <pwd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char *getInputFromUser()
{
    char ch;
    int size = 1;
    int index = 0;
    char *str = (char *)malloc(size * sizeof(char));
    while ((ch = getchar()) != '\n')
    {
        *(str + index) = ch;
        size++;
        index++;
        str = (char *)realloc(str, size * sizeof(char));
    }
    *(str + index) = '\0';

    return str;
}
// The function splits a given string into substrings and returns an array of these substrings as a result.
char **splitArgument(char *str)
{
    char *subStr;
    int size = 2;
    int index = 0;
    subStr = strtok(str, " ");
    char **argumnts = (char **)malloc(size * sizeof(char *));
    *(argumnts + index) = subStr;
    while ((subStr = strtok(NULL, " ")) != NULL)
    {
        size++;
        index++;
        *(argumnts + index) = subStr;
        argumnts = (char **)realloc(argumnts, size * sizeof(char *));
    }
    *(argumnts + (index + 1)) = NULL;

    return argumnts;
}
//// `getLocation` prints the current location, username, and hostname, and then prints "$ " into the prompt.
void getLocation()
{
    char location[BUFF_SIZE];

    if (getcwd(location, BUFF_SIZE) == NULL)
    {
        puts("Error");
    }
    else
    {
        struct passwd *pw;
        uid_t uid;
        uid = geteuid();
        pw = getpwuid(uid);

        struct utsname unameData;
        uname(&unameData);
        bold();
        blue();
        printf("%s@%s:%s", pw->pw_name,unameData.nodename,location);
        reset();
        printf("$ ");
    }
}
void logout(char *input)
{
    free(input);
    puts("log out");
    exit(EXIT_SUCCESS);
}
// `echo` prints the arguments sent to it, and `compose_path` constructs a path to a file from the argument strings it receives.
void echo(char **arg)
{
    while (*(++arg))
        printf("%s ", *arg);
    puts("");
}
char* compose_path(char** args,char*out_path){
    int arg_counter = 0;

    // too many arguments
    if (strncmp(args[0], "\"", 1) != 0 && args[1] != NULL){
        return NULL;
    }
    if (strncmp(args[0], "\"", 1) == 0){
        while(args[++arg_counter] != NULL);//count the number of argument
        if (args[arg_counter-1][strlen(args[arg_counter-1])-1] != '\"'){
            return NULL;
        }
         for(int path_idx=0; path_idx<arg_counter;path_idx++){
            char * curr_path;
            if(path_idx==0){
                curr_path = &args[path_idx][1];
            } else if (path_idx == arg_counter-1){
                curr_path = args[path_idx];
                curr_path[strlen(curr_path)-1]=0;
            }else{
                curr_path = args[path_idx];
            }
            strcat(out_path,curr_path);
            if(path_idx!=arg_counter-1){
                strcat (out_path,"/");
            }
        }
        return out_path;
    }
    return args[0];


}
// Changes the current directory according to the received argument and prints an error message if the directory does not exist.
void cd(char **arg)
{
    char out_path[1024]={0};
    char* path= compose_path(&arg[1],out_path);
   if(path==NULL){
    printf("invalid cd command\n");
    return;
   }
    if (chdir(path) != 0)
        printf("-myShell: cd: %s: No such file or directory\n", arg[1]);
}
//// Receives an array of strings and a filename, then concatenates the strings to the filename.
void get_next_cp_arg(char ** arguments, char name[], int*counter){
    char * curr_arg;
    int prev_counter = *counter;
     // if first argument starts with "
    if(arguments[*counter][0] == '\"'){
        while(1){
            curr_arg = arguments[*counter]; //get current argument
            if(curr_arg[strlen(curr_arg)-1] == '\"'){
                //last argument of first path
                break;
            }
            (*counter)++;
        }
    }

    for(int i=prev_counter;i<=*counter;i++){
        curr_arg = arguments[i];
        if(i==prev_counter && curr_arg[0] == '\"'){
            strcat(name, &curr_arg[1]);
        }else{
            strcat(name, curr_arg);
        }
        if(i != *counter){
            strcat(name, "/");
        }
    }

    if(name[strlen(name)-1] == '\"'){
        name[strlen(name)-1] = 0;
    }
}

//The cp function copies the content of the source file (src_name) to the destination file (dst_name).
void cp(char **arguments)
{
    char ch;
    FILE *src, *des;
    char src_name[100]={0}, dst_name[100]={0};
    int arg_counter=1;

    get_next_cp_arg(arguments, src_name, &arg_counter);
    arg_counter++; //move arg_counter to the second path start    
    get_next_cp_arg(arguments, dst_name, &arg_counter);

    if ((src = fopen(src_name, "r")) == NULL)
    {
        puts("Erorr");
        return;
    }
    if ((des = fopen(dst_name, "w")) == NULL)
    {
        puts("Erorr");
        fclose(src);
        return;
    }

    while ((ch = fgetc(src)) != EOF)
    {
        fputc(ch, des);
    }
    fclose(src);
    fclose(des);
}
void get_dir()
{
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir("./")) == NULL)
    {
        /* could not open directory */
        perror("");
        return;
    }
    /* print all the files and directories within directory */
    while ((ent = readdir(dir)) != NULL)
    {
        printf("%s ", ent->d_name);
    }
    puts("");
    closedir(dir);
}
void delete(char **arg)
{
      char out_path[1024]={0};
    char* path= compose_path(&arg[1],out_path);
     if(path==NULL){
    printf("invalid delete command\n");
    return;
   }
    if (unlink(path) != 0)
        printf("-myShell: unlink: %s: No such file or directory\n", arg[1]);
}
void systemCall(char **arg)
{
    pid_t pid = fork();
    if (pid == -1)
    {
        printf("fork err\n");
        return;
    }
    if (pid == 0 && execvp(arg[0], arg) == -1)
        exit(1);
}
void mypipe(char **argv1, char **argv2)
{
    int fildes[2];
    if (fork() == 0)
    {
        pipe(fildes);
        if (fork() == 0)
        {
            /* first component of command line */
            close(STDOUT_FILENO);
            dup(fildes[1]);
            close(fildes[1]);
            close(fildes[0]);
            /* stdout now goes to pipe */
            /* child process does command */
            execvp(argv1[0], argv1);
        }
        /* 2nd command component of command line */
        close(STDIN_FILENO);
        dup(fildes[0]);
        close(fildes[0]);
        close(fildes[1]);
        /* standard input now comes from pipe */
        execvp(argv2[0], argv2);
    }
}


void move(char **args) {
    const char *filename = args[1];
    const char *directory = args[2];

    if (access(filename, F_OK) == -1) {
        fprintf(stderr, "Error: Source file '%s' does not exist.\n", filename);
        return;
    }

    if (access(directory, F_OK) == -1) {
        fprintf(stderr, "Error: Destination directory '%s' does not exist.\n", directory);
        return;
    }

    char full_destination_path[256];
    snprintf(full_destination_path, sizeof(full_destination_path), "%s/%s", directory, filename);

    int result = rename(filename, full_destination_path);

    if (result == 0) {
        printf("File '%s' moved successfully to directory '%s'.\n", filename, directory);
    } else {
        perror("Error moving file");
    }
}

void echo_append(char **args) {
    const char *text = args[1];
    const char *filepath = args[2];

    FILE *file = fopen(filepath, "ab");

    if (file == NULL) {
        fprintf(stderr, "Error opening file: Could not open %s\n", filepath);
        return;
    }

    fprintf(file, "%s\n", text);

    fclose(file);
}

void echorite(char **args) {
    const char* text = args[1];
    const char* filepath = args[2];  

    FILE *file = fopen(filepath, "w"); // "w" mode for write (overwrite)

    if (file == NULL) {
        fprintf(stderr, "Error opening file: Could not open %s\n", filepath);
        return;
    }

    fprintf(file, "%s\n", text);

    fclose(file);
}


void reads(char **args) {
    const char *file_path = args[1];
    FILE *file = fopen(file_path, "r");

    if (file == NULL) {
        return;
    }

    char ch;
    while ((ch = fgetc(file)) != EOF) {
        putchar(ch);
    }

    fclose(file);
}

void wordCount(char **args) {
    const char *option = args[1];
    const char *file_path = args[2];

    // Open the file for reading
    FILE *file = fopen(file_path, "r");

    if (file == NULL) {
        fprintf(stderr, "Error: Could not open file '%s'\n", file_path);
        return;
    }

    int lines = 0;
    int words = 0;

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        lines++;

        // Count words in each line
        char *token = strtok(buffer, " \t\n");
        while (token != NULL) {
            words++;
            token = strtok(NULL, " \t\n");
        }
    }

    // Close the file
    fclose(file);

    // Print result based on option
    if (strcmp(option, "-l") == 0) {
        printf("Number of lines in '%s': %d\n", file_path, lines);
    } else if (strcmp(option, "-w") == 0) {
        printf("Number of words in '%s': %d\n", file_path, words);
    } else {
        fprintf(stderr, "Error: Invalid option '%s'\n", option);
        return;
    }
}
