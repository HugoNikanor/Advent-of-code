#define FUSE_USE_VERSION 26

#include <errno.h>
#include <fuse.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

#include "fs.h"

struct filesystem* fs;

static int getattr_f (const char* path, struct stat* st) {
	(void) path;
	memset(st, 0, sizeof(struct stat));

	if (strcmp(path, "/") == 0) {
		st->st_mode = S_IFDIR | 0755;
		st->st_nlink = 2;
		return 0;
	}
	
	struct file_table_entry* head = find_file_head(fs, path + 1);
	if (head == NULL) {
		return -ENOENT;
	}

	st->st_mode = S_IFREG | head->mode;
	st->st_nlink = fs->files[head->inode].links;
	st->st_size = 0x1000;
	return 0;

}

static int read_dir_f(const char* path, void* buf,
		fuse_fill_dir_t filler, off_t offset, struct fuse_file_info* fi) {
	(void) fi;
	(void) offset;
	if (strcmp(path, "/") != 0) return 0;

	for(int i = 0; i < 0x100; i++) {
		if (fs->head[i].mode != 0) {
			filler (buf, fs->head[i].name, NULL, 0);
		}
	}

	return 0;
	
}

static int read_f (const char* path, char* buf, size_t size, off_t offset, struct fuse_file_info* fi) {
	(void) fi;

	struct file_table_entry* entry = find_file_head (fs, path + 1);
	if (entry == NULL) {
		return -ENOENT;
	}

	memcpy (buf, fs->files[entry->inode].data + offset, size);

	return size;
}

static int open_f (const char* path, struct fuse_file_info* fi) {
	(void) path;
	(void) fi;
	return 0;
}

static int create_f (const char* path, mode_t mode, struct fuse_file_info* info) {
	(void) info;
	(void) mode;
	const char* name = path + 1;

	struct file_table_entry* entry;

	entry = find_file_head (fs, name);

	if (entry == NULL) {
		entry = find_free_head (fs);
	}

	entry->mode = 1;
	strcpy(entry->name, name);

	int i = find_free_file (fs);
	entry->inode = i;

	return 0;

}

static int release_f (const char* path, struct fuse_file_info* info) {
	(void) info;

	struct file_table_entry* entry = find_file_head (fs, path + 1);
	if (entry == NULL) {
		return -ENOENT;
	}

	// --fs->files[entry->inode].links;

	// entry->mode = 1;

	return 0;
}

static int unlink_f (const char* path) {
	struct file_table_entry* entry = find_file_head (fs, path + 1);
	if (entry == NULL) {
		return -ENOENT;
	}
	--fs->files[entry->inode].links;

	entry->mode = 0;

	return 0;
}

static int write_f (const char* path, const char* indata, size_t len, off_t off, struct fuse_file_info* info) {
	(void) info;

	struct file_table_entry* entry = find_file_head (fs, path + 1);
	if (entry == NULL) {
		return -ENOENT;
	}

	int64_t* data = fs->files[entry->inode].data;

	memcpy(data + off, indata, len);

	return len;
}

static int rename_f (const char* from, const char* to) {
	struct file_table_entry* entry = find_file_head (fs, from + 1);
	if (entry == NULL) {
		return -ENOENT;
	}
	memset(entry, 0, sizeof(entry->name));
	strcpy(entry->name, to);

	return 0;
}

static struct fuse_operations fuse_ops = {
	.getattr = getattr_f,
	.open = open_f,
	.release = release_f,
	.read = read_f,
	.readdir = read_dir_f,
	.create = create_f,
	.unlink = unlink_f,
	.write = write_f,
	.rename = rename_f,
};


int main (int argc, char* argv[argc]) {

	if (argc < 3) exit(1);

	const char* path = argv[argc - 1];
	argc--;

	int fd = open(path, O_CREAT|O_RDWR, S_IRWXU);

	ftruncate(fd, sizeof(struct filesystem));

	errno = 0;
	fs = mmap (NULL, sizeof(struct filesystem), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if (errno != 0) {
		printf("errno = %i\n", errno);
		exit(1);
	}

	if (strcmp(fs->fs_head.magic, "HUGO") != 0) {
		strcpy(fs->fs_head.magic, "HUGO");
		for (int i = 0; i < 0x100; i++) {
			fs->head[i].magic[0] = '(';
			fs->head[i].magic[7] = ')';
			fs->head[i].mode = 0;
		}
		memset(fs->sep, '=', 8);
	}

	return fuse_main (argc, argv, &fuse_ops, NULL);
}
