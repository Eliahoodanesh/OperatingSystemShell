#include "myShell.h"
#include "myFunction.h"

int main()
{
    welcome();
    while (1)
    {
        int piping_index = 0;
        int arg_index=0;
        getLocation();
        char *input = getInputFromUser();
        char **arguments = splitArgument(input);


        if (strncmp(arguments[0], "exit", 5) == 0)
            logout(input);
        while(arguments[arg_index] != NULL){
            if(strncmp(arguments[arg_index], "|", 1) == 0){
                piping_index = arg_index;
                break; 
            }
            arg_index++;
        }

        if (piping_index != 0)
        { 

            char *argv1[piping_index+1]; //"ls -l | something"
            for(int i=0;i<piping_index; i++){
                argv1[i] = arguments[i];
            }
            argv1[piping_index]=NULL;
            char **argv2 = &(arguments[piping_index+1]);
            mypipe(argv1, argv2);
            wait(NULL);
        }
        else if (strcmp(input, "echo") == 0)
            echo(arguments);
        else if (strcmp(input, "cd") == 0)
            cd(arguments);
        else if (strcmp(input, "cp") == 0)
            cp(arguments);
        else if (strcmp(input, "delete") == 0)
            delete (arguments);
        else if (strcmp(input, "dir") == 0)
            get_dir();
        else if (strcmp(input, "move") == 0)
            move(arguments);
        else if (strcmp(input, "echoppend") == 0)
            echo_append(arguments);
        else if (strcmp(input, "echorite") == 0)
            echorite(arguments);
         else if (strcmp(input, "read") == 0)
            reads(arguments); 
        else if (strcmp(input, "wordCount") == 0)
            wordCount(arguments); 
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
