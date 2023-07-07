import itertools

f = open("input.txt", "r")
values = [int(x) for x in f.read().split('\n')]

total_weight = sum(values)
group_max_weight = int(total_weight / 3)

subset_sums = set()
subset_sum_dict = {}

# https://stackoverflow.com/questions/4632322/finding-all-possible-combinations-of-numbers-to-reach-a-given-sum
def subset_sum(numbers, target, partial=[]):
    if tuple(partial) in subset_sums:
        if len(partial) not in subset_sum_dict:
            subset_sum_dict[len(partial)] = []
        subset_sum_dict[len(partial)].append(tuple(partial))
        return

    s = sum(partial)

    # check if the partial sum is equals to target
    if s == target: 
        subset_sums.add(tuple(partial))
        if len(partial) not in subset_sum_dict:
            subset_sum_dict[len(partial)] = []
        subset_sum_dict[len(partial)].append(tuple(partial))

    if s >= target:
        return  # if we reach the number why bother to continue
    
    for i in range(len(numbers)):
        n = numbers[i]
        remaining = numbers[i+1:]
        subset_sum(remaining, target, partial + [n]) 

# Fill set and dict
subset_sum(values, group_max_weight)

i = 1
while i not in subset_sum_dict:
    i += 1

def check(i):
    for subset in subset_sum_dict[i]:
        remaining = values[:]
        for num in subset:
            remaining.remove(num)
        
        # Remaining should have the remaining numbers after group 1
        for j in range(i, len(remaining)):
            remaining_subsets = list(itertools.combinations(tuple(remaining), j))

            for remaining_subset in remaining_subsets:
                if remaining_subset not in subset_sums:
                    continue
                
                rem = remaining[:]
                for num in remaining_subset:
                    rem.remove(num)
                
                if tuple(rem) not in subset_sums:
                    continue

                # At this point, add to some list
                prod = 1
                for v in subset:
                    prod *= v
                print('smaller qe found', prod)
                return prod

print(check(i))