import sys
import math

input = int(sys.argv[1])

def is_prime(num):
    limit = math.floor(math.sqrt(num))
    for i in range(2, limit + 1):
        if num % i == 0:
            return (False, i)
    return (True, 1)

primes = {2}
i = 3
while len(primes) < 100000:
    if is_prime(i)[0]:
        primes.add(i)
    i += 2

# https://math.stackexchange.com/questions/22721/is-there-a-formula-to-calculate-the-sum-of-all-proper-divisors-of-a-number
def factor_sum(factors):
    sum = 1
    for factor_num, factor_count in factors.items():
        temp_sum = 1
        temp_idx = 1
        while factor_count > 0:
            temp_sum += (factor_num ** temp_idx)
            temp_idx += 1
            factor_count -= 1
        sum *= temp_sum
    return sum

# Very brute force, not pretty
idx = 2
while True:
    i = idx
    factors = {}

    while i not in primes:
        for factor in primes:
            if i % factor == 0:
                # Add factor
                if factor not in factors:
                    factors[factor] = 0

                factors[factor] += 1
                i /= factor
                break

    if i not in factors:
        factors[int(i)] = 1
    else:
        factors[int(i)] += 1
    
    if factor_sum(factors) * 10 >= input:
        break
    idx += 1

print(idx)

# print(factorize(input))

# i = 1
# while True:
#     if sum(factorize(i)) * 10 >= input:
#         break
#     i += 1

# print(i)