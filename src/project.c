#include "project.h"

Project* read_file(char *file, int *n, float *sum){
    FILE *arq;
    *sum = 0;
    arq = fopen(file, "r");
    Project *projects;

    if (arq == NULL) {
        perror("Erro ao abrir o arquivo!\n");
        exit(1);
    } else {
        fscanf(arq, "%d", n);

        projects = (Project*)malloc(*n * sizeof(Project));
        for (int i = 0; i < *n; i++){
            fscanf(arq, "%d %f", &projects[i].id, &projects[i].art);
            *sum += projects[i].art;
            
            if(projects[i].art >= MAX_ART_SUM){
                printf("ERRO: A ART não pode ser maior que o limite coletivo de %.2f\n", MAX_ART_SUM);
                exit(1);
            }
        }
    }

    return projects;
}