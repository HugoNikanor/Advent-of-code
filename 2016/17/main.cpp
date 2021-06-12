#include <iostream>
#include <string>
#include <queue>

#include <openssl/md5.h>

enum direction {
	up = 0, down = 1, left = 2, right = 3
};

const char* dir_string (direction dir) {
	switch (dir) {
		case up    : return "U";
		case down  : return "D";
		case left  : return "L";
		case right : return "R";
	}
}

struct item {
	std::string path;
	int x, y;

	bool operator< (const item& that) const {
		// return this->x + this->y < that.x + that.y;
		return this->path.length() > that.path.length();
	}
};

int main() {

	std::string passcode;
	std::cin >> passcode;

	std::priority_queue<item> q;
	q.push({ "", 0, 0 });


	while (! q.empty()) {

		std::string path = q.top().path;
		int x = q.top().x;
		int y = q.top().y;
		q.pop();

		if (x == 3 && y == 3) {
			std::cout << "Found path!\n" << path << std::endl; 
			break;
		}

		// std::cout << "\n" << x << ", " << y << " [" << path << "]" << std::endl;

		std::string s = passcode + path;

		unsigned char buf[MD5_DIGEST_LENGTH];
		MD5((unsigned char*) s.c_str(), s.length(), buf);

		for (int i = 0; i < 4; i++) {
			direction dir = static_cast<direction>(i);
			int x_ = x, y_ = y;
			switch (dir) {
				case up:    y_ = y - 1; break;
				case down:  y_ = y + 1; break;
				case left:  x_ = x - 1; break;
				case right: x_ = x + 1; break;
			}
			int status; 
			if (i % 2 == 0) {
				status = (buf[i / 2] & 0xF0) >> 4;
			} else {
				status = buf[i / 2] & 0xF;
			}
			// std::cout << std::hex << status;
			if (x_ < 0 || x_ > 3 || y_ < 0 || y_ > 3) {
				continue;
			}
			if (status > 0xa) {
				/* open */
				// std::cout << "Adding " << x_ << ", " << y_ << "\n";
				q.push({ path + dir_string(dir), x_, y_});
			} else {
				/* closed */
				continue;
			}

		}

	}
	// std::cout << "Got stuck\n";

}
