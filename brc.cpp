#include <algorithm>
#include <chrono>
#include <fstream>     // <-- required for std::ifstream and file.rdbuf()
#include <iomanip>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <string>


struct TempData {
  int count = 0;
  double total = 0;
  double min = 0;
  double max = 0;
};


int countLinesInFile(std::ifstream &file)
{
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

  std::cout << countLinesInFile(infile) << std::endl;

  auto end = std::chrono::high_resolution_clock::now();
  auto elapsed = end - start;
  std::chrono::duration<double> sec = elapsed;
  std::cout << sec.count() << " seconds\n";
  start = end;


  std::string line;
  std::string city;

  // Read the file line by line
  while (std::getline(infile, line)) {
    if (line.empty())
      continue; // skip blank lines

    std::istringstream iss(line);
    std::string tempStr;

    // Split on ';'
    if (std::getline(iss, city, ';') && std::getline(iss, tempStr)) {
        double temp = std::stod(tempStr);
        auto [iter, inserted] = data.try_emplace(city, TempData{0, 0.0});
        iter->second.count += 1;
        iter->second.total += temp;
        iter->second.min = std::min(iter->second.min, temp);
        iter->second.max = std::max(iter->second.max, temp);
      } else {
        data.insert({city, {1, std::stod(tempStr)}});
    }
  }

  end = std::chrono::high_resolution_clock::now();
  elapsed = end - start;
  sec = elapsed;
  std::cout << sec.count() << " seconds\n";

  // for (auto d : data) {
  //   std::cout << d.first << ": " 
  //       << d.second.min << "/" 
  //       << std::fixed << std::setprecision(1)
  //       << (d.second.total / d.second.count) << "/" 
  //       << d.second.max << std::endl;
  // }

  infile.close();
  return 0;
}
