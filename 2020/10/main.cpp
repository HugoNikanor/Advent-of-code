/*
 * Compile with `g++ -Wall this.cpp'
 */

#include <bits/stdc++.h>

int main () {

	std::priority_queue<int, std::vector<int>, std::greater<int>> q;

	int v;
	while (! std::cin.eof()) {
		std::cin >> v;
		q.push(v);
	}

	int counts[4] = { 0 };

	int last = 0;
	while (! q.empty()) {
		int v = q.top();
		q.pop();
		// std::cout << "v = " << v << ", last = " << last << std::endl;
		counts[v - last] += 1;
		last = v;
	}
	++counts[3];

	for (int i = 0; i < 4; i++) {
		std::cout << "counts[" << i << "] = " << counts[i] << std::endl;
	}
	std::cout << counts[1] * counts[3] << std::endl;

	return 0;
}
