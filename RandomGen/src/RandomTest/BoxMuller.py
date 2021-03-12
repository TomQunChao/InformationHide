'''
Box-Muller
Method reference:https://baike.baidu.com/item/box-muller/4023794?fr=aladdin
'''
import numpy as np

class BoxMuller:
    # MT19937 m1 and m2 should use different seeds
    def __init__(self,m1,m2):
        self.m1=m1
        self.m2=m2
    
    def extractNormalRandom(self):
        theta=self.m2.extractFloat64()
        while theta==0:
            theta=self.m1.extractFloat64()
        theta=theta*np.pi*2
        r=self.m1.extractFloat64()
        while r==0:
            r=self.m1.extractFloat64()
        r=np.sqrt(-2*np.log(r))
        return r*np.cos(theta)