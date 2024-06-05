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
void print_option(Player *P);
int isplay(Player *P);

bool coin_check(double PRB); //coin probablity : PRB

void main(void){
	//variable
	Player P[6]; //player arr 1/2/3/4/5
	int turn = 0;

	//input num
	int player_num = input_num();

	//setting
	Player_setting(P, player_num);

	//print
	print_option(P);

	//roof
	while(isplay(P) != 0)
	{	
		turn++;
		if(turn > 5){
			print_option(P);
			break;
		}
		
	}

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

void print_option(Player *P){
	printf("\nCurrent game status--------------\n");
	
	for(int i = 1; i < 6; i++){
		printf("Player_%d", i);
		if(P[i].isPlayer == 0){printf(" is computer, SPRB : %.2f\n", P[i].SPRB);}
		else if(P[i].isPlayer == 1){printf(" is human, SPRB : %.2f\n", P[i].SPRB);}
		else if(P[i].isPlayer == -1){printf(" died\n");}
	}
	printf("---------------------------------\n\n");

}

int isplay(Player *P){
	int count = 5;
	for(int i = 1; i < 6; i++){
		if(P[i].isPlayer == -1){
			count--;
		}
	}
	if(count > 1){ return 1;}
	else return 0;

}


bool coin_check(double PRB){
	srand(time(NULL));

	int randnum = rand() % 100;

	if(randnum < PRB * 100){
		return true;
	}
	else return false;
}










