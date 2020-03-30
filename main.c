#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include "project.h"

#define MAX_SIZE_ART_GROUP 130

int verify_groups(int **combinations, int n, int qty){
    int sum;
    for(int i = 0; i < n; i++){
        sum = 0;
        for(int j = 0; j < qty; j++)
            sum += combinations[j][i];

        if(sum != 1)
            return 0;
    }

    return 1;
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("Favor passar um arquivo de entrada!\n");
		exit(1);
	}

	int n;
    float sum;
    int i, j;
	Project* projects = read_file(argv[1], &n, &sum);

    if(sum <= MAX_SIZE_ART_GROUP){
        printf("Todas as A.R.Ts podem ser agrupadas em apenas uma!\n");
        exit(0);
    }

	for (i = 0; i < n; i++) {
		printf("%d %.2f\n", projects[i].id, projects[i].art);
	}

    int number_of_combinations = pow(2, n) - 1;

    uint8_t **combinations = malloc(number_of_combinations * sizeof(uint8_t*));
    uint8_t combination[n];

    // parallel for aqui
    for (i = 0; i < number_of_combinations; i++) {
        float combination_sum = 0;
        
        for (j = n-1; j >= 0; j--) { 
            
            if (((i+1) >> j) & 1){
                combination[n-1-j] = 1;
                combination_sum += projects[n-1-j].art;
            }else{
                combination[n-1-j] = 0;
            }
        }

        if(combination_sum <= MAX_SIZE_ART_GROUP){
            combinations[i] = malloc(n * sizeof(uint8_t));
            memcpy(combinations[i], combination, n*sizeof(uint8_t));
        }else{
            combinations[i] = NULL;
        }
    }

    //função para eliminar linhas vazias da matriz
    for(i = 0; i < number_of_combinations-1; i++){
        if(combinations[i] == NULL){
            for(j = number_of_combinations-1; j > i; j--){
                number_of_combinations--;
                if(combinations[j] != NULL){
                    combinations[i] = combinations[j];
                    combinations[j] = NULL;
                    break;
                }                
            }
            if(combinations[i] == NULL)
                number_of_combinations--;

        }
    }


    for (int i = 0; i < number_of_combinations; i++) {        
        printf("%d ", i);
        for (int j = 0; j < n; j++) { 
            printf("%d", combinations[i][j]);
        }
        printf("\n");

    }

	return 0;
}