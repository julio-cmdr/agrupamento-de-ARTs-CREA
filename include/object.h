/**
 * Developed for Sicoob Credivertentes - São Tiago - MG
 *
 * Júlio Resende - julio.cmdr@gmail.com
 *
 */

#ifndef OBJECT_H
#define OBJECT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

enum {
	EMPTY = -1,
	RABBIT,
	FOX,
	ROCK,
};

typedef struct position{
	int l, c;
} Position;

typedef struct animal{
	Position pos;
	Position next_pos;
	int type;
	int generation;
	void *child;
	bool dead;
} Animal;

typedef struct fox{
	Animal data;
	int hungry;
}Fox;

typedef struct rabbit{
	Animal data;
}Rabbit;

typedef struct object {
	int type;
	int index;
} Object;

#define TYPEOF(a) ((Animal*)(a))->type

#define KILL(o) ((o).pos.l = -1, (o).pos.c = -1)

void object_print(int type, int l, int c);

void *new_rabbit(int l, int c);

void *new_fox(int l, int c);

#endif // OBJECT_H
