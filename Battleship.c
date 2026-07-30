#include <stdio.h>

#include <assert.h>

#include <stdlib.h>

#include <string.h>

// functions to define the colors which serve as markers: red, blue, green
void red () {
  printf("\033[1;31m");
}
void blue () {
  printf("\033[1;34m");
}
void green () {
  printf("\033[1;32m");
}
void reset () {
  printf("\033[0m");
}

// structure to define individual elements of each ship
struct ship { 
    char name[30];
    int length;
    int counter;
    int sunk_or_not;
};

// function that assigns values to each structure elements of each of the four ships
void initializeShips(struct ship *ships) { 

	int i;

    strcpy(ships[0].name, "Aircraft Carrier");
    ships[0].length = 6;
    ships[0].sunk_or_not = 0;
    ships[0].counter = 0;

    strcpy(ships[1].name, "Heavy Cruiser");
    ships[1].length = 4;
    ships[1].sunk_or_not = 0;
    ships[1].counter = 0;

    strcpy(ships[2].name, "Destroyer");
    ships[2].length = 3;
    ships[2].sunk_or_not = 0;
    ships[2].counter = 0;

    strcpy(ships[3].name, "Corvette");
    ships[3].length = 2;
    ships[3].sunk_or_not = 0;
    ships[3].counter = 0;

    strcpy(ships[4].name, "Bomb");
    ships[4].length = 1;

    for ( i = 0; i < 4; i++) {
        ships[i].counter = 0;
    }
}











// fundamental function that randomizes the placement of the ships
void initializeBoard(int dimension, 
	int battle_ship_spots[dimension+1][dimension+1], struct ship *ships) {

    int i, j, direction_of_ship, range_of_columns,
    range_of_rows, pointer, start_ship1_column, 
	start_ship1_row, x_position_of_bomb, y_position_of_bomb;


    for (i = 1; i <= dimension; i++) {
        for (j = 1; j <= dimension; j++) {
            battle_ship_spots[i][j] = 0;
        }
    }

    for (i = 0; i < 4; i++) {
		
		// randomizes the direction (horixontal or vertical)
        direction_of_ship = 1 + rand() % (2);
        
        if (direction_of_ship == 1) { // horizontal
			
			// defines range of columns and rows so the ships are within the board
            range_of_columns = dimension - ships[i].length + 1;
            range_of_rows = dimension;

            do {
                pointer = 0;
                start_ship1_column = 1 + rand() % (range_of_columns);
                start_ship1_row = 1 + rand() % (range_of_rows);
				
				// checks that no ships overlap
                for (j = 0; j < ships[i].length; j++) {
                    if (battle_ship_spots[start_ship1_row][start_ship1_column + j] != 0) {
                        pointer = 1;
                    }
                }

            } while (pointer == 1);

            for (j = 0; j < ships[i].length; j++) {
                battle_ship_spots[start_ship1_row][start_ship1_column + j] = i + 1;
            }
            
        } else { // vertical
            range_of_columns = dimension;
            range_of_rows = dimension - ships[i].length + 1;

            do {
                pointer = 0;
                start_ship1_column = 1 + rand() % (range_of_columns);
                start_ship1_row = 1 + rand() % (range_of_rows);

                for (j = 0; j < ships[i].length; j++) {
                    if (battle_ship_spots[start_ship1_row + j][start_ship1_column] != 0) {
                        pointer = 1;
                    }
                }

            } while (pointer == 1);

            for (j = 0; j < ships[i].length; j++) {
                battle_ship_spots[start_ship1_row + j][start_ship1_column] = i + 1;
            }
        }
    }
}






// function that randomly places the bomb on the board
void placeBomb(int * x_position_of_bomb, int * y_position_of_bomb, 
	int dimension, int battle_ship_spots[dimension+1][dimension+1]) {

    do {
        * x_position_of_bomb = 1 + rand() % (dimension);
        * y_position_of_bomb = 1 + rand() % (dimension);
    } while (battle_ship_spots[ * x_position_of_bomb][ * y_position_of_bomb] != 0);

    battle_ship_spots[ * x_position_of_bomb][ * y_position_of_bomb] = 5;
}

// function that initializes the 'letter_of_recognition'
void applyGameSymbols(int dimension, char letter_of_recognition[dimension+1][dimension+1], 
	int x_position_of_bomb, int y_position_of_bomb) {

    int i, j;
    char unexploredWater = '.';

    letter_of_recognition[x_position_of_bomb][y_position_of_bomb] = 'Z';

    for (i = 1; i <= dimension; i++) {
        for (j = 1; j <= dimension; j++) {
            letter_of_recognition[i][j] = unexploredWater;
        }
    }
}

// function that numbers rows and columns and 
//draws the board with the correct colors and letters in the appropriate coordinate
void drawBoard(int dimension, char letter_of_recognition[dimension+1][dimension+1]) {
    int i, j;

    for (i = 0; i <= dimension; i++) {
        if (i>0 && i<10){
          printf("  %d   ", i);
        }
        else if (i==0){
          printf("      ");
        }
        else{
          printf("  %d  ", i);
        }
        for (j = 1; j <= dimension; j++) {
            if (i==0) {
              if (j<9){
                printf("  %d    ", j);
              }
              else{
                printf("  %d   ", j);
              }
            }
            else{
              if (letter_of_recognition[i][j] == 'X') {
                  red();
                  printf("| %c |  ", letter_of_recognition[i][j]);
                  reset();
              } else if (letter_of_recognition[i][j] == 'O') {
                  blue();
                  printf("| %c |  ", letter_of_recognition[i][j]);
                  reset();
              } else if (letter_of_recognition[i][j] == 'Z'){
            	  green();
                  printf("| %c |  ", letter_of_recognition[i][j]);
                  reset();
              } else{
            	  printf("| %c |  ", letter_of_recognition[i][j]);
            }
			}
        }
        printf("\n");
    }
}
// function that gets player's coordinates and checks that they are valid
void getPlayerCoordinates(int * row, int * column, int dimension, 
	char letter_of_recognition[dimension+1][dimension+1]) {

    char unexploredWater = '.';
    int pass=0;
    
    printf("Enter valid numbers of the row and column that you want to hit.\n");
    do{
    	scanf("%d", & * row);
    	scanf("%d", & * column);
        if (* row < 1 || * row > dimension || * column < 1 || * column > dimension) {
            printf("The mumbers you entered are not valid. Try again!\n");
        } else {
          if (letter_of_recognition[ * row][ * column] != unexploredWater) {
              printf("You have already choosen that spot. Pick another row and column.\n");
          } else {
            pass=1;
          }
        }
        printf("row: %d column: %d\n",* row, * column);
	}while(pass==0);

}

// fundamental function that manages gameplay and counts the attempts
int gamePlay(int dimension, int battle_ship_spots[dimension+1][dimension+1], 
	char letter_of_recognition[dimension+1][dimension+1], int row, 
		int column, struct ship * ships, int * attempts) {

    int sunk_counter = 0, nh, flag = 1;

    printf("\033[0;0H\033[2J");
    while (sunk_counter != 4 && flag!=0) {
    	
    	if(* attempts == 0){
    		printf("Game started!\n");
		}
    	
    	* attempts = * attempts + 1;
    	   	
        drawBoard(dimension, letter_of_recognition);

        getPlayerCoordinates( & row, & column, dimension, letter_of_recognition);

        printf("\033[0;0H\033[2J");
        
        if (battle_ship_spots[row][column] == 5) {
            letter_of_recognition[row][column] = 'Z';
            printf("I am sorry! You just hit the bomb.\n");
            flag = 0;
            drawBoard(dimension, letter_of_recognition);
        } else if (battle_ship_spots[row][column] == 0) {
            letter_of_recognition[row][column] = 'O';
            printf("Your hit was unsucessful.\n");
        } else {
            letter_of_recognition[row][column] = 'X';
            printf("Your hit was sucessful!\n");
            nh = battle_ship_spots[row][column];
            ships[nh - 1].counter = ships[nh - 1].counter + 1;

            if (ships[nh - 1].counter == ships[nh - 1].length) {
                printf("You have sunk %s!\n", ships[nh - 1].name);
                ships[nh - 1].sunk_or_not = 1;
                sunk_counter = sunk_counter + 1;
            }
        }
        
    }

    return flag;
}

int main(void) {

    srand(time(NULL));

    struct ship ships[9];

    const int dimension = 15;
    int battle_ship_spots[dimension+1][dimension+1], attempts=0, range_of_columns, 
		range_of_rows, x_position_of_bomb, y_position_of_bomb, 
		gameresult, row, column;

    char letter_of_recognition[dimension+1][dimension+1];

    initializeShips(ships);

    initializeBoard(dimension, battle_ship_spots, ships);

    placeBomb( & x_position_of_bomb, & y_position_of_bomb, dimension, battle_ship_spots);
    
	  printf("\033[0;0H\033[2J");

    applyGameSymbols(dimension, letter_of_recognition, x_position_of_bomb, y_position_of_bomb);

    gameresult=gamePlay(dimension, battle_ship_spots, letter_of_recognition, row, 
						column, ships, & attempts);

    if (gameresult == 1) {
    	drawBoard(dimension, letter_of_recognition);
        printf("You won in %d moves! You sunk all of the ships!\n", attempts);
    } else {
        printf("You lost! Game is over!\n");
    }
    return 0;
}
