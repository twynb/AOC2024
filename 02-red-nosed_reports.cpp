#include <fstream>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>

std::vector<int> parse_line(std::string input) {
  std::vector<int> out = std::vector<int>();
  size_t pos = 0;
  size_t prev_pos = 0;
  while ((pos = input.find(" ", prev_pos + 1)) != input.npos) {
    int next = std::stoi(input.substr(prev_pos, pos - prev_pos));
    out.push_back(next);
    prev_pos = pos;
  }
  out.push_back(std::stoi(input.substr(prev_pos)));
  return out;
}

bool is_diff_acceptable(const int diff, const bool is_increasing) {
  if (diff == 0) {
    return false;
  }
  if ((is_increasing && diff < 0) || (!is_increasing && diff > 0)) {
    return false;
  }
  if (std::abs(diff) > 3) {
    return false;
  }
  return true;
}

bool is_safe_row(const std::vector<int> &numbers) {
  bool is_increasing = numbers[0] < numbers[1];
  for (size_t i = 0; i < numbers.size() - 1; i++) {
    const int diff = numbers[i + 1] - numbers[i];
    if (!is_diff_acceptable(diff, is_increasing)) {
      return false;
    }
  }
  return true;
}

bool is_safe_row(const std::vector<int> &numbers, const size_t ignored_index) {
  bool is_increasing = numbers[0] < numbers[1];
  if (ignored_index == 0) {
    is_increasing = numbers[1] < numbers[2];
  }
  if (ignored_index == 1) {
    is_increasing = numbers[0] < numbers[2];
  }
  for (size_t i = 0; i < numbers.size() - 1; i++) {
    if (i == ignored_index) {
      continue;
    }
    int next_number = numbers[i + 1];
    if (ignored_index == i + 1) {
      if (i + 2 >= numbers.size()) {
        // we reached the end
        return true;
      }
      next_number = numbers[i + 2];
    }
    const int diff = next_number - numbers[i];
    if (!is_diff_acceptable(diff, is_increasing)) {
      return false;
    }
  }
  return true;
}

// i'm not satisfied with this because it's o(n^2)
// could've done o(n) by iterating and manually skipping the number that gets ignored when it's found, but there's too many edge cases and i'm tired
bool is_safe_row_pt2(const std::vector<int> &numbers) {
  if (is_safe_row(numbers)) {
    return true;
  }
  for (size_t ignored_index = 0; ignored_index < numbers.size();
       ignored_index++) {
    if (is_safe_row(numbers, ignored_index)) {
      return true;
    }
  }
  return false;
}

int main() {
  std::ifstream input_file("02-1-input");
  if (!input_file.is_open()) {
    std::cout << "Input file couldn't be opened!\n";
    return 1;
  }
  std::string line;
  unsigned long safe_rows = 0;
  unsigned long safe_rows_pt2 = 0;
  bool is_increasing = false;
  while (getline(input_file, line)) {
    std::vector<int> numbers = parse_line(line);
    if (is_safe_row(numbers)) {
      safe_rows++;
    }
    if (is_safe_row_pt2(numbers)) {
      std::cout << "Safe row!\n";
      safe_rows_pt2++;
    }
  }
  std::cout << "Safe rows: " << std::to_string(safe_rows) << "\n";
  std::cout << "Safe rows with dampener: " << std::to_string(safe_rows_pt2)
            << "\n";
  return 0;
}
