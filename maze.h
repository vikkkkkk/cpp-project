#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <random>

class s21 {
 public:
	static std::shared_ptr<std::vector<std::vector<char>>> Functionality(unsigned width, unsigned height);
  static void Field(const std::shared_ptr<std::vector<std::vector<char>>>& maze);

 private:
	s21();
};