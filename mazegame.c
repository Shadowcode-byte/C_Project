1#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROWS 20
#define MAX_COLS 40
#define NUM_LEVELS 3

#define WALL '#'
#define PATH ' '
#define PLAYER 'P'
#define EXIT 'E'

typedef struct {
    int rows;
    int cols;
    char map[MAX_ROWS][MAX_COLS + 1];
    int playerRow;
    int playerCol;
    int exitRow;
    int exitCol;
} Level;

Level levels[NUM_LEVELS];
int currentLevel = 0;

void initLevel1() {
    levels[0].rows = 9;
    levels[0].cols = 15;
    char map[9][16] = {
        "###############",
        "#P    #       #",
        "# ### # ##### #",
        "# #   #     # #",
        "# # ##### # # #",
        "# #       # # #",
        "# ####### # # #",
        "#         #  E#",
        "###############"
    };
    for (int i = 0; i < 9; i++) {
        strcpy(levels[0].map[i], map[i]);
    }
    levels[0].playerRow = 1;
    levels[0].playerCol = 1;
    levels[0].exitRow = 7;
    levels[0].exitCol = 13;
}

void initLevel2() {
    levels[1].rows = 13;
    levels[1].cols = 25;
    char map[13][26] = {
        "#########################",
        "#P    #     #     #     #",
        "# ### # ### # ### # ### #",
        "# #   #   # #   # #   # #",
        "# # ##### # ### # # # # #",
        "# #       #     # # # # #",
        "# ######### ##### # # # #",
        "#         #       # # # #",
        "######### ####### # # # #",
        "#       #       # # # # #",
        "# ##### ####### # # # # #",
        "#     #         #   #  E#",
        "#########################"
    };
    for (int i = 0; i < 13; i++) {
        strcpy(levels[1].map[i], map[i]);
    }
    levels[1].playerRow = 1;
    levels[1].playerCol = 1;
    levels[1].exitRow = 11;
    levels[1].exitCol = 23;
}

void initLevel3() {
    levels[2].rows = 15;
    levels[2].cols = 35;
    char map[15][36] = {
        "###################################",
        "#P  #     #   #     #         #   #",
        "# # # ### # # # ### # ####### # # #",
        "# #   # # # #   #   #       # # # #",
        "# ##### # # ##### ######### # # # #",
        "#     # # #     #         # # # # #",
        "##### # # ##### ######### # # # # #",
        "#   # # #     #     #   # # # # # #",
        "# # # # ##### ##### # # # # # # # #",
        "# # # #     #     # # #   # # #   #",
        "# # # ##### ##### # # ##### # #####",
        "# #   #         # # #     # #     #",
        "# ##### ####### # # ##### # ##### #",
        "#               #   #           E#",
        "###################################"
    };
    for (int i = 0; i < 15; i++) {
        strcpy(levels[2].map[i], map[i]);
    }
    levels[2].playerRow = 1;
    levels[2].playerCol = 1;
    levels[2].exitRow = 13;
    levels[2].exitCol = 33;
}

void initAllLevels() {
    initLevel1();
    initLevel2();
    initLevel3();
}

void displayMaze(Level *level) {
    printf("\n--- MAZE GAME - Level %d ---\n\n", currentLevel + 1);
    
    for (int i = 0; i < level->rows; i++) {
        for (int j = 0; j < level->cols; j++) {
            if (i == level->playerRow && j == level->playerCol) {
                printf("%c", PLAYER);
            } else if (i == level->exitRow && j == level->exitCol) {
                printf("%c", EXIT);
            } else {
                printf("%c", level->map[i][j]);
            }
        }
        printf("\n");
    }
    
    printf("\nControls: W=Up, S=Down, A=Left, D=Right, Q=Quit\n");
    printf("Enter move: ");
}

int movePlayer(Level *level, int dRow, int dCol) {
    int newRow = level->playerRow + dRow;
    int newCol = level->playerCol + dCol;
    
    if (newRow >= 0 && newRow < level->rows && 
        newCol >= 0 && newCol < level->cols &&
        level->map[newRow][newCol] != WALL) {
        
        level->playerRow = newRow;
        level->playerCol = newCol;
        
        if (newRow == level->exitRow && newCol == level->exitCol) {
            return 1;
        }
    }
    return 0;
}

void playGame() {
    currentLevel = 0;
    initAllLevels();
    
    while (currentLevel < NUM_LEVELS) {
        Level *level = &levels[currentLevel];
        int levelComplete = 0;
        
        while (!levelComplete) {
            displayMaze(level);
            
            char input;
            scanf(" %c", &input);
            
            int dRow = 0, dCol = 0;
            
            if (input == 'q' || input == 'Q') {
                printf("\nReturning to menu...\n");
                return;
            }
            
            switch (input) {
                case 'w': case 'W': dRow = -1; break;
                case 's': case 'S': dRow = 1; break;
                case 'a': case 'A': dCol = -1; break;
                case 'd': case 'D': dCol = 1; break;
                default:
                    printf("Invalid input. Use W/A/S/D to move.\n");
                    continue;
            }
            
            levelComplete = movePlayer(level, dRow, dCol);
        }
        
        printf("\n*** CONGRATULATIONS! Level %d complete! ***\n", currentLevel + 1);
        currentLevel++;
        
        if (currentLevel < NUM_LEVELS) {
            printf("Get ready for Level %d...\n", currentLevel + 1);
            printf("Press Enter to continue...");
            getchar();
            getchar();
        }
    }
    
    printf("\n========================================\n");
    printf("       MAZE COMPLETED!\n");
    printf("   You are the Maze Champion!\n");
    printf("   All 3 levels conquered!\n");
    printf("========================================\n\n");
}

void showHowToPlay() {
    printf("\n--- HOW TO PLAY ---\n\n");
    printf("OBJECTIVE: Navigate through the maze and reach the Exit (E)\n\n");
    printf("CONTROLS:\n");
    printf("  W - Move Up\n");
    printf("  S - Move Down\n");
    printf("  A - Move Left\n");
    printf("  D - Move Right\n");
    printf("  Q - Quit to Menu\n\n");
    printf("SYMBOLS:\n");
    printf("  P - Your player position\n");
    printf("  E - Exit point\n");
    printf("  # - Wall (cannot pass through)\n");
    printf("  (space) - Walkable path\n\n");
    printf("There are 3 levels with increasing difficulty.\n\n");
}

int main() {
    int choice;
    int running = 1;
    
    while (running) {
        printf("\n--- MAZE GAME ---\n");
        printf("1. Start Game\n");
        printf("2. How to Play\n");
        printf("3. Exit\n");
        printf("Enter choice: ");
        
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                playGame();
                break;
            case 2:
                showHowToPlay();
                break;
            case 3:
                running = 0;
                printf("\nThanks for playing! Goodbye!\n\n");
                break;
            default:
                printf("Invalid choice. Please enter 1, 2, or 3.\n");
                break;
        }
    }
    
    return 0;
}