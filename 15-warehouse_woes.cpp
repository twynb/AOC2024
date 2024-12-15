#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

std::vector<size_t> get_map_keys(const std::map<size_t, uint8_t> &map) {
  std::vector<size_t> res;
  res.reserve(map.size());
  for (auto const &entry : map) {
    res.push_back(entry.first);
  }
  return res;
}

bool can_move(const std::vector<std::vector<uint8_t>> &map, size_t x, size_t y,
              const int horizontal_direction, const int vertical_direction) {
  while (x > 0 && x < map[y].size() - 1 && y > 0 && y < map.size() - 1) {
    x += horizontal_direction;
    y += vertical_direction;
    switch (map[y][x]) {
    case 1:
      return false;
    case 0:
      return true;
    case 4:
      if (vertical_direction != 0 &&
          !can_move(map, x + 1, y, horizontal_direction, vertical_direction)) {
        return false;
      }
      break;
    case 5:
      if (vertical_direction != 0 &&
          !can_move(map, x - 1, y, horizontal_direction, vertical_direction)) {
        return false;
      }
      break;
    }
  }
  // shouldn't happen
  std::cout << "can_move check went out of bounds!\n";
  return false;
}

void move_horizontal(std::vector<std::vector<uint8_t>> &map, size_t x, size_t y,
                     const int direction) {
  uint8_t current = map[y][x];
  map[y][x] = 0;
  bool keep_moving = true;
  do {
    x += direction;
    uint8_t next = map[y][x];
    keep_moving = next != 0;
    map[y][x] = current;
    current = next;
  } while (keep_moving);
}

void move_vertical(std::vector<std::vector<uint8_t>> &map, size_t initial_x,
                   size_t y, const int direction) {
  std::map<size_t, uint8_t> x_coordinates = {{initial_x, map[y][initial_x]}};
  map[y][initial_x] = 0;
  do {
    auto keys = get_map_keys(x_coordinates);
    y += direction;
    for (size_t x : keys) {
      auto entry = x_coordinates.find(x);
      uint8_t next = map[y][x];
      map[y][x] = entry->second;
      switch (next) {
      case 0:
        // nothing left to push here, so we won't push on
        x_coordinates.erase(entry);
        break;
      case 4:
        entry->second = next;
        // we can just insert here
        // if we're already in x_coordinates, we don't want to replace it
        // because we might not have iterated over that one yet, and we'll end
        // up at the same result either way. if we're not, we don't mess up
        // our iteration because we got the keys ahead of time.
        if (x_coordinates.insert({x + 1, map[y][x + 1]}).second) {
          // if insertion was not successful, we are just starting to move boxes
          // in that column so we need to clear it
          map[y][x + 1] = 0;
        };
        break;
      case 5:
        entry->second = next;
        // see above
        if (x_coordinates.insert({x - 1, map[y][x - 1]}).second) {
          // if insertion was not successful, we are just starting to move boxes
          // in that column so we need to clear it
          map[y][x - 1] = 0;
        }
        break;
      default:
        entry->second = next;
      }
    }
  } while (x_coordinates.size() > 0);
}

void do_move(std::vector<std::vector<uint8_t>> &map, size_t x, size_t y,
             const int horizontal_direction, const int vertical_direction) {
  if (horizontal_direction != 0) {
    move_horizontal(map, x, y, horizontal_direction);
  } else {
    move_vertical(map, x, y, vertical_direction);
  }
}

void simulate_movements(std::vector<std::vector<uint8_t>> &map,
                        const std::string &movements, size_t x, size_t y) {
  for (char dir : movements) {
    int horizontal_direction = 0;
    int vertical_direction = 0;
    switch (dir) {
    case '<':
      horizontal_direction = -1;
      break;
    case '>':
      horizontal_direction = 1;
      break;
    case '^':
      vertical_direction = -1;
      break;
    case 'v':
      vertical_direction = 1;
      break;
    default:
      std::cout << "Invalid direction string " << dir << "! Aborting...\n";
      return;
    }
    if (!can_move(map, x, y, horizontal_direction, vertical_direction)) {
      continue;
    }
    do_move(map, x, y, horizontal_direction, vertical_direction);
    x += horizontal_direction;
    y += vertical_direction;
  }
}

unsigned long simulate_and_count(std::vector<std::vector<uint8_t>> map,
                                 const std::string &movements, size_t x,
                                 size_t y) {
  simulate_movements(map, movements, x, y);
  unsigned long result = 0;
  for (size_t i = 0; i < map.size(); i++) {
    for (size_t j = 0; j < map[i].size(); j++) {
      if (map[i][j] == 3 || map[i][j] == 4) {
        result += 100 * i + j;
      }
    }
  }
  return result;
}

int main() {
  std::string line;
  std::ifstream input("15-1-input");
  std::vector<std::vector<uint8_t>> map = {};
  std::vector<std::vector<uint8_t>> map_part_two = {};
  std::string movements = {};
  size_t initial_x = 0;
  size_t initial_y = 0;
  size_t initial_x_part_two = 0;

  bool is_map = true;
  while (std::getline(input, line)) {
    if (line.size() == 0) {
      is_map = false;
      continue;
    }
    if (is_map) {
      std::vector<uint8_t> row = {};
      std::vector<uint8_t> row_part_two = {};
      row.reserve(line.size());
      row_part_two.reserve(line.size() * 2);
      for (char col : line) {
        switch (col) {
        case '#':
          row.push_back(1);
          row_part_two.push_back(1);
          row_part_two.push_back(1);
          break;
        case '@':
          initial_x = row.size();
          initial_y = map.size();
          initial_x_part_two = row_part_two.size();
          row.push_back(2);
          row_part_two.push_back(2);
          row_part_two.push_back(0);
          break;
        case 'O':
          row.push_back(3);
          row_part_two.push_back(4);
          row_part_two.push_back(5);
          break;
        default:
          row.push_back(0);
          row_part_two.push_back(0);
          row_part_two.push_back(0);
        }
      }
      map.push_back(row);
      map_part_two.push_back(row_part_two);
    } else {
      movements += line;
    }
  }

  std::cout << "Part one: "
            << simulate_and_count(map, movements, initial_x, initial_y) << "\n";
  std::cout << "Part two: "
            << simulate_and_count(map_part_two, movements, initial_x_part_two,
                                  initial_y)
            << "\n";
}
