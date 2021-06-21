#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

char* lastname_of(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for (p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  int len = strlen(p);
  if(len >= DIRSIZ)
    return p;
  memmove(buf, p, len);
  buf[len] = 0;
  return buf;
}

void dfs(char *path, char *filename) {
	int fd;
	struct dirent de;
	struct stat st;
	char buf[512], *p;

	if ((fd = open(path, O_RDONLY)) < 0) {
		fprintf(2, "find: cannot open %s\n", path);
		return;
	}

	if (fstat(fd, &st) < 0) {
		fprintf(2, "find: cannot stat %s\n", path);
		return;
	}

	char *lastname = lastname_of(path);

	switch(st.type) {
		case T_FILE:
			if (strcmp(lastname, filename) == 0)
				printf("%s\n", path);
			break;
		case T_DIR:
			if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
				fprintf(2, "find: path too long\n");
				return;
			}
			strcpy(buf, path);
			p = buf + strlen(buf);
			*p++ = '/';
			while(read(fd, &de, sizeof(de)) == sizeof(de)){
				if(de.inum == 0)
					continue;
				if (strcmp(".", de.name) == 0)
					continue;
				if (strcmp("..", de.name) == 0)
					continue;
				memmove(p, de.name, DIRSIZ);
				p[DIRSIZ] = 0;
				if (stat(buf, &st) < 0){
					fprintf(2, "find: cannot stat %s\n", buf);
					continue;
				}
				dfs(buf, filename);
			}
	}
	close(fd);
}

int main(int argc, char **argv) {

	if (argc != 3) {
		printf("usage: find <path> <filename>\n");
		exit(1);
	}

	dfs(argv[1], argv[2]);

	exit(0);
}
