#include <fstream>
#include <iostream>
#include <map>
#include <string>

std::map<unsigned long long, unsigned long long>
split_and_parse(const std::string &line) {
  std::map<unsigned long long, unsigned long long> out = {};
  size_t pos = 0;
  size_t prev_pos = 0;
  while ((pos = line.find(" ", prev_pos + 1)) != line.npos) {
    unsigned long long next = std::stoll(
        line.substr(prev_pos == 0 ? 0 : (prev_pos + 1),
                    prev_pos == 0 ? (pos - prev_pos) : (pos - prev_pos - 1)));
    out.insert({next, 1});
    prev_pos = pos;
  }
  out.insert({(unsigned long long)std::stoll(line.substr(prev_pos + 1)), 1});
  return out;
}

uint8_t count_digits(unsigned long long input) {
  uint8_t result = 0;
  while (input) {
    result++;
    input /= 10;
  }
  return result;
}

unsigned long long pow(unsigned long long const base,
                       unsigned long long const exp) {
  unsigned long long res = 1;
  for (unsigned long long i = 0; i < exp; i++) {
    res *= base;
  }
  return res;
}

void insert_or_add(std::map<unsigned long long, unsigned long long> &stones,
                   unsigned long long key, unsigned long long count) {
  auto const res = stones.insert({key, count});
  if (!res.second) {
    res.first->second += count;
  }
}

unsigned long long
get_stone_value(std::map<unsigned long long, unsigned long long> stones,
                uint8_t repetitions) {
  for (uint8_t i = 0; i < repetitions; i++) {
    // we'll almost always need this one, so put it at a fixed index
    std::map<unsigned long long, unsigned long long> stones_next = {};
    const auto end = stones.end();
    for (auto iter = stones.begin(); iter != end; ++iter) {
      if (iter->first == 0) {
        insert_or_add(stones_next, 1, iter->second);
        continue;
      }
      const uint8_t digits = count_digits(iter->first);
      if (digits % 2 == 1) {
        const unsigned long long val = iter->first * 2024;
        insert_or_add(stones_next, val, iter->second);
        continue;
      }
      const unsigned long long power_of_ten = pow(10, digits / 2);
      const unsigned long long left = iter->first / power_of_ten;
      insert_or_add(stones_next, left, iter->second);
      const unsigned long long right = iter->first % power_of_ten;
      insert_or_add(stones_next, right, iter->second);
    }
    stones = stones_next;
  }
  unsigned long long result = 0;
  for (auto iter = stones.begin(); iter != stones.end(); ++iter) {
    result += iter->second;
  }
  return result;
}

int main() {
  std::string line;
  std::ifstream input("11-1-input");
  if (!std::getline(input, line)) {
    std::cout << "Error reading the input!";
  }
  const std::map<unsigned long long, unsigned long long> stones =
      split_and_parse(line);
  std::cout << "Part one: " << get_stone_value(stones, 25) << "\n";
  std::cout << "Part two: " << get_stone_value(stones, 75) << "\n";
}
