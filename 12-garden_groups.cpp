#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

struct area_data {
  unsigned long long fields;
  unsigned long long borders;
};

void establish_initial_areas(
    const std::vector<std::string> &map_plants,
    std::vector<std::vector<unsigned int>> &map_areas,
    std::set<std::pair<unsigned int, unsigned int>> &mappings) {
  map_areas[0][0] = 1;
  unsigned int next_free_code = 2;
  for (size_t y = 0; y < map_plants.size(); y++) {
    for (size_t x = 0; x < map_plants[y].size(); x++) {
      if (y != map_plants.size() - 1) {
        if (map_plants[y + 1][x] == map_plants[y][x]) {
          map_areas[y + 1][x] = map_areas[y][x];
        } else if (x == 0) {
          map_areas[y + 1][x] = next_free_code;
          next_free_code++;
        }
      }
      if (x != map_plants[y].size() - 1) {
        if (map_plants[y][x + 1] == map_plants[y][x]) {
          if (map_areas[y][x + 1] == 0) {
            map_areas[y][x + 1] = map_areas[y][x];
          } else if (map_areas[y][x + 1] != map_areas[y][x]) {
            mappings.insert(std::minmax(map_areas[y][x], map_areas[y][x + 1]));
          }
        } else if (map_areas[y][x + 1] == 0) {
          map_areas[y][x + 1] = next_free_code;
          next_free_code++;
        }
      }
    }
  }
}

void insert_or_replace(std::map<unsigned int, unsigned int> &mappings,
                       unsigned int from, unsigned int to) {
  auto const res = mappings.insert({from, to});
  if (!res.second) {
    res.first->second = to;
  }
}

std::map<unsigned int, unsigned int>
get_mappings(const std::set<std::pair<unsigned int, unsigned int>> &pairs) {
  std::map<unsigned int, unsigned int> res = {};
  for (auto &pair : pairs) {
    unsigned int to = pair.first;
    std::map<unsigned int, unsigned int>::iterator existing_to;
    if ((existing_to = res.find(to)) != res.end()) {
      to = existing_to->second;
    }
    unsigned int from = pair.second;
    unsigned int from_before = -1;
    auto existing_from = res.find(from);
    if (existing_from != res.end()) {
      from_before = existing_from->second;
    }
    auto mm = std::minmax(to, from_before);
    unsigned int target = mm.first;
    unsigned int map_for = mm.second;
    insert_or_replace(res, from, target);
    insert_or_replace(res, map_for, target);
    // replace any existing references
    for (auto &mapping : res) {
      if (mapping.second == from || mapping.second == map_for) {
        mapping.second = target;
      }
    }
    // we now have a mapping where none of the targets are also keys, assuming
    // such a mapping came in.
  }
  return res;
}

std::vector<std::vector<unsigned int>>
get_map_areas(const std::vector<std::string> &map_plants) {
  std::vector<std::vector<unsigned int>> map_areas(
      map_plants.size(), std::vector<unsigned int>(map_plants[0].size(), 0));
  std::set<std::pair<unsigned int, unsigned int>> initial_pairs;
  map_areas.reserve(map_plants.size());

  establish_initial_areas(map_plants, map_areas, initial_pairs);

  auto mappings = get_mappings(initial_pairs);

  for (std::vector<unsigned int> &row : map_areas) {
    for (unsigned int &val : row) {
      auto map = mappings.find(val);
      if (map != mappings.end()) {
        val = map->second;
      }
    }
  }

  return map_areas;
}

unsigned long long
part_one(const std::vector<std::vector<unsigned int>> &map_areas) {
  std::map<unsigned int, area_data> area_codes = {};
  for (size_t y = 0; y < map_areas.size(); y++) {
    for (size_t x = 0; x < map_areas[y].size(); x++) {
      auto code = area_codes.insert({map_areas[y][x], {0, 0}}).first;
      code->second.fields++;
      if (x != map_areas[y].size() - 1) {
        if (map_areas[y][x + 1] != map_areas[y][x]) {
          code->second.borders++;
          area_codes.insert({map_areas[y][x + 1], {0, 0}})
              .first->second.borders++;
        }
      } else {
        code->second.borders++;
      }
      if (y != map_areas.size() - 1) {
        if (map_areas[y + 1][x] != map_areas[y][x]) {
          code->second.borders++;
          area_codes.insert({map_areas[y + 1][x], {0, 0}})
              .first->second.borders++;
        }
      } else {
        code->second.borders++;
      }
      if (y == 0) {
        code->second.borders++;
      }
      if (x == 0) {
        code->second.borders++;
      }
    }
  }
  unsigned long long result = 0;
  for (auto &area : area_codes) {
    result += area.second.borders * area.second.fields;
  }
  return result;
}

// i'm not redefining a new struct with the same fields for this, so in this
// function "borders" actually means "sides".
unsigned long long
part_two(const std::vector<std::vector<unsigned int>> &map_areas) {
  std::map<unsigned int, area_data> area_codes = {};
  for (size_t y = 0; y < map_areas.size(); y++) {
    for (size_t x = 0; x < map_areas[y].size(); x++) {
      auto code = area_codes.insert({map_areas[y][x], {0, 0}}).first;
      code->second.fields++;
      // we start a side if:
      // - we differ from the block in that direction
      // - AND (the previous block* is OOB OR is a different block OR the
      // previous block is the same as the block in that direction from it)
      // *(if the side is just being continued)
	  // so this is basically a corner detector
      if ((y == 0 || map_areas[y][x] != map_areas[y - 1][x]) &&
          (x == 0 || map_areas[y][x - 1] != map_areas[y][x] ||
           (y != 0 && map_areas[y - 1][x - 1] == map_areas[y][x]))) {
        code->second.borders++;
      }
      if ((x == 0 || map_areas[y][x] != map_areas[y][x - 1]) &&
          (y == map_areas.size() - 1 ||
           map_areas[y + 1][x] != map_areas[y][x] ||
           (x != 0 && map_areas[y + 1][x - 1] == map_areas[y][x]))) {
        code->second.borders++;
      }
      if ((y == map_areas.size() - 1 ||
           map_areas[y][x] != map_areas[y + 1][x]) &&
          (x == map_areas.size() - 1 ||
           map_areas[y][x + 1] != map_areas[y][x] ||
           (y != map_areas.size() - 1 &&
            map_areas[y + 1][x + 1] == map_areas[y][x]))) {
        code->second.borders++;
      }
      if ((x == map_areas.size() - 1 ||
           map_areas[y][x] != map_areas[y][x + 1]) &&
          (y == 0 || map_areas[y - 1][x] != map_areas[y][x] ||
           (x != map_areas.size() &&
            map_areas[y - 1][x + 1] == map_areas[y][x]))) {
        code->second.borders++;
      }
    }
  }
  unsigned long long result = 0;
  for (auto &area : area_codes) {
    result += area.second.borders * area.second.fields;
  }
  return result;
}

int main() {
  std::string line;
  std::ifstream input("12-1-input");
  std::vector<std::string> map_plants = {};
  while (std::getline(input, line)) {
    map_plants.push_back(line);
  }
  auto map_areas = get_map_areas(map_plants);

  std::cout << "Part one: " << part_one(map_areas) << "\n";
  std::cout << "Part two: " << part_two(map_areas) << "\n";
}
