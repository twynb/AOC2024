#include <chrono>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

/*
NOTE: I found the intuitive solution for part 2 (just do part 1, but with a
vector rather than a set to count duplicates) to be boring, so i also made a
prettier and faster solution. and while i was at it i might as well learn how to
do performance benchmarks in c++, hence all the chrono use.
   */

struct coordinates {
  size_t x;
  size_t y;
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

unsigned int get_score_for_trailhead(const std::vector<std::string> &lines,
                                     size_t x, size_t y) {
  std::set<coordinates> coords = {{x, y}};
  for (char i = '1'; i <= '9'; i++) {
    std::set<coordinates> new_coords = {};
    for (coordinates coord : coords) {
      if (coord.y > 0 && lines[coord.y - 1][coord.x] == i) {
        new_coords.insert({coord.x, coord.y - 1});
      }
      if (coord.x > 0 && lines[coord.y][coord.x - 1] == i) {
        new_coords.insert({coord.x - 1, coord.y});
      }
      if (coord.y < lines.size() - 1 && lines[coord.y + 1][coord.x] == i) {
        new_coords.insert({coord.x, coord.y + 1});
      }
      if (coord.x < lines[coord.y].size() - 1 &&
          lines[coord.y][coord.x + 1] == i) {
        new_coords.insert({coord.x + 1, coord.y});
      }
    }
    coords = new_coords;
  }
  return coords.size();
}

unsigned int part_one(const std::vector<std::string> &lines) {
  unsigned int result = 0;
  for (size_t y = 0; y < lines.size(); y++) {
    for (size_t x = 0; x < lines[y].size(); x++) {
      if (lines[y][x] != '0') {
        continue;
      }
      result += get_score_for_trailhead(lines, x, y);
    }
  }
  return result;
}

unsigned int get_rating_for_trailhead(const std::vector<std::string> &lines,
                                      size_t x, size_t y) {
  std::vector<coordinates> coords = {{x, y}};
  for (char i = '1'; i <= '9'; i++) {
    std::vector<coordinates> new_coords = {};
    for (coordinates coord : coords) {
      if (coord.y > 0 && lines[coord.y - 1][coord.x] == i) {
        new_coords.push_back({coord.x, coord.y - 1});
      }
      if (coord.x > 0 && lines[coord.y][coord.x - 1] == i) {
        new_coords.push_back({coord.x - 1, coord.y});
      }
      if (coord.y < lines.size() - 1 && lines[coord.y + 1][coord.x] == i) {
        new_coords.push_back({coord.x, coord.y + 1});
      }
      if (coord.x < lines[coord.y].size() - 1 &&
          lines[coord.y][coord.x + 1] == i) {
        new_coords.push_back({coord.x + 1, coord.y});
      }
    }
    coords = new_coords;
  }
  return coords.size();
}

unsigned int part_two(const std::vector<std::string> &lines) {
  unsigned int result = 0;
  for (size_t y = 0; y < lines.size(); y++) {
    for (size_t x = 0; x < lines[y].size(); x++) {
      if (lines[y][x] != '0') {
        continue;
      }
      result += get_rating_for_trailhead(lines, x, y);
    }
  }
  return result;
}

unsigned int part_two_fancy(const std::vector<std::string> &lines) {
  std::vector<unsigned int> accessible_map_line(lines[0].size(), 0);
  std::vector<std::vector<unsigned int>> accessible(lines.size(),
                                                    accessible_map_line);
  for (size_t y = 0; y < lines.size(); y++) {
    for (size_t x = 0; x < lines[y].size(); x++) {
      if (lines[y][x] != '1') {
        continue;
      }
      if (y > 0 && lines[y - 1][x] == '0') {
        accessible[y][x]++;
      }
      if (x > 0 && lines[y][x - 1] == '0') {
        accessible[y][x]++;
      }
      if (y < lines.size() - 1 && lines[y + 1][x] == '0') {
        accessible[y][x]++;
      }
      if (x < lines[y].size() - 1 && lines[y][x + 1] == '0') {
        accessible[y][x]++;
      }
    }
  }
  for (char i = '2'; i <= '8'; i++) {
    for (size_t y = 0; y < lines.size(); y++) {
      for (size_t x = 0; x < lines[y].size(); x++) {
        if (lines[y][x] != i) {
          continue;
        }
        const char prev = i - 1;
        if (y > 0 && lines[y - 1][x] == prev) {
          accessible[y][x] += accessible[y - 1][x];
        }
        if (x > 0 && lines[y][x - 1] == prev) {
          accessible[y][x] += accessible[y][x - 1];
        }
        if (y < lines.size() - 1 && lines[y + 1][x] == prev) {
          accessible[y][x] += accessible[y + 1][x];
        }
        if (x < lines[y].size() - 1 && lines[y][x + 1] == prev) {
          accessible[y][x] += accessible[y][x + 1];
        }
      }
    }
  }
  unsigned int result = 0;
  for (size_t y = 0; y < lines.size(); y++) {
    for (size_t x = 0; x < lines[y].size(); x++) {
      if (lines[y][x] != '9') {
        continue;
      }
      if (y > 0 && lines[y - 1][x] == '8') {
        result += accessible[y - 1][x];
      }
      if (x > 0 && lines[y][x - 1] == '8') {
        result += accessible[y][x - 1];
      }
      if (y < lines.size() - 1 && lines[y + 1][x] == '8') {
        result += accessible[y + 1][x];
      }
      if (x < lines[y].size() - 1 && lines[y][x + 1] == '8') {
        result += accessible[y][x + 1];
      }
    }
  }
  return result;
}

int main() {
  std::string line;
  std::ifstream input("10-1-input");
  std::vector<std::string> lines;
  while (std::getline(input, line)) {
    lines.push_back(line);
  }
  std::cout << "Part 1: " << part_one(lines) << "\n";
  unsigned int part_two_result;
  auto start_boring = std::chrono::system_clock::now();
  for (int i = 0; i < 1000; i++) {
    part_two_result = part_two(lines);
  }
  auto end_boring = std::chrono::system_clock::now();
  auto start_fancy = std::chrono::system_clock::now();
  std::cout << "Part 2: " << part_two_result << "\n";
  for (int i = 0; i < 1000; i++) {
    part_two_result = part_two_fancy(lines);
  }
  auto end_fancy = std::chrono::system_clock::now();
  std::cout << "Part 2 (fancy): " << part_two_result << "\n";

  std::chrono::duration<double> elapsed_boring = end_boring - start_boring;
  std::chrono::duration<double> elapsed_fancy = end_fancy - start_fancy;
  std::cout << "Boring solution took " << elapsed_boring.count() << "s\n";
  std::cout << "Fancy solution took " << elapsed_fancy.count() << "s\n";
}
