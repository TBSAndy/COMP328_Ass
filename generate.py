# generate a matrix with n * n + 1 
import random

n = int(input())
SIZE = 100

print(n)

for i in range(n):
    nums = []
    for i in range(n+1):
        nums.append(random.random() * SIZE)

    print(' '.join(map(str, nums)))
        



