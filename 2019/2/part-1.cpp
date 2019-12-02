#include <bits/stdc++.h>
using namespace std;

int mem[] = {
	// 1, 0, 0, 0, 99

	// 2,3,0,3,99

	// 2,4,4,5,99,0

	// 1,1,1,4,99,5,6,0,99

	1  , 0   , 0   , 3   ,
	1  , 1   , 2   , 3   ,
	1  , 3   , 4   , 3   ,
	1  , 5   , 0   , 3   ,
	2  , 1   , 10  , 19  ,
	1  , 6   , 19  , 23  ,
	2  , 23  , 6   , 27  ,
	2  , 6   , 27  , 31  ,
	2  , 13  , 31  , 35  ,
	1  , 10  , 35  , 39  ,
	2  , 39  , 13  , 43  ,
	1  , 43  , 13  , 47  ,
	1  , 6   , 47  , 51  ,
	1  , 10  , 51  , 55  ,
	2  , 55  , 6   , 59  ,
	1  , 5   , 59  , 63  ,
	2  , 9   , 63  , 67  ,
	1  , 6   , 67  , 71  ,
	2  , 9   , 71  , 75  ,
	1  , 6   , 75  , 79  ,
	2  , 79  , 13  , 83  ,
	1  , 83  , 10  , 87  ,
	1  , 13  , 87  , 91  ,
	1  , 91  , 10  , 95  ,
	2  , 9   , 95  , 99  ,
	1  , 5   , 99  , 103 ,
	2  , 10  , 103 , 107 ,
	1  , 107 , 2   , 111 ,
	1  , 111 , 5   , 0   ,
	99 , 2   , 14  , 0   , 0
};

int ogmem[sizeof(mem)];

enum opcode {
	add = 1, // r1, r2, r3
	mul = 2, // r1, r2, r3
	halt = 99, //
};

struct instr {
	opcode op;
	int r1, r2, r3;
};

void printmem (size_t len, int* mem) {
	instr* in = (instr*) mem;
	for (size_t i = 0; i < len / 4; i++) {
		cout << setw(3) << i << ": ";
		switch (in->op) {
			case add:  cout << "add"; break;
			case mul:  cout << "mul"; break;
			case halt: cout << "halt"; break;
			default: cout << "?? " << in->op; break;
		}
		cout << "\tr" << in->r1 << ",\tr" << in->r2 << ",\tr" << in->r3 << endl;
		in++;
	}
	for (size_t i = 0; i < len % 4; i++) {
		cout << mem[i + (len/4) * 4] << " ";
	}
	cout << endl << "result = " << mem[0] << endl;
}

int run_asm (int r1, int r2) {
	mem[1] = r1;
	mem[2] = r2;

	instr* instrs = (instr*) &mem;
	instr* in = instrs;

	while (in->op != halt) {
#ifdef debug
		cout << "mem[" << in->r3 << "] = " << in->r1 << " " <<
			( in->op == add ? "+"
			  : in->op == mul ? "*"
			  : in->op == halt ? "!!"
			  : "?" )
			<< " " << in->r2 << endl;
#endif
		switch (in->op) {
			case add:
				mem[in->r3] = mem[in->r1] + mem[in->r2];
				break;
			case mul:
				mem[in->r3] = mem[in->r1] * mem[in->r2];
				break;
			case halt:
				return mem[0];
		}
		in++;
	}

	return mem[0];
}

int main () {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	memcpy(ogmem, mem, sizeof(mem));

	// run_asm(12, 2);

	int result;
	for (int i = 0; i < 99; i++) {
		for (int j = 0; j < 99; j++) {
			memcpy(mem, ogmem, sizeof(mem));
			cout << i << ", " << j << " = ";
			result = run_asm(i, j);
			cout << result << endl;
			if (result == 19690720) {
				cout << "100 * i + j = " << 100 * i + j << endl;
				return 0;
			} else if (result == -1) {
				cout << "ERR\n";
				return 1;
			}
		}
	}

	return 1;
}
