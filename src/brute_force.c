#include "brute_force.h"

// verifica se uma determinada combinação de ARTS é válida de acordo com o valor de MAX_ART_SUM
int combination_is_valid(uint64_t combination, Project *projects, int number_of_projects){
    float sum = 0;
    
    for (int j = number_of_projects-1; j >= 0 && sum <= MAX_ART_SUM; j--){
        sum += projects[number_of_projects-1-j].art * (((combination) >> j) & 1);
    }

    return sum <= MAX_ART_SUM;
}

// verifica se um grupo de combinações contém todos os projetos, com cada um aparecendo apenas uma vez.
// verifica também se a última combinação do grupo é válida. Assume-se que as outras combinações já foram verificadas
int group_is_valid(uint64_t *group, int group_size, Project *projects, int number_of_projects, int thread_id){
    uint8_t bits_sum;
    float sum_last_combination = 0;

    for (int j = number_of_projects-1; j >= 0; j--){
        bits_sum = 0;
        for(int i = 0; i < group_size-1; i++){
            bits_sum += (((group[i]) >> j) & 1);
        }
        bits_sum += (((group[group_size + thread_id - 1]) >> j) & 1);

        sum_last_combination += projects[number_of_projects-1-j].art * (((group[group_size + thread_id - 1]) >> j) & 1);

        if(bits_sum != 1 || sum_last_combination > MAX_ART_SUM)
            return 0;
    }
    return 1;
}

int parallel_analysis(uint64_t *group, int group_size, Project *projects, int number_of_projects, uint64_t start, uint64_t number_of_combinations){
    int winner_thread_id = -1;

    #pragma omp parallel for
    for(uint64_t i = start; i < number_of_combinations; i++){
        group[group_size + omp_get_thread_num() - 1] = i;
        
        if(group_is_valid(group, group_size, projects, number_of_projects, omp_get_thread_num())){
            #pragma omp critical
		    {
                winner_thread_id = omp_get_thread_num();
            }
            break;   // -------------------------------------  isso gera erro! ----------------------------- //
        }
    }

    if(winner_thread_id > 0){
        group[group_size - 1] = group[group_size + winner_thread_id - 1];
        return 1;
    }
    return 0;
}


int generate_groups(uint64_t *group, int group_size, Project *projects, int number_of_projects, uint64_t start, uint64_t number_of_combinations, int level){

    for(uint64_t i = start; i <= number_of_combinations - group_size + level; i++){

        group[level] = i;

        if(combination_is_valid(i, projects, number_of_projects)){
            if(level < group_size-2){
                if(generate_groups(group, group_size, projects, number_of_projects, i+1, number_of_combinations, level+1)){
                    return 1;
                }
            }else{

                if(parallel_analysis(group, group_size, projects, number_of_projects, i+1, number_of_combinations))
                    return 1;
            }
        }
    }

    return 0;
}


int run_brute_force(Project* projects, int number_of_projects, int group_size, int num_threads){
    uint64_t number_of_combinations = pow(2, number_of_projects) - 1;
    
    uint64_t group[group_size + num_threads - 1];

    if(generate_groups(group, group_size, projects, number_of_projects, 1, number_of_combinations, 0)){

        printf("Grupo encontrado! Combinações em base 10:\n");

        for(int i = 0; i < group_size; i++)
            printf("%I64d\n", group[i]);
        
        return 1;

    }else{
        printf("Não é possível agrupar em grupos de %d\n", group_size);
        return 0;
    }
}