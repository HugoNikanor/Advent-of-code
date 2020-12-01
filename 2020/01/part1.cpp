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

	/* runs in O(n²), θ(n). See unordered_set.contains */
	for (int m : numbers) {
		if (numbers.contains(2020 - m)) {
			std::cout << m << " * " << 2020 - m << " = " << m * (2020 - m) << std::endl;
			break;
		}
	}

	return 0;
}
