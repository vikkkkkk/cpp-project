#include "maze.h"

int main() {
  const unsigned latitude = 30;
  const unsigned altitude = 15;
  auto maze = Maze::Functionality(latitude, altitude);
  Maze::Output(maze);
  return 0;
}
