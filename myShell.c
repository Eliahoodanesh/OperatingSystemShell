#include "myShell.h"
#include "myFunction.h"
// #include <stdio.h>

int main()
{
    welcome();
    while (1)
    {
        int piping = 0;
        getLocation();
        char *input = getInputFromUser();
        char **arguments = splitArgument(input);
        if (strncmp(arguments[0], "exit", 5) == 0)
            logout(input);
        if (strcmp(input, "echo") == 0)
            echo(arguments);
        else if (strcmp(input, "cd") == 0)
            cd(arguments);
        else if (strcmp(input, "cp") == 0)
            cp(arguments);
        else if (strcmp(input, "delete") == 0)
            delete (arguments);
        else if (strcmp(input, "dir") == 0)
            get_dir();
        else if (piping)
        {
            char *argv1[] = {arguments[0], NULL};
            char *argv2[] = {arguments[2], arguments[3], NULL};
            mypipe(argv1, argv2);
            wait(NULL);
        }
        else
        {
            systemCall(arguments);
            wait(NULL);
        }
        free(arguments);
        free(input);
    }
    return 1;
}
void welcome()
{
    puts("Welcome to my Shell");
    puts("\n");
    
    puts("   W   W EEEEE L     CCCC OOOOO MMMMM EEEEE\n");
    puts("   W W W E     L     C    O   O M   M E    \n");
    puts("   W W W EEEE  L     C    O   O MMMMM EEEE \n");
    puts("   W   W E     L     C    O   O M   M E    \n");
    puts("    W W  EEEEE LLLLL CCCC OOOOO M   M EEEEE\n");
    puts("\n");
    puts("https://github.com/Eliahoodanesh \n");
   


}




// void getLocation() {
//     char location[BUFF_SIZE];

//     if (getcwd(location, BUFF_SIZE) == NULL) {
//         puts("Error");
//     } else {
//         struct passwd *pw;
//         uid_t uid;
//         uid = geteuid();
//         pw = getpwuid(uid);

//        struct utsname unameData;
//         uname(&unameData);

// // Print username@hostname in green
//     puts("\033[1;32m%s@%s:\033[0m", pw->pw_name, unameData.nodename);

//     // Print directory path in blue
//     puts("\033[1;34m%s\033[0m", location);

//     // Print "$" in white
//     puts("\033[1;37m$\033[0m ");
//     } 