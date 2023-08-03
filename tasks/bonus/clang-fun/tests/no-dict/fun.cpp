#include <iostream>
#include <vector>
#include <string>

class Test {
public:
	Test() = default;

	int Method() const {
		return 0;
	}

	static const double Static_Method() {
		return 0;
	}
private:
	int field_;
	double bad_bield;
	int64_t reallyBad_;

	static const int kGood = 5;
	static const int kBad_ = 3;
	static int bad_val;

	int privateMethod() {
		return 0;
	}
};
int Test::bad_val = 0;

struct JustAStruct {
	int ok_field;
	int bad_field_;
	const int kGood = 3;

	static int static_ok;

	static int var_;

	size_t size() const {
		return 0;
	}
};
int JustAStruct::static_ok = 0;
int JustAStruct::var_ = 1;

std::vector<int> BuildDSU(size_t size) {
	return {};
}

int BadShortAB() {
	static int dummy1;
	return 0;
}

constexpr int fail() {
	return -1;
}

std::string BIGNAME(int F) {
	return "";
}

typedef int Dash;
using shitHappens = int;

std::string OhHiMark(bool hi_) {
	return "Anyway, how is your sex life?";
}

int O(int o) {
	return o;
}

enum class Ok {
	A = 0, B = 1, C = 2
};

enum NotOK {
	FAIL = 0
};

int main() {
	constexpr int kValue = 0;

	int just__few_words = 1;
	double _hello_world = 2.0;

	int _ = just__few_words;
	int Wrong = 3;

	std::string STOP_RIGHT_THERE_CRIMINAL_SCUM;

	Wrong = 5;

	struct {
		int surprise;
		int hi_there_;

		union myunion {
			int first_chair_;
			int second_chair;
		};

	} ok_name;

	return O(0);
}