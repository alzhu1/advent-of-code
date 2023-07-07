import sys
import math

input = int(sys.argv[1])

def get_factors(num):
    limit = math.floor(math.sqrt(num))
    factors = set()
    for i in range(1, limit + 1):
        if num % i == 0:
            if i * 50 >= num:
                factors.add(i)
            
            if int(num / i) * 50 >= num:
                factors.add(int(num / i))
    return factors

# Very brute force, not pretty
idx = 2
while True:
    if sum(get_factors(idx)) * 11 >= input:
        break

    idx += 1

print(idx)