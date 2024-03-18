local line = io.lines("input.txt")();
local binaryLine = "";

for i = 1, line:len() do
    local num = tonumber(line:sub(i, i), 16);
    local value = "";
    local bit = 8;

    while bit > 0 do
        if num >= bit then
            num = num - bit;
            value = value .. "1";
        else
            value = value .. "0";
        end

        bit = bit >> 1;
    end

    binaryLine = binaryLine .. value;
end

function handlePacketOperation(values, type)
    if type < 4 then
        local result = values[1];
        for i = 2, #values do
            if type == 0 then result = result + values[i];
            elseif type == 1 then result = result * values[i];
            elseif type == 2 then result = math.min(result, values[i]);
            elseif type == 3 then result = math.max(result, values[i]);
            end
        end
        return result;
    end

    if type == 5 then
        return values[1] > values[2] and 1 or 0;
    end

    if type == 6 then
        return values[1] < values[2] and 1 or 0;
    end
    
    if type == 7 then
        return values[1] == values[2] and 1 or 0;
    end

    print("ERROR");
    return -1;
end

function readPacket(binaryLine, currPos, addPadding)
    -- Assumes this is the start of a packet
    local basePos = currPos;
    local version = tonumber(binaryLine:sub(currPos, currPos + 2), 2);
    local type = tonumber(binaryLine:sub(currPos + 3, currPos + 5), 2);

    currPos = currPos + 6;

    if type == 4 then
        -- Literal
        local tempValue = "";
        while true do
            tempValue = tempValue .. binaryLine:sub(currPos + 1, currPos + 4);
            local checkPos = currPos;
            currPos = currPos + 5;

            if binaryLine:sub(checkPos, checkPos) == "0" then
                break;
            end
        end

        local value = tonumber(tempValue, 2);


        if addPadding then
            -- Make sure currPos is a multiple of 4 away from basePos
            currPos = currPos + 4 - ((currPos - basePos) % 4);
        end

        return value, version, currPos - basePos;
    end

    -- Operator packet
    local lengthType = tonumber(binaryLine:sub(currPos, currPos), 2);
    currPos = currPos + 1;

    if lengthType == 0 then
        -- Next 15 bits tells us how long the remaining packets are
        local packetBitLength = tonumber(binaryLine:sub(currPos, currPos + 14), 2)
        currPos = currPos + 15;

        local endPos = currPos + packetBitLength;
        local versionSum = 0;
        local values = {};
        while currPos < endPos do
            local subValue, subVersion, subLength = readPacket(binaryLine, currPos, false);

            currPos = currPos + subLength;

            values[#values+1] = subValue;
            versionSum = versionSum + subVersion;
        end

        return handlePacketOperation(values, type), versionSum + version, currPos - basePos;
    end

    -- Length type 1, next 11 bits tells us number of packets
    local packetCount = tonumber(binaryLine:sub(currPos, currPos + 10), 2)
    currPos = currPos + 11;

    local versionSum = 0;
    local values = {};
    while packetCount > 0 do
        local subValue, subVersion, subLength = readPacket(binaryLine, currPos, false);

        currPos = currPos + subLength;
        packetCount = packetCount - 1;

        values[#values+1] = subValue;
        versionSum = versionSum + subVersion;
    end

    return handlePacketOperation(values, type), versionSum + version, currPos - basePos;
end

local result, versionSum = readPacket(binaryLine, 1, true);

print("Part 1: ", versionSum);
print("Part 2: ", result);