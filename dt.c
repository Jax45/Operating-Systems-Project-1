#include<stdio.h>
#include<unistd.h>
#include<stdbool.h>

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
				exit(1);
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
	
	execl("/bin/ls", "ls", "-l", (char *)0);
	//printf("\nA sample C program with a makefile XD \n\n");
	return 0;
}
