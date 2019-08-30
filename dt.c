#include<stdio.h>
#include<unistd.h>
#include<stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
int main(int argc, char *argv[]) {

	int opt;
	int indentation = 4;
	bool symLink = false;
	bool typeInfo = false;
	bool permInfo = false;
	bool numOfLinks = false;
	bool UID = false;
	bool GID = false;
	bool size = false;
	bool time = false;
	 
	while((opt = getopt(argc, argv, "hI:Ltpiugsdl")) != -1){
		switch(opt){
			case 'h':
				printf("Extensive help statement: \n");
				return 0;
			case 'I':
				indentation = atoi(optarg);
				printf("indentation is: %d \n", indentation);
				break;
			case 'L':
				symLink = true;
				break;
			case 't':
				typeInfo = true;
				break;
			case 'p':
				permInfo = true;
				break;
			case 'i':
				//print number of links to inode file
				numOfLinks = true;
				break;
			case 'u':
				//print UID associated with file
				UID = true;
				break;
			case 'g':
				GID = true;
				break;
			case 's':
				size = true;
				break;
			case 'd':
				//time last modified
				time = true;
				break;
			case 'l':
				//print all with tpuig true
				typeInfo = true;
				permInfo = true;
				numOfLinks = true;
				UID = true;
				GID = true;
				size = true;
				break;
			default:
				printf("default\n");
		}	
	}
	char dirName[50] = "";
	if(argc < 2) {
		strncpy(dirName,".",strlen("."));
	}
	else {
		strncpy(dirName,argv[1],strlen(argv[1]));
	}	
	struct stat Stat;
    	if(stat(dirName,&Stat) < 0)    
        	return 1;
 	//pointer for the directory reading struct
	struct dirent *dirReader;
	
	//pointer to directory
	DIR *baseDir = opendir(dirName);
	
	if (baseDir == NULL){
		//error here
	}
	char filePath[50] = "";
	while ((dirReader = readdir(baseDir)) != NULL){
        	printf("%s\n", dirReader->d_name);
		
		//determine if the file has a . to start
		if (dirReader->d_name[0] == '.'){
			continue;
		}
		strncpy(filePath,dirName,50);
		strncat(filePath,"/",strlen("/"));
		strncat(filePath,dirReader->d_name,strlen(dirReader->d_name));
		
		printf("\nfilePath That was created: %s \n", filePath);
		//determine if the file is a directory
		if (stat(filePath,&Stat) < 0){
			printf("Could not do stat call on %s", filePath);
			return 1;
		}
		//check if directory
		if (S_ISDIR(Stat.st_mode)){
			printf("\t is a directory");
		}
		//if (stat(dirReader->d_name,&Stat) < 0)
		//	return 1; 
  		
		printf("Information for %s\n file size is %d bytes",dirReader->d_name,Stat.st_size);

	}
	closedir(baseDir);     
	
    	printf("Information for %s\n",dirName);
	printf("File Size: \t\t%d bytes\n",Stat.st_size);
    	printf("Number of Links: \t%d\n",Stat.st_nlink);
    	printf("File inode: \t\t%d\n",Stat.st_ino);	
			
	//execl("/bin/ls", "ls", "-l", (char *)0)i;
	//printf("\nA sample C program with a makefile XD \n\n");
	return 0;
}
