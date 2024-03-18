package.path = package.path .. ';../?.lua';
local dbg = require("../debugger");

local boxes = {};

for line in io.lines("input2.txt") do
    local instructionIter = line:gmatch("([^%s]+)");
    local instruction, rest = instructionIter(), instructionIter();

    local coords = {};
    for r in rest:gmatch("([^,]+)") do
        local firstPeriod = r:find(".", 1, true);

        coords[#coords+1] = {};
        local c1 = tonumber(r:sub(3, firstPeriod - 1));
        local c2 = tonumber(r:sub(firstPeriod + 2));

        coords[#coords][#coords[#coords]+1] = math.min(c1, c2);
        coords[#coords][#coords[#coords]+1] = math.max(c1, c2);
    end

    coords["type"] = instruction;
    boxes[#boxes+1] = coords;
end

function splitBox(finalBox, setBox)
    local nextFinalBoxSet = { finalBox };
    local setMinX, setMaxX = table.unpack(setBox[1]);
    local setMinY, setMaxY = table.unpack(setBox[2]);
    local setMinZ, setMaxZ = table.unpack(setBox[3]);

    local removedCount = 0;

    local iters = 0;

    while iters < #nextFinalBoxSet do
        local nextFinalBox = table.remove(nextFinalBoxSet, 1);

        local boxMinX, boxMaxX = table.unpack(nextFinalBox[1]);
        local boxMinY, boxMaxY = table.unpack(nextFinalBox[2]);
        local boxMinZ, boxMaxZ = table.unpack(nextFinalBox[3]);

        -- Check x
        local xRightSplit = boxMinX <= setMaxX and boxMaxX > setMaxX;
        local xLeftSplit = boxMaxX >= setMinX and boxMinX < setMinX;
        local noXSplit = not (xRightSplit or xLeftSplit);

        -- Check y
        local yRightSplit = boxMinY <= setMaxY and boxMaxY > setMaxY;
        local yLeftSplit = boxMaxY >= setMinY and boxMinY < setMinY;
        local noYSplit = not (yRightSplit or yLeftSplit);

        -- Check z
        local zRightSplit = boxMinZ <= setMaxZ and boxMaxZ > setMaxZ;
        local zLeftSplit = boxMaxZ >= setMinZ and boxMinZ < setMinZ;
        local noZSplit = not (zRightSplit or zLeftSplit);

        -- Check if finalBox is completely a subset
        if boxMinX >= setMinX and boxMaxX <= setMaxX and
           boxMinY >= setMinY and boxMaxY <= setMaxY and
           boxMinZ >= setMinZ and boxMaxZ <= setMaxZ then
            iters = -1;
            local boxCount = (boxMaxX - boxMinX + 1) * (boxMaxY - boxMinY + 1) * (boxMaxZ - boxMinZ + 1);
            removedCount = removedCount + boxCount;
            goto continue;
        end

        -- Splits
        if xRightSplit or xLeftSplit then
            nextFinalBoxSet[#nextFinalBoxSet+1] = {
                { math.max(boxMinX, setMinX), math.min(boxMaxX, setMaxX) },
                { boxMinY, boxMaxY },
                { boxMinZ, boxMaxZ }
            };

            if xRightSplit and boxMaxX > setMaxX then
                nextFinalBoxSet[#nextFinalBoxSet+1] = {
                    { setMaxX + 1, boxMaxX },
                    { boxMinY, boxMaxY },
                    { boxMinZ, boxMaxZ }
                }
            end

            if xLeftSplit and boxMinX < setMinX then
                nextFinalBoxSet[#nextFinalBoxSet+1] = {
                    { boxMinX, setMinX - 1 },
                    { boxMinY, boxMaxY },
                    { boxMinZ, boxMaxZ }
                }
            end

            iters = -1;
            goto continue;
        end

        if yRightSplit or yLeftSplit then
            nextFinalBoxSet[#nextFinalBoxSet+1] = {
                { boxMinX, boxMaxX },
                { math.max(boxMinY, setMinY), math.min(boxMaxY, setMaxY) },
                { boxMinZ, boxMaxZ }
            };

            if yRightSplit and boxMaxY > setMaxY then
                nextFinalBoxSet[#nextFinalBoxSet+1] = {
                    { boxMinX, boxMaxX },
                    { setMaxY + 1, boxMaxY },
                    { boxMinZ, boxMaxZ }
                }
            end

            if yLeftSplit and boxMinY < setMinY then
                nextFinalBoxSet[#nextFinalBoxSet+1] = {
                    { boxMinX, boxMaxX },
                    { boxMinY, setMinY - 1 },
                    { boxMinZ, boxMaxZ }
                }
            end

            iters = -1;
            goto continue;
        end

        if zRightSplit or zLeftSplit then
            nextFinalBoxSet[#nextFinalBoxSet+1] = {
                { boxMinX, boxMaxX },
                { boxMinY, boxMaxY },
                { math.max(boxMinZ, setMinZ), math.min(boxMaxZ, setMaxZ) }
            };

            if zRightSplit and boxMaxZ > setMaxZ then
                nextFinalBoxSet[#nextFinalBoxSet+1] = {
                    { boxMinX, boxMaxX },
                    { boxMinY, boxMaxY },
                    { setMaxZ + 1, boxMaxZ }
                }
            end

            if zLeftSplit and boxMinZ < setMinZ then
                nextFinalBoxSet[#nextFinalBoxSet+1] = {
                    { boxMinX, boxMaxX },
                    { boxMinY, boxMaxY },
                    { boxMinZ, setMinZ - 1 },
                }
            end

            iters = -1;
            goto continue;
        end

        if noXSplit and noYSplit and noZSplit then
            nextFinalBoxSet[#nextFinalBoxSet+1] = nextFinalBox;
        end

        ::continue::
        iters = iters + 1;
    end

    return nextFinalBoxSet, removedCount;
end

local count = 0;

local setBoxes = {};
for i = #boxes, 1, -1 do
    print('Checking box: ', i);
    local currBox = boxes[i];
    local minX, maxX = table.unpack(currBox[1]);
    local minY, maxY = table.unpack(currBox[2]);
    local minZ, maxZ = table.unpack(currBox[3]);

    local onCount = 0;
    if currBox["type"] == "on" then
        onCount = (maxX - minX + 1) * (maxY - minY + 1) * (maxZ - minZ + 1);

        local finalBoxSet = { currBox };
        for j = 1, #setBoxes do
            local currSetBox = setBoxes[j];

            local tempFinalBoxSet = {};
            for _, box in ipairs(finalBoxSet) do
                local tempSet, removed = splitBox(box, currSetBox);

                for _, b in ipairs(tempSet) do
                    tempFinalBoxSet[#tempFinalBoxSet+1] = b;
                end

                onCount = onCount - removed;
                if onCount < 0 then
                    onCount = 0;
                end
            end

            finalBoxSet = tempFinalBoxSet;
        end
    end

    count = count + onCount;
    setBoxes[#setBoxes+1] = currBox;
end

print(count);