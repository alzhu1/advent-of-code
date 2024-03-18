local schedule = {};
for i = 0, 8 do schedule[i] = 0 end

local fishCount = 0;
local state = io.open("input.txt", "r"):lines()();
for n in string.gmatch(state, "([^,]+)") do
    local num = tonumber(n);
    schedule[num] = schedule[num] + 1;
    fishCount = fishCount + 1;
end

local days = 0;
while days < 256 do
    local zeroAge = schedule[0];
    for i = 0, 7 do
        schedule[i] = schedule[i + 1];
    end

    schedule[6] = schedule[6] + zeroAge;
    schedule[8] = zeroAge;
    fishCount = fishCount + zeroAge;

    days = days + 1;
    if days == 80 then
        print("Part 1: ", fishCount);
    end
end

print("Part 2: ", fishCount);
