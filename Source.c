#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>


typedef struct Jedinec {
	int indexy[28];
	int fitness;
	int vystupna_mapa[120];
	double podiel_fitness;
}JEDINEC;

int vypln_mapu(int mapa[], int *pocet_kamenov) {
	int i;
	for (i = 0; i < 120; i++) {
		mapa[i] = 0;
	}

	mapa[17] = -1;
	mapa[25] = -1;
	mapa[40] = -1;
	mapa[50] = -1;
	mapa[68] = -1;
	mapa[69] = -1;


	for (i = 0; i < 120; i++) {
		if (mapa[i] == -1)
			(*pocet_kamenov)++;
	}

	return 0;
}

int generuj_index_vstupu_do_mapy(int vybrate_indexy[], int pocet_vybratych_indexov) {
	int pocet_skontrolovanych = 0, i = 0, j = 0;
	i = (rand() % 44) + 1;
	while (j < pocet_vybratych_indexov) {
		if (i == vybrate_indexy[j]) {
			i = (rand() % 44);
			j = 0;
		}
		else j++;
	}

	return i;
}

int generuj_smer_otocenia() {
	int smer_otocenia = -1;
	smer_otocenia = (rand() % 2) + 1;

	return smer_otocenia;
}

int generuj_prvu_generaciu(JEDINEC **generacia) {
	int i, nahodny_index, pocet_jedincov = 0, vybrate_indexy[20];

	while (pocet_jedincov < 20) {
		for (i = 0; i < 20; i++) {
			nahodny_index = generuj_index_vstupu_do_mapy(vybrate_indexy, i);
			(*generacia + pocet_jedincov)->indexy[i] = nahodny_index;
			vybrate_indexy[i] = nahodny_index;
		}

		while (i < 28) {
			(*generacia + pocet_jedincov)->indexy[i] = generuj_smer_otocenia();
			i++;
		}
		pocet_jedincov++;
	}
	
	return 0;	
}


int generacia_init(JEDINEC** generacia) {
	JEDINEC* nova_generacia;
	nova_generacia = (JEDINEC*)malloc(20 * sizeof(JEDINEC));
	for (int i = 0; i < 20; i++)
		for (int j = 0; j < 28; j++) {
			(nova_generacia + i)->indexy[j] = -1;
			(nova_generacia + i)->fitness = -1;
			(nova_generacia + i)->podiel_fitness = -1;
		}
		
	for(int i = 0; i < 20; i++)
		for (int j = 0; j < 120; j++)
		(nova_generacia + i)->vystupna_mapa[j]= -2;

	
	*generacia = nova_generacia;

	return 0;
}

int zobraz_mapu(int mapa[]) {
	for (int a = 0; a < 120; a++) {
		printf("%d   ", mapa[a]);
		if (a % 12 == 11)
			printf("\n");
	}	printf("\n\n\n\n\n");
}

int chod_dole(int *popisana_mapa, JEDINEC* jedinec, int poradie_jedinca, int *posledny_index, int pocet_vstupov, int *pocet_otociek, bool *koniec_tahu_jedinca) {
	int index_mapy = *posledny_index, smer_otocenia;

	if (popisana_mapa[index_mapy] != 0) {
		*koniec_tahu_jedinca = true;
		return 0;
	}

	while (index_mapy < 108 && popisana_mapa[index_mapy + 12] == 0) {
			popisana_mapa[index_mapy] = pocet_vstupov;				//1 je otocenie dolava, 2 je otocenia doprava
			index_mapy = index_mapy + 12;
			*posledny_index = index_mapy;
			continue;
	}
	popisana_mapa[index_mapy] = pocet_vstupov;
	//zobraz_mapu(popisana_mapa);

		if (index_mapy < 108 && (popisana_mapa[index_mapy + 12] == -1 || popisana_mapa[index_mapy + 12] != 0)) {
				popisana_mapa[index_mapy] = pocet_vstupov;
				smer_otocenia = (jedinec + poradie_jedinca - 1)->indexy[20 + ((*pocet_otociek) % 8)];
				(*pocet_otociek)++;
				if (smer_otocenia == 1) {	//doprava
					if (popisana_mapa[index_mapy + 1] == 0 && index_mapy  % 12 != 0) {
						*posledny_index = index_mapy + 1;
						chod_doprava(popisana_mapa, jedinec, poradie_jedinca, posledny_index, pocet_vstupov, pocet_otociek, koniec_tahu_jedinca);
					}

					else if (index_mapy  % 12 == 0) {
						popisana_mapa[index_mapy] = pocet_vstupov;
						return 0;
					}

					else {
						*koniec_tahu_jedinca = 1;
						popisana_mapa[index_mapy] = pocet_vstupov;
						return 0;
					}
				}

				else if (smer_otocenia == 2) {	//dolava
					if (popisana_mapa[index_mapy - 1] == 0 && index_mapy % 12 != 0) {
						*posledny_index = index_mapy - 1;
						chod_dolava(popisana_mapa, jedinec, poradie_jedinca, posledny_index, pocet_vstupov, pocet_otociek, koniec_tahu_jedinca);
					}

					else if (index_mapy % 12 == 0) {
						popisana_mapa[index_mapy] = pocet_vstupov;
						return 0;
					}

					else {
						*koniec_tahu_jedinca = 1;
						popisana_mapa[index_mapy] = pocet_vstupov;
						return 0;
					}
				}
			
		else return 0;
		}
		popisana_mapa[index_mapy] = pocet_vstupov;
	return 0;
}

int chod_hore(int *popisana_mapa, JEDINEC* jedinec, int poradie_jedinca, int* posledny_index, int pocet_vstupov, int *pocet_otociek, bool *koniec_tahu_jedinca) {
	int index_mapy = *posledny_index, smer_otocenia;

	if (popisana_mapa[index_mapy] != 0) {
		*koniec_tahu_jedinca = true;
		return 0;
	}

	while (index_mapy > 11 && popisana_mapa[index_mapy - 12] == 0) {
			popisana_mapa[index_mapy] = pocet_vstupov;
			index_mapy = index_mapy - 12;
			*posledny_index = index_mapy;
			continue;
		}

	if (index_mapy > 11 && (popisana_mapa[index_mapy - 12] == -1 || popisana_mapa[index_mapy - 12] != 0)) {
		smer_otocenia = (jedinec + poradie_jedinca - 1)->indexy[20 + ((*pocet_otociek) % 8)]; 
			(*pocet_otociek)++;
			popisana_mapa[index_mapy] = pocet_vstupov;
			if (smer_otocenia == 1) {	//dolava
				if (popisana_mapa[index_mapy - 1] == 0 && index_mapy % 12 != 0) {
					*posledny_index = index_mapy - 1;
					chod_dolava(popisana_mapa, jedinec, poradie_jedinca, posledny_index, pocet_vstupov, pocet_otociek, koniec_tahu_jedinca);
				}

				else if (index_mapy % 12 == 0) {
					popisana_mapa[index_mapy] = pocet_vstupov;
					return 0;
				}

				else {
					*koniec_tahu_jedinca = 1;
					popisana_mapa[index_mapy] = pocet_vstupov;
					return 0;
				}
			}

			else if (smer_otocenia == 2) {	//doprava
				if (popisana_mapa[index_mapy + 1] == 0 && index_mapy % 12 != 11 ) {
					*posledny_index = index_mapy + 1;
					chod_doprava(popisana_mapa, jedinec, poradie_jedinca, posledny_index, pocet_vstupov, pocet_otociek, koniec_tahu_jedinca);
				}

				else if (index_mapy % 12 == 11) {
					popisana_mapa[index_mapy] = pocet_vstupov;
					return 0;
				}

				else {
					*koniec_tahu_jedinca = 1;
					popisana_mapa[index_mapy] = pocet_vstupov;
					return 0;
				}
			}
		
		else return 0;
	}
	popisana_mapa[index_mapy] = pocet_vstupov;
//	zobraz_mapu(popisana_mapa);
	return 0;
}

int chod_doprava(int *popisana_mapa, JEDINEC* jedinec, int poradie_jedinca, int* posledny_index, int pocet_vstupov, int *pocet_otociek, bool *koniec_tahu_jedinca) {
	int index_mapy = *posledny_index, smer_otocenia;
	
	if (popisana_mapa[index_mapy] != 0) {
		*koniec_tahu_jedinca = true;
		return 0;
	}

	while (index_mapy % 12 != 11 && popisana_mapa[index_mapy + 1] == 0) {
			popisana_mapa[index_mapy] = pocet_vstupov;
			index_mapy++;
			*posledny_index = index_mapy;
			continue;
		
	}

		if (index_mapy % 12 != 11 && (popisana_mapa[index_mapy + 1] == -1 || popisana_mapa[index_mapy + 1] != 0)) {
				popisana_mapa[index_mapy] = pocet_vstupov;
				smer_otocenia = (jedinec + poradie_jedinca - 1)->indexy[20 + ((*pocet_otociek) % 8)];
				(*pocet_otociek)++;
				if (smer_otocenia == 1) {	//hore
					if (popisana_mapa[index_mapy - 12] == 0 && index_mapy > 11) {
						*posledny_index = index_mapy - 12;
						chod_hore(popisana_mapa, jedinec, poradie_jedinca, posledny_index, pocet_vstupov, pocet_otociek, koniec_tahu_jedinca);
					}

					else if (index_mapy <= 11) {
						popisana_mapa[index_mapy] = pocet_vstupov;
						return 0;
					}

					else {
						*koniec_tahu_jedinca = 1;
						popisana_mapa[index_mapy] = pocet_vstupov;
						return 0;
					}
				}

				else if (smer_otocenia == 2) {	//dole
					if (popisana_mapa[index_mapy + 12] == 0 && index_mapy % 12 != 11) {
						*posledny_index = index_mapy + 12;
						chod_dole(popisana_mapa, jedinec, poradie_jedinca, posledny_index, pocet_vstupov, pocet_otociek, koniec_tahu_jedinca);
					}

					else if (index_mapy % 12 == 11) {
						popisana_mapa[index_mapy] = pocet_vstupov;
						return 0;
					}
					else {
						popisana_mapa[index_mapy] = pocet_vstupov;
						*koniec_tahu_jedinca = 1;
						return 0;
					}
			}
		
		else return 0;
	}
		popisana_mapa[index_mapy] = pocet_vstupov;
		return 0;
}

int chod_dolava(int *popisana_mapa, JEDINEC* jedinec, int poradie_jedinca, int* posledny_index, int pocet_vstupov, int *pocet_otociek, bool *koniec_tahu_jedinca) {
	int index_mapy = *posledny_index, smer_otocenia;
	
	if (popisana_mapa[index_mapy] != 0) {
		*koniec_tahu_jedinca = true;
		return 0;
	}

	while (index_mapy % 12 != 0 && popisana_mapa[index_mapy-1] == 0) {
		popisana_mapa[index_mapy] = pocet_vstupov;
		index_mapy--;
		*posledny_index = index_mapy;
		continue;
	}

		if (index_mapy % 12 != 0 && (popisana_mapa[index_mapy - 1] == -1 || popisana_mapa[index_mapy - 1] != 0)) {
				popisana_mapa[index_mapy] = pocet_vstupov;
				smer_otocenia = (jedinec + poradie_jedinca - 1)->indexy[20 + ((*pocet_otociek) % 8)]; 
				(*pocet_otociek)++;
				if (smer_otocenia == 1) {	//dole
					if (popisana_mapa[index_mapy + 12] == 0 && index_mapy < 108) {
						*posledny_index = index_mapy + 12;
						chod_dole(popisana_mapa, jedinec, poradie_jedinca, posledny_index, pocet_vstupov, pocet_otociek, koniec_tahu_jedinca);
					}

					else if (index_mapy > 108) {
						popisana_mapa[index_mapy] = pocet_vstupov;
						return 0;
					}

					else {
						*koniec_tahu_jedinca = 1;
						popisana_mapa[index_mapy] = pocet_vstupov;
						return 0;
					}
						
				}

				else if (smer_otocenia == 2) {	//hore
					if (popisana_mapa[index_mapy - 12] == 0 && index_mapy > 11) {
						*posledny_index = index_mapy - 12;
						chod_hore(popisana_mapa, jedinec, poradie_jedinca, posledny_index, pocet_vstupov, pocet_otociek, koniec_tahu_jedinca);
					}
					
					else if (index_mapy <= 11) {
						popisana_mapa[index_mapy] = pocet_vstupov;
						return 0;
					}

					else {
						popisana_mapa[index_mapy] = pocet_vstupov;
						*koniec_tahu_jedinca = 1;
						return 0;
					}
						
				}
			
			else 
				return 0;
		}
		popisana_mapa[index_mapy] = pocet_vstupov;

		return 0;
}

int pocet_bodov(int popisana_mapa[]) {
	int body = 0;
	for (int a = 0; a < 120; a++) {
		if (popisana_mapa[a] != 0 && popisana_mapa[a] != -1)
			body++;
	}

	return body;
}

int zisti_fitness(int vstupna_mapa[], JEDINEC *jedinec, int poradie_jedinca) {
	int index_vstupu = 0, popisana_mapa[120], i, pocet_otociek = 0, pocet_vstupov = 1, pocet_posunov = 0, smer_otocenia = 0, fitness = 0, posledny_index;
	char smer_pohybu[6];
	bool koniec_tahu_jedinca = 0;

	posledny_index = (jedinec + poradie_jedinca - 1)->indexy[pocet_vstupov-1];

	for (i = 0; i < 120; i++)
		popisana_mapa[i] = vstupna_mapa[i];


	while (pocet_vstupov < 20 && koniec_tahu_jedinca == 0) {
		index_vstupu = (jedinec + poradie_jedinca - 1)->indexy[pocet_vstupov-1];

		if (index_vstupu >= 0 && index_vstupu <= 11) {
			strncpy(smer_pohybu, "dole", 4);
			posledny_index = index_vstupu;
			chod_dole(popisana_mapa, jedinec, poradie_jedinca, &posledny_index, pocet_vstupov, &pocet_otociek, &koniec_tahu_jedinca);
		}

		else if (index_vstupu >= 12 && index_vstupu <= 21)
		{
			if (index_vstupu == 12) posledny_index = 0;
			else if (index_vstupu == 13) posledny_index = 12;
			else if (index_vstupu == 14) posledny_index = 24;
			else if (index_vstupu == 15) posledny_index = 36;
			else if (index_vstupu == 16) posledny_index = 48;
			else if (index_vstupu == 17) posledny_index = 60;
			else if (index_vstupu == 18) posledny_index = 72;
			else if (index_vstupu == 19) posledny_index = 84;
			else if (index_vstupu == 20) posledny_index = 96;
			else if (index_vstupu == 21) posledny_index = 108;
			strncpy(smer_pohybu, "doprava", 6);
			chod_doprava(popisana_mapa, jedinec, poradie_jedinca, &posledny_index, pocet_vstupov, &pocet_otociek, &koniec_tahu_jedinca);
		}
			
		else if (index_vstupu >= 22 && index_vstupu <= 33)
		{
			posledny_index = index_vstupu + 86;
			strncpy(smer_pohybu, "hore", 4);
			chod_hore(popisana_mapa, jedinec, poradie_jedinca, &posledny_index, pocet_vstupov, &pocet_otociek, &koniec_tahu_jedinca);
		}
			

		else {
			if (index_vstupu == 43) posledny_index = 11;
			else if (index_vstupu == 42) posledny_index = 23;
			else if (index_vstupu == 41) posledny_index = 35;
			else if (index_vstupu == 40) posledny_index = 47;
			else if (index_vstupu == 39) posledny_index = 59;
			else if (index_vstupu == 38) posledny_index = 71;
			else if (index_vstupu == 37) posledny_index = 83;
			else if (index_vstupu == 36) posledny_index = 95;
			else if (index_vstupu == 35) posledny_index = 107;
			else if (index_vstupu == 34) posledny_index = 119;
			chod_dolava(popisana_mapa, jedinec, poradie_jedinca, &posledny_index, pocet_vstupov, &pocet_otociek, &koniec_tahu_jedinca);
		}
		pocet_vstupov++; 
	}

	if (koniec_tahu_jedinca == 1) {
		fitness = pocet_bodov(popisana_mapa);
		for(int a = 0; a < 120; a++)
			(jedinec + poradie_jedinca - 1)->vystupna_mapa[a] = popisana_mapa[a];
	}

	return fitness;
}

int spocitaj_celkove_fitness(JEDINEC* generacia) {
	int i = 0;
	float celkove_fitness = 0, celkove_fitness_jeden_diel;

	for (i = 0; i < 20; i++) 
		celkove_fitness += generacia[i].fitness;
	
//	printf("Celkove fitness %f\n", celkove_fitness);

	celkove_fitness_jeden_diel = celkove_fitness / 20;

	for (i = 0; i < 20; i++)
		generacia[i].podiel_fitness = (double) (celkove_fitness_jeden_diel * generacia[i].fitness);

	//for (i = 0; i < 20; i++)
	//	printf("Jedinec cislo %d, celkove fitness %d, podiel fitness %f\n", i, generacia[i].fitness, generacia[i].podiel_fitness);

	return 0;
}

//2 jedincov bude uplne novych 
int ruleta(JEDINEC** stara_generacia) {
	int vybraty_jedinec = -1, i, j, k, pravdepodobnost = 0, sucet_fitness = 0, vybrate_body_jedinca = 0, suma = 0, vybraty_druhy_rodic = 0, index_noveho_jedinca = 0;
	JEDINEC* nova_generacia = (JEDINEC*) malloc (20 * sizeof(JEDINEC));
	JEDINEC *stara_gen = *stara_generacia;


	for (int i = 0; i < 20; i++)
		for (int j = 0; j < 28; j++) {
			(nova_generacia + i)->indexy[j] = -1;
			(nova_generacia + i)->fitness = -1;
			(nova_generacia + i)->podiel_fitness = -1;
		}

	for (int i = 0; i < 20; i++)
		for (int j = 0; j < 120; j++)
			(nova_generacia + i)->vystupna_mapa[j] = -2;


	for (i = 0; i < 20; i++) 
		sucet_fitness += stara_gen[i].fitness;
	 
	for (k = 0; k < 9; k++) {
		vybrate_body_jedinca = (rand() % sucet_fitness);
		suma = 0;
		for (i = 0; i < 20; i++) {
			suma += stara_gen[i].fitness;
			if (suma > vybrate_body_jedinca)
				break;
		}
	

		while (vybraty_druhy_rodic == 0) {
			suma = 0;
			vybrate_body_jedinca = rand() % sucet_fitness;
			for (j = 0; j < 20; j++) {
				suma += stara_gen[i].fitness;
				if (suma > vybrate_body_jedinca) {
					if (i == j) 
						break;
					else {
						vybraty_druhy_rodic = 1;
						break;
					}
				}
			}
		}
		vybraty_druhy_rodic = 0;
		//printf("Rodic cislo 2 ma index %d\n", j);
		
		krizenie_rodicov(stara_gen, &nova_generacia, i, j, &index_noveho_jedinca);
	}

	int nahodny_index = 0, vybrate_indexy[20], pocet_jedincov = 18, o = 0;
	while (o < 2) {
		for (int a = 0; a < 20; a++)
			vybrate_indexy[a] = 0;
		for (i = 0; i < 20; i++) {
			nahodny_index = generuj_index_vstupu_do_mapy(vybrate_indexy, i);
			(nova_generacia + pocet_jedincov)->indexy[i] = nahodny_index;
			vybrate_indexy[i] = nahodny_index;
		}

		while (i < 28) {
			(nova_generacia + pocet_jedincov)->indexy[i] = generuj_smer_otocenia();
			i++;
		}
		pocet_jedincov++;
		o++;
	}

	/*printf("V rulete\n");
	for (int i = 0; i < 20; i++) {
		printf("Jedinec cislo %d: ", i);
		for (int j = 0; j < 28; j++)
			printf("%d ", nova_generacia[i].indexy[j]);
		printf("\n");
	}*/


	//stara_generacia = nova_generacia;
	free(stara_gen);
	*stara_generacia = nova_generacia;
	return 0;
}

int mutacia(JEDINEC *jedinec_s_mutaciou) {
	int gen_na_mutaciu = 0, novy_index = -1;

	gen_na_mutaciu = rand() % 28;

	if (gen_na_mutaciu < 20) {
		novy_index = generuj_index_vstupu_do_mapy(jedinec_s_mutaciou->indexy, 20);
	}
	
	else if (gen_na_mutaciu > 20 && gen_na_mutaciu < 28) {
		if (jedinec_s_mutaciou->indexy[gen_na_mutaciu] == 1)
			jedinec_s_mutaciou->indexy[gen_na_mutaciu] = 2;
		else
			jedinec_s_mutaciou->indexy[gen_na_mutaciu] = 1;
	}

	return 0;
}

int turnaj(JEDINEC **stara_generacia) {
	// vyberam 4 jedincov do turnaja 
	int pocet_vybratych_jedincov = 0, index_vyhercu_turnaja_jeden = -1, i = 0, j = 0, k = 0, vybraty_jedinec = 0, prve_pole_vybratych_jedincov[4] = { 0 };
	int  druhe_pole_vybratych_jedincov[4] = { 0 }, index_vyhercu_turnaja_dva = -1, max = -1, porovnavana_fitness = -1, index_noveho_jedinca = 0, vyherny_index = -1;
	JEDINEC* nova_generacia = (JEDINEC*)malloc(20 * sizeof(JEDINEC));
	JEDINEC* stara_gen = *stara_generacia;

	for (int i = 0; i < 20; i++)
		for (int j = 0; j < 28; j++) {
			(nova_generacia + i)->indexy[j] = -1;
			(nova_generacia + i)->fitness = -1;
			(nova_generacia + i)->podiel_fitness = -1;
		}

	for (int i = 0; i < 20; i++)
		for (int j = 0; j < 120; j++)
			(nova_generacia + i)->vystupna_mapa[j] = -2;


	for (k = 0; k < 9; k++) {
		vybraty_jedinec = rand() % 20;
		i = 0;
		for (int a = 0; a < 4; a++) {
			prve_pole_vybratych_jedincov[a] = -1;
			druhe_pole_vybratych_jedincov[a] = -1;
		}
			
		while (i < 4) {
			vybraty_jedinec = rand() % 20;
			prve_pole_vybratych_jedincov[i] = vybraty_jedinec;
			j = 0;
			pocet_vybratych_jedincov++;
			i++;	
		}


		pocet_vybratych_jedincov = 0;
		int o = 0, i = 0;
		while (i < 4) {
			vybraty_jedinec = rand() % 20;
			o = 0;
			while (o < 4) {
				if (vybraty_jedinec == prve_pole_vybratych_jedincov[o]) {
					vybraty_jedinec = rand() % 20;
					o = 0;
				}
				else o++;
			}
			pocet_vybratych_jedincov++;
			
			druhe_pole_vybratych_jedincov[i] = vybraty_jedinec;
			i++;
		}


		int bojovnik = prve_pole_vybratych_jedincov[0];
		// mam pole so 4 jedincami, idem vybrat najlepsieho z nich
		max = 0;
		for (i = 0; i < 4; i++) {
			bojovnik = prve_pole_vybratych_jedincov[i];
			porovnavana_fitness = stara_gen[bojovnik].fitness;
			if (porovnavana_fitness > max) {
				max = porovnavana_fitness;
				vyherny_index = bojovnik;
			}		
		}

		index_vyhercu_turnaja_jeden = vyherny_index;
		max = 0;

		//druhy turnaj
		vyherny_index = -1, porovnavana_fitness = 0, max = 0;
		for (i = 0; i < 4; i++) {
			bojovnik = druhe_pole_vybratych_jedincov[i];
			porovnavana_fitness = stara_gen[bojovnik].fitness;
			if (porovnavana_fitness > max) {
				max = porovnavana_fitness;
				vyherny_index = bojovnik;
			}	
		}
		index_vyhercu_turnaja_dva = vyherny_index;

		krizenie_rodicov(stara_gen, &nova_generacia, index_vyhercu_turnaja_jeden, index_vyhercu_turnaja_dva, &index_noveho_jedinca);
	}

	int nahodny_index = 0, vybrate_indexy[20], pocet_jedincov = 18, o = 0;
	while (o < 2) {
		for (int a = 0; a < 20; a++)
			vybrate_indexy[a] = 0;
		for (i = 0; i < 20; i++) {
			nahodny_index = generuj_index_vstupu_do_mapy(vybrate_indexy, i);
			(nova_generacia + pocet_jedincov)->indexy[i] = nahodny_index;
			vybrate_indexy[i] = nahodny_index;
		}

		while (i < 28) {
			(nova_generacia + pocet_jedincov)->indexy[i] = generuj_smer_otocenia();
			i++;
		}
		pocet_jedincov++;
		o++;
	}

	free(stara_gen);
	*stara_generacia = nova_generacia;
	return 0;
}

int krizenie_rodicov(JEDINEC *generacia, JEDINEC **nova_generacia, int prvy_rodic, int druhy_rodic, int *index_noveho_jedinca) {
	int miesto_rozdelenia = 0, i = 0, pravdepodobnost_mutacie = 0;

	JEDINEC* nova_gen = *nova_generacia;

	miesto_rozdelenia = (rand() % 26) + 1;

	for (i = 0; i < miesto_rozdelenia; i++) 
		(nova_gen+*index_noveho_jedinca)->indexy[i] = generacia[prvy_rodic].indexy[i];
	
	for(i = miesto_rozdelenia; i < 28; i++)
		(nova_gen + *index_noveho_jedinca)->indexy[i] = generacia[druhy_rodic].indexy[i];

	pravdepodobnost_mutacie = rand() % 101; //od 0 po 100
	if (pravdepodobnost_mutacie < 5)
		mutacia(nova_gen + *index_noveho_jedinca);

	(*index_noveho_jedinca)++;


	for (i = 0; i < miesto_rozdelenia; i++)
		(nova_gen + *index_noveho_jedinca)->indexy[i] = generacia[druhy_rodic].indexy[i];

	for (i = miesto_rozdelenia; i < 28; i++)
		(nova_gen + *index_noveho_jedinca)->indexy[i] = generacia[prvy_rodic].indexy[i];
	
	(*index_noveho_jedinca)++;

	*nova_generacia = nova_gen;

	return 0;
}

int main() {
	JEDINEC* generacia = NULL, *dalsie_generacie = NULL;
	int start_mapa[120], poradove_cislo_jedinca = 1, fitness_body = 0, pocet_kamenov = 0, obsah = 120, max = -1, mam_pohrabane = 0, pocet_pohrabanych = 0,
		pocet_generacii = 0, index_s_max = -1;
	clock_t start_cas, end;
	double time_spent = 0;
	float konstanta_pohrabania = 0;
	char sposob_vyberu_jedinca;

	printf("Ruleta (r) alebo turnaj (t)?\n");
	scanf("%c", &sposob_vyberu_jedinca);

	printf("Kolko zahradky treba pohrabat?\n");
	scanf("%f", &konstanta_pohrabania);

	srand(time(0));

	vypln_mapu(start_mapa, &pocet_kamenov);
	generacia_init(&generacia);
	generuj_prvu_generaciu(&generacia);

	
	start_cas = clock();
	while (mam_pohrabane == 0) {	
		pocet_generacii++;
		poradove_cislo_jedinca = 1;
	while (poradove_cislo_jedinca <= 20) {
		fitness_body = zisti_fitness(start_mapa, generacia, poradove_cislo_jedinca);
		(generacia + poradove_cislo_jedinca - 1)->fitness = fitness_body;
	
		poradove_cislo_jedinca++;
	}

	spocitaj_celkove_fitness(generacia);

	for (int j = 0; j < 20; j++) {
		pocet_pohrabanych = (generacia + j)->fitness;
		if (pocet_pohrabanych > max) {
			max = pocet_pohrabanych;
			index_s_max = j;
		}			
	}

	if (max > ((obsah - pocet_kamenov)* konstanta_pohrabania)) {
		mam_pohrabane = 1;
		break;
	}

	if(sposob_vyberu_jedinca == 'r')
		ruleta(&generacia);
	else if(sposob_vyberu_jedinca == 't')
		turnaj(&generacia);
	}
	

	zobraz_mapu((generacia + index_s_max)->vystupna_mapa);
	printf("\nPocet generacii %d\n", pocet_generacii);


	end = clock();
	time_spent = (double)(end - start_cas) / CLOCKS_PER_SEC;
	printf("Cas %f\n", time_spent);

	return 0;
}