local line = io.lines("input.txt")();
local lineIter = line:gmatch("([^:]+)");
lineIter();
local eqs = lineIter();

local eqIter = eqs:gmatch("([^,]+)");
local xLine = eqIter():gsub("%s+", "");
local yLine = eqIter():gsub("%s+", "");

local xDot = xLine:find(".", 1, true);
local yDot = yLine:find(".", 1, true);

local x1, x2 = tonumber(xLine:sub(xLine:find("=") + 1, xDot - 1)), tonumber(xLine:sub(xDot + 2));
local y1, y2 = tonumber(yLine:sub(yLine:find("=") + 1, yDot - 1)), tonumber(yLine:sub(yDot + 2));

-- Technically only works for negative y
local yUpperBound = math.abs(math.min(y1, y2)) - 1;
print("Part 1: ", (yUpperBound * (yUpperBound + 1)) >> 1);

-- Part 2
local xUpperBound = math.max(x1, x2);
local xLowerBound = 1;
while ((xLowerBound * (xLowerBound + 1)) >> 1) < math.min(x1, x2) do
    xLowerBound = xLowerBound + 1;
end

local yLowerBound = math.min(y1, y2);

local boxCount = 0;
for x = xLowerBound, xUpperBound do
    for y = yLowerBound, yUpperBound do
        -- Simulate
        local xVelo, yVelo = x, y;
        local currX, currY = 0, 0;

        while currX <= math.max(x1, x2) and currY >= math.min(y1, y2) do
            currX = currX + xVelo;
            currY = currY + yVelo;

            xVelo = xVelo == 0 and 0 or xVelo - 1;
            yVelo = yVelo - 1;

            -- If in bounding box, break
            if currX >= math.min(x1, x2) and currX <= math.max(x1, x2) and currY >= math.min(y1, y2) and currY <= math.max(y1, y2) then
                boxCount = boxCount + 1;
                break;
            end
        end
    end
end

print("Part 2: ", boxCount);