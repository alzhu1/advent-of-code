local prev = 0;
local part1Count = -1;

local window = {};
local index = 1;
local sum = 0;
local part2Count = 0;

for line in io.lines("input.txt") do
    local num = tonumber(line)

    -- Part 1
    if num > prev then
        part1Count = part1Count + 1
    end
    prev = num;

    -- Part 2
    if index < 4 then
        window[index] = num;
        sum = sum + num;
        index = index + 1
    else
        local prevSum = sum;
        sum = sum - window[1] + num;

        if sum > prevSum then
            part2Count = part2Count + 1
        end

        for i=1,2 do window[i] = window[i + 1] end
        window[3] = num;
    end
end

print("Part 1: " .. part1Count);
print("Part 2: " .. part2Count);