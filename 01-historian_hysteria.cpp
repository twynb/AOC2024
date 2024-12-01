#include <fstream>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>

void parse_data(std::vector<int, std::allocator<int>> *data_left,
                std::vector<int, std::allocator<int>> *data_right) {
  std::ifstream input_file("01-1-input");
  if (!input_file.is_open()) {
    std::cout << "Input file couldn't be opened!" << "\n";
    return;
  }
  std::string line;
  while (getline(input_file, line)) {
    int end_left = line.find(" ");
    int start_right = line.find_last_of(" ");
    int left = std::stoi(line.substr(0, end_left));
    int right = std::stoi(line.substr(start_right + 1));
    // ensure that vectors are sorted immediately
    auto insert_left_at =
        std::upper_bound(data_left->cbegin(), data_left->cend(), left);
    auto insert_right_at =
        std::upper_bound(data_right->cbegin(), data_right->cend(), right);
    data_left->insert(insert_left_at, left);
    data_right->insert(insert_right_at, right);
  }
}

void part_one(std::vector<int, std::allocator<int>> const &data_left,
              std::vector<int, std::allocator<int>> const &data_right) {
  unsigned long int total_difference = 0;
  for (int idx = 0; idx < data_left.size(); idx++) {
    const int diff = std::abs(data_left[idx] - data_right[idx]);
    total_difference += diff;
  }
  std::cout << "Total difference: " << total_difference << "\n";
}

void part_two(std::vector<int, std::allocator<int>> const &data_left,
              std::vector<int, std::allocator<int>> const &data_right) {
  unsigned long similarity_score = 0;

  const int *idx = data_left.data();
  const int *right_idx = data_right.data();
  while (idx - data_left.data() < data_left.size()) {
    const int current_number = *idx;
    while (*right_idx < current_number) {
      right_idx++;
    }
    while (*right_idx == current_number) {
      similarity_score += current_number;
      right_idx++;
    }
    idx++;
  }
  std::cout << "Similarity score: " << similarity_score << "\n";
}

int main() {
  std::vector<int, std::allocator<int>> data_left =
      std::vector<int, std::allocator<int>>();
  std::vector<int, std::allocator<int>> data_right =
      std::vector<int, std::allocator<int>>();
  parse_data(&data_left, &data_right);
  part_one(data_left, data_right);
  part_two(data_left, data_right);
  return 0;
}
