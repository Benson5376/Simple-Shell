#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LENGTH 1024

int Process_Pipe(char*, char**); // Process '|' symbol
int Is_All_Space(char*); // Check if the command is consisted entirely of space
int Input(char**); // Take input from terminal
void Process_Space(char*, char**); // Clear Spaces
int Check_Background_Execution(char**); // Check if there is a '$' symbol
void Execute(char**); //  Execute non-piped command
void Execute_Pipe(char**, char**); //  Execute piped commmand
int IO_Redirection(char**); // If it is redirection return 1, otherwise return 0
void Process_Command(char*, char**, char**); // Deal with Command using functions above

int main()
{
    char *command;
    char *arguments[MAX_LENGTH], *arguments_piped[MAX_LENGTH];
    while(1)
    {
        command = NULL;
        if(Input(&command) == 1)
        {
            Process_Command(command, arguments, arguments_piped);
        }
        if(command != NULL)
        {
            free(command);
        }
    }
}

int Process_Pipe(char* str, char** pipe_position)
{
    char *token = strtok(str, "|");
    int length = 0;
    while(token != NULL)
    {
        if(length == 2)
        {
            printf("Cannot deal with multiple pipes\n");
            return -1;
        }
        pipe_position[length++] = token;
        token = strtok(NULL, "|");
    }
    if(pipe_position[1] == NULL)
    {
        return 0;
    }
    return 1;
}

int Is_All_Space(char* str)
{
    for(int i = 0 ; i < strlen(str) ; i++)
    {
        if(str[i] != ' ')
        {
            return 0;
        }
    }
    return 1;
}

int Input(char** command)
{
    printf(">");
    (*command) = (char*)malloc(sizeof(char) * MAX_LENGTH);
    fgets((*command), 1000, stdin);
    if((*command)[strlen(*command) - 1] == '\n')
    {
        (*command)[strlen(*command) - 1] = '\0';
    }
    if(strlen((*command)) == 0 || Is_All_Space((*command)))
    {
        return 0;
    }
    return 1;
}

void Process_Space(char* str, char** arguments)
{
    int length = 0;
    char *token = strtok(str, " ");
    while(token != NULL)
    {
        arguments[length++] = token;
        token = strtok(NULL, " ");
    }
    arguments[length] = NULL;
}

int Check_Background_Execution(char** arguments)
{
    for(int i = 0 ; i < MAX_LENGTH && arguments[i] != NULL ; i++)
    {
        if(strcmp("&", arguments[i]) == 0)
        {
            arguments[i] = NULL;
            return 1;
        }
    }
    return 0;
}

void Execute(char** arguments)
{
    if(Check_Background_Execution(arguments) == 1) // Background Execution
    {
        pid_t pid = fork();
        if(pid < 0)
        {
            printf("Failed to fork child.");
        }
        else if(pid == 0)
        {
            if(execvp(arguments[0], arguments) < 0)
            {
                printf("Could not execute command.\n");
            }
        }
        else
        {
            signal(SIGCHLD,SIG_IGN);
        }
    }
    else
    {
        pid_t pid = fork();
        if(pid < 0)
        {
            printf("Failed to fork child.");
        }
        else if(pid == 0)
        {
            if(execvp(arguments[0], arguments) < 0)
            {
                printf("Could not execute command.\n");
            }
        }
        else if(pid > 0)
        {
            wait(NULL);
        }
    }
}

void Execute_Pipe(char** argv1, char** argv2)
{
    pid_t pid;
    int fd[2];

    pipe(fd);
    pid = fork();

    if(pid == 0)
    {
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
        close(fd[1]);
        execvp(argv1[0], argv1);
    }
    else
    {
        pid = fork();
        if(pid == 0)
        {
            dup2(fd[0], STDIN_FILENO);
            close(fd[1]);
            close(fd[0]);
            execvp(argv2[0], argv2);
        }
        else
        {
            int status;
            close(fd[0]);
            close(fd[1]);
            waitpid(pid, &status, 0);
        }
    }
}

int IO_Redirection(char** arguments)
{
    int flag = 0, index;
    for(int i = 0 ; i < MAX_LENGTH && arguments[i] != NULL ; i++) // Check if redirection
    {
        if(strcmp(">", arguments[i]) == 0)
        {
            flag = 1;
            arguments[i] = NULL;
            index = i + 1; // example : ls –l > a.txt, arguments[index] = "a.txt"
            break;
        }
    }
    if(flag == 0) // not redirection
    {
        return 0;
    }
    pid_t pid = fork();
    if(pid < 0)
    {
        printf("Failed to fork child.");
    }
    else if(pid == 0)
    {
        int out = creat(arguments[index] , 0644); // arguments[index] = "a.txt"
        dup2(out, STDOUT_FILENO);
        close(out);
        execvp(arguments[0], arguments);
    }
    else
    {
        wait(NULL);
    }
    return 1;
}

void Process_Command(char *str, char** arguments, char** arguments_piped)
{
    char *pipe_position[2] = {};
    int pipe_state = Process_Pipe(str, pipe_position);
    if(pipe_state == 1) // piped command
    {
        Process_Space(pipe_position[0], arguments);  // Clear spaces in the first part of command
        Process_Space(pipe_position[1], arguments_piped); // // Clear spaces in the second part of command
        Execute_Pipe(arguments, arguments_piped);
    }
    else if(pipe_state == 0) // non-piped command
    {
        Process_Space(str, arguments); // Clear spaces
        if(IO_Redirection(arguments) == 0) // If the command is redirection it will be executed in IO_Redirection, otherwise it will be executed in Execute below
        {
            Execute(arguments);
        }
    }
}
