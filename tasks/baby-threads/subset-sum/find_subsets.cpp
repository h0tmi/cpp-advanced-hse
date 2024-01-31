#include "find_subsets.h"

#include <map>
#include <tuple>
#include <stdexcept>
#include <utility>
#include <vector>

int FindSubset(const std::vector<int64_t>& data, std::map<std::pair<int64_t, int64_t>, int64_t>& dp,
               int64_t difference, size_t id) {
    if (id == data.size()) {
        if (difference == 0) {
            return 0;
        }
        return -1e5;
    }
    if (dp.count({id, difference})) {
        return dp[{id, difference}];
    }
    int op1 = data[id] + FindSubset(data, dp, difference + data[id], id + 1);
    int op2 = FindSubset(data, dp, difference - data[id], id + 1);
    int op3 = FindSubset(data, dp, difference, id + 1);
    return dp[{id, difference}] = std::max({op1, op2, op3});
}

Subsets FindEqualSumSubsets(const std::vector<int64_t>& data) {
    std::map<std::pair<int64_t, int64_t>, int64_t> dp;
    Subsets result;
    result.exists = (FindSubset(data, dp, 0, 0) == 0 ? false : true);
    return result;
}
