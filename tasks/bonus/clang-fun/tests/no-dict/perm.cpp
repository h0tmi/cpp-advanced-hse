
#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <vector>
#include <numeric>

// Represents a permutation of numbers {0, 1, ..., N - 1}.
typedef std::vector<size_t> Permutation;

Permutation Inverse(const Permutation &permutation) {
  Permutation result(permutation.size());
  for (size_t i = 0; i < permutation.size(); ++i) {
    result.at(permutation[i]) = i;
  }
  return result;
}

// Given two permutations E and I, returns their composition E o I.
Permutation Composition(const Permutation &external,
                        const Permutation &internal) {
  if (external.size() != internal.size()) {
    throw std::runtime_error("Cannot compose permutations of different sizes");
  }
  const size_t size = external.size();
  Permutation result(size);
  for (size_t i = 0; i < size; ++i) {
    result[i] = external[internal[i]];
  }
  return result;
}

// Effectively sums integers over requested segments.
// Uses long long for sums, so will work until it is overflown.
class ImmutableSummator {
public:
  typedef long long ValueType;

  template <class Iterator>
  ImmutableSummator(Iterator begin, Iterator end) : partial_sums_(1, 0) {
    // if input range A had N elements, partial sums has (N + 1) with the first
    // being 0 and i-th equal to sum of A[j] for j < i.

    // wtf
    //std::partial_sort(begin, end, std::back_inserter(partial_sums_));
  }

  ValueType operator()(size_t begin, size_t end) const {
    return partial_sums_.at(end) - partial_sums_.at(begin);
  }

private:
  std::vector<ValueType> partial_sums_;
};

// Represents a range of numbers: [begin; end).
struct Range {
  // Default-constructed range is an empty one.
  Range() : begin(0), end(0) {}

  Range(size_t begin, size_t end) : begin(begin), end(end) {}

  size_t begin;
  size_t end;
};

bool is_empty(const Range &range) { return range.begin >= range.end; }

bool operator==(const Range &first, const Range &second) {
  // empty ranges are equal no matter what their begin and end are exactly:
  // [0; 0) == [2; 2) == [3; 1) -- all of these represent an empty set.
  return (is_empty(first) && is_empty(second)) ||
         (first.begin == second.begin && first.end == second.end);
}

// Represents a decomposition of a numbers range into 3 parts:
// prefix number range, middle blocks range and suffix number range.
// Normally prefix and suffix indices are in source enumeration,
// that is correspond to the same indices of the input array, while
// blocksIndices is a range of indices of blocks presenting the middle
// part of a segment.
// For example, decomposing a range [2; 7) using blocks sized 2 can be
// [2; 2) as prefix - empty range, [1; 3) as blocks indices range - meaning
// blocks #1 and #2 which are [2; 4) and [4; 6) respectively, and
// [6; 7) as suffix.
struct Decomposition {
  Range prefix;
  Range blocks_indices;
  Range suffix;
};

// Converts block indices to original ones and vice versa.
// Non-negative integers are united into blocks with given size which
// are enumerated consecutively.
// For example, given size is equal to 3, the blocks are
//   #0 - [0; 3) = {0, 1, 2}
//   #1 - [3; 6) = {3, 4, 5}
//   #2 - [6; 9) = {6, 7, 8}
//   and so on.
// The last block can be incomplete, e.g. [9; 11) if total size is 11.
//
// Within this class index stands for block index and
// position stands for the index in the original array.
class BlockIndexer {
public:
  // Creates indexer with approx. sqrt(size) blocks with block
  // size equal to [ sqrt(size) ].
  explicit BlockIndexer(size_t size)
      : size_(size), block_size_(static_cast<size_t>(std::sqrt(size))) {}

  BlockIndexer(size_t size, size_t block_size)
      : size_(size), block_size_(block_size) {}

  size_t begin(size_t index) const { return index * block_size_; }

  size_t end(size_t index) const {
    return std::min(size_, (index + 1) * block_size_);
  }

  size_t index(size_t position) const { return position / block_size_; }

  size_t totalNumber() const { return (size_ - 1) / block_size_ + 1; }

  // Decomposes given range. It is guaranteed that prefix and suffix ranges
  // lengths
  // are no greater than block size, though can be equal to it.
  Decomposition Decompose(size_t begin_position, size_t end_position) const {
    Decomposition decomposition;
    if (begin_position >= end_position) {
      return decomposition;
    }

    const size_t first_block_index = index(begin_position);
    const size_t last_block_index = index(end_position - 1);

    if (first_block_index == last_block_index) {
      decomposition.prefix = Range(begin_position, end_position);
    } else {
      decomposition.prefix = Range(begin_position, end(first_block_index));
      decomposition.blocks_indices =
          Range(first_block_index + 1, last_block_index);
      decomposition.suffix = Range(begin(last_block_index), end_position);
    }
    return decomposition;
  }

private:
  size_t size_;
  size_t block_size_;
};

// Represents a numeric matrix.
class Matrix {
public:
  typedef int ValueType;
  typedef std::vector<ValueType>::const_iterator ColumnConstIterator;

  Matrix(size_t width, size_t height)
      : data_(width, std::vector<ValueType>(height)) {}

  ValueType operator()(size_t column, size_t row) const {
    return data_[column][row];
  }

  ValueType &operator()(size_t column, size_t row) {
    return data_[column][row];
  }

  ColumnConstIterator column_begin(size_t column) const {
    return data_[column].begin();
  }

  ColumnConstIterator column_end(size_t column) const {
    return data_[column].end();
  }

  size_t width() const { return data_.size(); }

  size_t height() const { return data_.empty() ? 0 : data_.front().size(); }

private:
  std::vector<std::vector<ValueType>> data_;
};

// Represents a summator which given a matrix handles queries
// of the form: sum a[x][i] over range y1 <= y < y2
// with O(1) complexity.
class ImmutableColumnSummator {
public:
  typedef ImmutableSummator::ValueType ValueType;

  explicit ImmutableColumnSummator(const Matrix &matrix) {
    column_summators_.reserve(matrix.width());
    for (size_t column = 0; column < matrix.width(); ++column) {
      column_summators_.emplace_back(matrix.column_begin(column),
                                     matrix.column_end(column));
    }
  }

  ValueType operator()(size_t column, size_t begin_row, size_t end_row) const {
    return column_summators_.at(column)(begin_row, end_row);
  }

private:
  std::vector<ImmutableSummator> column_summators_;
};

// Analyzes given permutation: calling it 'p', preprocesses it in O(N) time
// and handles queries of the form:
// how many numbers of p([b; e)) = {p[i] for i in [b; e)} lie within [B; E)
// in O(sqrt(N)) time using sqrt-decomposition.
class PermutationImageAnalyzer {
public:
  typedef size_t ValueType;

  // Returns the number of elements in the intersection of two sets:
  // permutation([rangeBegin; rangeEnd)) and [imageBegin; imageEnd).
  size_t CountNumbers(size_t range_begin, size_t range_end, size_t image_begin,
                      size_t image_end) const {
    const Decomposition range_decomposition =
        block_indexer_.Decompose(range_begin, range_end);
    const Decomposition image_decomposition =
        block_indexer_.Decompose(image_begin, image_end);

    ValueType count = 0;
    for (const Range &range :
         {range_decomposition.prefix, range_decomposition.suffix}) {
      count += CountNumbersTrivially(permutation_, range.begin, range.end,
                                     image_begin, image_end);
    }

    for (size_t block_index = range_decomposition.blocks_indices.begin;
         block_index < range_decomposition.blocks_indices.end; ++block_index) {
      count += static_cast<size_t>(blocks_summator_(
          block_index, image_decomposition.blocks_indices.begin,
          image_decomposition.blocks_indices.end));
    }

    if (range_decomposition.blocks_indices.begin <
        range_decomposition.blocks_indices.end) {
      for (const Range &range :
           {image_decomposition.prefix, image_decomposition.suffix}) {
        count += CountNumbersTrivially(
            permutation_,
            block_indexer_.begin(range_decomposition.blocks_indices.begin),
            block_indexer_.end(range_decomposition.blocks_indices.end - 1),
            range.begin, range.end);
      }
    }

    return count;
  }

private:
  friend class PermutationImageAnalyzerBuilder;

  PermutationImageAnalyzer(const Permutation &permutation,
                           const BlockIndexer &block_indexer,
                           const ImmutableColumnSummator &blocks_summator)
      : permutation_(permutation), block_indexer_(block_indexer),
        blocks_summator_(blocks_summator) {}

  // Returns answer to the query (see above) in O(input range length) time.
  static size_t CountNumbersTrivially(const Permutation &permutation,
                                      size_t range_begin, size_t range_end,
                                      size_t image_begin, size_t image_end) {
    if (image_begin >= image_end) {
      return 0;
    }

    size_t count = 0;
    for (size_t index = range_begin; index < range_end; ++index) {
      const size_t image = permutation[index];
      if (image_begin <= image && image < image_end) {
        ++count;
      }
    }
    return count;
  }

  Permutation permutation_;
  BlockIndexer block_indexer_;
  ImmutableColumnSummator blocks_summator_;
};

class PermutationImageAnalyzerBuilder {
public:
  PermutationImageAnalyzer Build(const Permutation &permutation) {
    BlockIndexer indexer(permutation.size());
    // matrix m[i][j] equal to the query answer (see above) for
    // i-th block as input range and j-th block as image range.
    Matrix matrix(indexer.totalNumber(), indexer.totalNumber());
    for (size_t i = 0; i < permutation.size(); ++i) {
      matrix(indexer.index(i), indexer.index(permutation[i])) += 1;
    }
    ImmutableColumnSummator blocks_summator(matrix);
    return PermutationImageAnalyzer(permutation, indexer, blocks_summator);
  }
};

// Shortcut for creating builder and calling its build method.
PermutationImageAnalyzer
BuildPermutationImageAnalyzer(const Permutation &permutation) {
  return PermutationImageAnalyzerBuilder().Build(permutation);
}

size_t ReadSize(std::istream &input_stream = std::cin) {
  size_t size;
  input_stream >> size;
  return size;
}

Permutation ReadPermutation(size_t size,
                            std::istream &input_stream = std::cin) {
  Permutation permutation(size);
  for (size_t &element : permutation) {
    input_stream >> element;
    // we expect the caller to use notation starting with 1,
    // while in C/C++ we always start from 0
    --element;
  }
  return permutation;
}

template <class Analyzer>
void ProcessQueries(const Analyzer &analyzer, size_t size) {
  const size_t n_queries = ReadSize();
  size_t x = 0;
  auto f = [&x, size_ = size](size_t z) { return (z + x - 1) % size_; };
  for (size_t i_query = 0; i_query < n_queries; ++i_query) {
    size_t a, b, c, d;
    std::cin >> a >> b >> c >> d;
    // We write (+ 1) to transform [begin; end] from the problem
    // to [begin; end + 1) used in our class.
    size_t count =
        analyzer.CountNumbers(std::min(f(a), f(b)), std::max(f(a), f(b)) + 1,
                              std::min(f(c), f(d)), std::max(f(c), f(d)) + 1);
    std::cout << count << "\n";
    x = count + 1;
  }
}

int TestAll();

int main() {
#ifdef TEST_MODE
  return TestAll();
#endif

  std::ios::sync_with_stdio(false);

  size_t size = ReadSize();
  Permutation first_permutation = ReadPermutation(size);
  Permutation second_permutation = ReadPermutation(size);
  Permutation single_permutation =
      Composition(Inverse(second_permutation), first_permutation);

  PermutationImageAnalyzer analyzer =
      BuildPermutationImageAnalyzer(single_permutation);
  ProcessQueries(analyzer, size);

  return 0;
}

// ===== TESTING ZONE =====

#include <cstdlib>
#include <sstream>
#include <string>

// Processes our usual queries with naive algorithm.
class TrivialAnalyzer {
public:
  explicit TrivialAnalyzer(const Permutation &permutation)
      : permutation_(permutation) {}

  size_t CountNumbers(size_t range_begin, size_t range_end, size_t image_begin,
                      size_t image_end) const {
    size_t count = 0;
    for (size_t i = range_begin; i < range_end; ++i) {
      const size_t image = permutation_.at(i);
      count += static_cast<size_t>(image_begin <= image && image < image_end);
    }
    return count;
  }

private:
  Permutation permutation_;
};

class TestNotPassedException : public std::runtime_error {
public:
  explicit TestNotPassedException(const char *what)
      : std::runtime_error(what) {}

  explicit TestNotPassedException(const std::string &what)
      : std::runtime_error(what.c_str()) {}
};

std::ostream &operator<<(std::ostream &os, const Permutation &permutation) {
  std::copy(permutation.begin(), permutation.end(),
            std::ostream_iterator<size_t>(os, " "));
  return os;
}

class Tester {
public:
  explicit Tester(const Permutation &permutation)
      : analyzer_(BuildPermutationImageAnalyzer(permutation)),
        trivial_analyzer_(permutation), permutation_(permutation) {}

  // throws exception if the test is not passed
  void operator()(size_t range_begin, size_t range_end, size_t image_begin,
                  size_t image_end) const {
    const size_t count =
        analyzer_.CountNumbers(range_begin, range_end, image_begin, image_end);
    const size_t correct_count = trivial_analyzer_.CountNumbers(
        range_begin, range_end, image_begin, image_end);
    if (count != correct_count) {
      std::ostringstream oss;
      oss << "Test not passed: got answer " << count
          << " while the correct one is " << correct_count << " for the mapping"
          << " of [" << range_begin << ", " << range_end << ")"
          << " to [" << image_begin << ", " << image_end << ");"
          << "\n\tthe permutation is " << permutation_ << "\n";
      throw TestNotPassedException(oss.str());
    }
  }

  size_t size() const { return permutation_.size(); }

private:
  PermutationImageAnalyzer analyzer_;
  TrivialAnalyzer trivial_analyzer_;
  Permutation permutation_;
};

int RandomInt(int begin, int end) { return rand() % (end - begin) + begin; }

Range RandomRange(size_t size) {
  size_t begin = RandomInt(0, size);
  size_t end = RandomInt(begin, size);
  return Range(begin, end);
}

void PerformRandomTest(const Tester &tester) {
  int size = tester.size();
  Range range = RandomRange(size);
  Range image = RandomRange(size);
  tester(range.begin, range.end, image.begin, image.end);
}

void TestIdentityPermutations(size_t max_size, size_t n_tests) {
  for (size_t size = 1; size <= max_size; ++size) {
    Permutation permutation(size);
    std::iota(permutation.begin(), permutation.end(), 0);
    Tester tester(permutation);
    for (size_t i_test = 0; i_test < n_tests; ++i_test) {
      PerformRandomTest(tester);
    }
  }
}

void TestRandomPermutations(size_t max_size, size_t n_permutations,
                            size_t n_tests_each) {
  for (size_t i_permutation = 0; i_permutation < n_permutations;
       ++i_permutation) {
    const size_t size = RandomInt(2, max_size);
    Permutation permutation(size);
    std::iota(permutation.begin(), permutation.end(), 0);
    std::random_shuffle(permutation.begin(), permutation.end());
    Tester tester(permutation);
    for (size_t i_test = 0; i_test < n_tests_each; ++i_test) {
      PerformRandomTest(tester);
    }
  }
}

#define REQUIRE_EQUAL(first, second)                                           \
  do {                                                                         \
    auto firstValue = (first);                                                 \
    auto secondValue = (second);                                               \
    if (firstValue != secondValue) {                                           \
      std::ostringstream oss;                                                  \
      oss << "Require equal failed: " << #first << " != " << #second << " ("   \
          << firstValue << " != " << secondValue << ")\n";                     \
      throw TestNotPassedException(oss.str());                                 \
    }                                                                          \
  } while (false)

void TestTrivialAnalyzer(size_t n_tests) {
  const size_t permutation[] = {0, 4, 3, 2, 1};
  const TrivialAnalyzer analyzer(
      Permutation(std::begin(permutation), std::end(permutation)));
  const size_t size =
      std::distance(std::begin(permutation), std::end(permutation));
  for (size_t i_test = 0; i_test < n_tests; ++i_test) {
    const Range range = RandomRange(size);
    for (size_t begin = 0; begin < size; ++begin) {
      // test empty input range
      REQUIRE_EQUAL(analyzer.CountNumbers(begin, begin, range.begin, range.end),
                    0);
      // test empty image range
      REQUIRE_EQUAL(analyzer.CountNumbers(range.begin, range.end, begin, begin),
                    0);
    }
  }
  REQUIRE_EQUAL(analyzer.CountNumbers(0, 1, 0, 3), 1);
  REQUIRE_EQUAL(analyzer.CountNumbers(0, 1, 0, 1), 1);
  REQUIRE_EQUAL(analyzer.CountNumbers(0, 5, 0, 3), 3);
  REQUIRE_EQUAL(analyzer.CountNumbers(1, 3, 0, 5), 2);
  REQUIRE_EQUAL(analyzer.CountNumbers(1, 4, 1, 4), 2);
  REQUIRE_EQUAL(analyzer.CountNumbers(2, 5, 1, 4), 3);
}

// Returns 0 if all tests passed and not 0 otherwise.
int TestAll() {
  std::srand(1543);

  try {
    TestTrivialAnalyzer(100);
    TestIdentityPermutations(10, 100);
    TestRandomPermutations(6, 100, 30);
    TestRandomPermutations(10, 100, 20);
    TestRandomPermutations(30, 100, 10);
    TestRandomPermutations(100, 10, 20);
    TestRandomPermutations(1000, 10, 5);
  } catch (std::exception &ex) {
    std::cerr << ex.what() << "\n";
    return 1;
  }
  return 0;
}
