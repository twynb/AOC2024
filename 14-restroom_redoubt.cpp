#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// structs

struct robot {
  long initial_x;
  long initial_y;
  long velocity_x;
  long velocity_y;
};

// utils

long mod(long a, long b) {
  long res = a % b;
  if (res < 0) {
    res += b;
  }
  return res;
}

// business logic

robot parse_robot(std::string line) {
  const size_t first_comma = line.find(",");
  const long initial_x = std::stol(line.substr(2, first_comma - 2));
  const long initial_y =
      std::stol(line.substr(first_comma + 1, line.find(" ") - first_comma - 1));
  const size_t velocity_start = line.find("v=") + 2;
  const size_t second_comma = line.rfind(",");
  const long velocity_x =
      std::stol(line.substr(velocity_start, second_comma - velocity_start));
  const long velocity_y = std::stol(line.substr(second_comma + 1));
  return {initial_x, initial_y, velocity_x, velocity_y};
}

unsigned long part_one(const std::vector<robot> &robots) {
  unsigned long top_left = 0;
  unsigned long top_right = 0;
  unsigned long bottom_left = 0;
  unsigned long bottom_right = 0;
  for (const robot &robot : robots) {
    long x = mod(robot.initial_x + robot.velocity_x * 100, 101);
    long y = mod(robot.initial_y + robot.velocity_y * 100, 103);
    if (x < 50) {
      if (y < 51) {
        top_left++;
      } else if (y > 51) {
        bottom_left++;
      }
    } else if (x > 50) {
      if (y < 51) {
        top_right++;
      } else if (y > 51) {
        bottom_right++;
      }
    }
  }
  return top_left * top_right * bottom_left * bottom_right;
}

int main() {
  std::string line;
  std::ifstream input("14-1-input");
  std::vector<robot> robots = {};
  while (std::getline(input, line)) {
    robots.push_back(parse_robot(line));
  }
  std::cout << "part one: " << part_one(robots) << "\n";
}
