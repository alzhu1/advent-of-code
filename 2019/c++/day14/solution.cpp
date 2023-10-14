#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "stdlib.h"

struct Formula {
    std::string target;
    uint64_t quantity;
    std::unordered_map<std::string, uint64_t> ingredients;
};

uint64_t getOre(uint64_t fuelTotal, std::unordered_map<std::string, Formula>& formulas, std::vector<std::string>& order) {
    std::unordered_map<std::string, uint64_t> totals;
    totals["FUEL"] = fuelTotal;
    for (auto target: order) {
        if (target == "ORE") {
            continue;
        }
        Formula f = formulas[target];
        uint64_t numIters = totals[target] / f.quantity;
        if (totals[target] % f.quantity != 0) {
            numIters++;
        }

        for (auto ing: f.ingredients) {
            std::string ingName = ing.first;
            uint64_t ingNum = ing.second;

            if (!totals.count(ingName)) {
                totals[ingName] = 0;
            }
            totals[ingName] += (ingNum * numIters);
        }
    }

    return totals["ORE"];
}

// Topological sort on DAG (target -> ingredients makes a DAG)
void dfsSort(std::string target, std::unordered_map<std::string, Formula>& formulas, std::vector<std::string>& order) {
    Formula formula = formulas[target];

    if (std::count(order.begin(), order.end(), target)) {
        return;
    }

    for (auto t: formula.ingredients) {
        dfsSort(t.first, formulas, order);
    }

    order.insert(order.begin(), target);
}

int main(int argc, char* argv[]) {
    std::string line;
    std::ifstream file("input.txt");

    std::unordered_map<std::string, Formula> formulas;
    if (file.is_open()) {

        while (getline(file, line)) {
            size_t loc = line.find(" => ");
            std::string ingredients = line.substr(0, loc);
            std::string target = line.substr(loc + 4);

            Formula formula;
            size_t targetLoc = target.find(" ");
            formula.quantity = stoll(target.substr(0, targetLoc));
            formula.target = target.substr(targetLoc + 1);

            while (ingredients.length() > 0) {
                size_t ingLoc = ingredients.find(", ");
                std::string currIngredient;
                if (ingLoc == std::string::npos) {
                    currIngredient = ingredients;
                    ingredients = "";
                } else {
                    currIngredient = ingredients.substr(0, ingLoc);
                    ingredients = ingredients.substr(ingLoc + 2);
                }

                size_t currIngLoc = currIngredient.find(" ");
                formula.ingredients[currIngredient.substr(currIngLoc + 1)] = stoll(currIngredient.substr(0, currIngLoc));
            }

            formulas[formula.target] = formula;
        }
    }

    std::vector<std::string> order;
    for (auto pair: formulas) {
        dfsSort(pair.first, formulas, order);
    }

    // Part 1
    std::cout << "ORE for 1 FUEL: " << getOre(1, formulas, order) << std::endl;

    // Part 2
    uint64_t currTotal = 0;
    uint64_t incrementer = 10000000;
    while (incrementer > 0) {
        uint64_t oreAtNext = getOre(currTotal + incrementer, formulas, order);
        if (oreAtNext > 1000000000000) {
            incrementer /= 10;
        } else {
            currTotal += incrementer;
        }
    }
    std::cout << "1 trillion ORE makes this much FUEL: " << currTotal << std::endl;
}