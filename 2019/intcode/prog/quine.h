int64_t prog[] = {
	109,1,204,-1,1001,100,1,100,1008,100,16,101,1006,101,0,99
};

/*
int64_t prog[] = {
	srb #1
	put (bp - 1) // # 0
	add mem[100] = mem[100] + 1
	eq mem[101] = mem[100] == #16
	jnq 0 if mem[101]
	halt
}
*/
