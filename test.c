#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "PacWarGuts.c"

// data from an individual battle
struct Battle {
	int rounds;
	int count1;
	int count2;
	char *a;
	char *b;
	int victor;
};

// Take two gene strings and run a tournament
struct Battle duel(char *a, char *b);

// Print the results of the tournament
void printBattle(struct Battle battle);


int main(){
	char a[50];
	char b[50];
	FILE *fptr;
	fptr = fopen("gene.txt","r");
	if(fptr==NULL){
		printf("ERROR");
		exit(1);
	}
	fscanf(fptr,"%s",a);
	fscanf(fptr,"%s",b);
	fclose(fptr);
	// return 0;

	struct Battle battle = duel(a,b);
	printBattle(battle);

	int tempScore = score(battle);
	printf("score: %d\n",tempScore);

	// char c[50];
	// winner(battle,c);
	// printf("Winner's gene: %s\n",c);
	// int tempInt = 3;
	// char tempChar = (char)(((int)'0')+tempInt);
	// printf("3 == %c\n",tempChar);

	return 0;
}

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

	if(battle.count1 > battle.count2)	
		battle.victor = 0;
	else 
		battle.victor = 1;

	return battle;
}

// return score for mite A
int score(struct Battle battle){
	if(battle.victor == 0)
	{
		if(battle.rounds < 100)
			return 20;
		else if(battle.rounds < 200)
			return 19;
		else if(battle.rounds < 300)
			return 18;
		else if(battle.rounds < 500)
			return 17;
		else if(battle.count1/battle.count2 > 10)
			return 13;
		else if(battle.count1/battle.count2 > 3)
			return 12;
		else if(battle.count1/battle.count2 > 1.5)
			return 11;
		else 
			return 10;
	}
	else
	{
		if(battle.rounds < 100)
			return 0;
		else if(battle.rounds < 200)
			return 1;
		else if(battle.rounds < 300)
			return 2;
		else if(battle.rounds < 500)
			return 3;
		else if(battle.count2/battle.count1 > 10)
			return 7;
		else if(battle.count2/battle.count1 > 3)
			return 8;
		else if(battle.count2/battle.count1 > 1.5)
			return 9;
		else 
			return 10;
	}
}

// // return the winner's char array
// void winner(struct Battle battle,char * winner){
// 	if(battle.victor == 0)
// 		winner = battle.a;
// 	else
// 		winner = battle.b;
// }

// print the results of a battle
void printBattle(struct Battle battle){
	printf("Gene 1:\n%s\nGene 2:\n%s\nRounds fought: %d\n",battle.a,battle.b,battle.rounds);
	printf("Remaining type-1 mites: %d\nRemaining type-2 mites: %d\n",battle.count1,battle.count2);
}