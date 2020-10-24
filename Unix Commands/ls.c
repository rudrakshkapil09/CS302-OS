//ls
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
    
int main(int args, char **argv)
{
	struct dirent **namelist;   	
	struct passwd *userInfo;
	struct group *userGroup;
	struct stat filestats;	

	int aflag = 0, lflag = 0;

	int n = 0;
	char path[200] = "./";

	//this directory no -l
	if (args < 2)
		n = scandir(".",&namelist,NULL, alphasort); 
	//other directory no -l
	else if (strcmp(argv[1], "-l") != 0 && strcmp(argv[1], "-a") != 0) {
		strcat(path, "/");
		strcat(path, argv[1]);
		n = scandir(path, &namelist, NULL, alphasort);

		//other directory, flags
		if (args == 3 && (strcmp(argv[2], "-l") == 0)) 
			lflag = 1;
		else if (args == 3 && (strcmp(argv[2], "-a") == 0))
			aflag = 1;
		else if (args == 4) {
			lflag = 1;
			aflag = 1;
		}		
	}
	//this directory, flag
	else {
		n = scandir(".",&namelist,NULL, alphasort); 
		if (args == 2 && (strcmp(argv[1], "-l") == 0)) 
			lflag = 1;
		else if (args == 2 && (strcmp(argv[1], "-a") == 0))
			aflag = 1;
		else if (args == 3) {
			lflag = 1;
			aflag = 1;
		}	
	}

	if (lflag == 1) {
		userInfo = getpwuid(geteuid());		
		userGroup = getgrgid(userInfo->pw_gid);
	}

	int n2 = n;
	n = 0;
	//printing required data
	for (; n < n2; n++) {
		if (aflag == 0 && namelist[n]->d_name[0] == '.')
			continue;

		char newPath[200];
		strcpy(newPath, path);
		strcat(newPath, namelist[n]->d_name);
 		
		if (lflag == 1) {
			if (stat(newPath, &filestats) < 0)
				continue;	
			printf(S_ISDIR(filestats.st_mode) ? "d" : "-");
			printf(filestats.st_mode & S_IRUSR ? "r" : "-");
			printf(filestats.st_mode & S_IWUSR ? "w" : "-");
			printf(filestats.st_mode & S_IXUSR ? "x" : "-");
			printf(filestats.st_mode & S_IRGRP ? "r" : "-");
			printf(filestats.st_mode & S_IWGRP ? "w" : "-");
			printf(filestats.st_mode & S_IXGRP ? "x" : "-");
			printf(filestats.st_mode & S_IROTH ? "r" : "-");
			printf(filestats.st_mode & S_IWOTH ? "w" : "-");
			printf(filestats.st_mode & S_IXOTH ? "x " : "- ");
			
			printf("%lu ", filestats.st_nlink);
			printf("%s ", userInfo->pw_name);
			printf("%s ", userGroup->gr_name);
			printf("%lu ", filestats.st_size);
			
			time_t now = time(0);
			struct tm now_t = *localtime(&now);
			struct tm time = *localtime(&filestats.st_ctime);
			switch (time.tm_mon) {
				case 0: printf("Jan "); break;
				case 1: printf("Feb "); break;
				case 2: printf("Mar "); break;
				case 3: printf("Apr "); break;
				case 4: printf("May "); break;
				case 5: printf("Jun "); break;
				case 6: printf("Jul "); break;
				case 7: printf("Aug "); break;
				case 8: printf("Sep "); break;
				case 9: printf("Oct "); break;
				case 10: printf("Nov "); break;
				case 11: printf("Dec "); break;
			}
			printf("%d ", time.tm_mday);
			if (time.tm_mday <= 9) printf(" "); 
			
			if (time.tm_year < now_t.tm_year)
				printf("%d\t", (time.tm_year+1900));
			else {
				printf("%d:", time.tm_hour);
				if (time.tm_min <= 9) printf("0");
				printf("%d\t", time.tm_min);
			}
		}
		printf("%s\n",namelist[n]->d_name);
		free(namelist[n]);
	}
	free(namelist);
}
