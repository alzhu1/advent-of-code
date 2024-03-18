local commons = {};
local lines = {};
local index = 1;

for line in io.lines("input.txt") do
    lines[index] = line;
    index = index + 1;

    for i = 1, #line do
        if commons[i] == nil then
            commons[i] = {}
        end

        local freq = commons[i][line:sub(i, i)] or 0;
        freq = freq + 1;
        commons[i][line:sub(i, i)] = freq;
    end
end

local gamma = "";
local epsilon = "";

for i = 1, #commons do
    local common = commons[i];

    local mostCommon = (common["0"] > common["1"]) and "0" or "1"
    local leastCommon = (common["0"] < common["1"]) and "0" or "1"

    gamma = gamma .. mostCommon;
    epsilon = epsilon .. leastCommon;
end

local gammaNum = tonumber(gamma, 2);
local epsilonNum = tonumber(epsilon, 2);

print(string.format("Part 1: %d * %d = %d", gammaNum, epsilonNum, gammaNum * epsilonNum));

-- Part 2
function getRating(moreCommon)
    local linesCopy = {};

    for i = 1, #lines do
        linesCopy[i] = lines[i];
    end

    local index = 1;
    while #linesCopy > 1 do
        local commons = {};
        local zeroStart = {};
        local oneStart = {};

        local choice = {}
        choice["0"] = zeroStart;
        choice["1"] = oneStart;

        for _, line in ipairs(linesCopy) do
            for i = 1, #line do
                if i == index then
                    -- Add to zeroStart/oneStart
                    local char = line:sub(i, i);

                    if char == "0" then
                        zeroStart[#zeroStart+1] = line;
                    else
                        oneStart[#oneStart+1] = line;
                    end
                end

                if commons[i] == nil then
                    commons[i] = {}
                end
        
                local freq = commons[i][line:sub(i, i)] or 0;
                freq = freq + 1;
                commons[i][line:sub(i, i)] = freq;
            end
        end

        local commonForIndex = commons[index];
        local mostCommon = (commonForIndex["0"] <= commonForIndex["1"]) and "1" or "0"
        local leastCommon = (commonForIndex["0"] <= commonForIndex["1"]) and "0" or "1"

        -- Set linesCopy based on moreCommon
        linesCopy = moreCommon and choice[mostCommon] or choice[leastCommon]

        index = index + 1;
    end

    return linesCopy[1];
end

local oxygenRating = tonumber(getRating(true), 2);
local co2Rating = tonumber(getRating(false), 2);

print(string.format("Part 2: %d * %d = %d", oxygenRating, co2Rating, oxygenRating * co2Rating));