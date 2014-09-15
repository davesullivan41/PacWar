import random

def randomGene():
	genome = ['-']*50
	for i in range(0,50):
		genome[i] = str(random.randint(0,3))
	genome = ''.join(genome)
	return genome

print "Generating new genes now"
threes = ['3']*50
threes = ''.join(threes)
ones = ['1']*50
ones = ''.join(ones)
f = open('gene.txt','w');
# f.write(ones)
# f.write('\n')
for i in range(0,7):
	f.write(randomGene())
	f.write('\n')
f.write(threes)

f.close()
print "Finished generating genes"