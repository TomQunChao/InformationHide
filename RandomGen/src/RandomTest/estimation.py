'''

'''
import numpy as np

def momentEstimationNormal(nums):
    nums=np.asarray(nums)
    mu=sum(nums)/len(nums)
    delta2=sum(nums**2)/len(nums)-mu**2
    return (mu,delta2)
def maxLikeEstimationNormal(nums):
    nums=np.asarray(nums)
    mu=sum(nums)/len(nums)
    delta2=sum((nums-mu)**2)/len(nums)
    return (mu,delta2)
def estimationExp(nums):
     nums=np.asarray(nums)
     return sum(nums)/len(nums)
    