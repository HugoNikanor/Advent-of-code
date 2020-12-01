/*
 * Compile with `g++ -Wall this.cpp'
 */

#include <bits/stdc++.h>
// using namespace std;

int main () {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(NULL);

	std::unordered_set<int> numbers;
	int n;
	while (std::cin >> n) {
		numbers.insert(n);
	}

	/* O(n³), θ(n²), see part1
	for (int a : numbers) {
		for (int b : numbers) {
			if (numbers.contains(2020 - a - b)) {
				std::cout << a << " * " << b << " * " << 2020 - a - b << " = " << a * b * (2020 - a - b) << std::endl;
				// break;
			}
		}
	}

	return 0;
}
