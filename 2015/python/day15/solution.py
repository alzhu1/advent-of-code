f = open("input.txt", "r")
values = f.read().split('\n')

# values = [
#     'Butterscotch: capacity -1, durability -2, flavor 6, texture 3, calories 8',
#     'Cinnamon: capacity 2, durability 3, flavor -2, texture -1, calories 3'
# ]

ingredient_properties = {}
ingredients = []
for value in values:
    ingredient, rest = value.split(': capacity ')
    capacity, rest = rest.split(', durability ')
    durability, rest = rest.split(', flavor ')
    flavor, rest = rest.split(', texture ')
    texture, calories = rest.split(', calories ')

    ingredient_properties[ingredient] = (int(capacity), int(durability), int(flavor), int(texture), int(calories))
    ingredients.append(ingredient)

cookie_scores = {}

# Tuple is in the order of ingredients array
# NOTE: Very slow and bad algorithm, would be better to check the final scores itself (use all 100 teaspoons)
def get_cookie_score(ingredients_tuple):
    if ingredients_tuple in cookie_scores:
        cookie_scores[ingredients_tuple]
        return

    capacity, durability, flavor, texture, calories = (0, 0, 0, 0, 0)
    for i in range(len(ingredients_tuple)):
        num_ingredients = ingredients_tuple[i]
        ingredient_props = ingredient_properties[ingredients[i]]

        capacity += num_ingredients * ingredient_props[0]
        durability += num_ingredients * ingredient_props[1]
        flavor += num_ingredients * ingredient_props[2]
        texture += num_ingredients * ingredient_props[3]
        calories += num_ingredients * ingredient_props[4]
    
    # Part 2
    modifier = 1 if calories == 500 else -1
    cookie_scores[ingredients_tuple] = max(capacity, 0) * max(durability, 0) * max(flavor, 0) * max(texture, 0) * modifier

    if sum(ingredients_tuple) >= 100:
        return
    
    for i in range(len(ingredients_tuple)):
        get_cookie_score(ingredients_tuple[:i] + ((ingredients_tuple[i] + 1),) + (ingredients_tuple[i+1:]))

get_cookie_score((0, 0, 0, 0))

def key_filter(pair):
    key, value = pair
    return sum(key) == 100

filtered_scores = dict(filter(key_filter, cookie_scores.items()))
print(max(filtered_scores.values()))