/**
 * Developed for Sicoob Credivertentes - São Tiago - MG
 *
 * heuristic.c
 *
 * Júlio Resende - julio.cmdr@gmail.com
 *
 */

#ifndef HEURISTIC_H
#define HEURISTIC_H

#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>
#include <omp.h>
#include <math.h>
#include "project.h"

int run_heuristic(Project* projects, int number_of_projects, int group_size);

#endif // HEURISTIC_H