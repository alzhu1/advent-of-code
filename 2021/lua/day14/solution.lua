local rules = {}
local readingTemplate = true;

local templatePairs = {}
local elementCount = {};
for line in io.lines("input.txt") do
    if line:len() == 0 then
        readingTemplate = false;
    elseif readingTemplate then
        for i = 1, line:len() - 1 do
            local pair = line:sub(i, i+1);
            if templatePairs[pair] == nil then
                templatePairs[pair] = 0;
            end
            templatePairs[pair] = templatePairs[pair] + 1;

            if elementCount[pair:sub(1, 1)] == nil then
                elementCount[pair:sub(1, 1)] = 0;
            end
            elementCount[pair:sub(1, 1)] = elementCount[pair:sub(1, 1)] + 1;
        end

        if elementCount[line:sub(line:len())] == nil then
            elementCount[line:sub(line:len())] = 0;
        end
        elementCount[line:sub(line:len())] = elementCount[line:sub(line:len())] + 1;
    else
        local ruleIter = line:gmatch("([^->]+)");
        local base, result = ruleIter():gsub("%s+", ""), ruleIter():gsub("%s+", "");
        rules[base] = result;
    end
end

local steps = 0;
while steps < 40 do
    local nextTemplatePairs = {};

    for pair, count in pairs(templatePairs) do
        if rules[pair] == nil then
            -- Not sure if this ever executes?
            nextTemplatePairs[pair] = count;
        else
            local firstNewPair = pair:sub(1, 1) .. rules[pair];
            local secondNewPair = rules[pair] .. pair:sub(2, 2);

            if nextTemplatePairs[firstNewPair] == nil then
                nextTemplatePairs[firstNewPair] = 0;
            end
            if nextTemplatePairs[secondNewPair] == nil then
                nextTemplatePairs[secondNewPair] = 0;
            end

            nextTemplatePairs[firstNewPair] = nextTemplatePairs[firstNewPair] + count;
            nextTemplatePairs[secondNewPair] = nextTemplatePairs[secondNewPair] + count;

            if elementCount[rules[pair]] == nil then
                elementCount[rules[pair]] = 0;
            end
            elementCount[rules[pair]] = elementCount[rules[pair]] + count;
        end
    end

    templatePairs = nextTemplatePairs;
    steps = steps + 1;


    local minCount, maxCount = nil, 0;
    for k, v in pairs(elementCount) do
        if minCount == nil then
            minCount = v;
        end

        minCount = math.min(minCount, v);
        maxCount = math.max(maxCount, v);
    end

    if steps == 10 then
        print("Part 1: ", maxCount - minCount);
    elseif steps == 40 then
        print("Part 2: ", maxCount - minCount);
    end
end