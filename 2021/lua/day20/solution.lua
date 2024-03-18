local lineIter = io.lines("input.txt");
local alg = lineIter();

local outerPointsFlipped = alg:sub(1, 1) == "#" and alg:sub(alg:len()) == ".";

local grid = {};
for line in lineIter do
    if line:len() > 0 then
        grid[#grid+1] = {};

        for i = 1, line:len() do
            grid[#grid][#grid[#grid]+1] = line:sub(i, i) == "#" and 1 or 0;
        end
    end
end

local iters = 0;
while iters < 50 do
    -- Each iteration should result in a grid size + 2 in all dirs
    local nextGrid = {};
    local nextGridSize = #grid + 2;

    local outsideValue = outerPointsFlipped and iters % 2 or 0;
    local count = 0;

    for r = 1, nextGridSize do
        nextGrid[#nextGrid+1] = {};

        local square = {
            [r - 2] = grid[r - 2] or {},
            [r - 1] = grid[r - 1] or {},
            [r] = grid[r] or {}
        };

        for c = 1, nextGridSize do
            local index = 0;

            local shiftValue = 8;
            for rr = r - 2, r do
                for cc = c - 2, c do
                    index = index + ((square[rr][cc] or outsideValue) << shiftValue);
                    shiftValue = shiftValue - 1;
                end
            end

            local value = alg:sub(index + 1, index + 1) == "#" and 1 or 0;
            nextGrid[#nextGrid][#nextGrid[#nextGrid]+1] = value;
            count = count + value;
        end
    end

    grid = nextGrid;

    iters = iters + 1;

    if iters == 2 then
        print("Part 1: ", count);
    end

    if iters == 50 then
        print("Part 2: ", count);
    end
end