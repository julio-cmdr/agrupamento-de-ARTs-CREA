#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include "project.h"

#define MAX_SIZE_ART_GROUP 100

//verifica se um grupo de combinações contém todos os projetos, com cada um aparecendo apenas uma vez.
int verify_group(uint8_t **group, int number_of_projects, int group_size){
    int sum;
    for(int i = 0; i < number_of_projects; i++){
        sum = 0;
        for(int j = 0; j < group_size; j++)
            sum += group[j][i];

        if(sum != 1)
            return 0;
    }

    return 1;
}

//função para eliminar linhas nulas da matriz
void remove_null_lines(uint8_t **combinations, unsigned int *number_of_combinations){
    for(unsigned int i = 0; i < *number_of_combinations-1; i++){
        if(combinations[i] == NULL){
            for(unsigned int j = *number_of_combinations-1; j > i; j--){
                (*number_of_combinations)--;
                if(combinations[j] != NULL){
                    combinations[i] = combinations[j];
                    combinations[j] = NULL;
                    break;
                }                
            }
            if(combinations[i] == NULL)
                (*number_of_combinations)--;
        }
    }
}


// cria combinações dos projetos em que a soma de todas as ARTs respeita o valor MAX_SIZE_ART_GROUP
void combine_projects(uint8_t **combinations, unsigned int number_of_combinations, Project* projects, int number_of_projects){
    unsigned int i;
    int j;
    uint8_t combination[number_of_projects];
    printf("Combinando as ARTs de acordo com o teto..\n");
    
    // parallel for aqui
    for (i = 0; i < number_of_combinations; i++) {
        float combination_sum = 0;
        
        for (j = number_of_projects-1; j >= 0; j--) {

            if (((i+1) >> j) & 1){
                combination[number_of_projects-1-j] = 1;
                combination_sum += projects[number_of_projects-1-j].art;
            }else{
                combination[number_of_projects-1-j] = 0;
            }
        }

        if(combination_sum <= MAX_SIZE_ART_GROUP){
            combinations[i] = malloc(number_of_projects * sizeof(uint8_t));
            memcpy(combinations[i], combination, number_of_projects*sizeof(uint8_t));
        }else{
            combinations[i] = NULL;
        }
    }
    printf("ARTs agrupadas!\n\n");
}


// função recursiva auxiliar de create_groups()
int create_groups_aux(uint8_t **combinations, unsigned int number_of_combinations, uint8_t **group, int group_size, int number_of_projects, int start, int level){

    for(unsigned int i = start; i <= number_of_combinations - group_size + level; i++){
        group[level] = combinations[i];

        if(level < group_size-1){
            if(create_groups_aux(combinations, number_of_combinations, group, group_size, number_of_projects, i+1, level+1))
                return 1;
        }else{

            if(verify_group(group, number_of_projects, group_size))
                return 1;

        }
    }

    return 0;
}

// agrupa as combinações de modo a conter todos os projetos, com cada um aparecendo apenas uma vez
void create_groups(uint8_t **combinations, unsigned int number_of_combinations, int number_of_projects, int group_size){
    printf("\nTentando agrupar as combinações em grupo de %d...\n", group_size);
    
    int NUM_THREADS = 1;
    uint8_t ***groups = malloc(NUM_THREADS * sizeof(uint8_t**));
    
    for(int i = 0; i < NUM_THREADS; i++)
        groups[i] = malloc(group_size * sizeof(uint8_t*));

    int success = 0;

    //parallel for
    for(unsigned int i = 0; i <= number_of_combinations - group_size; i++){
        groups[0][0] = combinations[i];
        success = create_groups_aux(combinations, number_of_combinations, groups[0], group_size, number_of_projects, i+1, 1);
        if(success)
            break;
    }

    if(success){
        printf("Grupo encontrado!\n");

        for(int i = 0; i < group_size; i++){
            for (int j = 0; j < number_of_projects; j++) { 
                printf("%d", groups[0][i][j]);
            }
            printf("\n");
        }
    }

}


int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Favor passar um arquivo de entrada!\n");
        exit(1);
    }

    int number_of_projects;
    float sum;

    Project* projects = read_file(argv[1], &number_of_projects, &sum);
    printf("Soma: %f\n", sum);

    if(sum <= MAX_SIZE_ART_GROUP){
        printf("Todas as A.R.Ts podem ser agrupadas em apenas uma!\n");
        exit(0);
    }

    for (int i = 0; i < number_of_projects; i++) {
        printf("%d %.2f\n", projects[i].id, projects[i].art);
    }
    printf("\n");

    unsigned int number_of_combinations = pow(2, number_of_projects) - 1;
    uint8_t **combinations = malloc(number_of_combinations * sizeof(uint8_t*));

    // função paralelizada internamente
    combine_projects(combinations, number_of_combinations, projects, number_of_projects);
    remove_null_lines(combinations, &number_of_combinations);

    for (unsigned int i = 0; i < number_of_combinations; i++) {        
        printf("%d ", i);
        for (int j = 0; j < number_of_projects; j++) { 
            printf("%d", combinations[i][j]);
        }
        printf("\n");
    }

    create_groups(combinations, number_of_combinations, number_of_projects, (int)ceil(sum/MAX_SIZE_ART_GROUP));

    return 0;
}