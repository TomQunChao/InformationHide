from matplotlib import pyplot as plt
import numpy as np
from scipy.special import gamma

def normalDistribute(x,mu,sigma2):
    return np.exp(-1*(x-mu)**2/
                  (2*sigma2))/np.sqrt(2*np.pi*sigma2)
def expDistribute(x,beta):
    return 1/beta*np.exp(-1*x/beta)
def ggdDistribute(x,beta,c):
    A = beta*c/(2*gamma(1/c))
    return A*np.exp(-np.power(np.abs(beta*x), c))

def readArr(f,arr):
    num=int(f.readline())
    for i in range(0,num):
        arr.append(float(f.readline()))
    str1=f.readline()
    a=[]
    while str1:
        temp=str1.split()
        for j in temp:
            a.append(float(j))
        str1=f.readline()
    return a
def getXArr(arr,num=10000):
    minX=min(arr)
    maxX=max(arr)
    x=[]
    for i in range(0,num):
        x.append(minX+i*(maxX-minX)/num)
    return np.asarray(x)
normalF=open("normal.hex")
expF=open("exp.hex")
ggd05F=open("ggd05.hex")
ggd1F=open("ggd1.hex")

normalArr=[]
expArr=[]
ggd05Arr=[]
ggd1Arr=[]

normalRes=readArr(normalF,normalArr)
expRes=readArr(expF,expArr)
ggd05Res=readArr(ggd05F,ggd05Arr)
ggd1Res=readArr(ggd1F,ggd1Arr)

print(normalRes)

# draw normal
x=getXArr(normalArr)
plt.subplot(2,2,1)
plt.hist(normalArr,bins=100,density=True)
plt.plot(x,normalDistribute(x,normalRes[0],normalRes[1]),label="normal-maxlike")
plt.plot(x,normalDistribute(x,normalRes[2],normalRes[3]),label="normal-moment")
plt.title("normal")
plt.legend()

# draw exp
x=getXArr(expArr)
plt.subplot(2,2,2)
plt.hist(expArr,bins=100,density=True)
plt.plot(x,expDistribute(x,expRes[0]))
plt.title("exp")

# hist ggd
x = []
for i in range(0,100000):
    x.append(-5+i*10/100000)
x = np.asarray(x)
print(x.shape,x)
plt.subplot(2,2,3)
plt.hist(ggd05Arr,bins=100,density=True,range=(-5,5))
plt.plot(x, ggdDistribute(x, 4, 0.5))
plt.title("GGD-0.5")

plt.subplot(2,2,4)
plt.hist(ggd1Arr,bins=100,density=True,range=(-5,5))
plt.plot(x, ggdDistribute(x, 4, 1))
plt.title("GGD-1")

plt.show()

