#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <format>
#include <sstream>
#include <ranges>

int sgn(int x) {
	return abs(x) / x;
}

bool check_safe (const std::vector<int> &report) {
	int last = report[0];
	/* Positive number means growing, negative means falling */
	int direction = sgn(report[1] - last);
	for (int el : report | std::views::drop(1)) {
		if (sgn(el - last) != direction)
			return false;
		if (abs(el - last) < 1)
			return false;
		if (abs(el - last) > 3)
			return false;
		last = el;
	}
	return true;
}

int main () {

	std::vector<std::vector<int>> reports;

	std::string line;
	do {
		std::getline(std::cin, line);
		if (line.size() == 0) break;
		auto strm = std::istringstream (line);
		int num;
		std::vector<int> report;
		do {
			strm >> num;
			report.push_back(num);
		} while (!strm.eof());
		reports.push_back(report);
	} while (!std::cin.eof());

	int safe_count = 0;
	for (auto &report : reports) {
		bool is_safe = check_safe (report);
		for (auto el : report) {
			std::cout << el << ", ";
		}

		std::cout << (is_safe ? "Safe" : "Unsafe") << "\n";

		safe_count += is_safe;
	}

	std::cout << safe_count << std::endl;

}
