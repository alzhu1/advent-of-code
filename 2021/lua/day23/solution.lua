package.path = package.path .. ';../?.lua';
local dbg = require("../debugger");
local PriorityQueue = require("PriorityQueue");

local state = {
    -- Hallways
    '@',
    '@',
    '@',
    '@',
    '@',
    '@',
    '@',

    -- Rooms (Ordered from left to right, top to bottom)
    { 'A', 'C' },
    { 'D', 'C' },
    { 'A', 'D' },
    { 'B', 'B' }
};

function getHallwayToOutsideRoomSteps(state, h, topRoomIndex)
    local outsideIndex = (topRoomIndex * 2 - 11) / 2;
    local steps = 0;

    if h < outsideIndex then
        while h < math.floor(outsideIndex) do
            steps = steps + (h == 1 and 1 or 2);
            h = h + 1;

            if state[h] ~= '@' then
                return -1;
            end
        end
    else
        while h > math.ceil(outsideIndex) do
            steps = steps + (h == 7 and 1 or 2);
            h = h - 1;

            if state[h] ~= '@' then
                return -1;
            end
        end
    end

    return steps + 1;
end

function getOutsideRoomToHallwaySteps(state, topRoomIndex)
    local outsideIndex = (topRoomIndex * 2 - 11) / 2;
    local stepMap = {};
    for i = 1, 7 do
        stepMap[i] = -1;
    end

    local leftHallway, rightHallway = math.floor(outsideIndex), math.ceil(outsideIndex);
    local leftSteps, rightSteps = 1, 1;

    while leftHallway > 0 do
        if state[leftHallway] ~= '@' then
            break;
        end

        stepMap[leftHallway] = leftSteps;
        leftSteps = leftSteps + (leftHallway == 2 and 1 or 2);
        leftHallway = leftHallway - 1;
    end

    while rightHallway <= 7 do
        if state[rightHallway] ~= '@' then
            break;
        end

        stepMap[rightHallway] = rightSteps;
        rightSteps = rightSteps + (rightHallway == 6 and 1 or 2);
        rightHallway = rightHallway + 1;
    end

    return stepMap;
end

-- Reddit optimization (found after initial solve)
-- A base 5 number to count all possible states fits within 64 bits
function getStateFromKey(stateKey, roomSize)
    local state = {};

    -- Work backwards
    for r = 11, 8, -1 do
        local room = {};
        for rr = roomSize, 1, -1 do
            local value = stateKey % 5;
            stateKey = (stateKey - value) // 5;

            room[rr] = string.char(string.byte('@') + value);
        end

        state[r] = room;
    end

    for h = 7, 1, -1 do
        local value = stateKey % 5;
        stateKey = (stateKey - value) // 5;
        state[h] = string.char(string.byte('@') + value);
    end

    return state;
end

function getKeyFromState(state, roomSize)
    local stateKey = 0;
    for h = 1, 7 do
        stateKey = (stateKey + (string.byte(state[h]) - string.byte('@'))) * 5;
    end

    for r = 8, 11 do
        local room = state[r];
        for rr = 1, roomSize do
            stateKey = stateKey + (string.byte(room[rr]) - string.byte('@'));
            if r ~= 11 or rr ~= roomSize then
                stateKey = stateKey * 5;
            end
        end
    end

    return stateKey;
end

local cache = {};

-- Dijkstra's
function search(initState, roomSize)
    local finalState = {};
    for h = 1, 7 do
        finalState[h] = '@';
    end
    for r = 8, 11 do
        finalState[r] = {};
        for rr = 1, roomSize do
            finalState[r][rr] = string.char(string.byte('A') - 8 + r);
        end
    end
    local finalStateKey = getKeyFromState(finalState, roomSize);

    local pQueue = PriorityQueue:new();
    pQueue:enqueue(getKeyFromState(initState, roomSize), 0);

    while pQueue:len() > 0 do
        local currStateKey, currEnergy = pQueue:dequeue();

        if currStateKey == finalStateKey then
            return currEnergy;
        end

        if cache[currStateKey] == nil then
            cache[currStateKey] = 1;

            local currState = getStateFromKey(currStateKey, roomSize);

            for h = 1, 7 do
                local hallway = currState[h];
                if hallway ~= '@' then
                    local hallwayIndex = string.byte(hallway) - string.byte('A');
                    local topRoomIndex = hallwayIndex + 8;
                    local stepsToOutside = getHallwayToOutsideRoomSteps(currState, h, topRoomIndex);

                    local isSettled = true;
                    for r = 1, roomSize do
                        local room = currState[topRoomIndex];
                        if room[r] ~= '@' and room[r] ~= hallway then
                            isSettled = false;
                            break;
                        end
                    end

                    if stepsToOutside > 0 and isSettled then
                        local stepCost = 10 ^ hallwayIndex;
                        for r = 1, roomSize do
                            if currState[topRoomIndex][r] ~= '@' then
                                break;
                            end

                            stepsToOutside = stepsToOutside + 1;

                            currState[topRoomIndex][r] = hallway;
                            currState[h] = '@';

                            pQueue:enqueue(getKeyFromState(currState, roomSize), currEnergy + (stepCost * stepsToOutside));

                            currState[topRoomIndex][r] = '@';
                            currState[h] = hallway;
                        end
                    end
                end
            end

            for rIndex = 8, 11 do
                local room = currState[rIndex];

                for rCurr = 1, roomSize do
                    local isAboveOpen = true;
                    for i = rCurr - 1, 1, -1 do
                        if room[i] ~= '@' then
                            isAboveOpen = false;
                            break;
                        end
                    end

                    if room[rCurr] ~= '@' and isAboveOpen then
                        local isSettled = true;
                        for i = rCurr, roomSize do
                            local inRoomValue = string.byte(room[i]) - string.byte('A') + 8;
                            if inRoomValue ~= rIndex then
                                isSettled = false;
                                break;
                            end
                        end

                        if not isSettled then
                            local stepMap = getOutsideRoomToHallwaySteps(currState, rIndex);

                            for h, steps in ipairs(stepMap) do
                                if steps > 0 then
                                    local stepsFromTop = steps + rCurr;

                                    local costIndex = string.byte(room[rCurr]) - string.byte('A');
                                    local stepCost = 10 ^ costIndex;
                                    local swapValue = currState[rIndex][rCurr];

                                    currState[h] = swapValue;
                                    currState[rIndex][rCurr] = '@';
                                    pQueue:enqueue(getKeyFromState(currState, roomSize), currEnergy + (stepCost * stepsFromTop));
                                    currState[h] = '@';
                                    currState[rIndex][rCurr] = swapValue;
                                end
                            end
                        end
                    end
                end
            end
        end
    end
end

local roomSize = #state[8];
print('Part 1: ', search(state, roomSize));

local part2Middle = {
    { 'D', 'D' },
    { 'C', 'B' },
    { 'B', 'A' },
    { 'A', 'C' }
};

-- Enable for part 2, disable for part 1
for i = 1, 4 do
    local p2MiddleRoom = part2Middle[i];
    local room = state[i + 7];
    room[4] = room[2];
    room[2] = p2MiddleRoom[1];
    room[3] = p2MiddleRoom[2];
end

print('Part 2: ', search(state, roomSize + 2));

-- DFS search (exhaustive)

-- function simulate(currState, currEnergy, cache)
--     local currStateKey = getKeyFromState2(currState);

--     if currEnergy >= 100000 then
--         return;
--     end

--     if cache[finalStateKey] ~= nil and cache[finalStateKey] <= currEnergy then
--         return;
--     end

--     if cache[currStateKey] ~= nil and cache[currStateKey] <= currEnergy then
--         return;
--     end

--     -- if cache[currStateKey] == nil then
--         cache[currStateKey] = currEnergy;

--         if currStateKey == finalStateKey or currStateKey == finalStateKey2 then
--             print('Found it: ', currEnergy);
--             return;
--         end

--         for h = 1, 7 do
--             local hallway = currState[h];
--             if hallway ~= '@' then
--                 local hallwayIndex = string.byte(hallway) - string.byte('A');
--                 local topRoomIndex = hallwayIndex + 8;
--                 local stepsToOutside = getHallwayToOutsideRoomSteps(currState, h, topRoomIndex);

--                 local isSettled = true;
--                 for r = 1, roomSize do
--                     local room = currState[topRoomIndex];
--                     if room[r] ~= '@' and room[r] ~= hallway then
--                         isSettled = false;
--                         break;
--                     end
--                 end

--                 if stepsToOutside > 0 and isSettled then
--                     local stepCost = 10 ^ hallwayIndex;
--                     for r = 1, roomSize do
--                         if currState[topRoomIndex][r] ~= '@' then
--                             break;
--                         end

--                         stepsToOutside = stepsToOutside + 1;

--                         currState[topRoomIndex][r] = hallway;
--                         currState[h] = '@';

--                         -- pQueue:enqueue(getKeyFromState(currState), currEnergy + (stepCost * stepsToOutside));
--                         simulate(currState, currEnergy + (stepCost * stepsToOutside), cache);

--                         currState[topRoomIndex][r] = '@';
--                         currState[h] = hallway;
--                     end
--                 end
--             end
--         end

--         for rIndex = 8, 11 do
--             local room = currState[rIndex];

--             for rCurr = 1, roomSize do
--                 local isAboveOpen = true;
--                 for i = rCurr - 1, 1, -1 do
--                     if room[i] ~= '@' then
--                         isAboveOpen = false;
--                         break;
--                     end
--                 end

--                 if room[rCurr] ~= '@' and isAboveOpen then
--                     local isSettled = true;
--                     for i = rCurr, roomSize do
--                         local inRoomValue = string.byte(room[i]) - string.byte('A') + 8;
--                         if inRoomValue ~= rIndex then
--                             isSettled = false;
--                             break;
--                         end
--                     end

--                     if not isSettled then
--                         local stepMap = getOutsideRoomToHallwaySteps(currState, rIndex);

--                         for h, steps in ipairs(stepMap) do
--                             if steps > 0 then
--                                 local stepsFromTop = steps + rCurr;

--                                 local costIndex = string.byte(room[rCurr]) - string.byte('A');
--                                 local stepCost = 10 ^ costIndex;
--                                 local swapValue = currState[rIndex][rCurr];

--                                 currState[h] = swapValue;
--                                 currState[rIndex][rCurr] = '@';
--                                 -- pQueue:enqueue(getKeyFromState(currState), currEnergy + (stepCost * stepsFromTop));
--                                 simulate(currState, currEnergy + (stepCost * stepsFromTop), cache);

--                                 currState[h] = '@';
--                                 currState[rIndex][rCurr] = swapValue;
--                             end
--                         end
--                     end
--                 end
--             end
--         end
--     -- end
-- end

-- simulate(state, 0, cache);