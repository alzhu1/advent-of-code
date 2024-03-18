local player1 = {
    ["start"] = 6,
    ["score"] = 0
}

local player2 = {
    ["start"] = 8,
    ["score"] = 0
}

player1["pos"] = player1["start"];
player2["pos"] = player2["start"];

-- Part 1
local increment = 6; -- 1 + 2 + 3

local turn = 0;
while player1["score"] < 1000 and player2["score"] < 1000 do
    local currPlayer = turn % 2 == 0 and player1 or player2;

    currPlayer["pos"] = (currPlayer["pos"] + increment) % 10;
    local scoreGain = currPlayer["pos"] == 0 and 10 or currPlayer["pos"];
    currPlayer["score"] = currPlayer["score"] + scoreGain;

    increment = (increment - 1) % 10;
    turn = turn + 1;
end

local loser = player1["score"] >= 1000 and player2 or player1;
print("Part 1: ", loser["score"] * turn * 3);

-- Part 2, recursive
local goal = 21;
local rollCounts = {
    [3] = 1,
    [4] = 3,
    [5] = 6,
    [6] = 7,
    [7] = 6,
    [8] = 3,
    [9] = 1
};

local cache = {};
function playGame(p1Pos, p1Score, p2Pos, p2Score, isPlayer1)
    if p1Score >= goal then
        return 1, 0;
    end

    if p2Score >= goal then
        return 0, 1;
    end

    local cacheKey = p1Pos .. "," .. p1Score .. "," .. p2Pos .. "," .. p2Score .. "," .. (isPlayer1 and 1 or 0);
    if cache[cacheKey] ~= nil then
        return cache[cacheKey][1], cache[cacheKey][2];
    end

    local p1Wins, p2Wins = 0, 0;

    -- Universe of rolling 1
    for i, freq in pairs(rollCounts) do
        local universeP1, universeP2 = 0, 0;
        if isPlayer1 then
            local p1NewPos = (p1Pos + i) % 10;
            local p1NewScore = p1Score + (p1NewPos == 0 and 10 or p1NewPos);
            universeP1, universeP2 = playGame(p1NewPos, p1NewScore, p2Pos, p2Score, not isPlayer1);
        else
            local p2NewPos = (p2Pos + i) % 10;
            local p2NewScore = p2Score + (p2NewPos == 0 and 10 or p2NewPos);
            universeP1, universeP2 = playGame(p1Pos, p1Score, p2NewPos, p2NewScore, not isPlayer1);
        end

        p1Wins = p1Wins + (universeP1 * freq);
        p2Wins = p2Wins + (universeP2 * freq);
    end

    cache[cacheKey] = { p1Wins, p2Wins };

    return p1Wins, p2Wins;
end

print("Part 2: ", playGame(player1["start"], 0, player2["start"], 0, 0));