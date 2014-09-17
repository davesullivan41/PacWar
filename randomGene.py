import random

def randomGene():
	genome = ['-']*50
	for i in range(0,50):
		genome[i] = str(random.randint(0,3))
	genome = ''.join(genome)
	print genome
	return genome

randomGene()