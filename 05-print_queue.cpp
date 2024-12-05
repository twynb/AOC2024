#include <fstream>
#include <iostream>
#include <map>
#include <math.h>
#include <set>
#include <string>
#include <vector>

/**
  mapping of numbers to a vector of numbers that cannot show up *before* them.
*/
std::map<int, std::set<int>> rules = std::map<int, std::set<int>>();
std::vector<std::vector<int>> lines = std::vector<std::vector<int>>();

bool is_line_ordered(const std::vector<int> &line) {
  std::set<int> already_seen = std::set<int>();
  for (const int number : line) {
    const auto rule = rules.find(number);
    if (rule == rules.end()) {
      already_seen.insert(number);
      continue;
    }
    for (const int not_allowed : rule->second) {
      if (already_seen.find(not_allowed) != already_seen.end()) {
        return false;
      }
    }
    already_seen.insert(number);
  }
  return true;
}

unsigned long part_one() {
  unsigned int result = 0;
  for (const std::vector<int> line : lines) {
    if (is_line_ordered(line)) {
      result += line[line.size() / 2];
    }
  }
  return result;
}

// this is probably a really inefficient sorting algorithm
// but im too tired to come up with something smarter
bool sort_line_run(std::vector<int> &line) {
  bool is_all_sorted = true;
  for (size_t idx = 0; idx < line.size(); idx++) {
    const auto rule = rules.find(line[idx]);
    if (rule == rules.end()) {
      continue;
    }
    for (size_t idx2 = 0; idx2 < idx; idx2++) {
      if (rule->second.find(line[idx2]) != rule->second.end()) {
        // we cannot be in this order, so swap
        int tmp = line[idx2];
        line[idx2] = line[idx];
        line[idx] = tmp;
        // note that we had to change something so we have to rerun
        is_all_sorted = false;
        break;
      }
    }
  }
  return is_all_sorted;
}

std::vector<int> sort_line(const std::vector<int> &line) {
  std::vector<int> sorted_line;
  sorted_line.assign(line.begin(), line.end());
  while (!sort_line_run(sorted_line)) {
    continue;
  }
  return sorted_line;
}

unsigned long part_two() {
  unsigned int result = 0;
  for (const std::vector<int> line : lines) {
    if (is_line_ordered(line)) {
      continue;
    }
    const std::vector<int> sorted_line = sort_line(line);
    result += sorted_line[sorted_line.size() / 2];
  }
  return result;
}

std::vector<int> split_and_parse(const std::string &line) {
  std::vector<int> out = std::vector<int>();
  size_t pos = 0;
  size_t prev_pos = 0;
  while ((pos = line.find(",", prev_pos + 1)) != line.npos) {
    int next = std::stoi(
        line.substr(prev_pos == 0 ? 0 : (prev_pos + 1),
                    prev_pos == 0 ? (pos - prev_pos) : (pos - prev_pos - 1)));
    out.push_back(next);
    prev_pos = pos;
  }
  out.push_back(std::stoi(line.substr(prev_pos + 1)));
  return out;
}

int main() {
  std::string line;
  std::ifstream input("05-1-input");
  while (std::getline(input, line)) {
    if (line.size() == 0) {
      continue;
    }
    size_t pipe_idx = line.find("|");
    if (pipe_idx != line.npos) {
      const int before = std::stoi(line.substr(0, pipe_idx));
      const int after = std::stoi(line.substr(pipe_idx + 1));
      auto vec = rules.find(before);
      if (vec != rules.end()) {
        vec->second.insert(after);
      } else {
        std::set<int> to_add = {after};
        rules.insert({before, to_add});
      }
      continue;
    }
    lines.push_back(split_and_parse(line));
  }
  std::cout << "Part 1: " << part_one() << "\n";
  std::cout << "Part 2: " << part_two() << "\n";
}
