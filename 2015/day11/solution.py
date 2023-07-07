import sys

input = bytearray(sys.argv[1], 'utf-8')

min_byte_int = 97 # a
max_byte_int = 122 # z

bad_letters = { b'i', b'o', b'l' }

def pair_check(pair):
    key, value = pair
    return len(key) == 2

def check_password(input_bytes):
    if b'i' in input_bytes or b'o' in input_bytes or b'l' in input_bytes:
        return False

    three_increasing = False

    unique_pairs = {}
    counter = 0
    last_byte = 0

    for i in range(len(input_bytes)):
        if i == 0:
            counter = 1
            last_byte = input_bytes[i]
            continue

        prev_byte = input_bytes[i-1]
        curr_byte = input_bytes[i]

        if last_byte == input_bytes[i]:
            counter += 1
        else:
            rep = chr(last_byte) * counter
            if rep not in unique_pairs:
                unique_pairs[rep] = 0
            
            unique_pairs[rep] += 1

            counter = 1
            last_byte = curr_byte
        
        # Also add case for last index
        if i == len(input_bytes) - 1:
            rep = chr(last_byte) * counter
            if rep not in unique_pairs:
                unique_pairs[rep] = 0
            
            unique_pairs[rep] += 1

            counter = 1
            last_byte = curr_byte

        if i > 1 and input_bytes[i-2] == prev_byte-1 and prev_byte == curr_byte-1:
            three_increasing = True
        
        if three_increasing and len(dict(filter(pair_check, unique_pairs.items()))) > 1:
            return True

    return False
    
# NOTE: very inefficient. likely improved if we skip i/o/l in this loop and increment it directly
while not check_password(input):
    idx = len(input) - 1

    while True:
        input[idx] += 1

        if input[idx] <= max_byte_int:
            break
        else:
            input[idx] = min_byte_int
            idx -= 1
    
    # Check for bad letters, if found then increment by 1 and set remaining after to a
    # TODO: not sure if improved by much?
    found_bad_letter = False
    for i in range(len(input)):
        if input[i] in bad_letters:
            input[i] += 1
            found_bad_letter = True
            continue
        
        if found_bad_letter:
            input[i] = min_byte_int

print(input)
