#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include "brute_force.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Favor passar um arquivo de entrada e também o número de threads!\n");
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

    for (int i = 0; i < number_of_projects; i++) {
        printf("%d %.2f\n", projects[i].id, projects[i].art);
    }
    printf("\n");
    
    run_brute_force(projects, number_of_projects, (int)ceil(sum/MAX_ART_SUM), num_threads);

    return 0;
}