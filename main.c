#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#define tokenDelimiter "\t\r\n\a "
#define bufferSizeTok 64


void ShellLoop();
char **readArgs(char *line);
char *readLine();
int execute();
int procLauncher(char **args);
int shell_cd(char **args);
int shell_help(char **args);
int shell_exit(char **args);

char *builtInStrings[] = {"cd", "help", "exit"};

int (*functionCommand[]) (char **) = {
        &shell_cd,
        &shell_help,
        &shell_exit
};

int builtInCmds(){
    return sizeof(builtInStrings) / sizeof(char *);
}

int main(int argc, char **argv) {
    puts("##################################");
    puts("##Hello and welcome to my Shell!##");
    puts("##################################");

    ShellLoop();

    return 0;
}

void ShellLoop() {
char *line;
char **argu;
int isRunning;
    do{
        line = readLine();
        argu = readArgs(line);
        isRunning = execute(argu);
    }while(isRunning);

}

int execute(char **args) {
    int i;

    if (args[0] == NULL){
        return 1;
    }
    for (i = 0; i < builtInCmds(); i++){
        if(strcmp(args[0], builtInStrings[i]) == 0){
            return (*functionCommand[i])(args);
        }
    }
    return  procLauncher(args);
}

char *readLine() {
    char *line = NULL;
    ssize_t lineSize = 128;

    line = (char *)malloc(lineSize * sizeof(char));
    if( line == NULL){
        exit(1);
    }

    printf(">");
    getline(&line, &lineSize, stdin);
    //Removes the automatic newline, that windows creates
    line = strtok(line, "\n");
    printf("You typed: '%s'\n", line);
    return line;
}

char **readArgs(char *line) {

    int bufferSize = bufferSizeTok, position = 0;
    char *token;
    char **tokens = malloc(bufferSize *sizeof(char*));

    if(!tokens){
        fprintf(stderr, "Allocation Error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, tokenDelimiter);
    while(token != NULL){
        tokens[position] = token;
        position++;

        if(position >= bufferSize){
            bufferSize += bufferSizeTok;
            tokens = realloc(tokens, bufferSize * sizeof(char*));
            if(!tokens){
                fprintf(stderr, "Allocation Error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, tokenDelimiter);
    }
    tokens[position] = NULL;
    return tokens;
}

int procLauncher(char **args){
    pid_t pid;
    int status;

    pid = fork();

    if(pid == 0){
        if (execvp(args[0], args) == -1){
            perror("Shelly");
        }
        exit(EXIT_FAILURE);
    } else if(pid < 0){
        perror("Shelly");
    } else {
        do{
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;
}

int shell_cd (char **args){
    char s[200];
if(args[1] == NULL){
    if(chdir("/mnt/c")){
        perror("ERROR IN chdir");
    }
} else {
    if(chdir(args[1]) != 0){
        perror("Shelly");
    }
}
    printf("%s\n", getcwd(s, 100));

return 1;
}

int shell_help (char **args){
int i;
printf("Justus SHELL\n"),
printf("The following are the built-in commands:\n");
for(i = 0; i < builtInCmds(); i++){
    printf(" %s\n" , builtInStrings[i]);
}
printf("Furthermore you can use the other common UNIX system commands\n ");
return 1;
}

int shell_exit (char **args){
return 0;
}





