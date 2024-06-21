#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

// ==================================================================
// ======================== OSS PHASE 2 =============================
// ==================================================================
typedef struct Player {
	int isPlayer; 	// -1: death, 0:computer, 1:player
	double SPRB;  // shooting probablity 0.01 ~ 1.0
	int chanceCard; // 1: Set probability 1.0, 2: Defence attack, 3: Skip next player's turn
	char haveChance; // if Player have a chance ard => 'O', if not => 'X'
	bool chanceUsed; // if player use chance => true, if not => false
	bool defence; // if player get defence card, set this true
	bool skip; // if player get skip card, set this true
}Player;
// ==================================================================
// ======================== OSS PHASE 2 =============================
// ==================================================================

int input_num();
void Player_setting(Player *P, int n);
void print_option(Player *P);
int isplay(Player *P);

bool coin_check(double PRB); //coin probablity : PRB
double generate_random(Player x, int turn);
void printLoadingAnimation(int iterations, int delay);

//player
bool getInput(char *str1, char *str2, Player *P, int turn);
void PRB_down(Player *P, int turn);
void PRB_up(Player *P, int turn);
void Bang(Player *P, int target, int turn);
void Tic(Player *P, int turn);
void get_chanceCard(Player *P, int turn);

//computer
void auto_choice(Player *P, int turn);
int auto_targeting(Player *P, int turn);

void main(void){
	//variable
	Player P[6]; //player arr 1/2/3/4/5
	int turn = 0;
	char input_1[10];
	char input_2[10];

	//input num
	int player_num = input_num();

	//setting
	Player_setting(P, player_num);

	//print
	print_option(P);

	//roof

	getchar();
	while(isplay(P) != 0)
	{	
		turn++;
		if(turn > 5){
			sleep(4);
			printf("\nNext turn\n");
			print_option(P);
			turn = 1;
		}

// ==================================================================
// ======================== OSS PHASE 2 =============================
// ==================================================================
		if(P[turn].skip == true){
        	printf("Skip Player %d's turn by chance card...\n", turn);
			printf("\n");
         	P[turn].skip = false;
     	}

		else if(P[turn].isPlayer != -1){
			sleep(4);
			printf("Now it's Player %d's turn,", turn);

			if(P[turn].isPlayer == 1){
				printf(" You choose Shoot or Pass or Chance.\nView current infomation intput [status all]\nPlayer %d, choose [shoot {n} or pass {up/down} or get chance] : ", turn);
				
				while (!getInput(input_1, input_2, P, turn)){
					printf("Player %d's turn\nYou choose shoot or pass or chance\n",turn);
				}
				printf("\n\n");

			}
			else if(P[turn].isPlayer == 0){
				printf(" computer player auto choice\n");
				auto_choice(P, turn);
			}
			
		}
// ==================================================================
// ======================== OSS PHASE 2 =============================
// ==================================================================
		
	}

	//winner
	for(int i = 1; i < 6; i++){
		if(P[i].isPlayer != -1){
			turn = i;
		}
	}
	printf("\nPlayer_%d winner!!!\nGood Game.\n\nEnd, 10 second\n",turn);
	sleep(10);

	return;
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

// ==================================================================
// ======================== OSS PHASE 2 =============================
// ==================================================================
void Player_setting(Player *P, int n){
	for(int i = 1; i < 6; i++){
		if(n > 0){
			P[i].isPlayer = 1;
			n--;
		}
		else{
			P[i].isPlayer = 0;
		}

		P[i].chanceCard = 0;
		P[i].haveChance = 'X';
		P[i].defence = false;
		P[i].skip = false;
		P[i].chanceUsed = false;
		P[i].SPRB = 0.1 * i;
	}

}

void print_option(Player *P){
	printf("\nCurrent game status-----------------------------------------\n");
	
	for(int i = 1; i < 6; i++){
		printf("Player_%d", i);
		if(P[i].isPlayer == 0){printf(" is computer, SPRB : %.2f\n", P[i].SPRB);}
		else if(P[i].isPlayer == 1){printf(" is human, SPRB : %.2f, ChanceCard: %d, HaveChance: %c\n", P[i].SPRB, P[i].chanceCard, P[i].haveChance);}
		else if(P[i].isPlayer == -1){printf(" died\n");}
	}
	printf("------------------------------------------------------------\n\n");

}
// ==================================================================
// ======================== OSS PHASE 2 =============================
// ==================================================================


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

// ==================================================================
// ======================== OSS PHASE 2 =============================
// ==================================================================
				else if(P[target].defence == true){
					printf("Player %d defence attack!!\n", target);
					P[turn].SPRB = generate_random(P[turn], turn);
					P[target].defence = false;
					P[target].chanceUsed == true;
					P[target].haveChance = 'X';
				}
				else{
					if(coin_check(P[turn].SPRB)){
						if(P[turn].haveChance == 'O' && P[turn].SPRB == 1.0) {P[turn].haveChance = 'X';}
						Bang(P, target, turn);
						return true;
					}
					else{
						Tic(P, turn);
						return true;
					}
				}
				return true;
			}
// ==================================================================
// ======================== OSS PHASE 2 =============================
// ==================================================================

			else if(strcmp(str1, "pass") == 0){
				if(strcmp(str2, "up") == 0){
					PRB_up(P, turn);
					return true;
				}
				else if(strcmp(str2, "down") == 0){
					PRB_down(P, turn);
					return true;
				}
				else{
					printf("Please try again. {up} or {down}\n");
					return false;
				} 
			}

// ==================================================================
// ======================== OSS PHASE 2 =============================
// ==================================================================
			else if(strcmp(str1, "get") == 0 && strcmp(str2, "chance") == 0){
				if(P[turn].chanceUsed == false){
					get_chanceCard(P, turn);
					P[turn].chanceUsed = true;
					return true;
				}
				else{
					printf("Player %d already use chance card...\n", turn);
					return false;
				}			
			}
// ==================================================================
// ======================== OSS PHASE 2 =============================
// ==================================================================

			else if(strcmp(str1, "status") == 0, strcmp(str2, "all") == 0){
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

void PRB_down(Player *P, int turn){
	P[turn].SPRB -= 0.3;
	if(P[turn].SPRB < 0){
		P[turn].SPRB = 0;
	}
	printf("Player_%d's shooting probablity set %.2f\n\n",turn, P[turn].SPRB);
	return;
}

void PRB_up(Player *P, int turn){
	P[turn].SPRB += 0.3;
	if(P[turn].SPRB > 1){
		P[turn].SPRB = 1;
	}
	printf("Player_%d's shooting probablity set %.2f\n\n",turn, P[turn].SPRB);
	return;
}

void Bang(Player *P, int target, int turn){
	printLoadingAnimation(12, 500);
	printf("!!!BANG!!!\n!!!BANG!!!\n!!!BANG!!!\n");
	printf("Player_%d is dead. \n",target);
	P[target].isPlayer = -1;
	P[turn].SPRB = generate_random(P[turn], turn);
	return;
}

void Tic(Player *P, int turn){
	printLoadingAnimation(12, 500);
	printf("...Tic...\n");
	P[turn].SPRB = generate_random(P[turn], turn);
	return;
}

double generate_random(Player x, int turn){

    double dorand = ((double) rand() / RAND_MAX) * 0.99 + 0.01;
	printf("Player_%d's shooting probablity set %.2f.\n\n",turn ,dorand);
	return dorand;
}

void auto_choice(Player *P, int turn){
	printf("Player_%d's SPRB is %.2f.....\n",turn, P[turn].SPRB);
	printLoadingAnimation(12, 500);
	if(coin_check(P[turn].SPRB)){
		int target = auto_targeting(P, turn);
		printf("\nThe Target is Player_%d\n",target);
		if(coin_check(P[turn].SPRB)){
			Bang(P, target, turn);
		}
		else{
			Tic(P, turn);
		}
	}
	else{
		if(P[turn].SPRB >= 0.5){
			printf("Pass down.\n");
			PRB_down(P, turn);
		}
		else{
			printf("Pass up.\n");
			PRB_up(P, turn);
		}
		
	}
}

int auto_targeting(Player *P, int turn){
	double max = 0;
	int target = 0;
	if(coin_check(0.75)){
		for(int i = 1; i < 6; i++)
		{
			if(i != turn && P[i].isPlayer != -1)
			{
				if(max < P[i].SPRB)
				{
					max = P[i].SPRB;
					target = i;
				}
			}
		}
		return target;
	}
	else{
		while(!coin_check(0.1)){
			if(target == turn){
				target++;
			}
			if(target > 5){
				target = 1;
			}
			target++;
		}
		return target;
	}
}

// ==================================================================
// ======================== OSS PHASE 2 =============================
// ==================================================================
void get_chanceCard(Player *P, int turn){
	
	printf("Player %d get a chance card!!", turn);
	srand(time(NULL));

	int randcard = (rand() % 3) + 1;
	int skipTarget = (turn % 5) + 1;
	
	P[turn].chanceCard = randcard;
	P[turn].haveChance = 'O';
	
	if(P[turn].chanceCard == 1) P[turn].SPRB = 1.0;
	else if(P[turn].chanceCard == 2) P[turn].defence = true;
	else {
		while(P[skipTarget].isPlayer == -1){
			skipTarget = (skipTarget % 5) + 1;
		}
		P[skipTarget].skip = true;
		P[turn].haveChance = 'X';
	}
}
// ==================================================================
// ======================== OSS PHASE 2 =============================
// ==================================================================
