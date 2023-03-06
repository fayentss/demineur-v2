#include <stdio.h>
#include <time.h>
#include <stdlib.h>

typedef struct Tile
{
    int isRevealed;
    int isBomb;
    int isBombAround;
    int isSafe;

} Tile;

typedef struct Grid
{
    Tile* t;
    int size;

} Grid;



typedef struct Difficulty
{
    int id;
    int size;
    int total_size;
    int bomb;
    int selected;

} Difficulty;

void createGrid(Grid* g)
{
    Tile* t;
    t = g->t;
    int total_size = g->size * g->size;


    for (int i = 0; i < total_size; i++)
    {
        t[i].isBomb = 0;
        t[i].isRevealed = 0;
        t[i].isSafe = 0;
    }
}

int getCoord1D(Grid* g, int x, int y)
{
    return (y * g->size + x);
}

int isInGrid(Grid* g, int x, int y)
{
    if (x >= 0 && x < g->size && y >= 0 && y < g->size)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int isBombAtTile(Grid* g, int x, int y)
{
    if (isInGrid(g, x, y) == 1)
    {
        return g->t[getCoord1D(g, x, y)].isBomb;
    }
    else
    {
        return 2;
    }
}

void reveleEmptyCase(Grid* g, int x, int y, int* reveledCase)
{
    if (isBombAtTile(g, x, y) == 0 && g->t[getCoord1D(g, x, y)].isBombAround == 0 && g->t[getCoord1D(g, x, y)].isRevealed == 0)
    {
        g->t[getCoord1D(g, x, y)].isRevealed = 1;
        *reveledCase -= 1;


        discoverCase(g, x - 1, y - 1, reveledCase);

        discoverCase(g, x, y - 1, reveledCase);

        discoverCase(g, x + 1, y - 1, reveledCase);

        discoverCase(g, x + 1, y, reveledCase);

        discoverCase(g, x + 1, y + 1, reveledCase);

        discoverCase(g, x, y + 1, reveledCase);

        discoverCase(g, x - 1, y + 1, reveledCase);

        discoverCase(g, x - 1, y, reveledCase);
    }
}

int discoverCase(Grid* g, int x, int y, int* reveledCase)
{
    if (isInGrid(g, x, y) == 0)
    {
        return 0;
    }
    else if (isBombAtTile(g, x, y, g->size) == 1)
    {
        g->t[getCoord1D(g, x, y)].isRevealed = 1;
        return 1;
    }
    else if (g->t[getCoord1D(g, x, y)].isRevealed == 1)
    {
        return 0;
    }

    else if (g->t[getCoord1D(g, x, y)].isBombAround > 0 && g->t[getCoord1D(g, x, y)].isRevealed == 0)
    {
        g->t[getCoord1D(g, x, y)].isRevealed = 1;
        *reveledCase -= 1;

        reveleEmptyCase(g, x - 1, y - 1, reveledCase);

        reveleEmptyCase(g, x, y - 1, reveledCase);

        reveleEmptyCase(g, x + 1, y - 1, reveledCase);

        reveleEmptyCase(g, x + 1, y, reveledCase);

        reveleEmptyCase(g, x + 1, y + 1, reveledCase);

        reveleEmptyCase(g, x, y + 1, reveledCase);

        reveleEmptyCase(g, x - 1, y + 1, reveledCase);

        reveleEmptyCase(g, x - 1, y, reveledCase);
    }
    else if (g->t[getCoord1D(g, x, y)].isBombAround == 0 && isBombAtTile(g, x, y, g->size) == 0)
    {
        reveleEmptyCase(g, x, y, reveledCase, g->size);
    }

    return 2;
}

void placeBombAround(Grid* g)
{
    for (int y = 0; y < g->size; y++)
    {
        for (int x = 0; x < g->size; x++)
        {
            if (isBombAtTile(g, x, y) == 0)
            {
                int bombs = 0;
                if (isBombAtTile(g, x - 1, y - 1) == 1)
                {
                    bombs += 1;
                }

                if (isBombAtTile(g, x, y - 1) == 1)
                {
                    bombs += 1;
                }

                if (isBombAtTile(g, x + 1, y - 1) == 1)
                {
                    bombs += 1;
                }

                if (isBombAtTile(g, x + 1, y) == 1)
                {
                    bombs += 1;
                }

                if (isBombAtTile(g, x + 1, y + 1) == 1)
                {
                    bombs += 1;
                }

                if (isBombAtTile(g, x, y + 1) == 1)
                {
                    bombs += 1;
                }

                if (isBombAtTile(g, x - 1, y + 1) == 1)
                {
                    bombs += 1;
                }

                if (isBombAtTile(g, x - 1, y) == 1)
                {
                    bombs += 1;
                }

                g->t[getCoord1D(g, x, y)].isBombAround = bombs;

            }
        }
    }
}

void placeBomb(Grid* g, int numberOfBombs)
{
    srand(time(NULL));

    int bomb = 0;
    while (bomb <= numberOfBombs)
    {
        int val = rand() % (g->size * g->size);
        if (g->t[val].isSafe != 1 && g->t[val].isBomb != 1)
        {
            g->t[val].isBomb = 1;
            bomb += 1;
        }
    }
}

void printgrid(Grid* g)
{
    printf("|  |");
    for (int i = 0; i < g->size; i++)
    {
        printf("|%d|", i);

    }
    printf("\n");

    int y = 0;
    printf("|0%d|", y);
    y += 1;
    for (int i = 0; i < (g->size * g->size); i++)
    {
        if ((i % g->size) > 9)
        {
            if (g->t[i].isRevealed == 1)
            {
                printf("\033[0;31m");
                if (g->t[i].isBomb == 1)
                {
                    printf("\033[0;33m");
                    printf("| x|");
                }
                else if (g->t[i].isBomb == 0)
                {
                    if (g->t[i].isBombAround > 0)
                    {
                        printf("| %d|", g->t[i].isBombAround);
                    }
                    else
                    {
                        printf("|  |");
                    }
                }

            }
            else
            {
                printf("\033[0m");
                printf("|  |");

            }
        }
        else
        {
            if (g->t[i].isRevealed == 1)
            {
                printf("\033[0;31m");
                if (g->t[i].isBomb == 1)
                {
                    printf("\033[0;33m");
                    printf("|x|");
                }
                else if (g->t[i].isBomb == 0)
                {
                    if (g->t[i].isBombAround > 0)
                    {
                        printf("|%d|", g->t[i].isBombAround);
                    }
                    else
                    {
                        printf("| |");
                    }
                }

            }
            else
            {
                printf("\033[0m");
                printf("| |");

            }
        }
        if (((i + 1) % g->size) == 0)
        {
            if (y < g->size)
            {
                printf("\033[0m");
                printf("\n");
                if (y > 9)
                {
                    printf("|%d|", y);
                }
                else
                {
                    printf("|0%d|", y);
                }
                y += 1;
            }
        }
        printf("\033[0m");

    }
    printf("\n");
}

void placeSafeTile(Grid* g, int playerChoiceX, int playerChoiceY)
{
    if (isInGrid(g, playerChoiceX, playerChoiceY) == 0)
        return;

    g->t[getCoord1D(g, playerChoiceX, playerChoiceY)].isSafe = 1;
}

void placeSafeZone(Grid* g, int playerChoiceX, int playerChoiceY)
{
    placeSafeTile(g, playerChoiceX, playerChoiceY);
    placeSafeTile(g, playerChoiceX - 1, playerChoiceY - 1);
    placeSafeTile(g, playerChoiceX, playerChoiceY - 1);
    placeSafeTile(g, playerChoiceX + 1, playerChoiceY - 1);
    placeSafeTile(g, playerChoiceX + 1, playerChoiceY);
    placeSafeTile(g, playerChoiceX + 1, playerChoiceY + 1);
    placeSafeTile(g, playerChoiceX, playerChoiceY + 1);
    placeSafeTile(g, playerChoiceX - 1, playerChoiceY + 1);
    placeSafeTile(g, playerChoiceX - 1, playerChoiceY);
}

int replay()
{
    char answer;
    printf("Veux tu rejouer ( O / N ) : ");
    while (scanf_s(" %c", &answer) != 1)
    {
        char c = getchar();
        while (c != '\n') {
            c = getchar();
        };
        printf("Erreur.\nVeux tu rejouer ( O / N ) : ");
    }

    if (answer == 'O')
    {
        return 1;
    }
    else if (answer == 'N')
    {
        return 0;
    }
    else
    {
        printf("Ta réponse n'est pas correct recommence\n");
        replay();
    }
}

int verifPlayerChoice(Grid* g, int x, int y)
{
    if (isInGrid(g, x, y) == 0)
    {
        return 0;
    }
    else if (g->t[getCoord1D(g, x, y)].isRevealed == 1)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

int main()
{
    Difficulty difficulties[3];

    difficulties[0].id = 1;
    difficulties[0].size = 10;
    difficulties[0].bomb = 10;

    difficulties[1].id = 2;
    difficulties[1].size = 18;
    difficulties[1].bomb = 40;

    difficulties[2].id = 3;
    difficulties[2].size = 24;
    difficulties[2].bomb = 99;

    int onoff = 1;
    while (onoff == 1)
    {
        int difficultyChoice;
        printf("Choisissez la difficultée ( 1, 2, 3 ) : ");

        while ((scanf_s("%d", &difficultyChoice) != 1) || (difficultyChoice < 1 || difficultyChoice > 3))
        {

            printf("Ton choix n'est pas valide\n");
            printf("Choisissez la difficultée ( 1, 2, 3 ) : ");
            char c = getchar();
            while (c != '\n') {
                c = getchar();
            };

        }
        printf("%d\n", difficultyChoice);

        int size = difficulties[difficultyChoice - 1].size;
        int total_size = size * size;
        int bombs = difficulties[difficultyChoice - 1].bomb;

        int WNL = 0;

        int reveledCase = total_size - size;

        Grid grid;

        grid.t = malloc(sizeof(Tile) * total_size);
        grid.size = size;

        createGrid(&grid);

        int playerChoiceX = 0;
        int playerChoiceY = 0;

        printgrid(&grid, size);

        printf("Choissez un colonne : ");
        while (scanf_s("%d", &playerChoiceX) != 1)
        {
            printf("erreur.Recommence.\n");
            char c = getchar();
            while (c != '\n') {
                c = getchar();
            };
        }
        printf("Choissez une ligne : ");
        while (scanf_s("%d", &playerChoiceY) != 1)
        {
            printf("erreur.Recommence.\n");
            char c = getchar();
            while (c != '\n') {
                c = getchar();
            };
        }

        while (verifPlayerChoice(&grid, playerChoiceX, playerChoiceY, size) == 0)
        {
            printf("Ton choix n'est pas bon recommence\n");
            printf("Choissez un colonne : ");
            while (scanf_s("%d", &playerChoiceX) != 1)
            {
                printf("erreur.Recommence.\n");
                char c = getchar();
                while (c != '\n') {
                    c = getchar();
                };
            }
            printf("Choissez un ligne : ");
            while (scanf_s("%d", &playerChoiceY) != 1)
            {
                printf("erreur.Recommence.\n");
                char c = getchar();
                while (c != '\n') {
                    c = getchar();
                };
            }
        }
        placeSafeZone(&grid, playerChoiceX, playerChoiceY, size);

        placeBomb(&grid, bombs);

        placeBombAround(&grid);

        discoverCase(&grid, playerChoiceX, playerChoiceY, &reveledCase);

        printgrid(&grid);

        while (WNL == 0)
        {
            printf("Choissez un colonne : ");
            while (scanf_s("%d", &playerChoiceX) != 1)
            {
                printf("Erreur.Choissez un colonne : ");
                char c = getchar();
                while (c != '\n') {
                    c = getchar();
                };
            }
            printf("Choissez un ligne : ");
            while (scanf_s("%d", &playerChoiceY) != 1)
            {
                printf("Erreur.Choissez un ligne : ");
                char c = getchar();
                while (c != '\n') {
                    c = getchar();
                };
            }

            while (verifPlayerChoice(&grid, playerChoiceX, playerChoiceY, size) == 0)
            {
                printf("Ton choix n'est pas bon recommence\n");
                printf("Choissez un colonne : ");
                while (scanf_s("%d", &playerChoiceX) != 1)
                {
                    printf("Erreur.Choissez un colonne : ");
                    char c = getchar();
                    while (c != '\n') {
                        c = getchar();
                    };
                }
                printf("Choissez une ligne : ");
                while (scanf_s("%d", &playerChoiceY) != 1)
                {
                    printf("Erreur.Choissez une ligne : ");
                    char c = getchar();
                    while (c != '\n') {
                        c = getchar();
                    };
                }
            }

            if (discoverCase(&grid, playerChoiceX, playerChoiceY, &reveledCase) == 1)
            {
                printgrid(&grid);
                printf("Tu as perdu\n");
                WNL = 1;
            }
            else if (reveledCase == 0)
            {
                printgrid(&grid);
                printf("Tu as gagne\n");
                WNL = 1;
            }
            else
            {
                printgrid(&grid);
            }


        }

        free(grid.t);

        if (replay() == 0)
        {
            printf("A bientot\n");
            onoff = 0;
        }
        else
        {
            printf("C'est reparti\n");
        }
    }
}