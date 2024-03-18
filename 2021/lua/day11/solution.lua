local grid = {}

for line in io.lines("input.txt") do
    grid[#grid+1] = {};

    for c in line:gmatch(".") do
        local currRow = #grid;
        grid[currRow][#grid[currRow]+1] = tonumber(c);
    end
end

local steps = 0;
local flashCount = 0;
while true do
    local tempFlashCount = 0;

    local flashPoints = {};
    local queue = {};
    local queueStart, queueEnd = 1, 1;

    -- Add 1 to all
    for i = 1, #grid do
        for j = 1, #grid[i] do
            grid[i][j] = grid[i][j] + 1;

            if grid[i][j] > 9 then
                queue[queueEnd] = { i, j };
                queueEnd = queueEnd + 1;
            end
        end
    end

    -- Check for flashes (BFS)
    while queueStart < queueEnd do
        local currPoint = queue[queueStart];
        queueStart = queueStart + 1;

        local gridRow = grid[currPoint[1]];
        local gridValue = -1;
        if gridRow ~= nil then
            gridValue = gridRow[currPoint[2]];
        else
            gridValue = -1;
        end

        -- Flash if greater than 9
        if gridValue > 9 then
            tempFlashCount = tempFlashCount + 1;

            -- Add to flashPoints
            if flashPoints[currPoint[1]] == nil then
                flashPoints[currPoint[1]] = {};
            end
            flashPoints[currPoint[1]][currPoint[2]] = 1;

            for i = -1, 1 do
                for j = -1, 1 do
                    local x, y = currPoint[1] + i, currPoint[2] + j;

                    -- Add 1 to valid neighbors (if it didn't flash before), and enqueue them
                    if grid[x] ~= nil and grid[x][y] ~= nil and (flashPoints[x] == nil or flashPoints[x][y] == nil) then
                        grid[x][y] = grid[x][y] + 1;
                        queue[queueEnd] = { x, y };
                        queueEnd = queueEnd + 1;
                    end
                end
            end

            grid[currPoint[1]][currPoint[2]] = 0;
        end
    end

    steps = steps + 1;

    flashCount = flashCount + tempFlashCount;
    if steps == 100 then
        print("Part 1: ", flashCount);
    end

    if tempFlashCount == 100 then
        print("Part 2: ", steps);
        break;
    end
end
