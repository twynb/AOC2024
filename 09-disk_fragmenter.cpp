#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

struct file_marker {
  unsigned long long id;
  // for part 1 uint8_t would've been enough, but part 2 maybe not
  uint16_t count;
  uint16_t space_after;
};

unsigned long long part_1(std::vector<file_marker> files) {
  size_t begin = 0;
  size_t end = files.size() - 1;
  unsigned long long result = 0;
  unsigned long long position = 0;
  while (begin <= end) {
    // go through all the blocks allocated by this file
    for (uint16_t i = 0; i < files[begin].count; i++) {
      result += files[begin].id * position;
      position++;
    }
    // then fill the free space afterwards with files from the end
    for (uint16_t i = 0; i < files[begin].space_after; i++) {
      // go to the next file if this one's all been written to the beginning
      if (files[end].count == 0) {
        end--;
      }
      if (end <= begin) {
        return result;
      }
      result += files[end].id * position;
      position++;
      files[end].count--;
    }
    begin++;
  }
  return result;
}

void move_file_marker(std::vector<file_marker> &files, size_t begin,
                      size_t end) {
  files[end - 1].space_after += files[end].count + files[end].space_after;
  files[end].space_after = files[begin].space_after - files[end].count;
  files[begin].space_after = 0;
  while (end - 1 > begin) {
    std::swap(files[end], files[end - 1]);
    end--;
  }
}

unsigned long long part_2(std::vector<file_marker> files) {
  // try to rearrange each marker
  size_t end = files.size() - 1;
  while (end > 0) {
    size_t begin = 0;
    bool did_swap = false;
    while (begin < end) {
      if (files[begin].space_after >= files[end].count) {
        move_file_marker(files, begin, end);
        did_swap = true;
        break;
      }
	  begin++;
    }
	// if we did swap, our next file got moved to the current end index
    if (!did_swap) {
      end--;
    }
  }

  unsigned long long position = 0;
  unsigned long long result = 0;
  for (file_marker &file : files) {
    // go through all the blocks allocated by this file
    for (uint16_t i = 0; i < file.count; i++) {
      result += file.id * position;
      position++;
    }
    position += file.space_after;
  }
  return result;
}

int main() {
  std::string line;
  std::ifstream input("09-1-input");
  if (!std::getline(input, line)) {
    std::cout << "The input couldn't be loaded!\n";
    return 1;
  }
  std::vector<file_marker> files = {};
  files.reserve(line.size() / 2 + 1);
  unsigned long long id = 0;
  for (size_t idx = 0; idx < line.size(); idx += 2) {
    uint16_t space_after = 0;
    if (idx + 1 < line.size()) {
      space_after = line[idx + 1] - '0';
    }
    files.push_back(file_marker{id, (uint16_t)(line[idx] - '0'), space_after});
    id++;
  }
  std::cout << "Part one: " << part_1(files) << "\n";
  std::cout << "Part two: ";
  std::cout << part_2(files);
  std::cout << "\n";
}
