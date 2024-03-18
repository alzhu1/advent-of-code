local counter = {};
local overlapCount = 0;

for line in io.lines("input.txt") do
    local lineIter = string.gmatch(line, "([^->]+)");

    local p1Iter = string.gmatch(lineIter(), "([^,]+)");
    local p2Iter = string.gmatch(lineIter(), "([^,]+)");

    local x1, y1 = tonumber(p1Iter()), tonumber(p1Iter());
    local x2, y2 = tonumber(p2Iter()), tonumber(p2Iter());

    -- Start from x1, x2 and go to y1, y2
    local xDelta = x1 < x2 and 1 or -1;
    local yDelta = y1 < y2 and 1 or -1;

    local x = x1;
    local y = y1;

    while true do
        if counter[x] == nil then
            counter[x] = {};
        end
    
        if counter[x][y] == nil then
            counter[x][y] = 0;
        end
    
        counter[x][y] = counter[x][y] + 1;
        if counter[x][y] == 2 then
            overlapCount = overlapCount + 1;
        end

        if x == x2 and y == y2 then
            break;
        end

        if x ~= x2 then
            x = x + xDelta;
        end

        if y ~= y2 then
            y = y + yDelta;
        end
    end
end

print(overlapCount);