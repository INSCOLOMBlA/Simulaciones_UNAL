import collections
import numpy as np
from numpy import genfromtxt
import glob
import math as ma
from scipy import optimize
import matplotlib.pyplot as plt
path = "*.csv"
def gaussian(x, mu, sigma):
	n=1/(ma.sqrt(2*ma.pi)*sigma)
	return n*np.exp(-0.5*((x-mu)/(sigma))**2)
def sort_list(list1, list2): 
    zipped_pairs = zip(list2, list1) 
    z = [x for _, x in sorted(zipped_pairs)] 
    return z 
for fname in glob.glob(path):
	my_data = genfromtxt(fname, delimiter='\t')
	conta=[]
	c=[]
	for i in range(len(my_data)):
		c.append(my_data[i][0])
		conta.append(my_data[i][1])
	ctr = collections.Counter(conta)	
	sep=zip(*collections.Counter(conta).items())
	a, b = zip(*collections.Counter(conta).items())
	bb=[]
	print(a)
	for item in b:
		bb.append(item/sum(b))
	bb=sort_list(bb,a)
	a=sorted(a)
	a.sort()
	fname=fname.replace('.csv','')
	fname=fname.replace('\\','')
	fname=fname.replace('corridas_','')
	file = open("Hist"+str(fname)+".txt", "w")
	print(fname)
	for index in range(len(a)):
		file.write(str(a[index])+" "+str(bb[index])+"\n")
	file.close()
	popt,_= optimize.curve_fit(gaussian, a, bb)
	plt.title(fname)
	plt.plot(a, bb,".")
	plt.plot(a, gaussian(a, *popt))
	plt.show()
