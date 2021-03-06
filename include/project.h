/**
 * Developed for Sicoob Credivertentes - São Tiago - MG
 *
 * project.c
 *
 * Júlio Resende - julio.cmdr@gmail.com
 *
 */

#ifndef PROJECT_H
#define PROJECT_H

#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <omp.h>

#define MAX_ART_SUM 233.94

typedef struct project {
	int id;
	float art;
}Project;

Project* read_file(char *file, int *n, float *sum);

#endif // PROJECT_H
