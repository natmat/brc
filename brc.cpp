

#include <sys/stat.h>

#include <algorithm>
#include <chrono>
#include <fcntl.h>
#include <fstream> // <-- required for std::ifstream and file.rdbuf()
#include <iomanip>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <string>
#include <sys/mman.h>
#include <unistd.h>

struct TempData {
  int count = 0;
  double total = 0;
  double min = 0;
  double max = 0;
};

int countLinesInFile(std::ifstream &file) {
  std::istreambuf_iterator<char> begin(file.rdbuf());
  std::istreambuf_iterator<char> end;
  std::size_t lineCount = std::count(begin, end, '\n');

  file.clear();                 // clear EOF
  file.seekg(0, std::ios::beg); // rewind for reuse

  std::cout << "Number of lines: " << lineCount << "\n";
  return static_cast<int>(lineCount);
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
    return 1;
  }

  auto start = std::chrono::high_resolution_clock::now();

  std::string filename = argv[1];
  std::ifstream infile(filename);

  std::map<std::string, TempData> data;

  if (!infile.is_open()) {
    std::cerr << "Error: could not open file " << filename << std::endl;
    return 1;
  }

  // std::cout << countLinesInFile(infile) << std::endl;

  // auto end = std::chrono::high_resolution_clock::now();
  // auto elapsed = end - start;
  // std::chrono::duration<double> sec = elapsed;
  // std::cout << sec.count() << " seconds\n";
  // start = end;


  // mmap the file and process
  int fdin = open(filename.c_str(), O_RDONLY);

  struct stat statBuf;

  if (fstat(fdin, &statBuf) < 0) {
    std::cerr << "Error: fstat failed for file " << filename << std::endl;
    close(fdin);
    return 1;
  }

  auto addr = mmap(0, statBuf.st_size, PROT_READ, MAP_PRIVATE, fdin, 0);
  if (addr == MAP_FAILED) {
    std::cerr << "Error: mmap failed for file " << filename << std::endl;
    return 1;
  }
  close(fdin);

  constexpr char separator = ';';
  constexpr char newline = '\n';

  auto endAddr = static_cast<char *>(addr) + statBuf.st_size;
  char *current = static_cast<char *>(addr);
  while (*current != newline && current < endAddr) {
    std::string city;
    std::string tempStr;

    while (*current != separator) {
        city.push_back(*current);
        current++;
    }
    current++; // skip separator
    
    while (*current != newline && current < endAddr) {
      tempStr.push_back(*current);
      current++;
    }
    double temp = std::stod(tempStr);

    auto [iter, inserted] = data.try_emplace(city, TempData{0, 0.0});
    // std::cout << "Processing city: " << city << ", temp: " << temp << std::endl;

    iter->second.count += 1;
    iter->second.total += temp;
    iter->second.min = std::min(iter->second.min, temp);
    iter->second.max = std::max(iter->second.max, temp);

    if (current < endAddr) {
      current++; // skip newline
    }
  }

  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start; // seconds as double
  std::cout << elapsed.count() << " us\n";
  
  //   for (auto d : data) {
  //     std::cout << d.first << ": "
  //         << d.second.min << "/"
  //         << std::fixed << std::setprecision(1)
  //         << (d.second.total / d.second.count) << "/"
  //         << d.second.max << std::endl;
  //   }

  infile.close();
  return 0;
}
