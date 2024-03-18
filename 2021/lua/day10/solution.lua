local openBracketsEnd = {
    ["("] = ")",
    ["["] = "]",
    ["{"] = "}",
    ["<"] = ">"
};
local openBracketsScore = {
    ["("] = 1,
    ["["] = 2,
    ["{"] = 3,
    ["<"] = 4
}
local closeBracketsScore = {
    [")"] = 3,
    ["]"] = 57,
    ["}"] = 1197,
    [">"] = 25137
};

local errorScore = 0;
local autocompleteScores = {};
for line in io.lines("input.txt") do
    local stack = {};
    local valid = true;

    for c in line:gmatch(".") do
        if openBracketsEnd[c] ~= nil then
            stack[#stack+1] = c;
        else
            local stackTop = stack[#stack];
            stack[#stack] = nil;

            if c ~= openBracketsEnd[stackTop] then
                valid = false;
                errorScore = errorScore + closeBracketsScore[c];
                break;
            end
        end
    end

    if valid then
        local tempScore = 0;
        for i = #stack, 1, -1 do
            tempScore = (tempScore * 5) + openBracketsScore[stack[i]];
        end

        autocompleteScores[#autocompleteScores+1] = tempScore;
    end
end

table.sort(autocompleteScores);

print("Part 1: ", errorScore);
print("Part 2: ", autocompleteScores[math.ceil(#autocompleteScores / 2)]);