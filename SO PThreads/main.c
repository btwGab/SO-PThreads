#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <pthread.h>

#define TAM_MATRIZ 100
#define TAM_MACROBLOCO 3
#define SEED 67
#define NUM_THREADS 2

int n_primos = 0;
int macrobloco_livre;
void algoritmoParalelo(int* cont, int** matriz);

int** Alocar_matriz_real(int m, int n) {
    int** v;
    int i;
    if (m < 1 || n < 1) {
        printf("** Erro: Parametro invalido **\n");
        return (NULL);
    }

    v = calloc(m, sizeof(int*));
    if (v == NULL) {
        printf("** Erro: Memoria Insuficiente **");
        return (NULL);
    }

    for (i = 0; i < m; i++) {
        v[i] = calloc(n, sizeof(int));
        if (v[i] == NULL) {
            printf("** Erro: Memoria Insuficiente **");
            return (NULL);
        }
    }

    return (v);
}

void aleatorizarMatriz(int** matriz) {
    for (int i = 0; i < TAM_MATRIZ; i++) {
        for (int j = 0; j < TAM_MATRIZ; j++) {
            matriz[i][j] = rand();
        }
    }
}

void printarMatriz(int** matriz) {
    for (int i = 0; i < TAM_MATRIZ; i++) {
        for (int j = 0; j < TAM_MATRIZ; j++) {
            // Imprime cada elemento separado por um espaço ou tabulação
            printf("%d ", matriz[i][j]);
        }
        // Pula para a próxima linha do terminal após terminar as colunas da linha atual
        printf("\n");
    }
}

int ehPrimo(int n) {
    if (n <= 1) {
        return 0;
    }

    int maximo = (int)sqrt(n);
    for (int i = 2; i <= maximo; i++) {
        if (n % i == 0) {
            return 0;
        }
    }

    return 1;
}

void algoritmoSerial(int* cont, int** matriz) {
    for (int i = 0; i < TAM_MATRIZ; i++) {
        for (int j = 0; j < TAM_MATRIZ; j++) {
            if (ehPrimo(matriz[i][j])) {
                n_primos += 1;
            }
        }
    }
}

void algoritmoParalelo(pthread_mutex_t *mutex,  int** matriz) {
    int cont = 0;
    for (int i = 0; i < TAM_MATRIZ; i++) {
        for (int j = 0; j < TAM_MACROBLOCO; j++) {
            if (ehPrimo(matriz[i][j])) {
                cont += 1;
            }
        }
    }
    pthread_mutex_lock(&mutex); // Se mutex estiver destrancado, tranca. caso contrario, thread espera o mutex ser destrancado

    n_primos += cont;

    pthread_mutex_unlock(&mutex); // Destranca a região crítica

    pthread_exit(0);
}

int macrobloco_atribuido() {

    return 0;
}

int main(int argc, char *agrv[]) {

    srand(SEED);
    struct timespec inicio, fim;

    //Criação da matriz aleatória
    int** matriz = Alocar_matriz_real(TAM_MATRIZ, TAM_MATRIZ);
    aleatorizarMatriz(matriz);
    printarMatriz(matriz);


    //Busca serial
    //clock_gettime(CLOCK_MONOTONIC, &inicio);

    algoritmoSerial(&n_primos, matriz);

    //clock_gettime(CLOCK_MONOTONIC, &fim);
    //double tempo_gasto = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1000000000.0;
    printf("\nBusca Serial\n");
    printf("Numero de primos: %d\n", n_primos);
    //printf("Tempo de execucao: %f\n", tempo_gasto);



    
    //Busca paralela
    pthread_t workers[NUM_THREADS];

    int tids[NUM_THREADS]; // Identificador da thread
    //pthread_attr_t attrs[NUM_THREADS]; // Atributos da thread

    for (int i = 0; i < NUM_THREADS; i++) {
        tids[i] = i;

        //pthread_attr_init(&attrs[i]); // Ajusta os atributos padrão da thread
        pthread_create(&tids, NULL, algoritmoParalelo, argv[1]); // cria a thread
    }

    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(workers[i], NULL); // Thread mãe espera as demais

    //clock_gettime(CLOCK_MONOTONIC, &inicio);

    pthread_mutex_t mutex; // Declaração da variável mutex
    pthread_mutex_init(&mutex, NULL); // Inicia mutex

    algoritmoParalelo(n_primos, matriz);

    //clock_gettime(CLOCK_MONOTONIC, &fim);
    //tempo_gasto = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1000000000.0;
    printf("\nBusca Paralela\n");
    printf("Numero de primos: %d\n", n_primos);
    //printf("Tempo de execução: %f\n", tempo_gasto);

    pthread_mutex_destroy(&mutex);

    //Liberação de ponteiros
    for (int i = 0; i < TAM_MATRIZ; i++) {
        free(matriz[i]);
    }
    free(matriz);

    return 0;
}