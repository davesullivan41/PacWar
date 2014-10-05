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
///////////////////////////////////////////////////////////////////////////
// Function declarations
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// Main callable functions
///////////////////////////////////////////////////////////////////////////

// returns a struct Battle between a and b. battle->score is the gene A's 
// final score. (gene B's final score is simply 20 - battle->score)
// battle->a is the mite A's gene. battle->b is mite B's gene
// battle->count1 is the total number of type A mites left, battle->count2
// is the total number of type B mites left. battle->rounds is the total number
// of rounds fought.
struct Battle *duel(char *a, char *b);

// pass a pointer to an empty length-50 gene array. This will fill that array
// with a new, relatively good gene. 
// This generates 8 random genes and runs a traditional tournament with them,
// returning the winner. Genetic swapping occurs after every battle
void getGoodGene(char *winner);

// same as getGoodGene, but instead of running a tournament of 8 genes, runs
// a tournament of 16 genes. This takes about twice as long to run.
void getBetterGene(char *winner);

///////////////////////////////////////////////////////////////////////////
// Helper functions
///////////////////////////////////////////////////////////////////////////

struct Contestants *createContestants();

void destroyContestants(struct Contestants *contestants);

struct Contestants *contestantCopy(struct Contestants *contestants);

struct Battle *createBattle();

void destroyBattle(struct Battle *battle);

void runManualTournament(struct Contestants *winners,char *winner);


// fill digits with 7 random numbers (0,49) inclusive
void randomDigits(int * digits);

// change a single digit within gene
void mutate(char *gene);

// set winner to the gene of the winning mite of battle and mutate 
// according to randomDigit
void getWinner(char * winner,struct Battle *battle,int randomDigit);

// print the results of a battle
void printBattle(struct Battle *battle);

///////////////////////////////////////////////////////////////////////////
// Function definitions
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// Main callable functions
///////////////////////////////////////////////////////////////////////////

// returns a Battle between a and b
struct Battle *duel(char *a, char *b){
	struct Battle *battle = createBattle();
	battle->rounds = 500;
	battle->count1 = 0;
	battle->count2 = 0;
	strcpy(battle->a,a);
	strcpy(battle->b,b);

	PacGene g[2];
	SetGeneFromString(battle->a,g);
	SetGeneFromString(battle->b,g+1);
	FastDuel(g,g+1,&battle->rounds,&battle->count1,&battle->count2);

	// Determine victor and score the battle
	if(battle->count1 > battle->count2)
	{	
		// battle->victor = 0;
		if(battle->rounds < 100)
			battle->score = 20;
		else if(battle->rounds < 200)
			battle->score = 19;
		else if(battle->rounds < 300)
			battle->score = 18;
		else if(battle->rounds < 500)
			battle->score = 17;
		else if(battle->count2 == 0 || battle->count1/battle->count2 >= 10)
			battle->score = 13;
		else if(battle->count1/battle->count2 >= 3)
			battle->score = 12;
		else if((float)battle->count1/(float)battle->count2 >= 1.5)
			battle->score = 11;
		else 
			battle->score = 10;
	}
	else
	{
		// battle->victor = 1;
		if(battle->rounds < 100)
			battle->score = 0;
		else if(battle->rounds < 200)
			battle->score = 1;
		else if(battle->rounds < 300)
			battle->score = 2;
		else if(battle->rounds < 500)
			battle->score = 3;
		else if(battle->count1 == 0 || battle->count2/battle->count1 >= 10)
			battle->score = 7;
		else if(battle->count2/battle->count1 >= 3)
			battle->score = 8;
		else if((float)battle->count2/(float)battle->count1 >= 1.5)
			battle->score = 9;
		else 
			battle->score = 10;
	}

	return battle;
}

// sets winner to the winning gene of tournament of random 8 genes
void getGoodGene(char * winner){
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
	// printf("Before random digit generation\n");
	randomDigits(digits);
	// printf("After random digit generation\n");


	// make them duel
	struct Battle *battle1 = duel(a,b);
	struct Battle *battle2 = duel(c,d);
	struct Battle *battle3 = duel(e,f);
	struct Battle *battle4 = duel(g,h);

	memset(a,0,sizeof(a));
	memset(b,0,sizeof(b));
	memset(c,0,sizeof(c));
	memset(d,0,sizeof(d));
	memset(e,0,sizeof(e));
	memset(f,0,sizeof(f));
	memset(g,0,sizeof(g));
	memset(h,0,sizeof(h));

	char winner1[50];
	getWinner(winner1,battle1,digits[0]);

	char winner2[50];
	getWinner(winner2,battle2,digits[1]);

	char winner3[50];
	getWinner(winner3,battle3,digits[2]);

	char winner4[50];
	getWinner(winner4,battle4,digits[3]);

	destroyBattle(battle1);
	destroyBattle(battle2);
	destroyBattle(battle3);
	destroyBattle(battle4);

	struct Battle *playoff1 = duel(winner1,winner2);
	struct Battle *playoff2 = duel(winner3,winner4);

	memset(winner1,0,sizeof(winner1));
	memset(winner2,0,sizeof(winner2));
	memset(winner3,0,sizeof(winner3));
	memset(winner4,0,sizeof(winner4));


	char finalist1[50];
	getWinner(finalist1,playoff1,digits[4]);

	char finalist2[50];
	getWinner(finalist2,playoff2,digits[5]);

	destroyBattle(playoff1);
	destroyBattle(playoff2);

	struct Battle *finalBattle = duel(finalist1,finalist2);

	memset(finalist1,0,sizeof(*finalist1));
	memset(finalist2,0,sizeof(*finalist2));

	getWinner(winner,finalBattle,digits[6]);
	destroyBattle(finalBattle);
}

void getBetterGene(char *winner){
	struct Contestants *contestants = createContestants();
	// generate random genes - python
	system("python randomGene.py");

	FILE *fptr;
	fptr = fopen("gene.txt","r");
	if(fptr==NULL){
		printf("ERROR");
		exit(1);
	}

	fscanf(fptr,"%s",contestants->a);
	fscanf(fptr,"%s",contestants->b);
	fscanf(fptr,"%s",contestants->c);
	fscanf(fptr,"%s",contestants->d);
	fscanf(fptr,"%s",contestants->e);
	fscanf(fptr,"%s",contestants->f);
	fscanf(fptr,"%s",contestants->g);
	fscanf(fptr,"%s",contestants->h);

	fclose(fptr);

	struct Contestants *seed = createContestants();
	// generate random genes - python
	system("python randomGene.py");

	// FILE *fptr;
	fptr = fopen("gene.txt","r");
	if(fptr==NULL){
		printf("ERROR");
		exit(1);
	}

	fscanf(fptr,"%s",seed->a);
	fscanf(fptr,"%s",seed->b);
	fscanf(fptr,"%s",seed->c);
	fscanf(fptr,"%s",seed->d);
	fscanf(fptr,"%s",seed->e);
	fscanf(fptr,"%s",seed->f);
	fscanf(fptr,"%s",seed->g);
	fscanf(fptr,"%s",seed->h);

	fclose(fptr);

	struct Battle *battle1 = duel(seed->a,contestants->a);
	struct Battle *battle2 = duel(seed->b,contestants->b);
	struct Battle *battle3 = duel(seed->c,contestants->c);
	struct Battle *battle4 = duel(seed->d,contestants->d);
	struct Battle *battle5 = duel(seed->e,contestants->e);
	struct Battle *battle6 = duel(seed->f,contestants->f);
	struct Battle *battle7 = duel(seed->g,contestants->g);
	struct Battle *battle8 = duel(seed->h,contestants->h);

	destroyContestants(contestants);

	struct Contestants *winners = createContestants();

	int digits[7];
	randomDigits(digits);

	getWinner(winners->a,battle1,digits[0]);
	getWinner(winners->b,battle2,digits[1]);
	getWinner(winners->c,battle3,digits[2]);
	getWinner(winners->d,battle4,digits[3]);

	randomDigits(digits);

	getWinner(winners->e,battle5,digits[0]);
	getWinner(winners->f,battle6,digits[1]);
	getWinner(winners->g,battle7,digits[2]);
	getWinner(winners->h,battle8,digits[3]);

	destroyBattle(battle1);
	destroyBattle(battle2);
	destroyBattle(battle3);
	destroyBattle(battle4);
	destroyBattle(battle5);
	destroyBattle(battle6);
	destroyBattle(battle7);
	destroyBattle(battle8);

	runManualTournament(winners,winner);
	destroyContestants(winners);
}

///////////////////////////////////////////////////////////////////////////
// Helper functions
///////////////////////////////////////////////////////////////////////////

// sets winner to the winning gene of tournament of random 8 genes
void runManualTournament(struct Contestants *winners, char *winner){
	int digits[7];
	randomDigits(digits);


	// make them duel
	struct Battle *battle1 = duel(winners->a,winners->b);
	struct Battle *battle2 = duel(winners->c,winners->d);
	struct Battle *battle3 = duel(winners->e,winners->f);
	struct Battle *battle4 = duel(winners->g,winners->h);
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

	destroyBattle(battle1);
	destroyBattle(battle2);
	destroyBattle(battle3);
	destroyBattle(battle4);

	struct Battle *playoff1 = duel(winner1,winner2);
	struct Battle *playoff2 = duel(winner3,winner4);
	// printBattle(playoff1);
	// printBattle(playoff2);

	char finalist1[50];
	getWinner(finalist1,playoff1,digits[4]);

	char finalist2[50];
	getWinner(finalist2,playoff2,digits[5]);

	destroyBattle(playoff1);
	destroyBattle(playoff2);

	struct Battle *finalBattle = duel(finalist1,finalist2);

	// printBattle(finalBattle);
	// printf("Final battle fought\n");
	getWinner(winner,finalBattle,digits[6]);
	destroyBattle(finalBattle);
	// return winner;
}
// fill digits with 7 random numbers (0,49) inclusive
void randomDigits(int * digits){
	// printf("Starting random number generation");
	// digits = int[6];
	uint32_t switchLocation = mt_seed();
	// printf("Random number: %d\n",switchLocation);
	digits[0] = switchLocation % 50;
	// printf("First digit: %d\n",firstDigit);
	switchLocation = switchLocation - digits[0];
	digits[1] = (switchLocation % (50^2)) % 50;
	// printf("Second digit: %d\n",secondDigit);
	switchLocation = switchLocation - digits[1]*50;
	digits[2] = (switchLocation % (50^3)) % 50;

	switchLocation = mt_seed();
	digits[3] = switchLocation % 50;
	// printf("First digit: %d\n",firstDigit);
	switchLocation = switchLocation - digits[3];
	digits[4] = (switchLocation % (50^2)) % 50;
	// printf("Second digit: %d\n",secondDigit);
	switchLocation = switchLocation - digits[4]*50;
	digits[5] = (switchLocation % (50^3)) % 50;
	// printf("Second digit: %d\n",secondDigit);
	switchLocation = switchLocation - digits[5]*(50^2);
	digits[6] = (switchLocation % (50^4)) % 50;
	// printf("Finished random number generation");
	// printf("Digit: %d\n",digits[0]);
	// printf("Digit: %d\n",digits[1]);
	// printf("Digit: %d\n",digits[2]);
	// printf("Digit: %d\n",digits[3]);
	// printf("Digit: %d\n",digits[4]);
	// printf("Digit: %d\n",digits[5]);
	// printf("Digit: %d\n",digits[6]);
}

void mutate(char *gene){
	int digits[7];
	randomDigits(digits);
	if(digits[0]%4 == 0)
	{
		gene[digits[1]] = '0';
	} else if (digits[0]%4 == 1)
	{
		gene[digits[1]] = '1';
	} else if (digits[0]%4 == 2)
	{
		gene[digits[1]] = '2';
	}else
	{
		gene[digits[1]] = '3';
	}

	if(digits[2]%4 == 0)
	{
		gene[digits[3]] = '0';
	} else if (digits[2]%4 == 1)
	{
		gene[digits[3]] = '1';
	} else if (digits[2]%4 == 2)
	{
		gene[digits[3]] = '2';
	}else
	{
		gene[digits[3]] = '3';
	}
}

// set winner to the gene of the winning mite of battle and mutate 
// according to randomDigit
void getWinner(char * winner, struct Battle *battle,int randomDigit){
	// printf("Before random number\n");
	// uint32_t switchLocation = mt_goodseed();
	// printf("After random number: %d\n",switchLocation);
	// start indexing at a random location
	// int index = switchLocation % 20;
	int index = randomDigit;
	// get the gene from a
	strcpy(winner,battle->a);
	// counter of number of genes to switch
	int count = 0;
	// switch a number of bits proportional to the score
	int digitsToSwitch = 50 - (50 * battle->score) / 19;
	// while we still have more bits to flip
	while(count < digitsToSwitch)
	{
		// set "winner" bits to bits from b
		winner[index] = battle->b[index];
		count++;
		// make sure index is always (0,49)
		index = (index+1) % 50;
	}
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

struct Battle *createBattle(){
	struct Battle *battle = malloc(sizeof(struct Battle));
	assert(battle != NULL);

	battle->a = malloc(sizeof(char[50]));
	battle->b = malloc(sizeof(char[50]));

	return battle;
}

void destroyBattle(struct Battle *battle){
	assert(battle != NULL);

	free(battle->a);
	free(battle->b);

	free(battle);
}

// print the results of a battle
void printBattle(struct Battle *battle){
	printf("Gene 1:\n%s\nGene 2:\n%s\nRounds fought: %d\n",battle->a,battle->b,battle->rounds);
	printf("Remaining type-1 mites: %d\nRemaining type-2 mites: %d\n",battle->count1,battle->count2);
	printf("Final score for mite A: %d\n",battle->score);
}