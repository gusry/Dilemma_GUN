#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

typedef struct Player {
	int isPlayer; 	// -1: death, 0:computer, 1:player
	double SPRB;  // shooting probablity 0.01 ~ 1.0
}Player;

int input_num();
void Player_setting(Player *P, int n);


void main(void){
	//variable
	Player P[6]; //player arr 1/2/3/4/5

	//input num
	int player_num = input_num();

	//setting
	Player_setting(P, player_num);


	printf("test\n");
}

int input_num(){
	int n;
	fflush(stdin);
	printf("Input Player numbers (Max:5) : ");
	scanf(" %d", &n);
	fflush(stdin);
	printf("Player numbers is : %d\nGame start.\n\n",n);
	if(n > 5 || n <= 0){
		printf("Invalid input human player (1 ~ 5)\n");
		printf("\n\n");
		return input_num();
	}	
	return n;
}

void Player_setting(Player *P, int n){
	for(int i = 1; i < 6; i++){
		if(n > 0){
			P[i].isPlayer = 1;
			n--;
		}
		else{
			P[i].isPlayer = 0;
		}
		P[i].SPRB = 0.1 * i;
	}

}