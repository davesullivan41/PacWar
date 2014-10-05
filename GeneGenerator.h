#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef PacWarGuts_c
#define PacWarGuts_c
#include "PacWarGuts.c"
#endif

#ifndef mtwist_c
#define mtwist_c
#include "mtwist.c"
#endif

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

struct Contestants{
	char *a;
	char *b;
	char *c;
	char *d;
	char *e;
	char *f;
	char *g;
	char *h;
};


struct Contestants *createContestants();

void destroyContestants(struct Contestants *contestants);

struct Contestants *contestantCopy(struct Contestants *contestants);

// returns a Battle between a and b
struct Battle duel(char *a, char *b);

////////////////////////////////////////////////////////////////////////////

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
		else if(battle.count2 == 0 || battle.count1/battle.count2 >= 10)
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
		else if(battle.count1 == 0 || battle.count2/battle.count1 >= 10)
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

struct Contestants *createContestants(){
	struct Contestants *contestants = malloc(sizeof(struct Contestants));
	assert(contestants != NULL);

	contestants->a = malloc(sizeof(char)*50);
	contestants->b = malloc(sizeof(char)*50);
	contestants->c = malloc(sizeof(char)*50);
	contestants->d = malloc(sizeof(char)*50);
	contestants->e = malloc(sizeof(char)*50);
	contestants->f = malloc(sizeof(char)*50);
	contestants->g = malloc(sizeof(char)*50);
	contestants->h = malloc(sizeof(char)*50);

	return contestants;
}

void destroyContestants(struct Contestants *contestants){
	assert(contestants != NULL);

	free(contestants->a);
	free(contestants->b);
	free(contestants->c);
	free(contestants->d);
	free(contestants->e);
	free(contestants->f);
	free(contestants->g);
	free(contestants->h);

	free(contestants);
}

struct Contestants *contestantCopy(struct Contestants *contestants){
	struct Contestants *toBeReturned = createContestants();

	strcpy(toBeReturned->a,contestants->a);
	strcpy(toBeReturned->b,contestants->b);
	strcpy(toBeReturned->c,contestants->c);
	strcpy(toBeReturned->d,contestants->d);
	strcpy(toBeReturned->e,contestants->e);
	strcpy(toBeReturned->f,contestants->f);
	strcpy(toBeReturned->g,contestants->g);
	strcpy(toBeReturned->h,contestants->h);

	return toBeReturned;
}
