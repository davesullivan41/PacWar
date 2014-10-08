#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "GeneGenerator.h"

#define VF_SIZE 32

struct GeneScore{
	char *gene;
	int score;
};

struct ValueFunction{
	struct GeneScore *geneScores[VF_SIZE];
};

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
struct GeneScore *createGoodGeneScore()
{
	char *a = malloc(sizeof(char)*50);
	getGoodGene(a);
	struct GeneScore *geneScore = createGeneScore(a);
	free(a);
	return geneScore;
}

// creates a pseudo-random "better" gene score
struct GeneScore *createBetterGeneScore()
{
	char *a = malloc(sizeof(char)*50);
	getBetterGene(a);
	struct GeneScore *geneScore = createGeneScore(a);
	free(a);
	return geneScore;
}

// free memory of gene score
void deleteGeneScore(struct GeneScore *geneScore){
	assert(geneScore != NULL);
	free(geneScore->gene);
	free(geneScore);
}

// create a value function with VF_SIZE "good" genes
struct ValueFunction *createValueFunction()
{
	struct ValueFunction *valueFunction = malloc(sizeof(struct ValueFunction));
	int i;
	for(i=0;i<VF_SIZE;i++){
		setValueFunctionGene(valueFunction,createGoodGeneScore(),i);
	}
	return valueFunction;
}

// free memory of value function
void deleteValueFunction(struct ValueFunction *valueFunction){
	free(valueFunction);
}

// set a specific valueFunction location to a specific geneScore
void setValueFunctionGene(struct ValueFunction *valueFunction,struct GeneScore *geneScore,int index)
{
	valueFunction->geneScores[index] = geneScore;
}

// get the score of a certain gene
struct GeneScore *getGeneScore(struct ValueFunction *valueFunction,int index)
{
	return valueFunction->geneScores[index];
}

// get the gene of a specific genescore
char *getGene(struct ValueFunction *valueFunction,int index)
{
	return valueFunction->geneScores[index]->gene;
}

int main(){

	// creates a value function set with VF_SIZE random good genes
	struct ValueFunction *valueFunction = createValueFunction();
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
		}
	}

	// print the score of every gene
	for(i=0;i<VF_SIZE;i++)
	{
		printf("Gene %d total score: %d\n",i,getGeneScore(valueFunction,i)->score);
	}
	return 0;
}