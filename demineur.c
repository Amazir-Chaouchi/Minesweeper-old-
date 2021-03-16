#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int nbColonnes;
int nbLignes;
cell** matrice;
int pourcentMines;

typedef struct cellule {
    int value;  // nombre de mines autour (0 - 8) ou -1 si mine sur case.
} cell;

/* ##### PROTOTYPES DES FONCTIONS ######################################################### */

int AskForFieldSize() {
    int dimension = 0;
    printf("Please enter field dimension :");
    scanf("%d", &dimension);
    return dimension;
}

void InitMatrice() {
    nbColonnes = AskForFieldSize();
    nbLignes = AskForFieldSize();

    matrice = malloc(sizeof(int*) * nbLignes);
    for (int i = 0; i < nbLignes; i++) {
        matrice[i] = malloc(sizeof(int) * nbColonnes);
        for (int j = 0; j < nbColonnes; j++) {
            matrice[i][j] = 0;
        }
    }
}

void DrawField() {
    printf("    ");
    for (int i = 1; i <= nbColonnes; i++) {
        printf("%3d ", i);
    }
    printf("\n");

    for (int i = 0; i < nbLignes; i++) {
        printf("%3d|", i + 1);
        for (int j = 0; j < nbColonnes; j++) {
            printf("%3d ", matrice[i][j]);
        }
        printf("\n");
    }
}

void FreeMatrice() {
    for (int i = 0; i < nbLignes; i++) {
        free(matrice[i]);
    }
    free(matrice);
}

/* ##### CORPS DU PROGRAMME ############################################################### */

void main() {
    InitMatrice();
    scanf("%d", &pourcentMines);
    srand(time(NULL));
    for (int i = 0; i < ((pourcentMines * nbColonnes * nbLignes) / 100); i++)
    {
        random_i = rand() % nbLignes;
        random_j = rand() % nbColonnes;
        matrice[random_i][random_j].value = -1;
    }
    int randMine = rand();

    DrawField();

    FreeMatrice();
}
