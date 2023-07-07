f = open("input.txt", "r")
values = f.read().split('\n')

end_time = 2503
reindeer_distances = {}
reindeer_points = {}
reindeer_rates = {}
reindeers = []

for value in values:
    reindeer, rest = value.split(' can fly ')
    rate, rest = rest.split(' km/s for ')
    rate_time, rest = rest.split(' seconds, but then must rest for ')
    rest_time = rest.split(' seconds.')[0]

    rate, rate_time, rest_time = int(rate), int(rate_time), int(rest_time)

    reindeer_distances[reindeer] = 0
    reindeer_points[reindeer] = 0
    reindeers.append(reindeer)

    reindeer_rates[reindeer] = (rate, rate_time, rest_time)

# Simulate race
for i in range(end_time):
    for reindeer in reindeers:
        # At this time calculate distance
        rate, rate_time, rest_time = reindeer_rates[reindeer]
        cycle_time = i % (rate_time + rest_time)

        if cycle_time < rate_time:
            reindeer_distances[reindeer] += rate

    # if i == 1:
    #     print(reindeer_distances)
    winning_reindeer = max(reindeer_distances, key=reindeer_distances.get)
    reindeer_points[winning_reindeer] += 1

print(max(reindeer_points.values()))