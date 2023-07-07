import json

f = open("input.txt", "r")

doc = json.loads(f.read())

total = 0

def traverse_json(json):
    if isinstance(json, list):
        for item in json:
            traverse_json(item)
    elif isinstance(json, dict):
        # Part 2
        has_red = 'red' in json.values()

        if not has_red:
            for _, value in json.items():
                traverse_json(value)
    elif isinstance(json, int):
        global total
        total += json

traverse_json(doc)
print(total)