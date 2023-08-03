#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

void BubbleSort(std::vector<int> *sequence) {
  for (size_t iteration = 0;
       iteration + 1 < sequence->size();
       ++iteration) {
    for (size_t i = 0; i + iteration + 1 < sequence->size(); ++i) {
      if ((*sequence)[i] > (*sequence)[i + 1]) {
        std::swap((*sequence)[i], (*sequence)[i + 1]);
      }
    }
  }
}

void SelectionSort(std::vector<int> *sequence) {
  for (size_t left_border = 0;
       left_border + 1 < sequence->size();
       ++left_border) {
    size_t min_element_index = std::min_element(sequence->begin() + left_border,
                                                sequence->end())
                                                - sequence->begin();
    std::swap((*sequence)[left_border], (*sequence)[min_element_index]);
  }
}

void OutputSequence(const std::string &name,
                    const std::vector<int> &sequence) {
  std::cerr << name;
  for (int x : sequence) {
    std::cerr << ' ' << x;
  }
  std::cerr << std::endl;
}

void TestBubbleSort(const std::vector<int> &test_sequence,
                    const std::vector<int> &correct_answer) {
  auto sequence = test_sequence;
  BubbleSort(&sequence);
  if (sequence != correct_answer) {
    std::cerr << "Test failed" << std::endl;
    OutputSequence("Test:", test_sequence);
    OutputSequence("Our result:", sequence);
    OutputSequence("Correct answer:", correct_answer);
    throw std::runtime_error("Test failed!");
  }
  //std::cerr << "Passed test!" << std::endl;
}

void RunUnitTests() {
  TestBubbleSort({}, {});

  TestBubbleSort({ 1 }, { 1 });

  TestBubbleSort({ 2, 1 }, { 1, 2 });

  TestBubbleSort({ 1, 2, 3 }, { 1, 2, 3 });

  TestBubbleSort({ 3, 2, 1 }, { 1, 2, 3 });

  TestBubbleSort({ 1, 2, 2, 1 }, { 1, 1, 2, 2 });

  TestBubbleSort({ 4, 3, 5, 1, 2 },
                 { 1, 2, 3, 4, 5 });

  std::cerr << "Passed unit tests!" << std::endl;
}

// RVO - Return Value Optimization
std::vector<int> GetRandomSequence(int size, int modulo) {
  std::vector<int> sequence(size);
  for (int &element : sequence) {
    element = rand() % modulo;
  }
  return sequence;
}

void RunStressTest(int max_size = 10,
                   int max_modulo = 10,
                   int num_attempts = 1000) {
  srand(1234);
  for (int size = 0; size <= max_size; ++size) {
    std::cerr << "size: " << size << std::endl;
    for (int modulo = 1; modulo <= max_modulo; ++modulo) {
      std::cerr << "modulo: " << modulo << std::endl;
      for (int attempt = 0; attempt < num_attempts; ++attempt) {
        auto test_sequence = GetRandomSequence(size, modulo);
        auto correct_answer = test_sequence;
        SelectionSort(&correct_answer);
        TestBubbleSort(test_sequence, correct_answer);
      }
    }
  }
  std::cerr << "Passed stress test!" << std::endl;
}

int main() {
  
  RunUnitTests();
  RunStressTest();

  return 0;
}