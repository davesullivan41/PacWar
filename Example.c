#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "GeneGenerator.h"

int main()
{
	// allocate some memory for the genes
	char *a = malloc(sizeof(char) * 50);
	char *b = malloc(sizeof(char) * 50);

	// generate a new gene that has won an 8-tournament
	getGoodGene(a);
	// generate a new gene that has won a 16-tournament
	getBetterGene(b);

	// run a duel between them
	struct Battle *battle = duel(a,b);
	
	// Note: getBetterGene may take about twice as long as getGoodGene, but
	// in my testing, gene b wins most duels.

	// printing helper function. For direct access to the final score,
	// battle->score will give the score for gene a.
	printBattle(battle);

	// clean up the heap when you're done -- don't want any memory leaks
	free(a);
	free(b);
	destroyBattle(battle);

	return 0;
}