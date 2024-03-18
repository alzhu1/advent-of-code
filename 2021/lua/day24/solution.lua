package.path = package.path .. ';../?.lua';
local dbg = require("../debugger");

-- Clues
-- The idea is kind of like you're "undoing" operations
-- Operation is undone if the following criteria is met (w_b = bth digit)

-- w4 == w3
-- w6 == w5 + 2
-- w7 == w2 + 6
-- w11 == w10 - 3
-- w12 == w9 + 7
-- w13 == w8 - 8
-- w14 == w1 - 7

-- Largest:  93997999296912
-- Smallest: 81111379141811

function simulate(modelNumber)
    local vars = {
        ['w'] = 0,
        ['x'] = 0,
        ['y'] = 0,
        ['z'] = 0
    };

    local index = 1;

    for line in io.lines('input.txt') do
        local instruction = line:sub(1, 3);

        if instruction == 'inp' then
            vars[line:sub(line:len())] = tonumber(modelNumber:sub(index, index));
            index = index + 1;
        else
            local writeTo = line:sub(5, 5);
            local operand = line:sub(7);

            if tonumber(operand) == nil then
                operand = vars[operand];
            else
                operand = tonumber(operand);
            end

            if instruction == 'add' then
                vars[writeTo] = vars[writeTo] + operand;
            end
            if instruction == 'mul' then
                vars[writeTo] = vars[writeTo] * operand;
            end
            if instruction == 'div' then
                vars[writeTo] = vars[writeTo] // operand;
            end
            if instruction == 'mod' then
                vars[writeTo] = vars[writeTo] % operand;
            end
            if instruction == 'eql' then
                vars[writeTo] = vars[writeTo] == operand and 1 or 0;
            end
        end
    end

    return vars['z'];
end

print('Part 1: ', 93997999296912, simulate('93997999296912'));
print('Part 2: ', 81111379141811, simulate('81111379141811'));