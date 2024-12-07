#include <cstdint>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

/*
   This entire solution is based around a bitfield structure for each field.
   The information for the fields is (from LSB to MSB):
   - Has been traversed from below
   - Has been traversed from the left
   - Has been traversed from above
   - Has been traversed from the right
   - Is an obstacle
   - Is the guard's starting position
   - Has been traversed for the guard's un-blocked route.
   - (nothing)
*/

bool does_block_for_direction(std::vector<std::vector<uint8_t>> field, size_t x,
                              size_t y, uint8_t direction) {
  // put obstruction in that field
  field[y][x] |= 0b10000;
  // move to the field we came from
  switch (direction) {
  case 0b1:
    x++;
    break;
  case 0b10:
    y--;
    break;
  case 0b100:
    x--;
    break;
  case 0b1000:
    y++;
    break;
  }
  // now attempt to walk out and see if we end up in a loop
  while (x >= 0 && y >= 0 && x < field[0].size() && y < field.size()) {
    size_t next_x = x;
    size_t next_y = y;
    switch (direction) {
    case 0b1000:
      if (y == 0) {
        // we got out, no infinite loop
        return false;
      }
      next_y--;
      break;
    case 0b100:
      if (x == field.size() - 1) {
        // we got out, no infinite loop
        return false;
      }
      next_x++;
      break;
    case 0b10:
      if (y == field[0].size() - 1) {
        // we got out, no infinite loop
        return false;
      }
      next_y++;
      break;
    case 0b1:
      if (x == 0) {
        // we got out, no infinite loop
        return false;
      }
      next_x--;
      break;
    }
    if (field[next_y][next_x] & 0b00010000) {
      direction = direction >> 1;
      if (!(direction & 0b1111)) {
        direction = 0b1000;
      }
      continue;
    }
    if (field[next_y][next_x] & direction) {
      std::cout << next_x << " " << next_y << "\n";
      // we have already been on this field in this direction, infinite loop
      // found!
      return true;
    }
    // now we've been here from this direction.
    field[next_y][next_x] |= direction;
    x = next_x;
    y = next_y;
  }
  return false;
}

std::pair<uint16_t, size_t>
traverse_and_check_if_loop_is_possible(std::vector<std::vector<uint8_t>> &field,
                                       size_t x, size_t y, uint8_t direction) {
  std::set<std::pair<size_t, size_t>> blocking_indices = {};
  uint16_t result = 1;
  while (x >= 0 && y >= 0 && x < field[0].size() && y < field.size()) {
    size_t next_x = x;
    size_t next_y = y;
    switch (direction) {
    case 0b1000:
      if (y == 0) {
        return std::pair<uint16_t, size_t>(result, blocking_indices.size());
      }
      next_y--;
      break;
    case 0b100:
      if (x == field.size() - 1) {
        return std::pair<uint16_t, size_t>(result, blocking_indices.size());
      }
      next_x++;
      break;
    case 0b10:
      if (y == field[0].size() - 1) {
        return std::pair<uint16_t, size_t>(result, blocking_indices.size());
      }
      next_y++;
      break;
    case 0b1:
      if (x == 0) {
        return std::pair<uint16_t, size_t>(result, blocking_indices.size());
      }
      next_x--;
      break;
    }
    if (field[next_y][next_x] & 0b00010000) {
      direction = direction >> 1;
      if (!(direction & 0b1111)) {
        direction = 0b1000;
      }
      continue;
    }
    if ((field[next_y][next_x] & 0b1000000) == 0) {
      result++;
      // note down that we have walked here for part 1
      field[next_y][next_x] |= 0b1000000;

      // check for part 2
      if ((field[next_x][next_y] & 0b100000) == 0 &&
          does_block_for_direction(field, next_x, next_y, direction)) {
        std::cout << "Found blocking index " << next_x << ", " << next_y << "\n";
        blocking_indices.insert(std::pair<size_t, size_t>(next_x, next_y));
      }
    }
    x = next_x;
    y = next_y;
  }
  return std::pair<uint16_t, size_t>(result, blocking_indices.size());
}

int main() {
  std::vector<std::vector<uint8_t>> field = {};
  size_t x = 0;
  size_t y = 0;
  uint8_t direction = 0;

  std::string line;
  std::ifstream input("06-1-input");
  while (std::getline(input, line)) {
    std::vector<uint8_t> row = {};
    row.reserve(line.length());
    for (unsigned char item : line) {
      switch (item) {
      case '.':
        row.push_back(0);
        break;
      case '#':
        row.push_back(0b00010000);
        break;
      case '^':
        x = row.size();
        y = field.size();
        direction = 0b1000;
        row.push_back(0b01100000);
        break;
      case '>':
        x = row.size();
        y = field.size();
        direction = 0b100;
        row.push_back(0b01100000);
        break;
      case 'v':
        x = row.size();
        y = field.size();
        direction = 2;
        row.push_back(0b01100000);
        break;
      case '<':
        x = row.size();
        y = field.size();
        direction = 3;
        row.push_back(0b01100000);
        break;
      }
    }
    field.push_back(row);
  }
  std::cout << x << " " << y << "\n";

  auto result = traverse_and_check_if_loop_is_possible(field, x, y, direction);
  std::cout << "Part one: " << result.first << "\n";
  std::cout << "Part two: " << result.second << "\n";
}
