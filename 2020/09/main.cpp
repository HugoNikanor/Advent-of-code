/*
 * Compile with `g++ -Wall this.cpp'
 */

#include <bits/stdc++.h>
// using namespace std;

bool find_pair(std::vector<int64_t>& q, size_t start, size_t len, int64_t target) {
	for (size_t i = start; i < start + len; i++) 
		for (size_t j = start; j < start + len; j++) {
			auto a = q[i];
			auto b = q[j];
			if (a == b) continue;
			if (a + b == target) return true;
		}
	return false;
}

std::pair<size_t, size_t> find_sum(std::vector<int64_t>& numbers, int64_t target) {
	size_t start = 0;
	size_t end = 0;
	int64_t sum = numbers[0];

	while (sum != target && end < numbers.size()) {
		if (sum < target) {
			sum += numbers[++end];
		} else if (sum > target) {
			sum -= numbers[start++];
		}
	}

	if (sum == target) {
		return { start, end };
	} else {
		return { 0, 0 };
	}
}

int main (int argc, char* argv[]) {
	// ios_base::sync_with_stdio(false);
	// cin.tie(NULL);

	size_t bufsize = 25;

	if (argc > 1) {
		bufsize = std::atoi(argv[1]);
	}

	std::vector<int64_t> numbers;
	while (! std::cin.eof()) {
		int64_t v;
		std::cin >> v;
		numbers.push_back(v);
	}

	int64_t invalid;
	for (size_t idx = 0; idx < numbers.size() - bufsize; idx++) {
		int64_t v = numbers[idx + bufsize];
		if (! find_pair(numbers, idx, bufsize, v)) {
			invalid = v;
			std::cout << "invalid: " << v << std::endl;
		}
	}

	auto p = find_sum(numbers, invalid);
	int64_t max = numbers[p.first];
	int64_t min = numbers[p.first];
	std::cout << numbers[p.first];
	for (size_t i = p.first + 1; i < p.second + 1; i++) {
		std::cout << ", " << numbers[i];
		max = std::max(max, numbers[i]);
		min = std::min(min, numbers[i]);
	}
	std::cout << std::endl;
	std::cout 
		<< "max = " << max << ", min = " << min << std::endl
		<< max << " + " << min << " = " << max + min << std::endl;


	return 0;
}
