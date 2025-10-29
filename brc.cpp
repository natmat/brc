#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

struct TempData {
  int count = 0;
  double total = 0;
  double min = 0;
  double max = 0;
};

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
    return 1;
  }

  std::string filename = argv[1];
  std::ifstream infile(filename);

  std::map<std::string, TempData> data;

  if (!infile.is_open()) {
    std::cerr << "Error: could not open file " << filename << std::endl;
    return 1;
  }

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

  for (auto d : data) {
    std::cout << d.first << ": " 
        << d.second.min << "/" 
        << std::fixed << std::setprecision(1)
        << (d.second.total / d.second.count) << "/" 
        << d.second.max << std::endl;
  }

  infile.close();
  return 0;
}
