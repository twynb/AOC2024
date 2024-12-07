#include <fstream>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>

struct equation {
  unsigned long long expected_result;
  std::vector<unsigned long long> operands;
};

equation parse_equation(const std::string &line) {
  size_t pos = line.find(":");
  const unsigned long long expected_result = std::stoll(line.substr(0, pos));
  std::vector<unsigned long long> operands = {};

  pos += 1;
  size_t prev_pos = pos;
  while ((pos = line.find(" ", prev_pos + 1)) != line.npos) {
    operands.push_back(
        std::stoll(line.substr(prev_pos + 1, pos - prev_pos - 1)));
    prev_pos = pos;
  }
  operands.push_back(std::stoll(line.substr(prev_pos + 1)));
  return equation{expected_result, operands};
}

bool can_equation_be_valid(const equation &eq) {
  std::vector<unsigned long long> possible_results = {eq.operands[0]};
  for (size_t i = 1; i < eq.operands.size(); i++) {
    std::vector<unsigned long long> next_results = {};
    for (unsigned long long existing : possible_results) {
      const unsigned long long after_mul = existing * eq.operands[i];
      const unsigned long long after_add = existing + eq.operands[i];
      if (after_mul <= eq.expected_result) {
        next_results.push_back(after_mul);
      }
      if (after_add <= eq.expected_result) {
        next_results.push_back(after_add);
      }
    }
    possible_results = next_results;
  }
  for (unsigned long long result : possible_results) {
    if (result == eq.expected_result) {
      return true;
    }
  }
  return false;
}

uint8_t count_digits(unsigned long long input) {
  uint8_t result = 0;
  while (input) {
    result++;
    input /= 10;
  }
  return result;
}

unsigned long long concat(unsigned long long lhs, unsigned long long rhs) {
	const uint8_t digits = count_digits(rhs);
	for (uint8_t i = 0; i < digits; i++) {
		lhs *= 10;
	}
	return lhs + rhs;
}

bool can_equation_be_valid_with_concat(const equation &eq) {
  std::vector<unsigned long long> possible_results = {eq.operands[0]};
  for (size_t i = 1; i < eq.operands.size(); i++) {
    std::vector<unsigned long long> next_results = {};
    for (unsigned long long existing : possible_results) {
      const unsigned long long after_mul = existing * eq.operands[i];
      const unsigned long long after_add = existing + eq.operands[i];
      unsigned long long after_concat = concat(existing, eq.operands[i]);
      if (after_mul <= eq.expected_result) {
        next_results.push_back(after_mul);
      }
      if (after_add <= eq.expected_result) {
        next_results.push_back(after_add);
      }
      if (after_concat <= eq.expected_result) {
        next_results.push_back(after_concat);
      }
    }
    possible_results = next_results;
  }
  for (unsigned long long result : possible_results) {
    if (result == eq.expected_result) {
      return true;
    }
  }
  return false;
}

int main() {
  std::string line;
  std::ifstream input("07-1-input");

  unsigned long long result = 0;
  unsigned long long result_part_two = 0;
  while (std::getline(input, line)) {
    const equation eq = parse_equation(line);
    if (can_equation_be_valid(eq)) {
      result += eq.expected_result;
    }
	if (can_equation_be_valid_with_concat(eq)) {
		result_part_two += eq.expected_result;
	}
  }

  std::cout << "Part one: " << result << "\n";
  std::cout << "Part two: " << result_part_two << "\n";
}
