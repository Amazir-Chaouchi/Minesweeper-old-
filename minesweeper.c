#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define MAX_ROW 40
#define MAX_COL 65
#define MIN_PERCENT 5
#define MAX_PERCENT 70

#define HIDDEN 0
#define VISIBLE 1
#define FLAGGED 2

#define MINE -1

/* TYPE DEFINITIONS */

typedef struct s_cell {
    int value;
    int status;
} t_cell;

typedef struct s_point {
    int x;
    int y;
}t_point;
    
/* GLOBAL VARIABLES DECLARATIONS */
bool gameOver = false;
int row, col, minePercent;
t_cell** matrix;

/* FUNCTION PROTOTYPES */
t_point GetCoordinates() {
    char buff[256];
    int argCounter = 0;
    t_point coordinates;
    
    do {
        printf("Enter coordinates :");
        fgets(buff, sizeof(buff), stdin);
        argCounter = sscanf(buff, "%d %d", &coordinates.x, &coordinates.y);
    } while (argCounter < 2 || coordinates.x < 0 || coordinates.x >= row || coordinates.y < 0 || coordinates.y >= col);
    
    printf("Selected cell : (%d ; %d)\nActualizing field...\n", coordinates.x, coordinates.y);
    
    return coordinates;
}
void EvaluateMove(t_point cell) {
    if(matrix[cell.x][cell.y].status == FLAGGED) {
        printf("This cell has already been flagged. Please select an other one.\n");
    }
    else {
        switch(matrix[cell.x][cell.y].value) {
            case MINE:
                gameOver = true;
                printf("You fell on a mine. GAMO OVER.\n");
                break;
            case 0:                
                for(int k = -1; k < 2; k++) {
                    for(int l = -1; l < 2; l++) {
                /* ...si cette case existe... */
                        if(cell.x+k >= 0 && cell.x+k < row && cell.y+l >= 0 && cell.y+l < col) {
                /* ...et si ce n'est ni une mine, ni une case flaggged... */
                            if(matrix[cell.x+k][cell.y+l].value != MINE && matrix[cell.x+k][cell.y+l].status != FLAGGED && matrix[cell.x+k][cell.y+l].status != VISIBLE) {
                                matrix[cell.x+k][cell.y+l].status = VISIBLE;
                                if(matrix[cell.x+k][cell.y+l].value == 0) {
                                    cell.x = cell.x+k;
                                    cell.y = cell.y+l;
                                    EvaluateMove(cell);
                                }
                            }
                        }
                    }
                }
                break;
            default:
                matrix[cell.x][cell.y].status = VISIBLE;
                break;
        }
    }
}
void InitField() {
    /* Demande taille de matrice + nb de mines (en %) */
    bool isFirstTry = true;
    char buff[256];
    int argCounter = 0;
    
    do {
        if(!isFirstTry) {
            printf("Wrong format or size too high.\n");
        }
        printf("Please enter field dimensions (row col minePercent).\n"
               "Note : row max = %d, col max = %d, minePercent must be included between %d and %d : ", MAX_ROW, MAX_COL, MIN_PERCENT, MAX_PERCENT);
        fgets(buff, sizeof(buff), stdin);
        argCounter = sscanf(buff, "%d %d %d", &row, &col, &minePercent);
        isFirstTry = false;
    } while(argCounter < 3 || row > MAX_ROW || col > MAX_COL || minePercent < MIN_PERCENT || minePercent > MAX_PERCENT);
    
    /* Allocation memoire. */
    matrix = (t_cell**)malloc(sizeof(t_cell*) * row);
    for(int i = 0; i < row; i++) {
        matrix[i] = (t_cell*)malloc(sizeof(t_cell) * col);
    }
    
    /* Initialisation des valeurs de chaque cellule */
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            matrix[i][j].value = 0;
            matrix[i][j].status = HIDDEN;
        }
    }
    
    t_point coord = GetCoordinates();
    matrix[coord.x][coord.y].status = VISIBLE;
    
    /* Calcul du nombre de mines et placement */
    int randRow, randCol;
    int nbMines = (row * col * minePercent) / 100;
    
    srand(time(NULL));
    /* Pour chaque mine, définir coordonnées aléatoires dans matrix */
    bool isFirstCellAround;
    int nbSetMines = 0;
    printf("nbSetMines: %d\n",nbSetMines);
    while (nbSetMines < nbMines) {
        randRow = rand() % row;
        randCol = rand() % col;
        
        isFirstCellAround = false;
        
        for(int k = -1; k < 2; k++) {
            for(int l = -1; l < 2; l++) {
                if(randRow+k >= 0 && randRow+k < row && randCol+l >= 0 && randCol+l < col) {
                    if(matrix[randRow+k][randCol+l].status == VISIBLE) {
                        isFirstCellAround = true;
                    }
                }
            }
        }
        
        if (matrix[randRow][randCol].value != MINE && matrix[randRow][randCol].status != VISIBLE && isFirstCellAround == 0) {
            matrix[randRow][randCol].value = MINE;
            nbSetMines++;
        }
    }
    
    /* Calculer les indices */
    /* Pour chaque case... */
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            /* ...si cette case est une mine... */
            if(matrix[i][j].value == MINE) {
                /* ...pour chaque case autour d'elle... */
                for(int k = -1; k < 2; k++) {
                    for(int l = -1; l < 2; l++) {
                        /* ...si cette case n'est pas une mine ET existe dans matrix... */
                        if(i+k >= 0 && i+k < row && j+l >= 0 && j+l < col) {
                            if(matrix[i+k][j+l].value != MINE) {
                                /* ...ajouter 1 à l'indice (value). */
                                matrix[i+k][j+l].value ++;
                            }
                        }
                    }
                }
            }
        }    
    }
    
    EvaluateMove(coord);
}
void PrintField() {
    /* Ligne d'entête : numéros de colonnes + ligne horizontale de séparation. */
    printf("   ");
    for(int i = 0; i < col; i++) {
        printf("%3d", i);
    }
    printf("\n");
    
    printf("   ");
    for(int i = 0; i < col; i++) {       
        printf("−−−");
    }
    printf("\b\n");
    
    /* Hidden values */
    for(int i = 0; i < row; i++) {
        printf("%2d|", i);
        for(int j = 0; j < col; j++) {
            if(matrix[i][j].status == HIDDEN) {
                printf("  .");
            }
            else{
                printf("%3d",matrix[i][j].value);
            }
        }
    printf("\n");
    }
    /* Real values for debugging 
    for(int i = 0; i < row; i++) {
        printf("%2d|", i);
        for(int j = 0; j < col; j++) {
            printf("%3d", matrix[i][j].value);
        }
        printf("\n");
    }*/
}
void FreeMatrix() {
    
    /* Deallocating memory */
    for(int i = 0; i < row; i++) {
            free(matrix[i]);
        }
    free(matrix);
}

/* MAIN PROGRAM */
int main() {    
    /* Initialisation du champ de mines */
    InitField();
    PrintField();
    
    /* Boucle de jeu (tant que !gameOver) */    
    while (!gameOver) {
        EvaluateMove(GetCoordinates());
        PrintField();
    }
    
    FreeMatrix();
    
    return 0;
}
