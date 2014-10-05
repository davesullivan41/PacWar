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

struct Contestants *eliminationRound(struct Contestants *seed);

struct Battle *createBattle();

void destroyBattle(struct Battle *battle);

// returns winner of tournament of random 8 genes
void randomTournament(char *winner,int print);

void runManualTournament(struct Contestants *winners,char *winner);

// returns a Battle between a and b
struct Battle *duel(char *a, char *b);

// fill digits with 7 random numbers (0,49) inclusive
void randomDigits(int * digits);

// change a single digit within gene
void mutate(char *gene);

// set winner to the gene of the winning mite of battle and mutate 
// according to randomDigit
void getWinner(char * winner,struct Battle *battle,int randomDigit);

// print the results of a battle
void printBattle(struct Battle *battle);

int main(){
	// run randomTournament to get random 8 genes for seed,
	// then pass Contesants struct to eliminationRound to produce winner
	// of seed genes and 8 random genes, then pass result to manual tournament
	// to find the winner. 

	// final code will run elimination round + manual tournament 8 times 
	// on the same set of seed genes and produce a new Contestants seed
	// which will be used to run the algorithm again
	// Thoughts: after each elimination round + manual tournament, write results
	// to log file
	// char winner[50];

	// printf("Before seed\n");
	struct Contestants *seed = createContestants();
	// printf("Before random tournaments 1-4\n");
	randomTournament(seed->a,0);
	randomTournament(seed->b,0);
	randomTournament(seed->c,0);
	randomTournament(seed->d,0);
	// printf("Before random tournaments 5-8\n");
	randomTournament(seed->e,0);
	randomTournament(seed->f,0);
	randomTournament(seed->g,0);
	// randomTournament(seed->h,0);
	strcpy(seed->h,"33333333333333333333333333333333333333333333333333");
	struct Contestants *winners;
	int i =0;
	FILE *f = fopen("winners.txt","w");
	if(f == NULL)
	{
		printf("Error opening file!\n");
		exit(1);
	}
	char *old = malloc(sizeof(char)*50);
	int mutateIt = 0;
	strcpy(old,"33333333333333333333333333333333333333333333333333");
	while(i<50)
	{
		// after initial seed, run 8 tournaments and store winners into a new contestants struct
		winners = createContestants();
		// printf("Before elimination round\n");
		struct Contestants *winners1 = eliminationRound(seed);
		runManualTournament(winners1,winners->a);
		if(i%100 == 0)
		{
			fprintf(f,"%s\n",winners->a);
			printf("%s\n",winners->a);
		}
		if(i%20 == 0)
		{
			mutateIt = 1;
			int i;
			for(i=0;i<50;i++)
			{
				if(winners->a[i] != old[i])
				{
					mutateIt = 0;
					break;
				}
			}
			if(mutateIt == 1)
			{
				mutate(winners->a);
			}
			strcpy(old,winners->a);
		}
		struct Contestants *winners2 = eliminationRound(seed);
		runManualTournament(winners2,winners->b);
		struct Contestants *winners3 = eliminationRound(seed);
		runManualTournament(winners3,winners->c);
		struct Contestants *winners4 = eliminationRound(seed);
		runManualTournament(winners4,winners->d);
		struct Contestants *winners5 = eliminationRound(seed);
		runManualTournament(winners5,winners->e);
		struct Contestants *winners6 = eliminationRound(seed);
		runManualTournament(winners6,winners->f);
		struct Contestants *winners7 = eliminationRound(seed);
		runManualTournament(winners7,winners->g);
		struct Contestants *winners8 = eliminationRound(seed);
		runManualTournament(winners8,winners->h);

		destroyContestants(seed);

		// printf("Before manual tournament\n");
		destroyContestants(winners1);
		destroyContestants(winners2);
		destroyContestants(winners3);
		destroyContestants(winners4);
		destroyContestants(winners5);
		destroyContestants(winners6);
		destroyContestants(winners7);
		destroyContestants(winners8);
	
		seed = contestantCopy(winners);
		destroyContestants(winners);
		// if(i%10 == 0)
		// {
		// 	printf("Iteration %d complete\n",i);
		// }
		i++;
	}
	fclose(f);	

	char *threes;
	threes = "33333333333333333333333333333333333333333333333333";

	char winner[50];
	winners = eliminationRound(seed);
	runManualTournament(winners,winner);

	// printf("Before final battle\n");
	struct Battle *battle = duel(winner,threes);
	printBattle(battle);

	return 0;	
}

struct Contestants *eliminationRound(struct Contestants *seed){
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

	return winners;
}

// sets winner to the winning gene of tournament of random 8 genes
void randomTournament(char * winner,int print){
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
	if(print)
		printBattle(finalBattle);
	// printf("Final battle fought\n");
	getWinner(winner,finalBattle,digits[6]);
	destroyBattle(finalBattle);
}

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