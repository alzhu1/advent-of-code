package.path = package.path .. ';../?.lua';
local PriorityQueue = require("PriorityQueue");

local grid = {};

for line in io.lines("input.txt") do
    local row = {};
    for i = 1, line:len() do
        row[#row+1] = tonumber(line:sub(i, i));
    end
    grid[#grid+1] = row;
end

-- Part 2: Turn grid to a 5x5
local gridSize = #grid;
for i = 0, gridSize - 1 do
    for j = 0, gridSize - 1 do
        local baseValue = grid[i + 1][j + 1];

        for ii = 0, 4 do
            for jj = 0, 4 do
                local nextI = i + (ii * gridSize);
                local nextJ = j + (jj * gridSize);
                local nextValue = (baseValue + ii + jj);
                if nextValue > 9 then
                    nextValue = nextValue - 9;
                end

                if grid[nextI + 1] == nil then
                    grid[nextI + 1] = {};
                end
                grid[nextI + 1][nextJ + 1] = nextValue;
            end
        end
    end
end

local pQueue = PriorityQueue:new();
pQueue:enqueue({ 1, 1 }, 0);

local cache = {};

local deltas = { {1, 0}, {0, 1}, {-1, 0}, {0, -1} };
local a = 0;
while pQueue:len() > 0 do
    local currPoint, currRisk = pQueue:dequeue();

    local cacheKey = currPoint[1] .. "," .. currPoint[2];

    if cache[cacheKey] == nil then
        cache[cacheKey] = 1;

        if currPoint[1] == #grid and currPoint[2] == #grid[currPoint[1]] then
            print("Final risk level: ", currRisk);
            break;
        end

        for _, delta in pairs(deltas) do
            local nextX = currPoint[1] + delta[1];
            local nextY = currPoint[2] + delta[2];

            if nextY >= 1 and nextY <= #grid and nextX >= 1 and nextX <= #grid[nextY] then
                local nextRisk = grid[nextY][nextX];

                pQueue:enqueue({ nextX, nextY }, currRisk + nextRisk);
            end
        end
    end
end