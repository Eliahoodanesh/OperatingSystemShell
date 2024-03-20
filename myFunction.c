#include "myFunction.h"
#include <aio.h>
#include <sys/utsname.h>
#include <pwd.h>

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
void echo(char **arg)
{
    while (*(++arg))
        printf("%s ", *arg);
    puts("");
}
void cd(char **arg)
{
    int arg_counter = 1;
    if (strncmp(arg[1], "\"", 1) != 0 && arg[2] != NULL) //cd "x
        puts("-myShell: cd: too many arguments");
    else if (strncmp(arg[1], "\"", 1) == 0)
    {
        while(arg[++arg_counter] != NULL);
        if (arg[arg_counter-1][strlen(arg[arg_counter-1])-1] != '\"'){
            puts("invalid command, missing last quote");
            return;
        }

        for(int path_idx=1; path_idx<arg_counter;path_idx++){
            char * curr_path;
            if(path_idx==1){
                curr_path = &arg[path_idx][1];
            } else if (path_idx == arg_counter-1){
                curr_path = arg[path_idx];
                curr_path[strlen(curr_path)-1]=0;
            }else{
                curr_path = arg[path_idx];
            }
            if (chdir(curr_path) != 0){
                printf("-myShell: cd: %s: No such file or directory\n", curr_path);
            }
           
        }
       
    }
    else if (chdir(arg[1]) != 0)
        printf("-myShell: cd: %s: No such file or directory\n", arg[1]);
}
void cp(char **arguments)
{
    char ch;
    FILE *src, *des;
    if ((src = fopen(arguments[1], "r")) == NULL)
    {
        puts("Erorr");
        return;
    }
    if ((des = fopen(arguments[2], "w")) == NULL)
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
    if (unlink(arg[1]) != 0)
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