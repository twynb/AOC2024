#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

struct coordinates {
  int x;
  int y;
};

coordinates operator+(coordinates const &lhs, coordinates const &rhs) {
  return coordinates{lhs.x + rhs.x, lhs.y + rhs.y};
}
coordinates operator-(coordinates const &lhs, coordinates const &rhs) {
  return coordinates{lhs.x - rhs.x, lhs.y - rhs.y};
}
bool operator<(coordinates const &lhs, coordinates const &rhs) {
  if (lhs.x == rhs.x) {
    return lhs.y < rhs.y;
  }
  return lhs.x < rhs.x;
}

unsigned int part_one(const std::map<char, std::vector<coordinates>> &antennas,
                      int size_x, int size_y) {
  std::set<coordinates> antinodes = {};
  for (auto it = antennas.begin(); it != antennas.end(); ++it) {
    const std::vector<coordinates> &coordinates = it->second;
    for (size_t idx1 = 0; idx1 < coordinates.size(); idx1++) {
      for (size_t idx2 = idx1 + 1; idx2 < coordinates.size(); idx2++) {
        const struct coordinates difference =
            coordinates[idx1] - coordinates[idx2];
        const struct coordinates antinode_1 = coordinates[idx1] + difference;
        const struct coordinates antinode_2 = coordinates[idx2] - difference;
        if (antinode_1.x >= 0 && antinode_1.x < size_x && antinode_1.y >= 0 &&
            antinode_1.y < size_y) {
          antinodes.insert(antinode_1);
        }
        if (antinode_2.x >= 0 && antinode_2.x < size_x && antinode_2.y >= 0 &&
            antinode_2.y < size_y) {
          antinodes.insert(antinode_2);
        }
      }
    }
  }
  return antinodes.size();
}

// own implementation because my compiler and/or the clangd lsp don't know
// std::gcd yet
int gcd(int a, int b) {
  while (b != 0) {
    int t = b;
    b = a % b;
    a = t;
  }
  return a;
}

coordinates minimum_vector(const coordinates coords) {
  const int divisor = gcd(coords.x, coords.y);
  return {coords.x / divisor, coords.y / divisor};
}

unsigned int part_two(const std::map<char, std::vector<coordinates>> &antennas,
                      int size_x, int size_y) {
  std::set<coordinates> antinodes = {};
  for (auto it = antennas.begin(); it != antennas.end(); ++it) {
    const std::vector<coordinates> &coordinates = it->second;
    for (size_t idx1 = 0; idx1 < coordinates.size(); idx1++) {
      for (size_t idx2 = idx1 + 1; idx2 < coordinates.size(); idx2++) {
        const struct coordinates difference =
            minimum_vector(coordinates[idx1] - coordinates[idx2]);
        // insert one of the starting nodes and all the nodes in one direction
        struct coordinates antinode = coordinates[idx1];
        do {
          antinodes.insert(antinode);
          antinode = antinode - difference;
        } while (antinode.x >= 0 && antinode.x < size_x && antinode.y >= 0 &&
                 antinode.y < size_y);
        // then all the nodes in the other direction from that starting node
        antinode = coordinates[idx1] + difference;
        while (antinode.x >= 0 && antinode.x < size_x && antinode.y >= 0 &&
               antinode.y < size_y) {
          antinodes.insert(antinode);
          antinode = antinode + difference;
        }
      }
    }
  }
  return antinodes.size();
}

int main() {
  std::string line;
  std::ifstream input("08-1-input");
  int size_x = 0;
  int size_y = 0;
  std::map<char, std::vector<coordinates>> antennas = {};
  while (std::getline(input, line)) {
    size_x = line.size();
    for (size_t idx = 0; idx < line.size(); idx++) {
      if (line[idx] == '.') {
        continue;
      }
      auto vec = antennas.find(line[idx]);
      if (vec != antennas.end()) {
        vec->second.push_back(coordinates{(int)idx, size_y});
      } else {
        std::vector<coordinates> to_add = {coordinates{(int)idx, size_y}};
        antennas.insert({line[idx], to_add});
      }
    }
    size_y++;
  }

  std::cout << "Part 1: " << part_one(antennas, size_x, size_y) << "\n";
  std::cout << "Part 2: " << part_two(antennas, size_x, size_y) << "\n";
}
