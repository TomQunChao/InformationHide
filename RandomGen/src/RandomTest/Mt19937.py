'''
Python code Implementation of Mersenne Twister
https://zh.wikipedia.org/wiki/%E6%A2%85%E6%A3%AE%E6%97%8B%E8%BD%AC%E7%AE%97%E6%B3%95
'''


class MT19937:
    
    def __init__(self, seed):
        self.mt = [0] * 624
        self.mt[0] = seed
        self.mti = 0
        for i in range(1, 624):
            self.mt[i] = self._int32(1812433253 * (self.mt[i - 1] ^ self.mt[i - 1] >> 30) + i)

    
    def _int32(self,x):
        return int(0xFFFFFFFF & x)

    def extract_number(self):
        if self.mti == 0:
            self.twist()
        y = self.mt[self.mti]
        y = y ^ y >> 11
        y = y ^ y << 7 & 2636928640
        y = y ^ y << 15 & 4022730752
        y = y ^ y >> 18
        self.mti = (self.mti + 1) % 624
        return self._int32(y)
        
    def twist(self):
        for i in range(0, 624):
            y = self._int32((self.mt[i] & 0x80000000) + (self.mt[(i + 1) % 624] & 0x7fffffff))
            self.mt[i] = (y >> 1) ^ self.mt[(i + 397) % 624]

            if y % 2 != 0:
                self.mt[i] = self.mt[i] ^ 0x9908b0df
                
    # my own method, return a random value between [0,1]
    MAX_VALUE=0xFFFFFFFF
    MIN_VALUE=0
    def extractFloat64(self):
        return self.extract_number()/float(self.MAX_VALUE-self.MIN_VALUE)