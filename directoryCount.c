#include <stdio.h>
#include <ftw.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>

#define _XOPEN_SOURCE 500
#define OPENNUMS 1024
off_t total_num;
int back(const char *dirpath, const struct stat *pstat, int flags, struct FTW *s)
{
	if ((flags == FTW_D) || (flags == FTW_F)) 
		{
			total_num += 1;
		}
	return 0;
}
int main(int argc, char *argv[])
{
	if (argc >  2) 
		{          // Make sure there are only two ways to run this program
			printf("Error argument!");
			return -1;
		}
	else {
		int ftw_res = 0;
		char *path;
		if (argc == 1)            //only have program name, get father path
			{
				char *path1 = argv[0];
				path = strdup(path1);
				dirname(path);
			}
		else if (argc  == 2)     // have program name and path
			{
				path = argv[1];
				printf("%s\n",path);
				
			}
		
		struct stat buf;
		if (lstat(path, &buf) < 0) 
			{
				printf("lstat error!\n");
				return -1;
			}
		if (!S_ISDIR(buf.st_mode)) 
			{
				printf("Not a directory!\n");
				return -1;
			}
		int res = nftw(path, back, OPENNUMS, 1/*FTW_PHYS*/);
		if (res < 0) 
			{
				printf("nftw error!\n");
				return -1;
			}
		else 
			{
				printf("total size : %ld\n", total_num-1);
				return 0;
			}
	}
	return 0;
}