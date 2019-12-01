#include <bits/stdc++.h>
using namespace std;

int main () {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	int i;

	long sum = 0;
	int fuel;
	while (cin >> i) {
		fuel = (i / 3) - 2;

		int part = fuel;
		do {
			part = max(0, part / 3 - 2);
			fuel += part;
		} while (part > 0);

		sum += fuel;
	}

	cout << "sum = " << sum << "\n";


	return 0;
}
