local positions = {}

local state = io.open("input.txt", "r"):lines()();
for n in string.gmatch(state, "([^,]+)") do
    positions[#positions+1] = tonumber(n);
end

table.sort(positions);

local median = positions[#positions / 2];
local fuelCost = 0;
for i = 1, #positions do
    local currCost = math.abs(positions[i] - median);

    fuelCost = fuelCost + currCost;
end

print("Part 1: ", fuelCost);

function getP2Derivative(positions, num)
    local value = 0;
    for i = 1, #positions do
        local currPos = positions[i];

        if num > currPos then
            value = value + 2 * (num - currPos) - 1;
        else
            value = value + 2 * (num - currPos) + 1;
        end
    end

    return value / 2;
end

local guessPos = median;
local currDerivative = getP2Derivative(positions, guessPos);
local delta = currDerivative < 0 and 1 or -1;

while true do
    local nextDerivative = getP2Derivative(positions, guessPos + delta);

    -- This means they have different signs
    if delta * nextDerivative > 0 then
        -- I thought it was the closest absolute value, might be wrong?
        -- if math.abs(nextDerivative) < math.abs(currDerivative) then
        --     currDerivative = nextDerivative;
        --     guessPos = guessPos + delta;
        -- end

        break;
    end

    currDerivative = nextDerivative;
    guessPos = guessPos + delta;
end

local fuelCost2 = 0;
for i = 1, #positions do
    local currCost = math.abs(positions[i] - guessPos);

    fuelCost2 = fuelCost2 + (currCost * (currCost + 1));
end

print("Part 2: ", fuelCost2 / 2);