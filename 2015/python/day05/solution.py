
# nice: 3+ vowels (aeiou), at least 1 letter twice in a row, does NOT contain ab, cd, pq, xy

f = open("input.txt", "r")
values = f.read().split('\n')

bad_words = { "ab", "cd", "pq", "xy" }
vowels = { 'a', 'e', 'i', 'o', 'u' }
nice_count = 0
for value in values:
    # Setup
    vowel_count = 0
    doubled = False
    nice = True
    for i in range(len(value)):
        char = value[i]

        if vowel_count < 3 and char in vowels:
            vowel_count += 1
        
        if i == 0:
            continue

        char_prev = value[i-1]

        two_string = char_prev + char
        if two_string in bad_words:
            nice = False
            break

        if not doubled and char_prev == char:
            doubled = True

    if nice and doubled and vowel_count >= 3:
        nice_count += 1
        
print(nice_count)