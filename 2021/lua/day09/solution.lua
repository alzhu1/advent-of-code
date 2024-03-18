local grid = {};
for line in io.lines("input.txt") do
    grid[#grid+1] = {};

    for c in line:gmatch(".") do
        local currRow = #grid;
        grid[currRow][#grid[currRow]+1] = tonumber(c);
    end
end

local deltas = { {1, 0}, {0, 1}, {-1, 0}, {0, -1} };
local riskLevel = 0;

local lowPoints = {};
for i = 1, #grid do
    for j = 1, #grid[i] do
        local currValue = grid[i][j];
        local higherNeighbors = 0;

        for _, delta in pairs(deltas) do
            local neighborRow = grid[i + delta[1]];
            if neighborRow ~= nil then
                local neighbor = neighborRow[j + delta[2]];
                higherNeighbors = higherNeighbors + ((neighbor == nil or neighbor > currValue) and 1 or 0);
            else
                higherNeighbors = higherNeighbors + 1;
            end
        end

        if higherNeighbors == 4 then
            riskLevel = riskLevel + currValue + 1;

            lowPoints[#lowPoints+1] = { i, j };
        end
    end
end

print("Part 1: ", riskLevel);

-- Part 2: Do a BFS at each low point
local sizes = {}

for _, lowPoint in pairs(lowPoints) do
    local queue = { lowPoint };
    local queueStart, queueEnd = 1, 2;
    
    local currSize = 0;
    while queueStart < queueEnd do
        local currPoint = queue[queueStart];
        queueStart = queueStart + 1;

        local gridRow = grid[currPoint[1]];
        local gridValue = nil;
        if gridRow ~= nil then
            gridValue = gridRow[currPoint[2]];
        end

        if gridValue ~= nil and gridValue ~= 9 then
            currSize = currSize + 1;
            -- Add neighbors
            for _, delta in pairs(deltas) do
                queue[queueEnd] = { currPoint[1] + delta[1], currPoint[2] + delta[2] };
                queueEnd = queueEnd + 1;
            end

            grid[currPoint[1]][currPoint[2]] = 9;
        end
    end

    sizes[#sizes+1] = currSize;
end

table.sort(sizes);

local product = 1;
for i = #sizes, #sizes - 2, -1 do
    product = product * sizes[i];
end

print("Part 2: ", product);