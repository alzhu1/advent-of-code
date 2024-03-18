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

function parseArray(line, currPos, level)
    -- Always assume it opens on a [ or a number
    local arr = {};

    if line:sub(currPos, currPos) ~= "[" then
        return tonumber(line:sub(currPos, currPos)), currPos + 1;
    end

    -- Get the first item
    arr[1], currPos = parseArray(line, currPos + 1, level + 1);

    if line:sub(currPos, currPos) ~= "," then
        print("CurrPos: ", currPos, line:sub(currPos, currPos));
    end

    -- Should be on a comma now, add 1 to currPos and get second item
    arr[2], currPos = parseArray(line, currPos + 1, level + 1);

    arr["level"] = level;

    if type(arr[1]) == "table" then
        arr[1]["parent"] = arr;
    end
    if type(arr[2]) == "table" then
        arr[2]["parent"] = arr;
    end

    return arr, currPos + 1;
end

local numbers = {};

for line in io.lines("input.txt") do
    print(line);
    local arr = parseArray(line, 1, 0);

    numbers[#numbers+1] = arr;
end

function updateNesting(number, levelDelta)
    if type(number) == "table" then
        number["level"] = number["level"] + levelDelta;
        updateNesting(number[1], levelDelta);
        updateNesting(number[2], levelDelta);
    end
end

function addNumbers(number1, number2)
    local arr = {};
    arr[1] = number1;
    arr[2] = number2;
    arr[1]["parent"] = arr;
    arr[2]["parent"] = arr;
    arr["level"] = -1;
    updateNesting(arr, 1);

    -- Handle explosions and splits
    while true do
        local explodingNumber = checkExplosion(arr);
        if explodingNumber ~= nil then
            explodeNumber(explodingNumber);
        else
            if handleSplit(arr, false, nil, 1) == false then
                break;
            end
        end
    end

    return arr;
end

function checkExplosion(number)
    if type(number) ~= "table" then
        return nil;
    end

    if number["level"] == 4 then
        return number;
    end

    return checkExplosion(number[1]) or checkExplosion(number[2]);
end

function explodeNumber(number)
    local nodeTraverse = function(dir)
        local addNode = number["parent"];
        local check = number;
        while addNode ~= nil and addNode[dir] == check do
            addNode = addNode["parent"];
            check = check["parent"];
        end

        if addNode ~= nil then
            local otherDir = dir == 1 and 2 or 1;
            if type(addNode[dir]) ~= "table" then
                addNode[dir] = addNode[dir] + number[dir];
            else
                addNode = addNode[dir];
    
                while type(addNode) == "table" do
                    if type(addNode[otherDir]) ~= "table" then
                        break;
                    end 
                    addNode = addNode[otherDir];
                end
    
                addNode[otherDir] = addNode[otherDir] + number[dir];
            end
        end
    end

    -- For number[1], find the first ancestor left child, rightmost 
    nodeTraverse(1);

    -- For number[2], find the first ancestor right child, leftmost
    nodeTraverse(2);

    local indexFromParent = number["parent"][1] == number and 1 or 2;
    number["parent"][indexFromParent] = 0;
end

function handleSplit(number, split, prevNumber, child)
    if split or number == nil then
        return split;
    end

    if type(number) == "table" then
        split = split or handleSplit(number[1], split, number, 1);
        split = split or handleSplit(number[2], split, number, 2);
        return split;
    end

    if number > 9 then
        local arr = {};

        if number % 2 == 0 then
            local halfNum = number >> 1;
            arr[1] = halfNum;
            arr[2] = halfNum;
        else
            arr[1] = math.floor(number / 2);
            arr[2] = math.ceil(number / 2);
        end
        arr["level"] = prevNumber["level"] + 1;
        arr["parent"] = prevNumber;

        prevNumber[child] = arr;
        return true;
    end
    return false;
end

function getMagnitude(number)
    if type(number) ~= "table" then
        return number;
    end

    return 3 * getMagnitude(number[1]) + 2 * getMagnitude(number[2]);
end

local sum = copy(numbers[1]);
for i = 2, #numbers do
    sum = addNumbers(sum, copy(numbers[i]));
end


print("Part 1: ", getMagnitude(sum));

local largest = 0;
for i = 1, #numbers do
    for j = 1, #numbers do
        if numbers[i] ~= numbers[j] then
            local number1 = copy(numbers[i]);
            local number2 = copy(numbers[j]);
            largest = math.max(largest, getMagnitude(addNumbers(number1, number2)));
        end
    end
end

print("Part 2: ", largest);