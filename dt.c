//Author: Jackson Hoenig
//Date: 9/4/2019
//Description: 

//dependencies
#include<stdio.h>
#include<libgen.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <time.h>

//Prototypes:
void depthFirst(char dir[], int level, int indentation);
void printFile(char[],char[],struct stat,int,int);
	
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
	//variable for option switch
	int opt;
	//default indentation is set to 4 spaces
	int indentation = 4;
	//to keep track of options and their optargs
	int numOfOptargs = 0;
		
	//use getopt to iterate through each option.
	while((opt = getopt(argc, argv, "hI:Ltpiugsdl")) != -1){
		switch(opt){
			//print usage and exit
			case 'h':
				printf("Usage: dt [Options] [Directory Name]\n"
					"Options:\n-I [integer]: use -I to change the indentation and follow it with the number of spaces for the indent.\n"
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
			//set indentation to number following this option
			case 'I':
				indentation = atoi(optarg);
				//validate indentation
				if(indentation < 0 || indentation > 10){
					printf("%s","Error: dt: Indentation given needs to be between 0 and 10 inclusive.\n");
					exit(2);
				}
				numOfOptargs += 2;
				break;
			//set variable to follow symbolic links to true
			case 'L':
				symLink = true;
				numOfOptargs++;
				break;
			//set variable to show file type to true
			case 't':
				typeInfo = true;
				numOfOptargs++;
				break;
			//set variable to show permissions to true
			case 'p':
				permInfo = true;
				numOfOptargs++;
				break;
			//set variable to show number of hard links to true
			case 'i':
				//print number of links to inode file
				numOfLinks = true;
				numOfOptargs++;
				break;
			//set variable to show UID to true
			case 'u':
				//print UID associated with file
				UID = true;
				numOfOptargs++;
				break;
			//set variable to show GID to true
			case 'g':
				GID = true;
				numOfOptargs++;
				break;
			//set variable to show file size to true
			case 's':
				size = true;
				numOfOptargs++;
				break;
			//set variable to show date last modified to true.
			case 'd':
				//time last modified
				showTime = true;
				numOfOptargs++;
				break;
			//set all option variables to true except d,s,I,and L
			case 'l':
				//print all with tpuig true
				typeInfo = true;
				numOfOptargs++;
				permInfo = true;
				numOfLinks = true;
				UID = true;
				GID = true;
				break;
			default:
				numOfOptargs++;
				printf("%s","You gave an option that was not handled. Use -h for help.\n");
				exit(3);
		}	
	}
	char dirName[50] = "";
	
	if(argc - numOfOptargs < 2) {
		strncpy(dirName,".",strlen("."));
	}
	else {
		if(strlen(argv[optind]) > 20 || strlen(argv[optind]) < 1){
			printf("%s","Error: dt: Directory given needs to be less than 20 characters long\n");
			exit(1);
		}
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
	//initialize stat structure
	struct stat Stat;
	
	//pointer to directory
	DIR *baseDir = opendir(dir);
	
	if(baseDir == NULL){
		//error here
		perror("Error: dt: directory is NULL");
		exit(1);
	}
	
	//create a buffer to hold spaces equal to the indentation * level in tree.
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
	//if we are at the base level make indent nothing
	else{
		strncpy(indent,"",strlen(""));
	}
	//read the baseDir and iterate through each file in that directory
	//the baseDir will either be the default ".", the argument given or the inner directory found.
	char filePath[50] = "";
	while ((dirReader = readdir(baseDir)) != NULL){
	
		//determine if the file has a . to start
		if (dirReader->d_name[0] == '.'){
			//if it does skip the file.
			continue;
		}
		//copy the directory to filePath and format it to get the path of the file.
		strncpy(filePath,dir,50);
                strncat(filePath,"/",strlen("/"));
		//now that we have the prefix path add the name to it.
                strncat(filePath,dirReader->d_name,strlen(dirReader->d_name));
		//try to stat call on filePath.
		if (lstat(filePath,&Stat) < 0){
                        perror("Error: dt: Could not complete stat call.");
                        exit(1);
                }
		//add the indent to filename and pass it to printFile.
		char fileName[50] = "";
		strncpy(fileName,indent,strlen(indent));
		strncat(fileName,dirReader->d_name,strlen(dirReader->d_name));	
		//print all of the info at this level
		printFile(fileName,filePath,Stat,level,indentation);
		
		
	}
	closedir(baseDir);
}
void printFile(char fileName[], char filePath[], struct stat Stat, int level, int indentation){
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
			//call function that called this function which is in effect recursive.
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
			
				//if the symlink points at directory show depthFirst at directory level.
				if(S_ISDIR(Stat.st_mode)){
					int newLevel = level + 1;
					//again just recursive call to that new directory
					depthFirst(filePath,newLevel,indentation);
				}
				//if it is only pointing to one file we need to format the filepath and filename to
				//be sent to printfile.
				else{
					//get file path from symlink
					char symPath[1024];
					ssize_t len;
					if ((len = readlink(filePath, symPath, sizeof(symPath)-1)) != -1){
						symPath[len] = '\0';
					
					
					//get file name from path.
					char* symName = basename(symPath);
					int newLevel = level + 1;
					
					//calculate indent for the symName
					char indent[50] = "";
					        if(newLevel != 0) {
					                int i = 0;
					                for(i = 0; i < newLevel; i++){
					                        int j = 0;
					                        for(j = 0; j < indentation; j++){
					                                strncat(indent," ",strlen(" "));
					                        }
					                }
					        }
					        else{
					                strncpy(indent,"",strlen(""));
					        }                        		
               				
					//add the indent to the symName at the start.
					char indentSymName[50] = "";
					strncpy(indentSymName,indent,strlen(indent));
					strncat(indentSymName,symName,strlen(symName));
					
					//then display the file that the symlink is pointing to.
					printFile(indentSymName,symPath,Stat,newLevel,indentation);
				}
			}
		}
	}
}

