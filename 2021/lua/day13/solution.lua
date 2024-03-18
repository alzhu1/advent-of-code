local points = {}

local lineIter = io.lines("input.txt");
local line = "";
while true do
    line = lineIter();

    if line:len() == 0 then
        break;
    end

    points[#points+1] = line;
    -- Cache line
    points[line] = 1;
end

-- Perform folds
local firstFold = true;
local maxX, maxY = 0, 0;
while true do
    line = lineIter();

    if line == nil then
        break;
    end

    maxX = 0;
    maxY = 0;

    local foldIter = line:gmatch("([^%s]+)");
    local _, _, foldEq = foldIter(), foldIter(), foldIter();

    local lineOrientation, lineValue = foldEq:sub(1, 1), tonumber(foldEq:sub(3));

    local newPoints = {};
    for i = 1, #points do
        local pointIter = points[i]:gmatch("([^,]+)");
        local x, y = tonumber(pointIter()), tonumber(pointIter());

        local newPoint = "";
        if lineOrientation == "y" then
            -- Fold bottom up: if smaller than value then keep it, greater and map it
            if y < lineValue then
                newPoint = points[i];
            else
                y = y - 2 * (y - lineValue);
                newPoint = x .. "," .. y;
            end
        else
            -- Fold right to left: if smaller than value then keep it, greater and map it
            if x < lineValue then
                newPoint = points[i]
            else
                x = x - 2 * (x - lineValue);
                newPoint = x .. "," .. y;
            end
        end

        if newPoints[newPoint] == nil then
            newPoints[#newPoints+1] = newPoint;
            newPoints[newPoint] = 1;

            maxX = math.max(maxX, x);
            maxY = math.max(maxY, y);
        end
    end

    points = newPoints;

    if firstFold then
        print("Part 1: ", #points);
        firstFold = false;
    end
end

print("Part 2:");
for y = 0, maxY do
    local row = "";
    for x = 0, maxX do
        local point = x .. "," .. y;
        if points[point] == nil then
            row = row .. ".";
        else
            row = row .. "#";
        end
    end

    print(row);
end