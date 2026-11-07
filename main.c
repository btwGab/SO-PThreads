#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define TAM_MATRIZ 1000
#define SEED 67

int **Alocar_matriz_real(int m, int n) {
    int **v;
    int i;
    if (m < 1 || n < 1) {
        printf ("** Erro: Parametro invalido **\n");
        return (NULL);
    }
 
    v = calloc(m, sizeof(int*));
    if (v == NULL) {
        printf("** Erro: Memoria Insuficiente **");
        return (NULL);
    }

    for ( i = 0; i < m; i++ ) {
        v[i] = calloc(n, sizeof(int));
        if (v[i] == NULL) {
            printf("** Erro: Memoria Insuficiente **");
            return (NULL);
        }
    }

    return (v);
}

void aleatorizarMatriz(int **matriz) {
    for (int i = 0; i < TAM_MATRIZ; i++) {
        for (int j = 0; j < TAM_MATRIZ; j++) {
            matriz[i][j] = rand(); 
        }
    }
}

void printarMatriz(int **matriz) {
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

    int maximo = (int) sqrt(n);
    for (int i = 2; i <= maximo; i++) {
        if (n % i == 0) {
            return 0;
        }
    }

    return 1;
}

void algoritmoSerial(int *cont, int **matriz) {
    for (int i = 0; i < TAM_MATRIZ; i++) {
        for (int j = 0; j < TAM_MATRIZ; j++) {
            if (ehPrimo(matriz[i][j])) {
                (*cont) += 1;
            }
        }
    }
}

void algoritmoParalelo(int *cont, int **matriz) {
    printf("A ser implementado");
}

int main() {

    srand(SEED);
    struct timespec inicio, fim;



    //Criação da matriz aleatória
    int **matriz = Alocar_matriz_real(TAM_MATRIZ, TAM_MATRIZ);
    aleatorizarMatriz(matriz);
    printarMatriz(matriz);
    

    //Busca serial
    int cont_serial = 0;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    algoritmoSerial(&cont_serial, matriz);

    clock_gettime(CLOCK_MONOTONIC, &fim);
    double tempo_gasto = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1000000000.0;
    printf("\nBusca Serial\n");
    printf("Número de primos: %d\n", cont_serial);
    printf("Tempo de execução: %f\n", tempo_gasto);



    //Busca paralela
    int cont_paralelo = 0;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    algoritmoParalelo(&cont_paralelo, matriz);

    clock_gettime(CLOCK_MONOTONIC, &fim);
    tempo_gasto = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1000000000.0;
    printf("\nBusca Paralela\n");
    printf("Número de primos: %d\n", cont_paralelo);
    printf("Tempo de execução: %f\n", tempo_gasto);



    //Liberação de ponteiros
    for (int i = 0; i < TAM_MATRIZ; i++) {
        free(matriz[i]);
    }
    free(matriz);

    return 0;
}