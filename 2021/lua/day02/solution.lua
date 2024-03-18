local x1, y1 = 0, 0;
local x2, y2, aim = 0, 0, 0;

local dirs = {
    forward = { 1, 0 },
    down = { 0, 1 },
    up = { 0 , -1 }
};

for line in io.lines("input2.txt") do
    local stringIter = string.gmatch(line, "[^%s]+");

    local dir = dirs[stringIter()];
    local size = tonumber(stringIter());

    -- Part 1
    x1 = x1 + (dir[1] * size);
    y1 = y1 + (dir[2] * size);

    -- Part 2
    aim = aim + (dir[2] * size);
    x2 = x2 + (dir[1] * size);
    y2 = y2 + (dir[1] * aim * size)
end

print("Part 1: " .. x1 * y1);
print("Part 2: " .. x2 * y2);