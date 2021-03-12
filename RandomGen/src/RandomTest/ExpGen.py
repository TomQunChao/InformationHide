'''
generate Exponential Distribution
'''
import numpy as np
class ExpGen:
    def __init__(self,beta,m):
        self.beta=beta
        self.m=m
    
    def extract(self):
        temp=self.m.extractFloat64()
        while temp==0:
            temp=self.m.extractFloat64()
        return -1*self.beta*np.log(1-temp)