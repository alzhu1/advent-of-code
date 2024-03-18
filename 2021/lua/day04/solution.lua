local file = io.open("input.txt", "r");

local lineIter = file:lines();

local order = lineIter();
local bingoBoards = {};
local bingoIndex = 0;

local nextLine = lineIter();
local rowIndex = 1;

while nextLine ~= nil do
    if #nextLine == 0 then
        bingoIndex = bingoIndex + 1;
        rowIndex = 1;

        bingoBoards[bingoIndex] = {};
    else
        bingoBoards[bingoIndex][rowIndex] = {};
        local colIndex = 1;
        for num in string.gmatch(nextLine, "[^%s]+") do
            bingoBoards[bingoIndex][rowIndex][colIndex] = tonumber(num);
            colIndex = colIndex + 1;
        end

        rowIndex = rowIndex + 1;
    end

    nextLine = lineIter();
end

function checkForWinner(bingoBoard)
    -- Horizontal/Vertical
    for i = 1, 5 do
        local row = {};
        local col = {};
        for j = 1, 5 do
            row[j] = bingoBoard[i][j];
            col[j] = bingoBoard[j][i];
        end

        local win = true;
        local win2 = true;
        for k = 1, 5 do
            if row[k] ~= -1 then
                win = false;
            end

            if col[k] ~= -1 then
                win2 = false;
            end
        end

        if win or win2 then
            return true;
        end
    end

    return false;
end

function markBoards(bingoBoards, markNum)
    for i = 1, #bingoBoards do
        for j = 1, 5 do
            for k = 1, 5 do
                if bingoBoards[i][j][k] == markNum then
                    bingoBoards[i][j][k] = -1;
                end
            end
        end
    end
end

function printScore(bingoBoard, lastCall)
    local sum = 0;
    for i = 1, 5 do
        for j = 1, 5 do
            local num = bingoBoard[i][j];
            local num2 = bingoBoard[i][j];
    
            if num > -1 then
                sum = sum + num;
            end
        end
    end

    print("Score: ", sum * lastCall);
end

local winCheck = {};
local winCount = 0;
for num in string.gmatch(order, "[^,]+") do
    markBoards(bingoBoards, tonumber(num));

    for i = 1, #bingoBoards do
        local didWin = checkForWinner(bingoBoards[i]);

        if didWin and winCheck[i] == nil then
            winCheck[i] = true;
            winCount = winCount + 1;

            if winCount == 1 then
                printScore(bingoBoards[i], tonumber(num));
            end

            if winCount == #bingoBoards then
                printScore(bingoBoards[i], tonumber(num));
            end
        end
    end
end