#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "PacWarGuts.c"
#include "mtwist.c"

// data from an individual battle
struct Battle {
	int rounds;
	int count1;
	int count2;
	char *a;
	char *b;
	// int victor;
	int score;
};

// returns winner of tournament of random 8 genes
void runTournament();

// returns a Battle between a and b
struct Battle duel(char *a, char *b);

// fill digits with 7 random numbers (0,49) inclusive
void randomDigits(int * digits);

// set winner to the gene of the winning mite of battle and mutate 
// according to randomDigit
void getWinner(char * winner,struct Battle battle,int randomDigit);

// print the results of a battle
void printBattle(struct Battle battle);

int main(){
	int score = 0;
	char winner[50];
	char *threes;
	threes = "33333333333333333333333333333333333333333333333333";
	struct Battle battle;
	int i = 0;
	// printf("Before loop\n");
	while(i < 1)
	{
		runTournament(winner,1);
		// printf("After tournament\n");
 		// battle = duel(winner,threes);
 		if(battle.score > 10)
 		{
 			break;
 		}
 		i++;
	}
	// char winner2[50];
	// printf("After final duel\n");
	// getWinner(winner2,battle);

	// printf("Winning gene: %s\n",winner2);

	// printBattle(battle);

	return 0;	
}
// sets winner to the winning gene of tournament of random 8 genes
void runTournament(char * winner,int print){
	// generate random genes - python
	system("python randomGene.py");
	// read genes from file gene.txt
	char a[50];
	char b[50];
	char c[50];
	char d[50];
	char e[50];
	char f[50];
	char g[50];
	char h[50];

	FILE *fptr;
	fptr = fopen("gene.txt","r");
	if(fptr==NULL){
		printf("ERROR");
		exit(1);
	}

	fscanf(fptr,"%s",a);
	fscanf(fptr,"%s",b);
	fscanf(fptr,"%s",c);
	fscanf(fptr,"%s",d);
	fscanf(fptr,"%s",e);
	fscanf(fptr,"%s",f);
	fscanf(fptr,"%s",g);
	fscanf(fptr,"%s",h);

	fclose(fptr);
	// printf("File read and closed\n");
	int digits[7];
	randomDigits(digits);


	// make them duel
	struct Battle battle1 = duel(a,b);
	struct Battle battle2 = duel(c,d);
	struct Battle battle3 = duel(e,f);
	struct Battle battle4 = duel(g,h);
	// printBattle(battle1);
	// printBattle(battle2);
	// printBattle(battle3);
	// printBattle(battle4);

	// printf("Battle 1 fought\n");
	char winner1[50];
	getWinner(winner1,battle1,digits[0]);

	char winner2[50];
	getWinner(winner2,battle2,digits[1]);

	char winner3[50];
	getWinner(winner3,battle3,digits[2]);

	char winner4[50];
	getWinner(winner4,battle4,digits[3]);
	// printf("Winners found\n");

	struct Battle playoff1 = duel(winner1,winner2);
	struct Battle playoff2 = duel(winner3,winner4);
	// printBattle(playoff1);
	// printBattle(playoff2);

	char finalist1[50];
	getWinner(finalist1,playoff1,digits[4]);

	char finalist2[50];
	getWinner(finalist2,playoff2,digits[5]);

	struct Battle finalBattle = duel(finalist1,finalist2);
	if(print)
		printBattle(finalBattle);
	// printf("Final battle fought\n");
	getWinner(winner,finalBattle,digits[6]);
}

// returns a Battle between a and b
struct Battle duel(char *a, char *b){
	struct Battle battle;
	battle.rounds = 500;
	battle.count1 = 0;
	battle.count2 = 0;
	battle.a = a;
	battle.b = b;

	PacGene g[2];
	SetGeneFromString(battle.a,g);
	SetGeneFromString(battle.b,g+1);
	FastDuel(g,g+1,&battle.rounds,&battle.count1,&battle.count2);

	// Determine victor and score the battle
	if(battle.count1 > battle.count2)
	{	
		// battle.victor = 0;
		if(battle.rounds < 100)
			battle.score = 20;
		else if(battle.rounds < 200)
			battle.score = 19;
		else if(battle.rounds < 300)
			battle.score = 18;
		else if(battle.rounds < 500)
			battle.score = 17;
		else if(battle.count1/battle.count2 >= 10)
			battle.score = 13;
		else if(battle.count1/battle.count2 >= 3)
			battle.score = 12;
		else if((float)battle.count1/(float)battle.count2 >= 1.5)
			battle.score = 11;
		else 
			battle.score = 10;
	}
	else
	{
		// battle.victor = 1;
		if(battle.rounds < 100)
			battle.score = 0;
		else if(battle.rounds < 200)
			battle.score = 1;
		else if(battle.rounds < 300)
			battle.score = 2;
		else if(battle.rounds < 500)
			battle.score = 3;
		else if(battle.count2/battle.count1 >= 10)
			battle.score = 7;
		else if(battle.count2/battle.count1 >= 3)
			battle.score = 8;
		else if((float)battle.count2/(float)battle.count1 >= 1.5)
			battle.score = 9;
		else 
			battle.score = 10;
	}

	return battle;
}

// fill digits with 7 random numbers (0,49) inclusive
void randomDigits(int * digits){
	// digits = int[6];
	uint32_t switchLocation = mt_goodseed();
	// printf("Random number: %d\n",switchLocation);
	digits[0] = switchLocation % 50;
	// printf("First digit: %d\n",firstDigit);
	switchLocation = switchLocation - digits[0];
	digits[1] = (switchLocation % (50^2)) % 50;
	// printf("Second digit: %d\n",secondDigit);
	switchLocation = switchLocation - digits[1]*50;
	digits[2] = (switchLocation % (50^3)) % 50;

	switchLocation = mt_goodseed();
	digits[3] = switchLocation % 50;
	// printf("First digit: %d\n",firstDigit);
	switchLocation = switchLocation - digits[3];
	digits[4] = (switchLocation % (50^2)) % 50;
	// printf("Second digit: %d\n",secondDigit);
	switchLocation = switchLocation - digits[4]*50;
	digits[5] = (switchLocation % (50^3)) % 50;
	// printf("Second digit: %d\n",secondDigit);
	switchLocation = switchLocation - digits[5]*50^2;
	digits[6] = (switchLocation % (50^4)) % 50;
	printf("Digit: %d\n",digits[0]);
	printf("Digit: %d\n",digits[1]);
	printf("Digit: %d\n",digits[2]);
	printf("Digit: %d\n",digits[3]);
	printf("Digit: %d\n",digits[4]);
	printf("Digit: %d\n",digits[5]);
	printf("Digit: %d\n",digits[6]);
}

// set winner to the gene of the winning mite of battle and mutate 
// according to randomDigit
void getWinner(char * winner, struct Battle battle,int randomDigit){
	// printf("Before random number\n");
	// uint32_t switchLocation = mt_goodseed();
	// printf("After random number: %d\n",switchLocation);
	// start indexing at a random location
	// int index = switchLocation % 20;
	int index = randomDigit;
	// get the gene from a
	strcpy(winner,battle.a);
	// counter of number of genes to switch
	int count = 0;
	// switch a number of bits proportional to the score
	int digitsToSwitch = 50 - (5 * battle.score) / 2;
	// while we still have more bits to flip
	while(count < digitsToSwitch)
	{
		// set "winner" bits to bits from b
		winner[index] = battle.b[index];
		count++;
		// make sure index is always (0,49)
		index = (index+1) % 50;
	}
}

// print the results of a battle
void printBattle(struct Battle battle){
	printf("Gene 1:\n%s\nGene 2:\n%s\nRounds fought: %d\n",battle.a,battle.b,battle.rounds);
	printf("Remaining type-1 mites: %d\nRemaining type-2 mites: %d\n",battle.count1,battle.count2);
	printf("Final score for mite A: %d\n",battle.score);
}