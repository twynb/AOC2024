#include <fstream>
#include <iostream>
#include <string>
#include <vector>

bool check_in_direction(const std::vector<std::string> &data, size_t line,
                        size_t col, int vertical_direction,
                        int horizontal_direction) {
  return data[line + vertical_direction][col + horizontal_direction] == 'M' &&
         data[line + vertical_direction * 2][col + horizontal_direction * 2] ==
             'A' &&
         data[line + vertical_direction * 3][col + horizontal_direction * 3] ==
             'S';
}

unsigned long check_for_xmas(const std::vector<std::string> &data, size_t line,
                             size_t col) {
  unsigned long result = 0;
  // horizontal: left
  if (col > 2 && check_in_direction(data, line, col, 0, -1)) {
    result++;
  }
  // horizontal: right
  if (col < data[line].size() - 3 &&
      check_in_direction(data, line, col, 0, 1)) {
    result++;
  }
  // vertical: up
  if (line > 2 && check_in_direction(data, line, col, -1, 0)) {
    result++;
  }
  // vertical: down
  if (line < data.size() - 3 && check_in_direction(data, line, col, 1, 0)) {
    result++;
  }
  // diagonal: up left
  if (line > 2 && col > 2 && check_in_direction(data, line, col, -1, -1)) {
    result++;
  }
  // diagonal: up right
  if (line > 2 && col < data[line].size() - 3 &&
      check_in_direction(data, line, col, -1, 1)) {
    result++;
  }
  // diagonal: down left
  if (line < data.size() - 3 && col > 2 &&
      check_in_direction(data, line, col, 1, -1)) {
    result++;
  }
  // diagonal: down right
  if (line < data.size() - 3 && col < data[line].size() - 3 &&
      check_in_direction(data, line, col, 1, 1)) {
    result++;
  }
  return result;
}

unsigned long part_1(const std::vector<std::string> &data) {
  unsigned long result = 0;
  for (size_t line = 0; line < data.size(); line++) {
    for (size_t col = 0; col < data[line].size(); col++) {
      if (data[line][col] == 'X') {
        result += check_for_xmas(data, line, col);
      }
    }
  }
  return result;
}

bool check_for_cross_mas(const std::vector<std::string> &data, size_t line,
                         size_t col) {
  const char up_left = data[line - 1][col - 1];
  const char up_right = data[line - 1][col + 1];
  const char down_left = data[line + 1][col - 1];
  const char down_right = data[line + 1][col + 1];
  return (up_left == 'S' || down_right == 'S') &&
         (up_left == 'M' || down_right == 'M') &&
         (up_right == 'S' || down_left == 'S') &&
         (up_right == 'M' || down_left == 'M');
}

unsigned long part_2(const std::vector<std::string> &data) {
  unsigned long result = 0;
  // we can skip the outermost lines and columns, can't have a cross around
  // those.
  for (size_t line = 1; line < data.size() - 1; line++) {
    for (size_t col = 1; col < data[line].size() - 1; col++) {
      if (data[line][col] == 'A' && check_for_cross_mas(data, line, col)) {
        result++;
      }
    }
  }
  return result;
}

int main() {
  std::vector<std::string> data = std::vector<std::string>();

  std::string line;
  std::ifstream input("04-1-input");
  while (std::getline(input, line)) {
    data.push_back(line);
  }

  std::cout << "XMAS matches: " << part_1(data) << "\n";
  std::cout << "CROSS-MAS matches: " << part_2(data) << "\n";
}

