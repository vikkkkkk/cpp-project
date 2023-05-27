#pragma once
#include <iostream>
#include <memory>
#include <random>
#include <vector>

class Maze {
public:
  static std::shared_ptr<std::vector<std::vector<char>>>
  Functionality(unsigned latitude, unsigned altitude) {
    if ((latitude < 1) || (altitude < 1))
      return nullptr;

    const unsigned outputAltitude = altitude * 2 + 1;
    const unsigned outputLatitude = latitude * 2 + 1;
    auto cave = std::make_shared<std::vector<std::vector<char>>>();
    cave->reserve(outputAltitude);

    std::vector<unsigned> rowSet;
    rowSet.reserve(latitude);
    for (unsigned i = 0; i < latitude; ++i)
      rowSet.push_back(0);

    initializecave(cave, outputAltitude, outputLatitude);
    createPaths(cave, latitude, altitude);
    removeDeadEnds(cave, latitude, outputAltitude, rowSet);
    connectLastRow(cave, latitude, outputAltitude, rowSet);

    return cave;
  }

  static void
  Output(const std::shared_ptr<std::vector<std::vector<char>>> &cave) {
    if (cave == nullptr)
      return;

    for (unsigned i = 0; i < cave->size(); ++i) {
      for (unsigned j = 0; j < cave->at(0).size(); ++j)
        std::cout << cave->at(i).at(j);
      std::cout << std::endl;
    }
  }

private:
  Maze() = default;

  static void
  initializecave(std::shared_ptr<std::vector<std::vector<char>>> &cave,
                 unsigned outputAltitude, unsigned outputLatitude) {
    for (unsigned i = 0; i < outputAltitude; ++i) {

      std::vector<char> row;
      row.reserve(outputLatitude);
      for (unsigned j = 0; j < outputLatitude; ++j)

        if ((i % 2 == 1) && (j % 2 == 1))
          row.push_back(' ');
        else if (((i % 2 == 1) && (j % 2 == 0) && (j != 0) &&
                  (j != outputLatitude - 1)) ||
                 ((j % 2 == 1) && (i % 2 == 0) && (i != 0) &&
                  (i != outputAltitude - 1)))
          row.push_back(' ');
        else
          row.push_back('@');
      cave->push_back(std::move(row));
    }
  }

  static void createPaths(std::shared_ptr<std::vector<std::vector<char>>> &cave,
                          unsigned latitude, unsigned altitude) {
    std::vector<unsigned> rowSet;
    rowSet.reserve(latitude);
    for (unsigned i = 0; i < latitude; ++i)
      rowSet.push_back(0);
    unsigned set = 1;

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(0, 2);

    for (unsigned i = 0; i < altitude; ++i) {
      for (unsigned j = 0; j < latitude; ++j)
        if (rowSet[j] == 0)
          rowSet[j] = set++;

      for (unsigned j = 0; j < latitude - 1; ++j) {
        const auto right_wall = dist(mt);
        if ((right_wall == 1) || (rowSet[j] == rowSet[j + 1]))
          cave->at(i * 2 + 1).at(j * 2 + 2) = '@';
        else {
          const auto changing_set = rowSet[j + 1];
          for (unsigned l = 0; l < latitude; ++l)
            if (rowSet[l] == changing_set)
              rowSet[l] = rowSet[j];
        }
      }

      for (unsigned j = 0; j < latitude; ++j) {
        const auto bottomWall = dist(mt);
        unsigned int countCurrentSet = 0;
        for (unsigned l = 0; l < latitude; ++l)
          if (rowSet[j] == rowSet[l])
            countCurrentSet++;
        if ((bottomWall == 1) && (countCurrentSet != 1))
          cave->at(i * 2 + 2).at(j * 2 + 1) = '@';
      }

      if (i != altitude - 1) {
        for (unsigned j = 0; j < latitude; ++j) {
          unsigned countHole = 0;
          for (unsigned l = 0; l < latitude; ++l)
            if ((rowSet[l] == rowSet[j]) &&
                (cave->at(i * 2 + 2).at(l * 2 + 1) == ' '))
              countHole++;
          if (countHole == 0)
            cave->at(i * 2 + 2).at(j * 2 + 1) = ' ';
        }
        for (unsigned j = 0; j < latitude; ++j)
          if (cave->at(i * 2 + 2).at(j * 2 + 1) == '@')
            rowSet[j] = 0;
      }
    }
  }

  static void
  removeDeadEnds(std::shared_ptr<std::vector<std::vector<char>>> &cave,
                 unsigned latitude, unsigned outputAltitude,
                 const std::vector<unsigned> &rowSet) {
    for (unsigned int i = 0; i < latitude - 1; ++i) {
      if (rowSet[i] != rowSet[i + 1])
        cave->at(outputAltitude - 2).at(i * 2 + 2) = ' ';
    }
  }

  static void
  connectLastRow(std::shared_ptr<std::vector<std::vector<char>>> &cave,
                 unsigned latitude, unsigned outputAltitude,
                 const std::vector<unsigned> &rowSet) {
    for (unsigned j = 0; j < latitude - 1; ++j) {
      if (rowSet[j] != rowSet[j + 1])
        cave->at(outputAltitude - 2).at(j * 2 + 2) = ' ';
    }
  }
};
