#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "GeneGenerator.h"

#ifndef VF_SIZE
#define VF_SIZE 64
#endif

struct GeneScore{
	char *gene;
	int score;
};

struct ValueFunction{
	struct GeneScore *geneScores[VF_SIZE];
};

// creates a gene with score of zero
struct GeneScore *createGeneScore(char *gene);

// creates a pseudo-random "good" gene score
struct GeneScore *createGoodGeneScore();

// creates a pseudo-random "better" gene score
struct GeneScore *createBetterGeneScore();

// create an n-th level gene score
struct GeneScore *createNthGeneScore(int N);

// free memory of gene score
void destroyGeneScore(struct GeneScore *geneScore);

// create a value function with VF_SIZE "good" genes
struct ValueFunction *createValueFunction();

// free memory of value function
void destroyValueFunction(struct ValueFunction *valueFunction);

// set a specific valueFunction location to a specific geneScore
void setValueFunctionGene(struct ValueFunction *valueFunction,struct GeneScore *geneScore,int index);

// get the score of a certain gene
struct GeneScore *getGeneScore(struct ValueFunction *valueFunction,int index);

// get the gene of a specific genescore
char *getGene(struct ValueFunction *valueFunction,int index);

// return 1 if geneA has a higher score than B, -1 if its less, 0 if they're equal
int compareGenes(struct GeneScore *geneScoreA,struct GeneScore *geneScoreB);

// swap gene i and gene j
void swap(struct GeneScore **geneScore,int i,int j);

// sort array using quicksort algorithm
void geneSort(struct GeneScore **geneScores,int length);

// sort when only one gene is not in the right spot
void insertionSort(struct GeneScore **geneScores,int length);

void updateValueFunction(struct ValueFunction *valueFunction,struct GeneScore *geneScore);

// test genescore against the value function
void testGene(struct ValueFunction *valueFunction,struct GeneScore *geneScore);
//////////////////////////////////////////////////////////
// Function definitions
//////////////////////////////////////////////////////////

// creates a gene with score of zero
struct GeneScore *createGeneScore(char *gene){
	// allocate space for structure
	struct GeneScore *geneScore = malloc(sizeof(struct GeneScore));
	assert(geneScore != NULL);

	// allocate space for gene
	geneScore->gene = malloc(sizeof(char)*50);
	assert(geneScore != NULL);
	strcpy(geneScore->gene,gene);

	// set score to 0
	geneScore->score = 0;

	return geneScore;
}

// creates a pseudo-random "good" gene score
struct GeneScore *createGoodGeneScore(){
	char *a = malloc(sizeof(char)*50);
	getGoodGene(a);
	struct GeneScore *geneScore = createGeneScore(a);
	free(a);
	return geneScore;
}

// creates a pseudo-random "better" gene score
struct GeneScore *createBetterGeneScore(){
	char *a = malloc(sizeof(char)*50);
	getBetterGene(a);
	struct GeneScore *geneScore = createGeneScore(a);
	free(a);
	return geneScore;
}

// create an n-th level gene score
struct GeneScore *createNthGeneScore(int N){
	char *a = malloc(sizeof(char)*50);
	getNGene(N,a);
	struct GeneScore *geneScore = createGeneScore(a);
	free(a);
	return geneScore;
}

// free memory of gene score
void destroyGeneScore(struct GeneScore *geneScore){
	assert(geneScore != NULL);
	free(geneScore->gene);
	free(geneScore);
}

// create a value function with VF_SIZE "good" genes
struct ValueFunction *createValueFunction(){
	struct ValueFunction *valueFunction = malloc(sizeof(struct ValueFunction));
	int i;
	for(i=0;i<VF_SIZE;i++){
		setValueFunctionGene(valueFunction,createGoodGeneScore(),i);
	}
	return valueFunction;
}

// free memory of value function
void destroyValueFunction(struct ValueFunction *valueFunction){
	int i;
	for(i=0;i<VF_SIZE;i++)
	{
		free(valueFunction->geneScores[i]);
	}
	free(valueFunction);
}

// set a specific valueFunction location to a specific geneScore
void setValueFunctionGene(struct ValueFunction *valueFunction,struct GeneScore *geneScore,int index){
	valueFunction->geneScores[index] = geneScore;
}

// get the score of a certain gene
struct GeneScore *getGeneScore(struct ValueFunction *valueFunction,int index){
	return valueFunction->geneScores[index];
}

// get the gene of a specific genescore
char *getGene(struct ValueFunction *valueFunction,int index){
	return valueFunction->geneScores[index]->gene;
}

// return 1 if geneA has a higher score than B, -1 if its less, 0 if they're equal
int compareGenes(struct GeneScore *geneScoreA,struct GeneScore *geneScoreB){
	if(geneScoreA->score > geneScoreB->score)
	{
		return 1;
	}
	else if(geneScoreA->score < geneScoreB->score)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

// swap gene i and gene j
void swap(struct GeneScore **geneScore,int i,int j){
	struct GeneScore *temp = geneScore[i];
	geneScore[i] = geneScore[j];
	geneScore[j] = temp;
	// destroyGeneScore(temp);
}

// sort array using quicksort algorithm
void geneSort(struct GeneScore **geneScores,int length){
	if(length < 2){
		return;
	}
	int i = 0;
	int j;
	for(j=1;j<length;j++)
	{
		if(compareGenes(geneScores[i],geneScores[j]) == 1){
			swap(geneScores,i,i+1);
			if(j != i+1){
				swap(geneScores,i,j);
			}
			i++;
		}
	}
	geneSort(&geneScores[0],i);
	geneSort(&geneScores[i+1],length-i-1);
}

void insertionSort(struct GeneScore **geneScores,int length)
{
   int i, j;
   struct GeneScore *key;
   for (i = 1; i < length; i++)
   {
       key = geneScores[i];
       j = i-1;
       while (j >= 0 && compareGenes(geneScores[j],key) == 1)
       {
           geneScores[j+1] = geneScores[j];
           j = j-1;
       }
       geneScores[j+1] = key;
   }
}

// test genescore against the value function
void testGene(struct ValueFunction *valueFunction,struct GeneScore *geneScore){
	int i;
	struct Battle *battle;
	// don't test against lowest-valued gene
	for(i=1;i<VF_SIZE;i++)
	{
		battle = duel(geneScore->gene,getGene(valueFunction,i));
		geneScore->score += battle->score;
		destroyBattle(battle);
	}
}

void updateValueFunction(struct ValueFunction *valueFunction,struct GeneScore *geneScore){
	testGene(valueFunction,geneScore);
	// printf("New gene total score: %d\n",geneScore->score);
	if(geneScore->score > getGeneScore(valueFunction,0)->score)
	{
		struct Battle *battleA;
		struct Battle *battleB;
		int i;
		for(i=1;i<VF_SIZE;i++)
		{
			battleA = duel(getGene(valueFunction,i),geneScore->gene);
			battleB = duel(getGene(valueFunction,i),getGene(valueFunction,0));
			getGeneScore(valueFunction,i)->score += (battleA->score - battleB->score);
			destroyBattle(battleA);
			destroyBattle(battleB);
		}
		memcpy(valueFunction->geneScores[0],geneScore,sizeof(struct GeneScore));
		insertionSort(valueFunction->geneScores,VF_SIZE);
	}
}

int main(){
	int iterations;
	int print;
	int silent;
	printf("Enter total number of iterations:\n");
	scanf("%d",&iterations);
	printf("Enter number of iterations to average score over: \n");
	scanf("%d",&print);
	printf("Enter 1 to run program in background and save results to results.txt: \n");
	scanf("%d",&silent);
	if(silent == 1)
	{
		daemon(1,0);
	}

	// creates a value function set with VF_SIZE random good genes
	struct ValueFunction *valueFunction = createValueFunction();
	if(silent != 1)
		printf("Genes built\n");
	int i;
	int j;
	struct Battle *battle;
	// makes each gene fight each other gene and calculates their total score
	for(i=0;i<VF_SIZE;i++)
	{
		for(j=i;j<VF_SIZE;j++)
		{
			battle = duel(getGene(valueFunction,i),getGene(valueFunction,j));
			getGeneScore(valueFunction,i)->score += battle->score;
			getGeneScore(valueFunction,j)->score += (20 - battle->score);
			destroyBattle(battle);
		}
	}
	geneSort(valueFunction->geneScores,VF_SIZE);
	// print the score of every gene
	// for(i=0;i<VF_SIZE;i++)
	// {
	// 	printf("Gene %d total score: %d\n",i,getGeneScore(valueFunction,i)->score);
	// }

	char *threes = "33333333333333333333333333333333333333333333333333";
	// battle = duel(getGene(valueFunction,VF_SIZE-1),threes);
	// printf("Best gene score against threes: %d\n",battle->score);
	// destroyBattle(battle);
	// test adding another gene
	struct GeneScore *geneScore = createBetterGeneScore();
	float average = 0;
	for(i=0;i<iterations;i++)
	{
		updateValueFunction(valueFunction,geneScore);
		average += ((float)geneScore->score)/(float)print;
		if(i % print == 0 && i != 0){
			if(silent == 1)
			{
				FILE *fptr;
				fptr = fopen("results.txt","w");
				for(i=0;i<VF_SIZE;i++)
				{
					fprintf(fptr,"%s #%d average score: %f\n",getGene(valueFunction,i),i,(float)getGeneScore(valueFunction,i)->score/VF_SIZE);
				}	
				battle = duel(getGene(valueFunction,VF_SIZE-1),threes);
				fprintf(fptr,"Best gene score against threes: %d\n",battle->score);
				destroyBattle(battle);
				fclose(fptr);
				average = (float)0;

			}
			else
			{	
				printf("Average score: %f\n",average);
				average = (float)0;
			}
		}
		// geneScore = createBetterGeneScore();
		geneScore = createNthGeneScore(3);
	}
	destroyGeneScore(geneScore);

	battle = duel(getGene(valueFunction,VF_SIZE-1),threes);
	// print the score of every gene
	if(silent == 1)
	{
		FILE *fptr;
		fptr = fopen("results.txt","w");
		for(i=0;i<VF_SIZE;i++)
		{
			fprintf(fptr,"%s #%d average score: %f\n",getGene(valueFunction,i),i,(float)getGeneScore(valueFunction,i)->score/VF_SIZE);
		}
		fprintf(fptr,"Best gene score against threes: %d\n",battle->score);
		fclose(fptr);
	}
	else
	{
		for(i=0;i<VF_SIZE;i++)
		{
			printf("Gene %d average score: %f\n",i,(float)getGeneScore(valueFunction,i)->score/VF_SIZE);
			printf("Gene: %s\n",getGene(valueFunction,i));
		}	
	}
	destroyBattle(battle);
	destroyValueFunction(valueFunction);


	return 0;
}