#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include "project.h"

#define MAX_SIZE_ART_GROUP 130

int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("Favor passar um arquivo de entrada!\n");
		exit(1);
	}

	int n;
    float sum;
	Project* projects = read_file(argv[1], &n, &sum);

	for (int i = 0; i < n; i++) {
		printf("%d %.2f\n", projects[i].id, projects[i].art);
	}

    uint8_t **combinations = malloc(pow(2, n) * sizeof(uint8_t*));
    uint8_t combination[n];

    for (int i = 1; i < pow(2, n); i++) {
        float combination_sum = 0;
        
        for (int j = n-1; j >= 0; j--) { 
            
            if ((i >> j) & 1){
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



    for (int i = 1; i < pow(2, n); i++) {        
        if(combinations[i] != NULL){
            printf("%d ", i);
            for (int j = 0; j < n; j++) { 
                printf("%d", combinations[i][j]);
            }
            printf("\n");
        }
    }

	return 0;
}