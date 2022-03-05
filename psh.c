#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "psh.h"
#include "queue.h"

#define TRUE 1

void psh_redirect(char* file);

int pipePidVaccinated[2];

Queue *queue_unvaccinated = NULL;

void kill_unvaccinated();
void kill_vaccinated();
void child_vaccinated_signal(int sig);
void child_unvaccinated_signal(int sig);
void print_gandalf();

char *psh_read_line(void) {
    // Inicializando variáveis necessárias
    char *line = NULL;
    size_t bufsize = 0;

    // Casos de erro
    if (getline(&line, &bufsize, stdin) == -1){
        if (feof(stdin)) {
            exit(EXIT_SUCCESS); // EOF
        } 
        else  {
            perror("readline");
            exit(EXIT_FAILURE);
        }
    }

    // Se tudo correto, a linha é lida e retornada
    return line;
}

#define PSH_TOK_BUFSIZE 64
#define PSH_TOK_DELIMITADOR " \t\r\n\a"
#define PSH_ARGS_NUMBER 10
Processos *psh_split_line(char *line) {

    // Inicializando e alocando variáveis necessárias
    int bufsize = PSH_TOK_BUFSIZE;
    int total_words = 0;
    char **tokens = malloc(bufsize * sizeof(char*)); // matriz contendo em cada posição as palavras separadas por espaço
    char *token;

    if (!tokens) { // Caso erro de alocação
        fprintf(stderr, "psh: allocation error (tokens)\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, PSH_TOK_DELIMITADOR); // divisão

    int qtd_procs = 1; // conta o numero de processos por linha (max 5);

    while (token != NULL) {
        // Organização das instruções no vetor de strings
        tokens[total_words] = token;
        if(strcmp(token, ";") == 0){
            qtd_procs++; // incremento no num de processos
        }
        total_words++;

        if (total_words >= bufsize) {  // Necessidade de realocar aumentando a quantidade de caracteres
            // no vetor de strings para as instruções
            bufsize += PSH_TOK_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
                fprintf(stderr, "psh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, PSH_TOK_DELIMITADOR);
        // TODO Lidar com \n, pois cria uma matriz com somente um processo de comando '\n'
    }

    char ***matriz_proc = (char ***) malloc(qtd_procs * sizeof(char**)); // matriz contendo em cada posição um comando

    char **proc = malloc(PSH_ARGS_NUMBER * sizeof(char*));
    int j = 0; //contador de palavras em um processo
    int k = 0; //contador de processos do vetor de processos
    for (int i = 0; i < total_words; ++i) {
        if (strcmp(tokens[i], ";") != 0) {
            token = (char *) malloc(bufsize * sizeof(char));
            strcpy(token, tokens[i]);
            proc[j++] = token;
        } else {
            matriz_proc[k++] = proc;
            proc = malloc(5 * sizeof(char*));
            j = 0;
        }
    }
    matriz_proc[k] = proc;

    return initProc(qtd_procs, matriz_proc);
}

void kill_unvaccinated(int signal) {
    // printf("is_empty: %d\n", is_empty(queue_unvaccinated));
    while (!is_empty(queue_unvaccinated)) {
        int pid_unvaccinated = first(queue_unvaccinated);
        queue_unvaccinated = pop(queue_unvaccinated);
        printf("Novo inicio dps POP na MAIN:\n");
        print_queue(queue_unvaccinated);
        kill(pid_unvaccinated, signal);
        printf("matou o %d\n", pid_unvaccinated);
    }
    printf("cabou\n");
}

void kill_vaccinated(int signal) {
    int pid_vaccinated;
    close(pipePidVaccinated[1]);
    read(pipePidVaccinated[0], &pid_vaccinated, sizeof(int));
    close(pipePidVaccinated[0]);
    printf("PID VACCINATED PARA MATAR:%d\n", pid_vaccinated);
    killpg(pid_vaccinated, signal);
}

void handle_sigvacinado(int sig) {
    //kill_unvaccinated(sig);
    printf("Estou vacinada… desista!!\n");
}

void child_vaccinated_signal(int sig) {
    printf("TODO implement\n");
}

void child_unvaccinated_signal(int sig) {
    printf("TODO implement\n");
}

int exist_output_token(char **proc) {
    // echo ; echo2 > output.txt ; echo 3
    char *find_char = NULL;
    int j = 0;
    while ((find_char = proc[j]) != NULL){
        if (strcmp(find_char, ">") == 0) {
            return 1;
        }
        j++;
    }
    return 0;
}

char **clean_proc(char **proc, char **file) {
    int sizeof_proc = 0;

    while(proc[sizeof_proc] != NULL) sizeof_proc++;

    int sizeof_newproc = sizeof_proc - 2;
    char **str_return = (char **) malloc(sizeof_newproc * sizeof(char *));

    for (int j = 0; j < sizeof_newproc; j++) str_return[j] = strdup(proc[j]);
    *file = strdup(proc[sizeof_proc - 1]);
    
    return str_return;
}

int psh_launch(Processos *proc) {
    pid_t pid, wpid;
    char ***matriz = getMatriz(proc);

    int qtd_procs = getQtdProcs(proc); // tamanho do vetor de referências à processos / tamanho de uma referência de getQtdProcs(proc)
    
    if (qtd_procs > 1) { // para um grupo de processos (vacinados)
        for (int i = 0; i < qtd_procs; ++i) {
            if (matriz[i] != NULL) {

                int status;
                pid = fork();
                if (pid == -1) perror("psh");  // Error forking
                else if (pid == 0) { // Child proc
                    pid_t pgid_vacinados = 0;
                    
                    if(i == 0){ // 1o processo filho
                        close(pipePidVaccinated[0]);
                        pgid_vacinados = getpid();
                        write(pipePidVaccinated[1], &pgid_vacinados, sizeof(pgid_vacinados));
                        close(pipePidVaccinated[1]);
                    } else { // resto dos filhos
                        read(pipePidVaccinated[0], &pgid_vacinados, sizeof(pgid_vacinados));
                        write(pipePidVaccinated[1], &pgid_vacinados, sizeof(pgid_vacinados));
                        close(pipePidVaccinated[0]);
                        close(pipePidVaccinated[1]);
                    }
                    printf("------%d\n", pgid_vacinados);
                    int teste = setpgid(getpid(), pgid_vacinados);
                    printf("------ teste setpgid:%d\n", teste);
                    printf("pid: %d \t pgid: %d\n", getpid(), getpgid(getpid()));

                    printf("ID DO GRUPO DO PAI: %d\n", getpgid(getppid()));
                    
                    signal(SIGINT, SIG_IGN);
                    signal(SIGTSTP, SIG_IGN);

                    if (execvp(matriz[i][0], matriz[i]) == -1) {
                        perror("psh");
                    }

                    // exit(EXIT_FAILURE); //mata o processo 
                    //return TRUE;
                } else {
                    do {
                        wpid = waitpid(-1, &status, WNOHANG);
                        if (WTERMSIG(status) == SIGUSR1) {
                            printf("O processo %d morreu com o sinal %d", wpid, WTERMSIG(status));
                            break;
                        }
                    } while(!WIFEXITED(status) && !WIFSIGNALED(status));
                }
            }
        }
    } else { // para processos isolados (não vacinados)
        char **processo_n_vacinado = matriz[0];
        if (processo_n_vacinado != NULL) {

            int status;
            pid = fork(); 
            if (pid == -1) perror("psh");  // Error forking
            else if (pid == 0) { // Child proc
                setpgid(getpid(), getpid()); // o próprio id do processo será o seu id do grupo
                printf("pid_unvaccinated: %d\n\tID DO GRUPO DO PAI: %d\n\tGrupo filho nao vacinado: %d\n", getpid(), getpgid(getppid()), getpgid(getpid()));

                int exist = exist_output_token(processo_n_vacinado);

                char *file = NULL;
                //Quando há um redirecionamento de saida no processo, precisamos retirar o simbolo para redirecionar e o arquivo de saida
                if(exist){
                    char **processo_redirecionar = clean_proc(processo_n_vacinado, &file);
                    printf("FILE: %s\n", file);
                    printf("entrou para psh redirect\n");
                    // char* process = clean_proc(processo_n_vacinado);
                    psh_redirect(file);
                    if (execvp(processo_redirecionar[0], processo_redirecionar) == -1) {
                        perror("psh");
                    }
                    destroy_proc(processo_redirecionar);
                } else {
                    if (execvp(processo_n_vacinado[0], processo_n_vacinado) == -1) {
                        perror("psh");
                    }
                }

                exit(EXIT_FAILURE);
            } else { // Parent comands
                printf("pid filho nao vacinado: %d\n", pid);
                queue_unvaccinated =  push(queue_unvaccinated, pid);
            }
        }
    }

    return TRUE;
}

int psh_execute(Processos *args) {

    if (strcmp(getMatriz(args)[0][0], "") == 0) {
        // An empty command was entered.
        return 1;
    }

    return psh_launch(args);
}

void psh_loop(void) {
    char *line;
    Processos *args; //matriz 3
    int statusLoop;
    int status;
    // int pipePidVaccinated[2];

    Queue *queue_unvaccinated = NULL;

    if(pipe(pipePidVaccinated) < 0){
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    signal(SIGINT,  handle_sigvacinado); /* Ctrl + C */
    signal(SIGQUIT, handle_sigvacinado); /* Ctrl + \ */ 
    signal(SIGTSTP, handle_sigvacinado); /* Ctrl + Z */
    // signal(SIGTERM, handle_sigvacinado); 
    // signal(SIGUSR1, handle_sigvacinado); // não precisa falar q está vacinada

    do {
        printf("psh> ");
        line = psh_read_line(); // leitura da linha

        args = psh_split_line(line); // separa instruções

        //COMANDO TERM:
        if (getQtdProcs(args) == 1 && (strcmp(getMatriz(args)[0][0], "term")==0)){
            kill_unvaccinated(SIGTERM);
            kill_vaccinated(SIGTERM);
            exit(1);
        }

        statusLoop = psh_execute(args); // executa instruções

        // VE SE ALGUM FILHO MORREU DE SIGUSR1
        waitpid(-1, &status, WNOHANG);
        if (WTERMSIG(status) == SIGUSR1){
            printf("ENTREI COMO SIGUSR1!!\n");
            kill_unvaccinated(SIGUSR1);
            kill_vaccinated(SIGUSR1);
            //matar vacinados e psh (não esquecer de liberar memória)
            print_gandalf(); // imprime o gandalf
        } 

        free(line);

        destroy_matrix(args);
    } while (statusLoop);
}

void print_gandalf(){
    printf(
        "                       ,---.\n"
        "                       /    |\n"
        "                      /     |\n"
        "                     /      |\n"
        "                    /       |\n"
        "               ___,'        |\n"
        "             <  -'          :\n"
        "              `-.__..--'``-,_\\_\n"
        "                 |o/ ` :,.)_`>\n"
        "                 :/ `     ||/)\n"
        "                 (_.).__,-` |\\ \n"
        "                 /( `.``   `| :\n"
        "                 \'`-.)  `  ; ;\n"
        "                 | `       /-<\n"
        "                 |     `  /   `.\n"
        " ,-_-..____     /|  `    :__..-'\\ \n"
        "/,'-.__\\  ``-./ :`      ;       \\ \n"
        "`\\ `\\  `\\  \\ :  (   `  /  ,   `. \\ \n"
        "  \\` \\   \\   |  | `   :  :     .\\ \\ \n"
        "   \\ `\\_  ))  :  ;     |  |      ): :\n"
        "  (`-.-'\\ ||  |\\ \\   ` ;  ;       | |\n"
        "   \\-_   `;;._   ( `  /  /_       | |\n"
        "    `-.-.// ,'`-._\\__/_,'         ; |\n"
        "       \\:: :     /     `     ,   /  |\n"
        "        || |    (        ,' /   /   |\n"
        "        ||                ,'   /    |\n"
        "________ Unfortunately all process died!________\n"
        "___ Vaccination should be a social contract!____\n"
        "____Cooperation was the morally right choice!___\n"
    );
}

void psh_redirect(char* file){
    int fd = open(file,O_WRONLY|O_CREAT|O_TRUNC,0644); //fopen ("saida.txt", "w");

    if(fd == -1){
        perror("psh");
        exit(EXIT_FAILURE);
    }
    if(dup2(fd, STDOUT_FILENO) == -1){
        perror("psh");
        exit(EXIT_FAILURE);
    }
}

// ls > saida.txt

// void psh_output(char* file, char *process) {
//     FILE *fp = popen(process, "r");

//     pclose(fp);
// }