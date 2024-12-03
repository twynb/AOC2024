#include <fstream>
#include <iostream>
#include <iterator>
#include <math.h>
#include <regex>
#include <string>

std::regex mul_regex("mul\\((\\d{1,3}),(\\d{1,3})\\)");
/**
  match existing mul, or "do()", or "don't()".
*/
std::regex
    mul_regex_with_conditionals("do\\(\\)|don't\\(\\)|mul\\((\\d{1,3}),(\\d{1,3})\\)");

unsigned long long check_muls(std::string &line) {
  auto words_begin = std::sregex_iterator(line.begin(), line.end(), mul_regex);
  auto words_end = std::sregex_iterator();

  unsigned long long result = 0;
  for (std::sregex_iterator i = words_begin; i != words_end; i++) {
    std::smatch match = *i;
    std::smatch::iterator submatch_iter = match.begin();
    unsigned long long fac = 1;
    for (std::advance(submatch_iter, 1); submatch_iter != match.end();
         std::advance(submatch_iter, 1)) {
      fac *= std::stoi(*submatch_iter);
    }
    result += fac;
  }
  return result;
}

unsigned long long check_muls_with_conditional(std::string &line, bool *is_active) {
  auto words_begin = std::sregex_iterator(line.begin(), line.end(), mul_regex_with_conditionals);
  auto words_end = std::sregex_iterator();

  unsigned long long result = 0;
  for (std::sregex_iterator i = words_begin; i != words_end; i++) {
	std::smatch match = *i;
	std::smatch::iterator submatch_iter = match.begin();
	std::cout << *submatch_iter << "\n";
	if (!*is_active) {
		*is_active = *submatch_iter == "do()";
		continue;
	}
	if (*submatch_iter == "don't()") {
		*is_active = false;
		continue;
	}
	if (*submatch_iter == "do()") {
		continue;
	}
    unsigned long long fac = 1;
    for (std::advance(submatch_iter, 1); submatch_iter != match.end();
         std::advance(submatch_iter, 1)) {
      fac *= std::stoi(*submatch_iter);
    }
    result += fac;
  }
  return result;
}

int main() {
  std::ifstream input_file("03-1-input");
  if (!input_file.is_open()) {
    std::cout << "Input file couldn't be opened!\n";
    return 1;
  }
  std::string line;
  unsigned long long total = 0;
  unsigned long long total_with_conditional = 0;
  bool condition_is_active = true;
  while (getline(input_file, line)) {
    total += check_muls(line);
	total_with_conditional += check_muls_with_conditional(line, &condition_is_active);
  }
  std::cout << "Result: " << total << "\n";
  std::cout << "Result with conditionals: " << total_with_conditional << "\n";
  return 0;
}
