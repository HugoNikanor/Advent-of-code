/*
 * Compile with `g++ -Wall this.cpp'
 */

#include <bits/stdc++.h>
// using namespace std;

int num[6] = { 0 };
int cc = 0;
std::vector<int> my_nums;

int min_code = 178416;
int max_code = 676461;

int tonum(int num[6]) {
	int out = 0;
	for (int i = 0; i < 6; i++) {
		out *= 10;
		out += num[i];
	}
	return out;
}

void g(int prev, int pos) {
	for (int j = prev; j < 10; j++) {
		num[pos] = j;
		if (pos == 5) {

			int count = 1;
			int last = num[0];
			for (int i = 1; i < 6; i++) {
				if (num[i] == last) ++count;
				else {
					if (count == 2) {
						int n = tonum(num);
						if (min_code <= n && n <= max_code) {
							++cc;
							my_nums.push_back(n);
							count = 0;
							break;
						}
					}
					count = 1;
				}

				last = num[i];
			}

			if (count == 2) {
				int n = tonum(num);
				if (min_code <= n && n <= max_code) {
					++cc;
					my_nums.push_back(n);
					// bryt mycket mycket mer
					// break;
				}
			}


		} else {
			g(j, pos + 1);
		}
	}
}

void f () {
	for (int i = 1; i <= 6; i++) {
		num[0] = i;
		g(i, 1);
	}
}


int main () {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(NULL);


	f ();

	for (int i : my_nums) {
		std::cout << i << ", " << std::endl;
	}
	std::cout << "len = " << my_nums.size() << std::endl;
	std::cout << "Count = " << cc << std::endl;


	return 0;
}
