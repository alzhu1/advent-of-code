f = open("input.txt", "r")
values = f.read().split('\n')

boss_stats = []
for value in values:
    _, stat_value = value.split(': ')

    # Should be HP, then Damage
    boss_stats.append(int(stat_value))

boss_hp = boss_stats[0]
boss_damage = boss_stats[1]

player_hp = 50
player_mp = 500

visited_states = {}

def simulate_battle(player_hp, player_mp, boss_hp, shield_turns, poison_turns, recharge_turns, is_player_turn, spent_mp):
    key = (player_hp, player_mp, boss_hp, shield_turns, poison_turns, recharge_turns, is_player_turn)
    if key in visited_states and visited_states[key] <= spent_mp:
        # We visited this state in a better position, let's prevent going down this path
        return float('inf')

    visited_states[key] = spent_mp
    
    if boss_hp <= 0:
        return spent_mp

    # Part 2: If player turn, take 1 damage and fail if dead
    if is_player_turn:
        player_hp -= 1
        if player_hp <= 0:
            return float('inf')

    # If any effects are on, handle them
    player_armor = 7 if shield_turns > 0 else 0
    boss_take_damage = 3 if poison_turns > 0 else 0
    player_mp_gain = 101 if recharge_turns > 0 else 0

    player_mp += player_mp_gain
    boss_hp -= boss_take_damage
    if boss_hp <= 0:
        return spent_mp

    # Decrement effects (if any)
    shield_turns = max(0, shield_turns - 1)
    poison_turns = max(0, poison_turns - 1)
    recharge_turns = max(0, recharge_turns - 1)

    # Check for player/boss turn
    if not is_player_turn:
        # In boss turn, player takes damage
        player_hp -= max(boss_damage - player_armor, 1)
        if player_hp <= 0:
            return float('inf')
        
        # Switch back to player turn
        return simulate_battle(player_hp, player_mp, boss_hp, shield_turns, poison_turns, recharge_turns, not is_player_turn, spent_mp)
    else:
        # Player turn: test with all 5 actions and return the min spent cost from actions

        # If cannot cast, player loses
        if player_mp < 53:
            return float('inf')

        drain_action_cost = float('inf')
        shield_action_cost = float('inf')
        poison_action_cost = float('inf')
        recharge_action_cost = float('inf')

        # Magic Missile
        magic_missile_action_cost = simulate_battle(player_hp, player_mp - 53, boss_hp - 4, shield_turns, poison_turns, recharge_turns, not is_player_turn, spent_mp + 53)

        # Drain
        if player_mp >= 73:
            drain_action_cost = simulate_battle(player_hp + 2, player_mp - 73, boss_hp - 2, shield_turns, poison_turns, recharge_turns, not is_player_turn, spent_mp + 73)

        # Shield
        if player_mp >= 113 and shield_turns == 0:
            shield_action_cost = simulate_battle(player_hp, player_mp - 113, boss_hp, 6, poison_turns, recharge_turns, not is_player_turn, spent_mp + 113)

        # Poison
        if player_mp >= 173 and poison_turns == 0:
            poison_action_cost = simulate_battle(player_hp, player_mp - 173, boss_hp, shield_turns, 6, recharge_turns, not is_player_turn, spent_mp + 173)

        # Recharge
        if player_mp >= 229 and recharge_turns == 0:
            recharge_action_cost = simulate_battle(player_hp, player_mp - 229, boss_hp, shield_turns, poison_turns, 5, not is_player_turn, spent_mp + 229)

        # Return the action that results in the smallest amount of spent mp
        return min(magic_missile_action_cost, drain_action_cost, shield_action_cost, poison_action_cost, recharge_action_cost)

# Start sim, should return min mp spent
print(simulate_battle(player_hp, player_mp, boss_hp, 0, 0, 0, True, 0))