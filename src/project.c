#include "project.h"

Project* read_file(char *file, int *n, float *sum){
    FILE *arq;
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
        }
    }
    
    return projects;
}

/*
Ecosystem ecosystem_init(char *file) {
	Ecosystem eco;

	FILE *arq;
	arq = fopen(file, "r");

	eco.animal_count[0] = eco.animal_count[1] = eco.rock_count = 0;

	if (arq == NULL) {
		perror("Erro ao abrir o arquivo!\n");
		exit(1);
	} else {
		fscanf(arq, "%d %d %d %d %d %d %d\n", &eco.gen_proc_rabbits, &eco.gen_proc_fox, &eco.gen_food_fox, &eco.n_gen, &eco.l, &eco.c, &eco.n);

		eco.matrix = malloc(eco.l * sizeof *eco.matrix);
		for (int i = 0; i < eco.l; i++) {
			eco.matrix[i] = malloc(eco.c * sizeof *eco.matrix[i]);

			for (int j = 0; j < eco.c; j++) {
				eco.matrix[i][j].type = EMPTY;
				eco.matrix[i][j].index = -1;
			}
		}

		eco.animals[RABBIT] = malloc(eco.l * eco.c * sizeof *eco.animals[RABBIT]);
		eco.animals[FOX] = malloc(eco.l * eco.c * sizeof *eco.animals[FOX]);

		char type[7];
		int l, c;
		void *(*constructors[2])(int, int) = {
			new_rabbit, new_fox
		};

		for (int i = 0; i < eco.n; i++) {
			fscanf(arq,"%s %d %d\n", type, &l, &c);

			if(strcmp(type, "ROCHA") == 0){
				eco.matrix[l][c].type = ROCK;
				eco.matrix[l][c].index = -1;
				eco.rock_count++;
			} else {
				int animal_id = (type[0] % 2 == 0);
				eco.animals[animal_id][eco.animal_count[animal_id]] = constructors[animal_id](l, c);
				eco.matrix[l][c].index = eco.animal_count[animal_id];
				eco.matrix[l][c].type = animal_id;
				eco.animal_count[animal_id]++;
			}
		}
		fclose(arq);
	}
	return eco;
}

bool cell_has_object(const Ecosystem *eco, int l, int c, int type) {
	if (l < 0 || l >= eco->l || c < 0 || c >= eco->c) {
		return false;
	}

	return eco->matrix[l][c].type == type;
}

bool get_next_cell(const Ecosystem *eco, int obj_type, int obj_index, int cell_type) {
    Animal *animal = eco->animals[obj_type][obj_index];
    int l = animal->pos.l;
    int c = animal->pos.c;

    Position adj[4];

    int lo, co, p = 0;
    for(int i = 0; i < 4; i++){

		lo = !(i & 1) * (i - 1);
		co = (i & 1) * (2 - i);

		if (cell_has_object(eco, l + lo, c + co, cell_type)) {
			adj[p].l = l + lo;
			adj[p].c = c + co;
			p++;
		}
    }

	if(p > 0){
		// fórmula para escolha da próxima célula
		animal->next_pos = adj[(eco->current_gen + l + c) % p];
		return true;
	}

	return false;
}

void move_rabbit(Ecosystem *eco, int index){
	Rabbit *rabbit = (Rabbit*) eco->animals[RABBIT][index];

	if (get_next_cell(eco, RABBIT, index, EMPTY)){
		if(rabbit->data.generation == eco->gen_proc_rabbits){
			rabbit->data.child = new_rabbit(rabbit->data.pos.l, rabbit->data.pos.c);
			rabbit->data.generation = -1;
		}
	}

	rabbit->data.generation++;
}

void move_fox(Ecosystem *eco, int index){
	Fox *fox = (Fox*)eco->animals[FOX][index];

	fox->hungry++;
	bool moved = get_next_cell(eco, FOX, index, RABBIT);

	if (!moved) {
		// Killing the fox.
		if (fox->hungry == eco->gen_food_fox) {
			eco->matrix[fox->data.pos.l][fox->data.pos.c].index = -1;
			eco->matrix[fox->data.pos.l][fox->data.pos.c].type = EMPTY;

			fox->data.dead = true;

			return;
		} else {
			// Tries to go to an empty cell.
			moved = get_next_cell(eco, FOX, index, EMPTY);
		}
	}

	if (moved) {
		if(fox->data.generation == eco->gen_proc_fox) {
			fox->data.child = new_fox(fox->data.pos.l, fox->data.pos.c);
			fox->data.generation = -1;
		}
	}
	fox->data.generation++;
}

void ecosystem_resolve_conflicts(Ecosystem *eco, int animal1_index, int type1, int type2) {
	Animal *animal1 = eco->animals[type1][animal1_index];
	Animal *animal2 = NULL;

	if (animal1->dead == false) {
		int d = type1 - type2;

		#pragma omp parallel for private(animal2)
		for (int animal2_index = 0; animal2_index < eco->animal_count[type2]; animal2_index++) {
			animal2 = eco->animals[type2][animal2_index];

			bool kill = false;
			bool kill_by_index = false;
			if (
				animal1 != animal2 && !animal2->dead &&
				animal1->next_pos.l == animal2->next_pos.l &&
				animal1->next_pos.c == animal2->next_pos.c
			) {
				if (d == 0) { // Animais são do mesmo tipo.
					if (animal1->generation < animal2->generation) {
						kill = true;
					} else if (animal1->generation == animal2->generation) {
						kill_by_index = true;
						if (type1 == FOX) {
							if (((Fox *) animal1)->hungry > ((Fox *) animal2)->hungry) {
								kill = true;
								kill_by_index = false;
							} else if (((Fox *) animal1)->hungry < ((Fox *) animal2)->hungry) {
								kill_by_index = false;
							}
						}

						if (kill_by_index) {
							kill = animal1_index > animal2_index;
						}
					}
				} else {
					if (d < 0) { // Animal 2 é raposa e vai comer o coelho.
						kill = true;

						// Reseta a fome.
						((Fox *) eco->animals[FOX][animal2_index])->hungry = 0;
					}
				}

				if (kill) {
					eco->matrix[animal1->pos.l][animal1->pos.c].index = -1;
					eco->matrix[animal1->pos.l][animal1->pos.c].type = EMPTY;

					eco->animals[type1][animal1_index]->dead = true;

					//break;
				}
			}
		}
	}
}

void ecosystem_normalize(Ecosystem *eco, int type) {
	for (int i = 0; i < eco->animal_count[type]; i++) {
		if (eco->animals[type][i] ->dead == true) {
			eco->animal_count[type]--;
			while (i < eco->animal_count[type] && eco->animals[type][eco->animal_count[type]]->dead == true) {
				free(eco->animals[type][eco->animal_count[type]]);
				eco->animal_count[type]--;
			}

			if (i < eco->animal_count[type]) {
				Animal *animal = eco->animals[type][eco->animal_count[type]];

				eco->matrix[animal->pos.l][animal->pos.c].index = i;

				free(eco->animals[type][i]);
				eco->animals[type][i] = animal;
			}
		}
	}
}

void ecosystem_update_position(Ecosystem *eco, int animal_index, int type) {
	Animal *animal = eco->animals[type][animal_index];

	if (animal->dead == true) {
		return;
	}

	if (animal->child != NULL) {
		#pragma omp critical
		{
			eco->animals[type][eco->animal_count[type]] = animal->child;
			eco->matrix[animal->pos.l][animal->pos.c].index = eco->animal_count[type]++;
			animal->child = NULL;
		}
	} else {
		eco->matrix[animal->pos.l][animal->pos.c].type = EMPTY;
		eco->matrix[animal->pos.l][animal->pos.c].index = -1;
	}

	animal->pos = animal->next_pos;
	eco->matrix[animal->pos.l][animal->pos.c].type = type;
	eco->matrix[animal->pos.l][animal->pos.c].index = animal_index;
}

int gverbose = 0;

void ecosystem_print(const Ecosystem *eco) {
	if (gverbose == 0) {
		return;
	}

	if (gverbose > 1) {
		system("clear");
	}

	if (gverbose == 1) {
		printf("%3d | %4d - %4d : %8d\n", eco->current_gen, eco->animal_count[RABBIT], eco->animal_count[FOX], eco->animal_count[RABBIT] + eco->animal_count[FOX]);
		return;
	}

	printf("Gen %d\n", eco->current_gen);
	printf("%3d | %4d - %4d : %8d\n", eco->current_gen, eco->animal_count[RABBIT], eco->animal_count[FOX], eco->animal_count[RABBIT] + eco->animal_count[FOX]);

	for(int i = 0; i < eco->c * 3 + 2; i++) {
		putchar('-');
	}
	putchar('\n');


	for(int i = 0; i < eco->l; i++){
		putchar('|');
		for(int j = 0; j < eco->c; j++){
			if(eco->matrix[i][j].type != EMPTY){
				int type = eco->matrix[i][j].type;
				printf("\033[0;%sm%2d \033[0m", type == RABBIT ? "34" : (type == FOX ? "31" : "33"), eco->matrix[i][j].index);
			}else{
				printf(" - ");
			}
		}
		printf("|\n");
	}

	for(int i = 0; i < eco->c * 3 + 2; i++) {
		putchar('-');
	}
	putchar('\n');

	getchar();
}
*/