#include <map>
#include <string>

struct TempData {
    int count = 0;
    double total = 0;
    double min = 0;
    double max = 0;
};

class BrcMap {

  public:
    BrcMap() {}

    std::map<std::string, struct TempData> &getMap() { return dataMap; }

  private:
    std::map<std::string, struct TempData> dataMap;
};
