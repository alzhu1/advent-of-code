
# nice: a pair of any two letters that appears 2+ times without overlapping
# Contains 1+ letters which repeats with exactly 1 letter in between

f = open("input.txt", "r")
values = f.read().split('\n')

nice_count = 0
for value in values:
    # Setup
    doubled = False
    between = False

    double_dict = {}
    last_double = ""
    for i in range(len(value)):
        if i == 0:
            continue

        # Check doubled
        char = value[i]
        char_prev = value[i-1]

        if not doubled and str(char_prev + char) != last_double:
            last_double = str(char_prev + char)
            if last_double not in double_dict:
                double_dict[last_double] = 0
            
            double_dict[last_double] += 1
            if double_dict[last_double] > 1:
                doubled = True
        else:
            last_double = ""
        
        if not between and i > 1 and value[i-2] == char:
            between = True

        if between and doubled:
            nice_count += 1
            break

print(nice_count)