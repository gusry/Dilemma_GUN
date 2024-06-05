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

void printLoadingAnimation(int iterations, int delay);

bool getInput(char *str1, char *str2, Player *P, int turn);

void main(void){
	//variable
	Player P[6]; //player arr 1/2/3/4/5
	int turn = 0;
	char input_1[6];
	char input_2[6];

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
			printLoadingAnimation(12, 500);
			print_option(P);
			break;
		}

		if(P[turn].isPlayer != -1){

			printf("Now it's Player %d's turn,", turn);

			if(P[turn].isPlayer == 1){
				printf(" You choose Shoot or Pass\nView current infomation intput [status all]\nPlayer %d, choose [shoot {n} or pass {up/down}] : ", turn);
				
				while (!getInput(input_1, input_2, P, turn)){
					printf("Now it's Player %d's turn\nYou choose shoot or pass\n",turn);
				}
				printf("\n\n");

			}

			else if(P[turn].isPlayer == 0){
				printf(" computer player auto chice\n");
			}
			
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

void printLoadingAnimation(int iterations, int delay){
    const char *loadingMessage = "Wait";
    int messageLength = 7;
    int i, j;

    for (i = 0; i < iterations; i++) {
        printf("\r");
        printf("%s", loadingMessage);
        for (j = 0; j <= i % 4; j++) {
            printf(".");
        }
        fflush(stdout);

        usleep(delay * 1000);
    }

    printf("\n");
}

bool getInput(char *str1, char *str2, Player *P, int turn){
    char inputBuffer[16];
    if(fgets(inputBuffer, sizeof(inputBuffer), stdin) != NULL){
        inputBuffer[strcspn(inputBuffer, "\n")] = '\0';
        if(sscanf(inputBuffer, "%s %s", str1, str2) == 2){
			if(strcmp(str1, "shoot") == 0){
				int target = atoi(str2);

				if(target == turn || target <= 0 || target > 5){
					printf("Please try again. Target num {1 ~ 5}\n");
					return false;
				}
				if(P[target].isPlayer == -1){
					printf("Please try again. Target already dead. \n");
					return false;
				}

				return true;
			}
			else if(strcmp(str1, "pass") == 0){
				if(strcmp(str2, "up") == 0){
					return true;
				}
				else if(strcmp(str2, "down") == 0){
					return true;
				}
				else{
					printf("Please try again. {up} or {down}\n");
					return false;
				} 
			}
			else if(strcmp(str1, "status") == 0){
				print_option(P);
				return false;
			}
			else{
				printf("Please try again.\n");
				return false;
			}

			return true;
		}
	else{
        printf("Error: Input reading failed.\n");
		printf("Please try again.\n");
        return false;
    }
	}
}







