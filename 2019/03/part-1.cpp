#include <bits/stdc++.h>
using namespace std;

enum directions { U, D, L, R };
struct point { int x, y; unsigned int steps = 0;

	point operator+ (struct point& that) {
		return point { this->x + that.x,
		               this->y + that.y };

	}

	point operator- (const point& that) {
		return point { this->x - that.x,
		               this->y - that.y };
	}

	friend ostream& operator<< (ostream&, point&);
};

ostream& operator<< (ostream& out, point& p) {
	out << "p{" << p.x << ", " << p.y << "}";
	return out;
}

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
	/*
min_y = -4687, max_y = 1833, min_x = -7900, max_x = 3339
min_y = -1872, max_y = 5673, min_x = -2385, max_x = 8124
	 */

	return root;
}

unsigned int arr[10361][16025];
point origin { 7900, 4687 };

void color (cable_move* root) {
	point cursor = origin;

	int steps = 0;

	for (cable_move* c = root; c != nullptr; c = c->next) {
		for (int i = 0; i < c->len; i++) {
			switch (c->dir) {
				case U: ++cursor.y; break;
				case D: --cursor.y; break;
				case L: --cursor.x; break;
				case R: ++cursor.x; break;
			}
			++steps;
			auto* ptr = &arr[cursor.y][cursor.x];
			if (*ptr == 0) *ptr = steps;
		}
	}

}

vector<point> find_pts (cable_move* root) {
	point cursor = origin;

	vector<point> pts;

	unsigned int steps = 0, stops;

	for (cable_move* c = root; c != nullptr; c = c->next) {
		for (int i = 0; i < c->len; i++) {
			switch (c->dir) {
				case U: ++cursor.y; break;
				case D: --cursor.y; break;
				case L: --cursor.x; break;
				case R: ++cursor.x; break;
			}
			++steps;
			if ((stops = arr[cursor.y][cursor.x])) {
				point n = cursor;
				n.steps = stops + steps;
				pts.push_back(n);
			}
		}
	}

	return pts;
}

int main () {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	auto* c1 = parse_move ();
	auto* c2 = parse_move ();


	color ( c1);
	auto res = find_pts ( c2);

	// int min_manhatan = INT_MAX;
	unsigned int min_steps = INT_MAX;
	for (auto i : res) {
		min_steps = min (min_steps, i.steps);
	}

	cout << "min steps = " << min_steps << endl;

	return 0;
}
