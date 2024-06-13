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
void Player_setting(Player *P, int n,int m);
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

//computer
void auto_choice(Player *P, int turn);
int auto_targeting(Player *P, int turn);

void main(void){
	//variable
	Player P[6]; //player arr 1/2/3/4/5
	int turn = 0;
	char input_1[6];
	char input_2[6];

	//input num으로 human player, computer player의 수를 받음
	int h10c_num = input_num();
	int human_num=h10c_num/10;
	int computer_num=h10c_num%10;

	//setting
	Player_setting(P, human_num, computer_num);

	//print
	print_option(P);

	//roof
	while(isplay(P) != 0)
	{	
		turn++;
		if(turn > 5){
			sleep(4);
			printf("\nNext turn\n");
			print_option(P);
			turn = 1;
		}

		if(P[turn].isPlayer != -1){
			sleep(4);
			printf("Now it's Player %d's turn,", turn);

			if(P[turn].isPlayer == 1){
				printf(" You choose Shoot or Pass\nView current infomation intput [status all]\nPlayer %d, choose [shoot {n} or pass {up/down}] : ", turn);
				
				while (!getInput(input_1, input_2, P, turn)){
					printf("Player %d's turn\nYou choose shoot or pass\n",turn);
				}
				printf("\n\n");

			}

			else if(P[turn].isPlayer == 0){
				printf(" computer player auto choice\n");
				auto_choice(P, turn);
			}
			
		}
		
	}

	//winner
	for(int i = 1; i < 6; i++){
		if(P[i].isPlayer != -1){
			turn = i;
		}
	}

	//human이 살아남았을 때와 아닐 때로 분류
	if(P[turn].isPlayer){
		printf("\nPlayer_%d winner!!!\nGood Game.\n\nEnd, 10 second\n",turn);
		sleep(10);
	}
	else{
		printf("\nThere is no human left.\nGood Game.\n\nEnd, 10 second\n");
		sleep(10);
	}
	return;
}

int input_num(){ // 5명 미만의 player가 가능하도록 수정
	int n,m; // n은 human 수, m은 coputer 수 
	fflush(stdin);
	printf("Input human Player numbers (human+computer Max:5) : ");
	scanf(" %d", &n);
	fflush(stdin);
	printf("Input total Player numbers (human+computer Max:5) : ");
	scanf(" %d", &m);
	fflush(stdin);
	printf("Human Player numbers is : %d\n Computer Player numbers is : %d\nGame start.\n\n",n,m);
	if(n > 5 || n <= 0){
		printf("Invalid input human player (1 ~ 5)\n");
		printf("\n\n");
		return input_num();
	}
	if(n+m > 5 || m < 0){
		printf("Invalid input total player (1 ~ 5)\n");
		printf("\n\n");
		return input_num();
	}		
	return 10*n+m;
}

void Player_setting(Player *P, int n,int m){
	for(int i = 1; i < 6; i++){
		if(n > 0){
			P[i].isPlayer = 1;
			n--;
		}
		else{
			if(m>0){
				P[i].isPlayer = 0;
				m--;
			}
			else
				P[i].isPlayer = -1;
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
	int survive_human=0;
	for(int i = 1; i < 6; i++){
		if(P[i].isPlayer == 1){
			survive_human++;
		}
		if(P[i].isPlayer == -1){
			count--;
		}
	}
	if(count > 1 && survive_human>0){ return 1;}
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

				else{
					if(coin_check(P[turn].SPRB)){
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
	// 확률이 바뀌는 부분 삭제
	return;
}

void Tic(Player *P, int turn){
	printLoadingAnimation(12, 500);
	printf("...Tic...\n");
	// 확률이 바뀌는 부분 삭제
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







