#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
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

// Represents a range of indices [begin; end).
struct Range {
  // Default-constructed range is an empty one.
  Range() : begin(0), end(0) {}

  Range(size_t begin, size_t end) : begin(begin), end(end) {}

  size_t begin;
  size_t end;
};

bool is_empty(const Range &range) { return range.begin >= range.end; }

bool operator==(const Range &first, const Range &second) {
  return (is_empty(first) && is_empty(second)) ||
         (first.begin == second.begin && first.end == second.end);
}

// Represents a 2D point.
template <class T>
struct Point {
  typedef T Coordinate;

  Point(T x, T y) : x(x), y(y) {}

  T x;
  T y;
};

// Returns smallest integer of the form 2^n greater than or equal to given
// number.
// Throws if 2^n exceeds max value of size_t.
size_t NextPowerOfTwo(size_t number) {
  if (number >= std::numeric_limits<size_t>::max() / 2) {
    throw std::runtime_error("Next power of two exceeds type limit");
  }
  size_t result = 1;
  while (result < number) {
    result <<= 1;
  }
  return result;
}

// Represents partition of a line by given set of points.
// Consider N points of a line, they partition the line into (N + 1) parts
// numerated from 0 to N from left to right. Each point belongs to the part
// to its left.
// For example, 3 integer points [1, 2, 4] form the following partition:
//     #0: (-infty; 1]
//     #1: (1; 2]
//     #2: (2; 4]
//     #3: (4; +infty)
// Empty partition has a single part with index 0 - the whole line.
template <class T>
class LinePartition {
 public:
  LinePartition() = default;

  template <class Iterator>
  LinePartition(Iterator begin, Iterator end) {
    Assign(begin, end);
  }

  static LinePartition Unite(const LinePartition &first,
                             const LinePartition &second) {
    LinePartition result;
    auto &points = result.points_;
    points.resize(first.num_points() + second.num_points());
    std::merge(first.points_.begin(), first.points_.end(),
               second.points_.begin(), second.points_.end(), points.begin());
    points.erase(std::unique(points.begin(), points.end()), points.end());
    return result;
  }

  template <class Iterator>
  void Assign(Iterator begin, Iterator end) {
    points_.assign(begin, end);
    std::sort(points_.begin(), points_.end());
    points_.erase(std::unique(points_.begin(), points_.end()), points_.end());
  }

  size_t num_points() const { return points_.size(); }

  size_t part_index(const T &point) const {
    return std::distance(
        points_.begin(),
        std::lower_bound(points_.begin(), points_.end(), point));
  }

 private:
  std::vector<T> points_;
};

template <class Coordinate>
class ImmutableSegmentTree2DBuilder;

// Represents an immutable 2D segment tree: class for counting number of
// points within given rectangle, when the set of points is predefined.
template <class TCoordinate>
class ImmutableSegmentTree2D {
 public:
  typedef TCoordinate Coordinate;
  typedef ::Point<TCoordinate> Point;

  size_t CountPoints(TCoordinate x_begin, TCoordinate x_end, TCoordinate y_begin,
                     TCoordinate y_end) const {
    if (size_ == 0) {
      return 0;
    }

    const Range root_range(0, size_);
    const Range query_range(x_partition_.part_index(x_begin),
                            x_partition_.part_index(x_end));
    return CountPoints(kRootIndex, root_range, query_range, y_end) -
           CountPoints(kRootIndex, root_range, query_range, y_begin);
  }

 private:
  ImmutableSegmentTree2D() : size_(0) {}

  ImmutableSegmentTree2D(size_t size,
                         const LinePartition<TCoordinate> &x_partition,
                         const std::vector<LinePartition<TCoordinate>> &segments)
      : size_(size), x_partition_(x_partition), segments_(segments) {}

  friend class ImmutableSegmentTree2DBuilder<TCoordinate>;

  static const size_t kRootIndex = 1;

  static size_t left_child_index(size_t index) { return 2 * index; }

  static size_t right_child_index(size_t index) { return 2 * index + 1; }

  size_t CountPoints(size_t index, const Range &segment_range,
                     const Range &query_range, TCoordinate y_end) const {
    if (query_range.begin >= query_range.end) {
      return 0;
    }
    if (segment_range == query_range) {
      return segments_.at(index).part_index(y_end);
    }

    const size_t middle = (segment_range.begin + segment_range.end) / 2;
    return CountPoints(
               left_child_index(index), Range(segment_range.begin, middle),
               Range(query_range.begin, std::min(query_range.end, middle)),
               y_end) +
           CountPoints(
               right_child_index(index), Range(middle, segment_range.end),
               Range(std::max(query_range.begin, middle), query_range.end),
               y_end);
  }

  // number of leaf elements which is equal to number of non-leaf elements
  // but the element #0
  size_t size_;
  LinePartition<TCoordinate> x_partition_;
  std::vector<LinePartition<TCoordinate>> segments_;
};

template <class Coordinate>
class ImmutableSegmentTree2DBuilder {
 public:
  typedef ImmutableSegmentTree2D<Coordinate> SegmentTree;
  typedef ::Point<Coordinate> Point;

  template <class PointIterator>
  SegmentTree Build(PointIterator begin, PointIterator end) {
    LinePartition<Coordinate> x_partition;
    std::vector<Point> points(begin, end);
    {
      // all given points' X coordinates
      std::vector<Coordinate> xs(points.size());
      std::transform(points.begin(), points.end(), xs.begin(),
                     [](const Point &point) { return point.x; });
      x_partition.Assign(xs.begin(), xs.end());
    }

    const size_t size = NextPowerOfTwo(x_partition.num_points() + 1);
    std::vector<LinePartition<Coordinate>> segments(2 * size);
    {
      // all given points' Y coordinates grouped by X partition index
      std::vector<std::vector<Coordinate>> ys(size);
      for (const Point &point : points) {
        ys.at(x_partition.part_index(point.x)).push_back(point.y);
      }
      for (size_t i = 0; i < size; ++i) {
        segments.at(size + i).Assign(ys[i].begin(), ys[i].end());
      }
    }
    for (size_t i = size - 1; i > 0; --i) {
      segments.at(i) = LinePartition<Coordinate>::Unite(
          segments.at(SegmentTree::left_child_index(i)),
          segments.at(SegmentTree::right_child_index(i)));
    }
    return SegmentTree(size, x_partition, segments);
  }
};

template <class PointIterator>
ImmutableSegmentTree2D<
    typename std::iterator_traits<PointIterator>::value_type::Coordinate>
BuildImmutableSegmentTree2D(PointIterator begin, PointIterator end) {
  return ImmutableSegmentTree2DBuilder<typename std::iterator_traits<
      PointIterator>::value_type::Coordinate>()
      .Build(begin, end);
}

// Class which given a permutation 'p' preprocesses it in O(N) time
// and handles queries of the form:
// how many numbers of p([b; e)) = {p[i] for i in [b; e)} lie within [B; E)
// in O(log^2(N)) time using 2D segment tree.
class PermutationImageAnalyzer {
 public:
  size_t CountNumbers(size_t range_begin, size_t range_end, size_t image_begin,
                      size_t image_end) const {
    return segment_tree_.CountPoints(range_begin, range_end, image_begin,
                                     image_end);
  }

 private:
  typedef ImmutableSegmentTree2D<int> SegmentTree;

  explicit PermutationImageAnalyzer(const SegmentTree &segment_tree)
      : segment_tree_(segment_tree) {}

  friend class PermutationImageAnalyzerBuilder;

  SegmentTree segment_tree_;
};

class PermutationImageAnalyzerBuilder {
 public:
  PermutationImageAnalyzer Build(const Permutation &permutation) {
    std::vector<Point<int>> points;
    points.reserve(permutation.size());
    for (size_t i = 0; i < permutation.size(); ++i) {
      points.emplace_back(i, permutation[i]);
    }
    auto segment_tree =
        BuildImmutableSegmentTree2D(points.begin(), points.end());
    return PermutationImageAnalyzer(segment_tree);
  }
};

PermutationImageAnalyzer BuildPermutationImageAnalyzer(
    const Permutation &permutation) {
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
  auto f = [&x, size](size_t z) { return (z + x - 1) % size; };
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
        trivial_analyzer_(permutation),
        permutation_(permutation) {}

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

#define REQUIRE_EQUAL(first, second)                                         \
  do {                                                                       \
    auto firstValue = (first);                                               \
    auto secondValue = (second);                                             \
    if (firstValue != secondValue) {                                         \
      std::ostringstream oss;                                                \
      oss << "Require equal failed: " << #first << " != " << #second << " (" \
          << firstValue << " != " << secondValue << ")\n";                   \
      throw TestNotPassedException(oss.str());                               \
    }                                                                        \
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
