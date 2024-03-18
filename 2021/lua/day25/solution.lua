package.path = package.path .. ';../?.lua';
local dbg = require("../debugger");

local grid = {};

local east = {};
local south = {};

local rowSize, colSize = 0, 0;

for line in io.lines('input.txt') do
    grid[#grid+1] = {};
    east[#east+1] = {};

    for cc = 1, line:len() do
        if south[cc] == nil then
            south[cc] = {};
        end
        local c = line:sub(cc, cc);

        grid[#grid][#grid[#grid]+1] = c;

        if c == '>' then
            east[#east][cc] = c;
        end

        if c == 'v' then
            south[cc][#east] = c;
        end
    end

    rowSize = rowSize + 1;
    colSize = line:len();
end

function printGrid(grid)
    for r = 1, #grid do
        local s = '';
        for c = 1, #grid[r] do
            s = s .. grid[r][c];
        end
        print(s);
    end
end

function printGridES(east, south)
    for r = 1, rowSize do
        local s = '';
        for c = 1, colSize do
            if east[r][c] ~= nil then
                s = s .. east[r][c]
            elseif south[c][r] ~= nil then
                s = s .. south[c][r];
            else
                s = s .. '.';
            end
        end

        print(s);
    end
end

-- printGridES(east, south);

local moved = true;
local steps = 0;
while moved do
    moved = false;

    -- Handle east first
    -- local r, c = 1, 1;
    -- for r = 1, rowSize do
    --     local currRow = east[r];

    --     local c = 1;
    --     local first = currRow[1] or south[1][r];
    --     while c <= colSize do

    --         if currRow[c] ~= nil then
    --             local next = c + 1 > colSize and 1 or c + 1;
    --             local item = currRow[next] or south[next][r];

    --             if (next == 1 and first == nil) or (next > 1 and item == nil) then
    --                 currRow[c] = nil;
    --                 currRow[next] = '>';
    --                 c = c + 1;
    --                 moved = true;
    --             end
    --         end

    --         c = c + 1;
    --     end
    -- end

    -- for c = 1, colSize do
    --     local currCol = south[c];

    --     local r = 1;
    --     local first = currCol[1] or east[1][c];
    --     while r <= rowSize do

    --         if currCol[r] ~= nil then
    --             local next = r + 1 > rowSize and 1 or r + 1;
    --             local item = currCol[next] or east[next][c];

    --             if (next == 1 and first == nil) or (next > 1 and item == nil) then
    --                 currCol[r] = nil;
    --                 currCol[next] = 'v';
    --                 r = r + 1;
    --                 moved = true;
    --             end
    --         end

    --         r = r + 1;
    --     end
    -- end

    -- Handle east first
    for r = 1, rowSize do
        local c = 1;
        local first = grid[r][1];
        while c <= colSize do
            local next = c + 1 > colSize and 1 or c + 1;
            if grid[r][c] == '>' then
                local shouldMove = (next == 1 and first == '.') or (next > 1 and grid[r][next] == '.');
                if shouldMove then
                    grid[r][next] = '>';
                    grid[r][c] = '.';
                    c = c + 1;
                    moved = true;
                end
            end

            c = c + 1;
        end
    end

    for c = 1, colSize do
        local r = 1;
        local first = grid[r][c];
        while r <= rowSize do
            local next = r + 1 > rowSize and 1 or r + 1;
            if grid[r][c] == 'v' then
                local shouldMove = (next == 1 and first == '.') or (next > 1 and grid[next][c] == '.');
                if shouldMove then
                    grid[next][c] = 'v';
                    grid[r][c] = '.';
                    r = r + 1;
                    moved = true;
                end
            end

            r = r + 1;
        end
    end

    steps = steps + 1;
end

print(steps);