#include <iostream>
#include <fstream>
#include <functional>
#include <string>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct Planet {
    Planet(): name("COM"), orbiting(nullptr) {}
    Planet(std::string _name): name(_name), orbiting(nullptr) {}

    std::string name;
    Planet* orbiting;
    std::vector<Planet*> orbitedBy;
};

int processPlanet(Planet* planet, std::unordered_map<Planet*, uint32_t>& planetMap) {
    if (planetMap.count(planet)) {
        return planetMap[planet];
    }

    if (planet->orbiting == nullptr) {
        std::cout << "No orbit for " << planet->name << std::endl;
        std::cout << planet->orbiting << std::endl;
        return 0;
    }

    int numOrbits = 1 + processPlanet(planet->orbiting, planetMap);
    planetMap[planet] = numOrbits;
    return numOrbits;
}

int main(int argc, char* argv[]) {
    std::string line;
    std::ifstream file("input.txt");

    std::unordered_map<std::string, Planet*> planets;
    std::unordered_map<Planet*, uint32_t> planetMap;

    if (file.is_open()) {
        while (getline(file, line)) {
            std::stringstream ss(line);
            std::string orbited;
            std::string orbiting;

            getline(ss, orbited, ')');
            getline(ss, orbiting);

            Planet* orbitedPlanet;
            if (planets.count(orbited)) {
                orbitedPlanet = planets[orbited];
            } else {
                orbitedPlanet = new Planet(orbited);
                planets[orbited] = orbitedPlanet;
            }

            Planet* orbitingPlanet;
            if (planets.count(orbiting)) {
                orbitingPlanet = planets[orbiting];
            } else {
                orbitingPlanet = new Planet(orbiting);
                planets[orbiting] = orbitingPlanet;
            }

            orbitingPlanet->orbiting = orbitedPlanet;
            orbitedPlanet->orbitedBy.push_back(orbitingPlanet);
        }
    }

    for (auto planet: planets) {
        processPlanet(planet.second, planetMap);
    }

    uint32_t count = 0;
    for (auto p: planetMap) {
        count += p.second;
    }

    std::cout << count << std::endl;

    std::vector<std::pair<Planet*, int> > queue;
    std::unordered_set<Planet*> visited;
    Planet* target = planets["SAN"]->orbiting;

    queue.push_back(std::make_pair(planets["YOU"], -1));
    while (queue.size() > 0) {
        std::pair<Planet*, int> item = queue[0];
        queue.erase(queue.begin());

        if (item.first == nullptr || visited.count(item.first)) {
            continue;
        }
        visited.insert(item.first);

        if (item.first == target) {
            std::cout << "Found at " << item.second << std::endl;
            break;
        }

        // Traverse
        queue.push_back(std::make_pair(item.first->orbiting, item.second + 1));
        for (auto orbitedByPlanet: item.first->orbitedBy) {
            queue.push_back(std::make_pair(orbitedByPlanet, item.second + 1));
        }
    }
}