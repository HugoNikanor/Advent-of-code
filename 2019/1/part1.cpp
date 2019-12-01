#include <bits/stdc++.h>
using namespace std;

int main () {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	int i;

	long sum = 0;
	while (cin >> i) {
		sum += (i / 3) - 2;
	}

	cout << "sum = " << sum << "\n";


	return 0;
}
