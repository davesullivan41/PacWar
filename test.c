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
		battle.victor = 1;
	else 
		battle.victor = 2;

	return battle;
}


void printBattle(struct Battle battle){
	printf("Gene 1:\n%s\nGene 2:\n%s\nRounds fought: %d\n",battle.a,battle.b,battle.rounds);
	printf("Remaining type-1 mites: %d\nRemaining type-2 mites: %d\n",battle.count1,battle.count2);
}