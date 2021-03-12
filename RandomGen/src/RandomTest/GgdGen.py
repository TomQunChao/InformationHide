
from ExpGen import ExpGen
class GgdGen:
    def __init__(self,beta,m1,m2,m3):
        self.m1=m1
        self.m2=m2
        self.beta=beta
        self.em1=ExpGen(beta,m2)
        self.em2=ExpGen(beta**0.5,m2)
        self.em3=ExpGen(beta**0.5,m3)
    
    GAMMA1=1
    GAMMA2=1
    GAMMA3=2
    GAMMA6=120
    
    def extractGgd05(self):
        y=self.em2.extract()
        z=self.em3.extract()
        if self.m1.extract_number()>self.m1.MAX_VALUE/2:
            return y+z
        else:
            return -(y+z)
    def extractGgd1(self):
        if self.m1.extract_number()>self.m1.MAX_VALUE/2:
            return self.em1.extract()
        else:
            return -1*self.em1.extract()
    
        