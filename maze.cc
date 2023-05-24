#include "maze.h"

static std::shared_ptr<std::vector<std::vector<char>>> Functionality(unsigned latitude, unsigned altitude) {
  if(latitude < 1 || altitude < 1) // проверяем ограничения параметров на ноль 
    return nullptr;
}
