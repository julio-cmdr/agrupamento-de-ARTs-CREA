#include "brute_force.h"
#include "heuristic.h"

void print_group(Project *projects, int number_of_projects, uint64_t *group, int group_size){
    printf("Grupo encontrado! Combinações em base 10:\n");

    for(int i = 0; i < group_size; i++){
        printf("%I64d\n", group[i]);
        for (int j = number_of_projects-1; j >= 0; j--){
            printf("%d", (int)(((group[i]) >> j) & 1));
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("ERRO: Favor passar um arquivo de entrada e também o número de threads, para caso seja necessário executar o força bruta!\n");
        exit(1);
    }

    int number_of_projects;
    int num_threads = atoi(argv[2]);
    float sum;

    omp_set_num_threads(num_threads);

    Project* projects = read_file(argv[1], &number_of_projects, &sum);
    printf("Soma: %.2f\n", sum);

    if(sum <= MAX_ART_SUM){
        printf("Todas as A.R.Ts podem ser agrupadas em apenas uma coletiva!\n");
        exit(0);
    }
    
    if(number_of_projects > 64){
        printf("Esse código não foi projetado para lidar com mais de 64 ARTs");
        exit(1);
    }

    for (int i = 0; i < number_of_projects; i++) {
        printf("%d %.2f\n", projects[i].id, projects[i].art);
    }
    printf("\n");

    uint64_t *group;

    for(int i = (int)ceil(sum/MAX_ART_SUM); i <= number_of_projects; i++){

        printf("Tentando agrupar as ARTs em %d grupos usando uma heurística gulosa...\n", i);

        group = run_heuristic(projects, number_of_projects, i);
        if(group == NULL){
            printf("Não foi possível agrupar as ARTs em %d grupos utilizando a heurística!\n", i);
            printf("Tentando agrupar as ARTs em %d grupos usando força bruta em %d threads. Isso pode demorar muito...\n", i, num_threads);
            group = run_brute_force(projects, number_of_projects, i, num_threads);

            if(group == NULL){
                printf("Não foi possível agrupar as ARTs em %d grupos!\n", i);
            }
        }

        if(group != NULL){
            print_group(projects, number_of_projects, group, i);
            break;
        }

    }

    return 0;
}