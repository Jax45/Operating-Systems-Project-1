#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
void depthFirst(char dir[], int level, int indentation);

//Global Option Variables:
bool symLink = false;
bool permInfo = false;
bool numOfLinks = false;
bool typeInfo = false;
bool UID = false;
bool GID = false;
bool size = false;
bool showTime = false;

int main(int argc, char *argv[]) {

	int opt;
	int indentation = 4;
	int numOfOptargs = 0;
		
	while((opt = getopt(argc, argv, "hI:Ltpiugsdl")) != -1){
		switch(opt){
			case 'h':
				printf("Usage: dt [Options] [Directory Name]\n"
					"Options:\n-I [integer]: use -I to change the indentation and follow it with the number of spaces for the indent."
					"-L: follow symbolic links\n"
					"-t: show file type information\n"
					"-p: show file permissions\n"
					"-i: show number of hard links\n"
					"-u: show UID of file\n-g: show GID of file\n"
					"-s: show size of file\n"
					"-d: show last modified date of file\n"
					"-l: display file permissions, type, number of hard links, UID, and GID of the file.\n");
				numOfOptargs++;
				return 0;
			case 'I':
				indentation = atoi(optarg);
				numOfOptargs += 2;
				break;
			case 'L':
				symLink = true;
				numOfOptargs++;
				break;
			case 't':
				typeInfo = true;
				numOfOptargs++;
				break;
			case 'p':
				permInfo = true;
				numOfOptargs++;
				break;
			case 'i':
				//print number of links to inode file
				numOfLinks = true;
				numOfOptargs++;
				break;
			case 'u':
				//print UID associated with file
				UID = true;
				numOfOptargs++;
				break;
			case 'g':
				GID = true;
				numOfOptargs++;
				break;
			case 's':
				size = true;
				numOfOptargs++;
				break;
			case 'd':
				//time last modified
				showTime = true;
				numOfOptargs++;
				break;
			case 'l':
				//print all with tpuig true
				typeInfo = true;
				numOfOptargs++;
				permInfo = true;
				numOfLinks = true;
				UID = true;
				GID = true;
				size = true;
				break;
			default:
				numOfOptargs++;
		}	
	}
	char dirName[50] = "";
	
	if(argc - numOfOptargs < 2) {
		strncpy(dirName,".",strlen("."));
	}
	else {
		strncpy(dirName,argv[optind],strlen(argv[optind]));
	}
	//print header line
	printf("|-------------Name-------------|");
	if(permInfo){
		printf("---Perms---|");	
	}	
	if(typeInfo){
		printf("----File  Type----|");
	}
	if(numOfLinks){
		printf("-Link Number-|");
	}
	if(UID){
		printf("-UID-|");
	}
	if(GID){
		printf("-GID-|");
	}
	if(size){
		printf("-Size-|");
	}
	if(showTime){
		printf("---Date Last Modified---|");	
	}
	printf("\n");
	depthFirst(dirName,0,indentation);	
	return 0;
}

void depthFirst(char dir[], int level, int indentation) {
	//pointer for the directory reading struct
	struct dirent *dirReader;
	
	struct stat Stat;
	
	//pointer to directory
	DIR *baseDir = opendir(dir);
	
	if(baseDir == NULL){
		//error here
		perror("Error: dt: directory is NULL");
		exit(1);
	}

	char indent[50] = "";
	if(level != 0) {
		int i = 0;
		for(i = 0; i < level; i++){
			int j = 0;
			for(j = 0; j < indentation; j++){
				strncat(indent," ",strlen(" "));
			}
		}
	}
	else{
		strncpy(indent,"",strlen(""));
	}
	char filePath[50] = "";
	while ((dirReader = readdir(baseDir)) != NULL){
	
		//determine if the file has a . to start
		if (dirReader->d_name[0] == '.'){
			//if it does skip the file.
			continue;
		}
		strncpy(filePath,dir,50);
                strncat(filePath,"/",strlen("/"));
                strncat(filePath,dirReader->d_name,strlen(dirReader->d_name));
		//try to stat call on filePath.
		if (lstat(filePath,&Stat) < 0){
                        perror("Error: dt: Could not complete stat call.");
                        exit(1);
                }
		char fileName[50] = "";
		strncpy(fileName,indent,strlen(indent));
		strncat(fileName,dirReader->d_name,strlen(dirReader->d_name));	
		//print all of the info at this level
		printf("|%-30s|",fileName);
		
		//check if perms enabled
		if (permInfo) {
			printf((S_ISDIR(Stat.st_mode)) ? "d" : "-");
   			printf((Stat.st_mode & S_IRUSR) ? "r" : "-");
			printf((Stat.st_mode & S_IWUSR) ? "w" : "-");
			printf((Stat.st_mode & S_IXUSR) ? "x" : "-");
			printf((Stat.st_mode & S_IRGRP) ? "r" : "-");
			printf((Stat.st_mode & S_IWGRP) ? "w" : "-");
			printf((Stat.st_mode & S_IXGRP) ? "x" : "-");
			printf((Stat.st_mode & S_IROTH) ? "r" : "-");
			printf((Stat.st_mode & S_IWOTH) ? "w" : "-");
			printf((Stat.st_mode & S_IXOTH) ? "x " : "- ");
			printf("|");
		}
		
		//print file type
		if(typeInfo){
			char type[18] = "";
			switch(Stat.st_mode & S_IFMT) {
				case S_IFBLK:  strncpy(type,"block device",strlen("block device"));
					break;
				case S_IFCHR:  strncpy(type,"character device",strlen("character device"));
					break;
				case S_IFDIR:  strncpy(type,"directory",strlen("directory"));
					break;
				case S_IFIFO:  strncpy(type,"FIFO/pipe",strlen("FIFO/pipe"));
					break;
				case S_IFLNK:  strncpy(type,"symlink",strlen("symlink"));
					break;
				case S_IFREG:  strncpy(type,"regular file",strlen("regular file"));
					break;
				case S_IFSOCK: strncpy(type,"socket",strlen("socket"));
					break;
				default: strncpy(type,"unknown file",strlen("unknown file"));
					break;	
			}
			printf("%-18s|",type);	
		}
		
		//print number of Hard links
		if(numOfLinks){
			printf("%-13ld|",(long)Stat.st_nlink);
		}

		//print the UID
		if(UID){
			printf("%-5ld|",(long)Stat.st_uid);
		}
				
		//print the GID
		if(GID){
			printf("%-5ld|",(long)Stat.st_gid);  
		}
		
		if(size){
			//convert size to KB, MB, or GB
			long long fileSize = Stat.st_size;
			if(fileSize >= 1000 && fileSize < 10000){
				//file is in KB
				printf("%-5lldK|",fileSize/10);
			}
			else if(fileSize >= 10000 && fileSize < 100000){
				//file is in MB
				printf("%-5lldM|",fileSize/100);
			}
			else if(fileSize >= 100000){
				//file is in GB
				printf("%-5lldG|",fileSize/1000);
			}
			else{
				printf("%-5lld |",fileSize);
			}
		}

		if(showTime){
			printf("%s", ctime(&Stat.st_mtime));
		}
		//because ctime automatically starts newline, only print newline if -d is not used.
		else{
			printf("\n");
		}	
		//check if directory
		if (S_ISDIR(Stat.st_mode)){
			int newLevel = level + 1;
                        depthFirst(filePath,newLevel,indentation);
		}
		
		if(symLink) {
			//check if symlink and if so follow it
			if (S_ISLNK(Stat.st_mode)){
			
				//do another stat call but this time do stat instead of lstat.
				//this is to get the data of where the symlink is pointing to.
				if (stat(filePath,&Stat) < 0){
                        		perror("Error: dt: Could not complete stat call.");
                        		exit(1);
                		}
				if(S_ISDIR(Stat.st_mode)){
					int newLevel = level + 1;
					depthFirst(filePath,newLevel,indentation);
				}
				/*char buf[1024];
				size_t size;
				if ((size = readlink(filePath, buf, sizeof(buf)-1)) != -1){
					buf[size] = '\0';
					depthFirst(buf,level,indentation);
				}*/
			}
		}
	}
	
	closedir(baseDir);
}

