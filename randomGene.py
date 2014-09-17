import random

def randomGene():
	genome = ['-']*50
	for i in range(0,50):
		genome[i] = str(random.randint(0,3))
	genome = ''.join(genome)
	return genome

gene1 = randomGene()
gene2 = randomGene()
f = open('gene.txt','w');
f.write(gene1)
f.write('\n')
f.write(gene2)