/*
 * Compile with `g++ -Wall this.cpp'
 */

#include <bits/stdc++.h>

int main () {

	std::priority_queue<int64_t> q;

	int64_t v;
	while (! std::cin.eof()) {
		std::cin >> v;
		q.push(v);
	}

	q.push(0);
	
	int64_t max = q.top();
	std::vector<int64_t> steps (max + 4, 0);
	steps[max + 3] = 1;

	while (! q.empty()) {
		int64_t v = q.top();
		q.pop();

		steps[v] = 0; /* not setting this gives answer * 2 ... */
		for (int64_t i = 1; i <= 3; i++) {
			steps[v] += steps[v + i];
		}
	}

	std::cout << steps[0] << std::endl;

	return 0;
}
