/**
 * Developed for Sicoob Credivertentes - São Tiago - MG
 *
 * brute_force.c
 *
 * Júlio Resende - julio.cmdr@gmail.com
 *
 */

#ifndef BRUTE_FORCE_H
#define BRUTE_FORCE_H

#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <omp.h>
#include <math.h>
#include "project.h"

int run_brute_force(Project* projects, int number_of_projects, int group_size, int num_threads);

#endif // BRUTE_FORCE_H
