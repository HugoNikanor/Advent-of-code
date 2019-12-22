struct file_table_entry {
	char magic[8];
	int64_t mode; // does file exists?
	u_int64_t inode;
	char name[0x100 - 8 - 2 *sizeof(int64_t)];
};

struct fs_header {
	char magic[4];
	u_int8_t future_use[16 * 5 + 12];
};

struct file {
	int64_t links;
	int64_t data[0x1000];
};

struct filesystem {
	struct fs_header fs_head;
	struct file_table_entry head[0x100];
	char sep[8];
	struct file files[0x100];
};

struct file_table_entry* find_file_head (struct filesystem* fs, const char* name) {
	for (int i = 0; i < 0x100; i++) {
		if (strcmp(fs->head[i].name, name) == 0) {
			return &fs->head[i];
		}
	}
	return NULL;
}

struct file_table_entry* find_free_head (struct filesystem* fs) {
	for (int i = 0; i < 0x100; i++) {
		if (fs->head[i].mode == 0) {
			return &fs->head[i];
		}
	}
	return NULL;
}

int find_free_file (struct filesystem* fs) {
	for (int i = 0; i < 0x100; i++) {
		if (fs->files[i].links == 0) {
			++fs->files[i].links;
			return i;
		}
	}

	return -1;
}
