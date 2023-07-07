f = open("input.txt", "r")
values = f.read().split('\n')

end_time = 2503
reindeer_distances = {}

for value in values:
    reindeer, rest = value.split(' can fly ')
    rate, rest = rest.split(' km/s for ')
    rate_time, rest = rest.split(' seconds, but then must rest for ')
    rest_time = rest.split(' seconds.')[0]

    rate, rate_time, rest_time = int(rate), int(rate_time), int(rest_time)

    # Start with cycles
    cycles = int(end_time / (rate_time + rest_time))
    reindeer_distances[reindeer] = rate * rate_time * cycles

    remaining_time = end_time - (cycles * (rate_time + rest_time))

    if rate_time < remaining_time:
        reindeer_distances[reindeer] += rate * rate_time
    else:
        reindeer_distances[reindeer] += rate * max(0, remaining_time - rate_time)

print(max(reindeer_distances.values()))