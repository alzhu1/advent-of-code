import math

f = open("input.txt", "r")
values = f.read().split('\n')

boss_stats = []
for value in values:
    _, stat_value = value.split(': ')

    # Should be HP, then Damage, then Armor
    boss_stats.append(int(stat_value))

boss_stats = tuple(boss_stats)
print(boss_stats)

def does_player_win(player_stats):
    player_hp, player_damage, player_armor = player_stats
    boss_hp, boss_damage, boss_armor = boss_stats

    boss_takes_damage = max(1, player_damage - boss_armor)
    player_takes_damage = max(1, boss_damage - player_armor)

    turns_to_kill_boss = math.ceil(boss_hp / boss_takes_damage)
    turns_to_kill_player = math.ceil(player_hp / player_takes_damage)

    return turns_to_kill_boss <= turns_to_kill_player

weapons = [(8, 4), (10, 5), (25, 6), (40, 7), (74, 8)]
armors = [(13, 1), (31, 2), (53, 3), (75, 4), (102, 5)]
rings = [(25, (1, 0)), (50, (2, 0)), (100, (3, 0)), (20, (0, 1)), (40, (0, 2)), (80, (0, 3))]

states = {

}
winners = set()

min_cost = float('inf')
max_cost = float('-inf')

# TODO: probably a faster way to do this (brute force works cause small amount of options)
def traverse_items(weapon_idxs, armor_idxs, ring_idxs):
    curr_damage = 0
    curr_armor = 0

    cost = 0

    if len(weapon_idxs) == 1:
        weapon_cost, weapon_damage = weapons[weapon_idxs[0]]
        curr_damage += weapon_damage
        cost += weapon_cost
    
    if len(armor_idxs) == 1:
        armor_cost, armor_armor = armors[armor_idxs[0]]
        curr_armor += armor_armor
        cost += armor_cost

    for ring_idx in ring_idxs:
        ring_cost, rest = rings[ring_idx]
        ring_damage, ring_armor = rest
        curr_damage += ring_damage
        curr_armor += ring_armor
        cost += ring_cost

    # Part 1
    # if does_player_win((100, curr_damage, curr_armor)):
    #     global min_cost
    #     min_cost = min(min_cost, cost)
    #     return

    # Part 2
    if not does_player_win((100, curr_damage, curr_armor)):
        global max_cost
        print(weapon_idxs, armor_idxs, ring_idxs, cost)
        max_cost = max(max_cost, cost)

    # We don't win, so look at other options
    
    # Try armors
    if len(armor_idxs) == 0:
        for i in range(len(armors)):
            traverse_items(weapon_idxs, [i], ring_idxs)
    
    # Try rings
    if len(ring_idxs) < 2:
        for i in range(len(rings)):
            if i in ring_idxs:
                continue
            ring_idxs_copy = ring_idxs[:]
            ring_idxs_copy.append(i)
            traverse_items(weapon_idxs, armor_idxs, ring_idxs_copy)

# Weapon is required
for i in range(len(weapons)):
    traverse_items([i], [], [])

print(min_cost)
print(max_cost)