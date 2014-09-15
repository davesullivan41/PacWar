import random

def randomGene():
	genome = ['-']*50
	for i in range(0,50):
		genome[i] = str(random.randint(0,3))
	genome = ''.join(genome)
	return genome

# print "Generating new genes now"
threes = ['3']*50
threes = ''.join(threes)
f = open('gene.txt','w');
for i in range(0,8):
	f.write(randomGene())
	f.write('\n')
# f.write(threes)

f.close()