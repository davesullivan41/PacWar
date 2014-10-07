PacWar
======
This program is written for the class project found here: https://www.clear.rice.edu/comp440/modules/termproject/, which uses a genetic algorithms to build the best "pac-mite" species for an imaginary world. Each pac-mite has a set of 50 genes which control its behavior in all possible situations. 

Most of the code was provided by Professor Subramanian, but the following files were written by myself:

GeneGenerator.h
Example.c
randomGene.py
test.c

GeneGenerator.h provides functions which create "good" and "better" pac-mites. 

A "good" pac-mite is developed through the following process:
First, generate 8 completely random genes. This is done by randomGene.py
Next, take each gene and have it fight against one other gene in the set. (see link at top of page for details of fighting mechanism). Then, a score is computed for the fight relative to one of the mites. 
Then, this score is used to combine the two genes into a single new gene. This is done by taking a percentage of genes from each mite corresponding to how well they did in the fight. For example, if one gene defeated the other without contest and receive the highest possible score, then the new gene will be an identical copy of that winner gene. If neither pac-mite performed better than its competitor, then the new gene will be half built from one gene and half built from the other. 
After the first round, there are 4 new mites, and the process discussed above is repeated until only a single mite remains. 

The "better" pac-mite follows a nearly identical process, except that it starts off with 16 random genes, rather than 8. 

By iterating the above process, fairly good genes can be found, but it is unlikely that extremely good genes will be found. 

That is why the next step is to develop a "value function" that can take a gene as an argument and return a measure of how good that gene is. Our current standard for this measure is the percentage of all other (4^50) genes that the given gene can defeat. Calculating this directly is (for obvious reasons) impractical, but this function can be approximated.

The value function will be approximated by building a set of n genes that are known to be good, and then calculating how many of those genes an input gene can defeat. This value function will be continually updating by inserting into it any gene that achieves a score that is higher than the gene in the set with the minimum score.

After a certain (yet to be determined) number of iterations, this value function can then be used to build a "killer" gene. 
