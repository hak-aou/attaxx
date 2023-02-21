/* Walid BAKHTI, Hakim AOUDIA, TP 1 */

#include <stdio.h>
#include <stdlib.h>
#include <MLV/MLV_all.h>
#include <math.h>
#include <time.h>


/* CONSTANTE */
#define TAILLE_PLATEAU 7
#define TAILLE_MAX_NOM 20
#define TAILLE_ECRAN 600
#define DECALE 50                   /* Decalage bords de l'écran */
#define DISTANCE_POSE_PION 2        /* Amélioration */
#define DISTANCE_CONTAMINATION 1    /* Amélioration */


/* STRUCTURE */
typedef struct {
    char nom[TAILLE_MAX_NOM];
    char symbol;
    int score;
} Joueur;

typedef struct {
    char plateau [TAILLE_PLATEAU + 2][TAILLE_PLATEAU + 2];
    Joueur * joueurs [2];
} Plateau;




/* -------------- FONCTIONS DE BASE -------------- */

/*
* Créer un plateau de taille TAILLE_PLATEAU*TAILLE_PLATEAU
* en posant les pions des 2 joueurs dans chaque coins.
*
*
*@param Plateau *p Adresse du Plateau
*
*/
void plateauInitialisation(Plateau *p){
    int x = 0;
    int y = 0;
    for(x=0; x<TAILLE_PLATEAU+2; x++) {
        for(y=0; y<TAILLE_PLATEAU+2; y++) {
            if(x==0 || y==0 || x==TAILLE_PLATEAU+1 || y==TAILLE_PLATEAU+1){
                p->plateau[x][y] = '*';
            } else {
                p->plateau[x][y] = '.';
            }

            if((x==1 && y==1) || (x==TAILLE_PLATEAU && y==TAILLE_PLATEAU)){
                p->plateau[x][y] = p->joueurs[1]->symbol;
            }
            if((x==TAILLE_PLATEAU && y==1) || (x==1 && y==TAILLE_PLATEAU)){
                p->plateau[x][y] = p->joueurs[0]->symbol;
            }
      }

   }
}


/*
* Affiche le plateau dans le terminal
*
*
* @param Plateau p qui représente le plateau 
*
*/
void affichePlateau(Plateau p){
    int x, y;
    printf("\n");

    for(x=0; x<TAILLE_PLATEAU+2; x++) {
        for(y=0; y<TAILLE_PLATEAU+2; y++) {
            printf("%c ", p.plateau[x][y]);
        }
        printf("\n");
   }
}


/*
* Détermine si la case ou le joueur pose est valable dans les règles du jeu
*
*
* @param Plateau *p Adresse du Plateau
* int a, int b Valeur x et y d'une case du plateau
* int tour, donne le tour actuel qui va être le joueur actif
*
* @return Renvoie 0 lorsque il y a une case ennemi autour de la case souhaitait et que celle ci est un '.'
* sinon renvoie 1 lorsque n'y a pas d'ennemie autour ou que la case n'est pas un '.'
*/
int caseAutour(Plateau *p, int a, int b, int tour){
    int x;
    int y;
    
    for(x=a-DISTANCE_POSE_PION; x<a+1+DISTANCE_POSE_PION; x++){
        for(y=b-DISTANCE_POSE_PION; y<b+1+DISTANCE_POSE_PION; y++){
            if(p->plateau[x][y] == p->joueurs[abs(tour-1)]->symbol && p->plateau[a][b] == '.'){
                return 0;
            }
        }
    }

    return 1;
}


/*
* Contamine les pions autour de la case a,b les pions ennemis deviennent alors des pions alliées
*
*
* @param Plateau *p Adresse du Plateau
* int a, int b Valeur x et y d'une case du plateau
* int tour donne le tour actuel qui va être le joueur actif
*/
void caseContamineAutour(Plateau *p, int a, int b, int tour){    
    int x;
    int y;

    for(x=a-DISTANCE_CONTAMINATION; x<a+1+DISTANCE_CONTAMINATION; x++){
        for(y=b-DISTANCE_CONTAMINATION; y<b+1+DISTANCE_CONTAMINATION; y++){
            if(p->plateau[x][y] != '.' && p->plateau[x][y] != '*'){
                p->plateau[x][y] = p->joueurs[tour]->symbol;
            }
        }
    }
    
}


/*
* La fonction demande au joueur de poser un pion à l'aide du clavier, elle change ensuite la case du plateau
* en rajoutant le pion du joueur et en contaminant autour de celui ci
*
* @param Plateau *p Adresse du Plateau
* int tour donne le tour actuel qui va être le joueur actif
*/
void placePion(Plateau *p, int tour){
    int x;
    int y;
    printf("\n%s (%c), veuillez saisir les coordonees ou jouer (entre 1 et %d) : ", p->joueurs[tour]->nom, p->joueurs[tour]->symbol, TAILLE_PLATEAU);
    scanf("%d %d", &x, &y);

    while(caseAutour(p, x, y, tour)){
        printf("%s (%c), veuillez saisir les coordonees ou jouer (entre 1 et %d) : ", p->joueurs[tour]->nom, p->joueurs[tour]->symbol, TAILLE_PLATEAU);
        scanf("%d %d", &x, &y);
    }

    /* Pose pion */
    caseContamineAutour(p, x, y, tour);
    p->plateau[x][y] = p->joueurs[tour]->symbol;
}


/*
* Permet de compter le nombre de pion de chaque joueur et modifier le score directement
*
* @param Plateau *p Adresse du Plateau
*/
void compteScore(Plateau *p){
    int x;
    int y;
    p->joueurs[0]->score = 0;
    p->joueurs[1]->score = 0;
    for(x=0; x<TAILLE_PLATEAU+2; x++) {
        for(y=0; y<TAILLE_PLATEAU+2; y++) {
            if(p->plateau[x][y] == p->joueurs[0]->symbol){
                p->joueurs[0]->score++;
            }
            if(p->plateau[x][y] ==  p->joueurs[1]->symbol){
                p->joueurs[1]->score++;
            }
        }
   }
   
}


/*
* Vérifie si la partie est fini ou si elle continue
*
* @param Plateau *p Adresse du Plateau
* @return renvoie 1 si la partie peut continuer sinon renvoie 0 si elle est fini
*/ 
int checkFinPartie(Plateau *p){
    int x;
    int y;

    if(p->joueurs[0]->score == 0 || p->joueurs[1]->score == 0){
        return 0;
    }

    for(x=1; x<TAILLE_PLATEAU+1; x++) {
        for(y=0; y<TAILLE_PLATEAU+1; y++) {
            if(p->plateau[x][y] == '.'){
                return 1;
            }
        }
   }
   
   return 0;
}


/* -------------- PARTIE GRAPHIQUE -------------- */


/*
* Créer une fenêtre et dessine à l'écran les grilles du plateau avec un decalage sur chaque cote
*
* @param Plateau *p Adresse du Plateau
*/ 
void drawGrid(Plateau *p){
    int x, y, x1, y1;
    int bordure = (TAILLE_ECRAN - (DECALE * 2)) / TAILLE_PLATEAU;

    /* float largeur = TAILLE_ECRAN/TAILLE_PLATEAU; */
    MLV_create_window("ok", "ok", TAILLE_ECRAN, TAILLE_ECRAN);
	MLV_draw_filled_rectangle(0 , 0, TAILLE_ECRAN, TAILLE_ECRAN, MLV_COLOR_WHITE);

    for(x = 0; x < TAILLE_PLATEAU; x++){
        for(y = 0; y < TAILLE_PLATEAU; y++){
            x1 = (DECALE+x*bordure);
            y1 = (DECALE+y*bordure);
            MLV_draw_rectangle(x1, y1, bordure, bordure, MLV_COLOR_BLACK);
        }
    }
}


/*
* Dessine à l'écran les pions de chaque joueur
*
* @param Plateau *p Adresse du Plateau
*/ 
void drawPion(Plateau *p){
    float bordure = (TAILLE_ECRAN-(DECALE*2))/TAILLE_PLATEAU;
    
    int x, y;
    for ( x = 0 ; x < TAILLE_PLATEAU+1 ; x += 1 ){
        for ( y = 0 ; y < TAILLE_PLATEAU+1 ; y += 1 ){
            if(p->plateau[x][y] == 'o'){
                MLV_draw_filled_circle( (DECALE+x*bordure)-(bordure/2), (DECALE+y*bordure)-(bordure/2), bordure/4, MLV_COLOR_BLACK);
            }			
            if(p->plateau[x][y] == 'x'){
                MLV_draw_filled_circle( (DECALE+x*bordure)-(bordure/2), (DECALE+y*bordure)-(bordure/2), bordure/4, MLV_COLOR_GREY);
            }

        }
    }
    MLV_actualise_window();

}


/*
* Dessine dans les coins de la fenêtre le score de chaque joueur avec leurs pseudo
*
* @param Plateau *p Adresse du Plateau
*/ 
void drawJoueurScore(Plateau *p){
    char scoreTxt[TAILLE_PLATEAU*TAILLE_PLATEAU];
	int decalage = 0;
    int i;
	MLV_draw_filled_rectangle(0 , 0, TAILLE_ECRAN, 50, MLV_COLOR_WHITE);

	for(i = 0; i < 2; i++){
		if(i == 1){
			decalage = 500-(strlen(p->joueurs[i]->nom)*8);
		} else {
			decalage = 0;
		}

		/* Affiche Pseudo Joueur */
		MLV_draw_text(35+decalage, 10, "Joueur ", MLV_COLOR_BLACK);
		MLV_draw_text(85+decalage, 10, p->joueurs[i]->nom, MLV_COLOR_BLACK);
		
		/* Convertis int en string */
		sprintf(scoreTxt, "%d", p->joueurs[i]->score);
		
		/* Affiche Score */
		MLV_draw_text(35+decalage, 25, "Score ", MLV_COLOR_BLACK);
		MLV_draw_text(85+decalage, 25, scoreTxt, MLV_COLOR_BLACK);
		MLV_actualise_window();

		if(i==0){
			MLV_draw_filled_circle(18, 25, 12, MLV_COLOR_BLACK);
		} 
		if(i==1) {
			MLV_draw_filled_circle(18+decalage, 25, 12, MLV_COLOR_GREY);
		}
	}
	MLV_actualise_window();

}


/*
* Dessine dans la fenêtre quel est le joueur gagnant et de combien de points il a gagné
*
* @param Plateau *p Adresse du Plateau
*/ 
void drawFinVictoire(Plateau *p){
    int taillePseudo;
    char scoreJ1Txt[TAILLE_PLATEAU*TAILLE_PLATEAU];
    char scoreJ2Txt[TAILLE_PLATEAU*TAILLE_PLATEAU];
    int jVictoire = 0;

	MLV_draw_filled_rectangle(0 , 0, TAILLE_ECRAN, TAILLE_ECRAN, MLV_COLOR_WHITE);

    sprintf(scoreJ1Txt, "%d", p->joueurs[0]->score);
    sprintf(scoreJ2Txt, "%d", p->joueurs[1]->score);
    
	if(p->joueurs[0]->score == p->joueurs[1]->score){
		MLV_draw_text(TAILLE_ECRAN/2 - 100, TAILLE_ECRAN/2, "Égalité ", MLV_COLOR_BLACK);
        MLV_draw_text(TAILLE_ECRAN/2 - 50, TAILLE_ECRAN/2,  scoreJ1Txt, MLV_COLOR_BLACK);
        MLV_draw_text(TAILLE_ECRAN/2, TAILLE_ECRAN/2, "à ", MLV_COLOR_BLACK);
        MLV_draw_text(TAILLE_ECRAN/2 + 20, TAILLE_ECRAN/2,  scoreJ2Txt, MLV_COLOR_BLACK);

    } else if(p->joueurs[0]->score > p->joueurs[1]->score){
		jVictoire = 0;
    } else {
		jVictoire = 1;
    }
    
	taillePseudo = (strlen(p->joueurs[jVictoire]->nom)*8);
	 
	MLV_draw_text(TAILLE_ECRAN/2 - 100, TAILLE_ECRAN/2, "Bravo ", MLV_COLOR_BLACK);
	MLV_draw_text(TAILLE_ECRAN/2 - 55, TAILLE_ECRAN/2,  p->joueurs[jVictoire]->nom, MLV_COLOR_RED);
	MLV_draw_text(TAILLE_ECRAN/2 - 55+taillePseudo, TAILLE_ECRAN/2, "vous avez gagne", MLV_COLOR_BLACK);
	MLV_draw_text(TAILLE_ECRAN/2 + 70+taillePseudo, TAILLE_ECRAN/2,  scoreJ2Txt, MLV_COLOR_BLACK);
	MLV_draw_text(TAILLE_ECRAN/2 + 90+taillePseudo, TAILLE_ECRAN/2, "à", MLV_COLOR_BLACK);
	MLV_draw_text(TAILLE_ECRAN/2 + 100+taillePseudo, TAILLE_ECRAN/2,  scoreJ1Txt, MLV_COLOR_BLACK);
	
    
    MLV_actualise_window();
    MLV_wait_seconds(2);	
}


/*
* Dessine en bas de la fenêtre quel joueur doit jouer
*
* @param Plateau *p Adresse du Plateau
* int tour donne le tour actuel qui va être le joueur actif
*/ 
void drawTourJoueur(Plateau *p, int tour){
	MLV_draw_filled_rectangle(0 , TAILLE_ECRAN-40, TAILLE_ECRAN, TAILLE_ECRAN-40, MLV_COLOR_WHITE);
	MLV_actualise_window();
	
	MLV_draw_text(25, TAILLE_ECRAN-30, "Au tour du joueur ", MLV_COLOR_BLACK);
	MLV_draw_text(150, TAILLE_ECRAN-30, p->joueurs[tour]->nom, MLV_COLOR_BLACK);
	MLV_draw_text(155+(strlen(p->joueurs[tour]->nom)*8), TAILLE_ECRAN-30, " de jouer", MLV_COLOR_BLACK);
	if(tour==0){
		MLV_draw_filled_circle(235+(strlen(p->joueurs[tour]->nom)*8), TAILLE_ECRAN-30, 12, MLV_COLOR_BLACK);
	} else {
		MLV_draw_filled_circle(235+(strlen(p->joueurs[tour]->nom)*8), TAILLE_ECRAN-30, 12, MLV_COLOR_GREY);
	}
	MLV_actualise_window();
}


/*
* Permet de récupérer les coordonnées en pixels de la souris du joueur et d'assigner à x et y
* a quel cases ces pixels correspondent sur le plateau
*
* @param Adresse du Plateau, 
* int x, int y deux entiers représentant une case
*/ 
void saisieSouris(int *x, int *y){
	int bordure = (TAILLE_ECRAN-(DECALE*2))/TAILLE_PLATEAU;

	MLV_wait_mouse(&(*x), &(*y));
	while(*x<DECALE || *y<DECALE){
		MLV_wait_mouse(&(*x), &(*y));
	}
	

	*x = (int) (*x-DECALE)/(bordure)+1;
	*y = (int) (*y-DECALE)/(bordure)+1;
}


/*
* Détermine si la case ou le joueur à cliqué est valide ou non selon si les
* coordonnées ne sortent pas du plateau, que la case soit vide et qu'il y a un ennemi a proximité
*
* @param Adresse du Plateau, 
* int x, int y deux entiers représentant une case, 
* int tour donne le tour actuel qui va être le joueur actif
*/ 
int valideSaisie(Plateau *p, int x, int y, int tour){
	if((0<x && x <= TAILLE_PLATEAU) && (0<y && y <= TAILLE_PLATEAU) && p->plateau[x][y] == '.' && caseAutour(p, x, y, tour) == 0){
		p->plateau[x][y] = p->joueurs[tour]->symbol;
		return 0;
	} 
	return 1;
	
}



/* -------------- AMELIORATIONS -------------- */

/*
* Créer des trous dans le plateau de façon aléatoires
*
* @param Adresse du Plateau
* int nbTrou un entier donnant le nombre de trou à créer
*/ 
void trouDansPlateau(Plateau *p, int nbTrou){
	int i;
	for(i=0; i<nbTrou; i++){
		int x = MLV_get_random_integer(1, TAILLE_PLATEAU);
		int y = MLV_get_random_integer(1, TAILLE_PLATEAU);
		
		if(p->plateau[x][y] == '.'){
			p->plateau[x][y] = '*';
		}		
	}

}


/*
* Dessine les trous dans le plateau sous forme de cercle rouge
* 
*
* @param Plateau *p Adresse du Plateau
*/ 
void drawTrou(Plateau *p){
    float bordure = (TAILLE_ECRAN-(DECALE*2))/TAILLE_PLATEAU;
    
    int x, y;
    for(x = 1; x < TAILLE_PLATEAU+1; x += 1){
        for(y = 1; y < TAILLE_PLATEAU+1; y += 1){
            if(p->plateau[x][y] == '*'){
                MLV_draw_filled_circle((DECALE+x*bordure)-(bordure/2), (DECALE+y*bordure)-(bordure/2), bordure/2, MLV_COLOR_RED);
            }			

        }
    }
    MLV_actualise_window();

}


/*
* Bot simple qui permet de poser un pion aléatoirement une case sur le plateau
* 
*
* @param Adresse du Plateau,
* int *x, int *y deux entiers représentant une case, 
* int tour * int tour donne le tour actuel qui va être le joueur actif
*/ 
void botSimple(Plateau *p, int *x, int *y, int tour){
    int i, j, n;
    int place_valide[TAILLE_PLATEAU*TAILLE_PLATEAU][2];
    int cmp = 0;
    for(i = 1 ;i < TAILLE_PLATEAU+1; i += 1 ){
        for(j = 1 ;j < TAILLE_PLATEAU+1; j += 1 ){
            if (caseAutour(&(*p), i, j, tour) == 0){
                place_valide[cmp][0] = i;
                place_valide[cmp][1] = j;
                cmp += 1;
            }
        }
    }

    n = rand() % cmp;
    MLV_wait_milliseconds(250);
    *x = place_valide[n][0];
    *y = place_valide[n][1];
}



/*
* Fais une copie de la structure plateau en le dupliquant
* 
*
* @param Plateau *p1, Plateau *p2 Adresse du Plateau p1 et Adresse du Plateau p2
*/ 
void duplication(Plateau *p1, Plateau *p2){
    int i,j;
    for (i=0; i<TAILLE_PLATEAU+2; i++){
        for (j=0; j<TAILLE_PLATEAU+2; j++){
            (*p1).plateau[i][j] = (*p2).plateau[i][j];
        }
    }
    (*p1).joueurs[0] = (*p2).joueurs[0];
    (*p1).joueurs[1] = (*p2).joueurs[1];
}


/*
* Permet de tester sur une copie d'un plateau et renvoie le nouveau score
* 
*
* @param Plateau *p Adresse du Plateau
* int x, int y deux entiers représentant une case
* @return Renvoie le score du bot
*/ 
int botTeste(Plateau *p, int x, int y){
    caseContamineAutour(&(*p), x, y, 1);
    compteScore(&(*p));
    return p->joueurs[1]->score;
}


/*
* Permet de tester sur une copie d'un plateau et renvoie le nouveau score
* 
*
* @param Plateau *p Adresse du Plateau
* int *x, int *y deux entiers représentant une case
* int tour représente le joueur actif
*/ 
void botDiff(Plateau *p, int *x, int *y, int tour){
    Plateau p1;
    int i,j;
    int place_valide[2];
    int tmp = 0;
    int max = 0;

    duplication(&p1, &(*p));
    srand(time(NULL));

    for(i = 1 ;i < TAILLE_PLATEAU+1; i += 1){
        for(j = 1 ;j < TAILLE_PLATEAU+1; j += 1){
            if(caseAutour(&(p1), i, j, tour) == 0){
                tmp = botTeste(&p1, i, j);
                duplication(&p1, &(*p));
                if ( tmp > max){
                    max = tmp;
                    place_valide[0] = i;
                    place_valide[1] = j;
                }
            }
        }
    }
    MLV_wait_milliseconds(250);
    *x = place_valide[0];
    *y = place_valide[1];
}



int main(int argc, char* argv[]){
    Plateau p;
    Joueur j1;
    Joueur j2;

    int modeGraphique = 0;
    int botmode = 0;
    int difficulte = 0;
    int tour = 0;
    int i;

    srand(time(NULL));

    if(argc <= 2){
        printf("Veuillez donner au moins 2 paramètres \n -a Mode ASCII OU -g Mode Graphique\n -o Joueur vs Bot OU -h Joueur vs Joueur\n");
        exit(EXIT_SUCCESS);
    }

    for(i = 1; i < argc; i += 1){
        if ( strcmp(argv[i], "-a") == 0 ){
            modeGraphique = 0;
        }
        if ( strcmp(argv[i], "-g") == 0 ){
            modeGraphique = 1;
        }
        if ( strcmp(argv[i], "-h") == 0 ){
            botmode = 0;
        }
        if ( strcmp(argv[i], "-o") == 0 ){
            botmode = 1;
        }
    }


    j1.score = 2;
    j2.score = 2;
    j1.symbol = 'o';
    j2.symbol = 'x';

    /* Initialisation */
    printf("Quel est le nom du premier joueur (symbol %c) : ", j1.symbol);
    scanf("%s", (j1).nom);
	
    if ( botmode == 0 ){
        printf("Quel est le nom du second joueur (symbol %c) : ", j2.symbol);
        scanf("%s", (j2).nom);
    } else {
        printf("Quel est la difficulté du bot (simple : 1), (difficile : 2) : ");
        scanf("%d", &difficulte);
        strcpy(j2.nom, "bot");
    }
    p.joueurs[0] = &j1;
    p.joueurs[1] = &j2;

    plateauInitialisation(&p);

	trouDansPlateau(&p, 4); /* Amélioration */

	
	/* DESSIN INITIALISATION */
	if(modeGraphique){
		drawGrid(&p);
		drawPion(&p);
        drawTrou(&p);
		drawJoueurScore(&p);
		drawTourJoueur(&p, 0);
	} else {
		affichePlateau(p);
	}


    /* Boucle principale */
    while(checkFinPartie(&p)){
		if(modeGraphique){
			/* MODE GRAPHIQUE */
			drawTourJoueur(&p, tour%2);
			if (botmode){

                /* Joueur Vs Bot */
                int x, y;
                if(tour%2){
                    /* Tour Bot */
                    if(difficulte==1){
                        botSimple(&p, &x, &y, tour%2);
                    } else {
                        botDiff(&p, &x, &y, tour%2);
                    }
                    valideSaisie(&p, x, y, tour%2);
                } else {
                    /* Tour Joueur */
                    saisieSouris(&x, &y);
                    while(valideSaisie(&p, x, y, tour%2)){
                        saisieSouris(&x, &y);
                    }
                }
                caseContamineAutour(&p, x, y, tour%2);

            } else {

                /* Joueur vs Joueur */
                int x, y;
                saisieSouris(&x, &y);
                while(valideSaisie(&p, x, y, tour%2)){
                    saisieSouris(&x, &y);
                }
                caseContamineAutour(&p, x, y, tour%2);

            }
			compteScore(&p);
			drawPion(&p);
			drawJoueurScore(&p);
			MLV_actualise_window();

		} else {
			/* MODE ASCII */
            if (botmode){

                int x = 0, y = 0;
                if(tour%2){
                    /* Tour Bot */
                    if(difficulte==1){
                        botSimple(&p, &x, &y, tour%2);
                    } else {
                        botDiff(&p, &x, &y, tour%2);
                    }
                    valideSaisie(&p, x, y, tour%2);
                } else {
                    /* Tour Joueur */
                    placePion(&p, tour%2);

                }
                caseContamineAutour(&p, x, y, tour%2);
            } else {
                /* Joueur vs Joueur */
                placePion(&p, tour%2);
            }


			affichePlateau(p);
			compteScore(&p);
			printf("Score actuel : %s(%c) %d - %s(%c) %d\n", j1.nom, j1.symbol, j1.score, j2.nom, j2.symbol, j2.score);
		}
		

        tour++;
    }

    /* Donne le résultat de la partie dans le terminal */
    if(modeGraphique == 0){
        if(p.joueurs[0]->score == p.joueurs[1]->score){
            printf("\nEgalite %d a %d.", p.joueurs[0]->score, p.joueurs[1]->score);
        } else if(p.joueurs[0]->score > p.joueurs[1]->score)
            printf("\nBravo %s, vous avez gagne %d a %d.", p.joueurs[0]->nom, p.joueurs[0]->score, p.joueurs[1]->score);
        else{
            printf("\nBravo %s, vous avez gagne %d a %d.", p.joueurs[1]->nom, p.joueurs[1]->score, p.joueurs[0]->score);
        }

    } else {
        drawFinVictoire(&p);
    }

    return 0;
}
