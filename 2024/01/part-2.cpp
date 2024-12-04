#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <format>
#include <unordered_map>

int main() {
	int a, b;

	std::vector<int> left;
	std::unordered_map<int, int> right;

	do {
		std::cin >> a >> b >> std::ws;
		left.push_back(a);
		right[b]++;
	} while (! std::cin.eof());

	long similarity_score = 0;
	for (auto el : left) {
		similarity_score += el * right[el];
	}

	std::cout << similarity_score << std::endl;


	return 0;
}
