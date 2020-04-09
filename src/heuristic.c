#include "heuristic.h"

void selection_sort(Project *projects, int number_of_projects){
    int max_id;
    Project aux;
    for(int i = 0; i < number_of_projects; i++){
        max_id = i;
        for(int j = i+1; j < number_of_projects; j++){
            if(projects[j].art > projects[max_id].art){
                max_id = j;
            }
        }
        aux = projects[i];
        projects[i] = projects[max_id];
        projects[max_id] = aux;
    }
}

uint64_t bit_update(uint64_t src, uint64_t pos) {
    return src | (1ULL << pos);
} 

// A Heurística inicialmente ordena os projetos em ordem decrescente e depois vai tentando lotar o grupo de combinações
// A combinação é representada por um inteiro positivo 64 bits, em que cada bit representa a presença de um produto na combinaçãos
uint64_t *run_heuristic(Project* projects, int number_of_projects, int group_size){
    
    uint64_t *group = calloc(group_size, sizeof(uint64_t));
    
    selection_sort(projects, number_of_projects);

    float sum;
    uint64_t bitmap = 0;
    uint64_t masc = 0;

    for(int i = 0; i < group_size; i++){
        sum = 0;
        for(int j = 0; j < number_of_projects; j++){
            masc = 0;
            masc = bit_update(masc, j);

            if(sum + projects[j].art <= MAX_ART_SUM && ((masc & bitmap) == 0)){
                group[i] = bit_update(group[i], j);
                bitmap = bit_update(bitmap, j);
                sum += projects[j].art;
            }
        }
    }
    
    
    uint64_t complemento = pow(2, 64) - pow(2, number_of_projects);
    
    if(bitmap + complemento == pow(2, 64) - 1){    
        return group;
    }else{
        return NULL;
    }
}