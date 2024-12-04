#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <format>

int main() {
	int a, b;
	std::vector<int> l1, l2;

	do {
		std::cin >> a >> b >> std::ws;
		// std::cout << std::format("a={}, b={}\n", a, b);
		l1.push_back(a);
		l2.push_back(b);
	} while (! std::cin.eof());

	std::sort (l1.begin(), l1.end());
	std::sort (l2.begin(), l2.end());

	// std::cout << std::format("lengths: {}, {}\n", l1.size(), l2.size());

	int sum = 0;
	for (size_t i = 0; i < l1.size(); i++) {
		// std::cout << std::format("|{} - {}| = {}\n", l1[i], l2[i], abs(l1[i] - l2[i]));
		sum += abs(l1[i] - l2[i]);
	}

	std::cout << std::format("sum = {}\n", sum);
}
