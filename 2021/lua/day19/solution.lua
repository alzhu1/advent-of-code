-- https://stackoverflow.com/questions/640642/how-do-you-copy-a-lua-table-by-value
function copy(obj, seen)
    if type(obj) ~= 'table' then return obj end
    if seen and seen[obj] then return seen[obj] end
    local s = seen or {}
    local res = setmetatable({}, getmetatable(obj))
    s[obj] = res
    for k, v in pairs(obj) do res[copy(k, s)] = copy(v, s) end
    return res
end

local scanners = {};

for line in io.lines("input.txt") do
    if line:len() > 0 then
        if line:sub(1, 2) == "--" then
            scanners[#scanners+1] = {};
        else
            local currScanner = scanners[#scanners];
            currScanner[#currScanner+1] = {};

            for num in string.gmatch(line, "[^,]+") do
                currScanner[#currScanner][#currScanner[#currScanner]+1] = tonumber(num);
            end

            -- For scanner 0 only, at start
            if #scanners == 1 then
                currScanner[line] = 1;
            end
        end
    end
end

-- Assume scanner 0's position and orientation is the origin
scanners[1]["pos"] = { 0, 0, 0 };

function checkOverlappingScanners(baseScanner, checkScanner)
    local checkScannerCopy = copy(checkScanner);

    -- This is technically double the amount of permutations, but I'm lazy
    local permutationCount = 0;
    while permutationCount < 48 do
        for i1, p1 in ipairs(baseScanner) do
            for i2, p2 in ipairs(checkScannerCopy) do
                -- The assumption is that p2 maps to p1, and we check if there are other mapping points

                local delta = {};
                for i = 1, 3 do
                    delta[i] = (p1[i] + baseScanner["pos"][i]) - p2[i];
                end

                local matchCount = 1;
                for ii2, pp2 in ipairs(checkScannerCopy) do
                    if ii2 ~= i2 then
                        local mappedPoint = 
                            pp2[1] + delta[1] - baseScanner["pos"][1] .. "," ..
                            pp2[2] + delta[2] - baseScanner["pos"][2] .. "," ..
                            pp2[3] + delta[3] - baseScanner["pos"][3];

                        if baseScanner[mappedPoint] ~= nil then
                            matchCount = matchCount + 1;
                        end
                    end
                end

                if matchCount >= 12 then
                    checkScannerCopy["pos"] = delta;

                    for _, pp2 in ipairs(checkScannerCopy) do
                        local key = pp2[1] .. "," .. pp2[2] .. "," .. pp2[3];
                        checkScannerCopy[key] = 1;
                    end

                    return checkScannerCopy;
                end

            end
        end

        permutationCount = permutationCount + 1;

        for _, p in ipairs(checkScannerCopy) do
            -- z flips every iteration
            p[3] = -p[3];

            -- y flips every 2 iters
            if permutationCount % 2 == 0 then
                p[2] = -p[2];
            end

            -- x flips every 4 iters
            if permutationCount % 4 == 0 then
                p[1] = -p[1];
            end

            -- Swap y and z every 8 iters
            if permutationCount % 8 == 0 then
                local temp = p[2];
                p[2] = p[3];
                p[3] = temp;
            end

            -- And rotate all directions every 16 iters
            if permutationCount % 16 == 0 then
                local temp = p[1];
                p[1] = p[2];
                p[2] = p[3];
                p[3] = temp;
            end
        end
    end

    return nil;
end

local baseIndices = { 1 };
local index = 1;
while #baseIndices < #scanners do
    local baseScanner = scanners[baseIndices[index]];

    for i = 1, #scanners do
        if scanners[i]["pos"] == nil then
            local check = checkOverlappingScanners(baseScanner, scanners[i]);

            if check ~= nil then
                scanners[i] = check;
                baseIndices[#baseIndices+1] = i;
            end
        end
    end

    print("Base indices: ", #baseIndices);

    index = index + 1;
end

local count = {};
for i = 1, #scanners do
    for _, point in ipairs(scanners[i]) do
        local mappedPoint = {};
        for j = 1, 3 do
            mappedPoint[j] = point[j] + scanners[i]["pos"][j];
        end

        local mapKey = mappedPoint[1] .. "," .. mappedPoint[2] .. "," .. mappedPoint[3];
        if count[mapKey] == nil then
            count[#count+1] = mappedPoint;
            count[mapKey] = 1;
        end
    end
end

print("Part 1: ", #count);

local manhattan = 0;
for i = 1, #scanners do
    for j = i + 1, #scanners do
        local pos1 = scanners[i]["pos"];
        local pos2 = scanners[j]["pos"];

        local dist = 0;
        for k = 1, 3 do
            dist = dist + math.abs(pos1[k] - pos2[k]);
        end
        manhattan = math.max(dist, manhattan);
    end
end

print("Part 2: ", manhattan);