#include <bits/stdc++.h>
using namespace std;

enum directions { U, D, L, R };
struct point { int x, y;
	point operator+ (struct point& that) {
		return point { this->x + that.x,
		               this->y + that.y };

	}
};
struct cable_move {
	directions dir;
	int len = 0;
	point start, end;
	cable_move* next = nullptr;
};
enum state { dir, num };

cable_move* parse_move () {
	char str[0x1000];

	cin.getline (str, 0x1000);

	state st = dir;
	auto* root = new cable_move;
	auto* m = root;
	for (char* c = (char*) &str; *c != 0; c++) {
		switch (st) {
			case dir:
				switch (*c) {
					case 'R': m->dir = R; break;
					case 'U': m->dir = U; break;
					case 'D': m->dir = D; break;
					case 'L': m->dir = L; break;
				}
				st = num;
				break;
			case num:
				if (*c == ',') {
					st = dir;
					m->next = new cable_move;
					m = m->next;
					break;
				}
				m->len *= 10;
				m->len += *c - '0';
				break;
		}
	}

	int min_y = 0, max_y = 0, min_x = 0, max_x = 0;

	root->start = { 0, 0 };
	for (cable_move* c = root; c != nullptr; c = c->next) {
		point p;
		switch (c->dir) {
			case U:
				p = { 0, c->len };
				break;
			case D:
				p = { 0, - c->len };
				break;
			case L:
				p = { - c->len, 0 };
				break;
			case R:
				p = { c->len, 0 };
				break;
		}
		c->end = c->start + p;

		min_y = min(min_y, c->end.y);
		max_y = max(max_y, c->end.y);

		min_x = min(min_x, c->end.x);
		max_x = max(max_x, c->end.x);

		if (c->next) c->next->start = c->end;
	}

	cout << "min_y = " << min_y << ", max_y = " << max_y
		<< ", min_x = " << min_x << ", max_x = " << max_x << endl;

	return root;
}

int main () {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	auto* c1 = parse_move ();

	auto* c2 = parse_move ();

	return 0;
}
