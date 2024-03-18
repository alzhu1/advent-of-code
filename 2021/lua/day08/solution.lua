TOP = 1;
MIDDLE = 2;
BOTTOM = 3;
TOPLEFT = 4;
TOPRIGHT = 5;
BOTTOMLEFT = 6;
BOTTOMRIGHT = 7;

local count = 0;
local sum = 0;

function getNumberFromWires(wireMap, sequence)
    if sequence:len() == 2 then
        return 1;
    end

    if sequence:len() == 4 then
        return 4;
    end

    if sequence:len() == 3 then
        return 7;
    end

    if sequence:len() == 7 then
        return 8;
    end

    -- For the rest, check sequences
    local sequenceMap = {};
    for c in sequence:gmatch(".") do
        sequenceMap[c] = 1;
    end

    -- 0 if middle is missing
    -- 6 if length 6 and topright missing
    -- 9 if length 6 and topleft missing
    if sequence:len() == 6 then
        if sequenceMap[wireMap[MIDDLE]] == nil then
            return 0;
        elseif sequenceMap[wireMap[TOPRIGHT]] == nil then
            return 6;
        elseif sequenceMap[wireMap[BOTTOMLEFT]] == nil then
            return 9;
        end
    end

    -- 5 if topleft is present
    -- Otherwise, 2 if bottomright present, else 3
    if sequence:len() == 5 then
        if sequenceMap[wireMap[TOPLEFT]] == 1 then
            return 5;
        elseif sequenceMap[wireMap[BOTTOMLEFT]] == 1 then
            return 2;
        else
            return 3;
        end
    end
end

for line in io.lines("input.txt") do
    local lineIter = string.gmatch(line, "([^|]+)");
    local inputs = lineIter();

    local inputStrings = {};
    local numberStrings = {};
    for i in string.gmatch(inputs, "([^%s]+)") do
        inputStrings[#inputStrings+1] = i;
        
        if i:len() == 2 then
            numberStrings[1] = i;
        end

        if i:len() == 4 then
            numberStrings[4] = i;
        end

        if i:len() == 3 then
            numberStrings[7] = i;
        end

        if i:len() == 7 then
            numberStrings[8] = i;
        end
    end

    local wireMap = {};

    -- Strategy:
    local temp = {};

    -- Top wire: wires(7) = wires(1) + top wire
    for c in numberStrings[7]:gmatch(".") do
        temp[c] = 1;
    end
    for c in numberStrings[1]:gmatch(".") do
        temp[c] = nil;
    end
    for wire in pairs(temp) do
        wireMap[wire] = TOP;
        wireMap[TOP] = wire;
    end

    -- Bottom wire: wires(9) (len 6 string) = wires(4) + top wire + bottom wire
    for i = 1, #inputStrings do
        temp = {};
        
        if inputStrings[i]:len() == 6 then
            for c in inputStrings[i]:gmatch(".") do
                temp[c] = 1;
            end

            for c in numberStrings[4]:gmatch(".") do
                temp[c] = nil;
            end
            temp[wireMap[TOP]] = nil;

            local liveCount = 0;
            local wire = "";
            for a in pairs(temp) do
                wire = a;
                liveCount = liveCount + 1;
            end

            if liveCount == 1 then
                wireMap[wire] = BOTTOM;
                wireMap[BOTTOM] = wire;

                -- Will use this later
                numberStrings[9] = inputStrings[i];
                break;
            end
        end
    end

    -- Middle wire: wires(3) (len 5 string) = wires(7) + bottom + middle
    for i = 1, #inputStrings do
        temp = {};
        
        if inputStrings[i]:len() == 5 then
            for c in inputStrings[i]:gmatch(".") do
                temp[c] = 1;
            end

            for c in numberStrings[7]:gmatch(".") do
                temp[c] = nil;
            end
            temp[wireMap[BOTTOM]] = nil;

            local liveCount = 0;
            local wire = "";
            for a in pairs(temp) do
                wire = a;
                liveCount = liveCount + 1;
            end

            if liveCount == 1 then
                wireMap[wire] = MIDDLE;
                wireMap[MIDDLE] = wire;

                -- Will use this later
                numberStrings[3] = inputStrings[i];
                break;
            end
        end
    end

    -- Topleft wire: wires(4) = wires(1) + middle wire + topleft wire
    temp = {};
    for c in numberStrings[4]:gmatch(".") do
        temp[c] = 1;
    end
    for c in numberStrings[1]:gmatch(".") do
        temp[c] = nil;
    end
    temp[wireMap[MIDDLE]] = nil;
    for wire in pairs(temp) do
        wireMap[wire] = TOPLEFT;
        wireMap[TOPLEFT] = wire;
    end

    -- Bottomright wire: find the len 5 string that does not contain a common letter
    temp = {}
    for i = 1, #inputStrings do
        for c in inputStrings[i]:gmatch(".") do
            temp[c] = temp[c] == nil and 1 or temp[c] + 1;
        end
    end
    for a, b in pairs(temp) do
        -- BR wire appears in all but 1 number (so check for 10 - 1)
        if b == 9 then
            wireMap[BOTTOMRIGHT] = a;
            wireMap[a] = BOTTOMRIGHT;
            break;
        end
    end

    -- Topright wire: wires(1) = bottomright + topright
    temp = {};
    for c in numberStrings[1]:gmatch(".") do
        temp[c] = 1;
    end
    temp[wireMap[BOTTOMRIGHT]] = nil;
    for wire in pairs(temp) do
        wireMap[wire] = TOPRIGHT;
        wireMap[TOPRIGHT] = wire;
    end

    -- Bottomleft wire: wires(8) = wires(9) + bottomleft
    temp = {};
    for c in numberStrings[8]:gmatch(".") do
        temp[c] = 1;
    end
    for c in numberStrings[9]:gmatch(".") do
        temp[c] = nil;
    end
    for wire in pairs(temp) do
        wireMap[wire] = BOTTOMLEFT;
        wireMap[BOTTOMLEFT] = wire;
    end

    local outputs = lineIter();

    local tempSum = 0;
    for o in string.gmatch(outputs, "([^%s]+)") do
        tempSum = (tempSum + getNumberFromWires(wireMap, o)) * 10;
        if o:len() ~= 5 and o:len() ~= 6 then
            count = count + 1;
        end
    end

    tempSum = tempSum / 10;
    sum = sum + tempSum;
end

print("Part 1: ", count);
print("Part 2: ", sum);