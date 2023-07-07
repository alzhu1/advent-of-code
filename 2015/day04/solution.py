# Basically just count the number of ( and ), subtract it
import sys
import hashlib

# Secret key
input_string = sys.argv[1]
num = 0

while True:
    result = hashlib.md5((input_string + str(num)).encode()).hexdigest()
    # Part 1: 5 zeroes
    # Part 2: 6 zeroes

    # Guessing an optimization is to check the bytes themselves, for 0's, instead of string comp
    if result[0:6] == "000000":
        break
    num += 1

print(num)


