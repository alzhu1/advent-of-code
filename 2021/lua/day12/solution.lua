function dfs(graph, currLoc, cache, canVisitTwice, visitedTwice)
    if canVisitTwice then
        if (currLoc == "start" and cache[currLoc] ~= nil) or (visitedTwice and cache[currLoc] ~= nil) then
            return 0;
        end
    elseif cache[currLoc] ~= nil then
        return 0;
    end

    if currLoc == "end" then
        return 1;
    end

    if currLoc:match("%u") == nil then
        if cache[currLoc] == nil then
            cache[currLoc] = 0;
        end
        cache[currLoc] = cache[currLoc] + 1;

        if cache[currLoc] == 2 then
            visitedTwice = true;
        end
    end

    local edges = graph[currLoc];

    local sum = 0;
    for _, node in pairs(edges) do
        local cacheCopy = {};
        for k, v in pairs(cache) do
            cacheCopy[k] = v;
        end
        sum = sum + dfs(graph, node, cacheCopy, canVisitTwice, visitedTwice);
    end

    return sum;
end

local graph = {};
for line in io.lines("input.txt") do
    local lineIter = line:gmatch("([^-]+)");
    local n1, n2 = lineIter(), lineIter();

    if graph[n1] == nil then
        graph[n1] = {};
    end
    if graph[n2] == nil then
        graph[n2] = {};
    end

    graph[n1][#graph[n1]+1] = n2;
    graph[n2][#graph[n2]+1] = n1;
end

print("Part 1: ", dfs(graph, "start", {}, false, false));
print("Part 2: ", dfs(graph, "start", {}, true, false));