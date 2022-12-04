#define  _GNU_SOURCE
 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
 
FILE *fptr;
char character;
char path[4096] = "";
 
void readPath(FILE *f, char c);
void updateFile(FILE *f, char* path);
void checkUser(char *u);
void delay(int milisec);
 
int main(int argc, char* argv[]){
 
	char *buf = "";
	char fpath[4096] = "";
	char complete[4096] = "";
	char *user;
	char answer[3];
	char command[50];
 
	checkUser(buf);
 
	if(argc >= 2){
	    char input[1024];
	    strcpy(input, argv[1]);
	    printf("SCIEZKA: %s\n",argv[1]);
 
	    if(argv[1][0] == '/'){
			user = getenv("USER");
			if(strcmp(user,"root") == 0){
				printf("PODANO SCIEZKE BEZWZGLEDNA\n");
				strcpy(fpath, input);
				printf("%s\n",fpath);
			}	
	    }
		else{
			user = getenv("USER");
			char cwd[2048];
			if(strcmp(user,"root") == 0){
				printf("PODANO SCIEZKE WZGLEDNA\n");
				if(strstr(input,"../")){
					chdir("..");
					getcwd(cwd, sizeof(cwd));
					strcat(fpath, cwd);
					strcat(fpath, "/");
					char *result = input;
					if(strstr(input,"../")){
						result += 3;
					}
					strcat(fpath, result);
					printf("%s\n", fpath);
				}
				else if(strstr(input,"~/")){
					chdir("~");
					getcwd(cwd, sizeof(cwd));
					strcat(fpath, cwd);
					strcat(fpath, "/");
					char *result = input;
					if(strstr(input,"~/")){
						result += 2;
					}
					strcat(fpath, result);
					printf("%s\n", fpath);
				}
				else if(strstr(input,"./")){
					getcwd(cwd, sizeof(cwd));
					strcat(fpath, cwd);
					strcat(fpath, "/");
					char *result = input;
					if(strstr(input,"./")){
						result += 2;
					}
					strcat(fpath, result);
					printf("%s\n", fpath);
				}
				else{
					getcwd(cwd, sizeof(cwd));
					strcat(fpath, cwd);
					strcat(fpath, "/");
					strcat(fpath, input);
				}
				printf("%s\n",fpath);
			}
	    }
	}
 
	readPath(fptr, character);
 
	char* startingPath = strstr(path, fpath);
	if(startingPath != NULL){
		int startingPathCount = strlen(startingPath);
		int fpathCount = strlen(fpath);
		int pathCount = strlen(path);
 
		strncpy(complete, path, pathCount - startingPathCount);
		if(complete[(strlen(complete)-1)] == ':'){
			complete[(strlen(complete)-1)] = '\0';
		}
 
		strcat(complete, startingPath + fpathCount);
 
		user = getenv("USER");
		if(strcmp(user,"root") == 0){
			printf("Sciezka zostala usunieta z $PATH\n");
		}
	}
	else{
		int pathLength = strlen(path);
		int leng = pathLength - 2;
		if(path[(strlen(path) - 1)] == '"'){
			leng = pathLength - 1;
		}
		strncpy(complete, path, leng); 
		strcat(complete, ":");
		strcat(complete, fpath);
		strcat(complete, "\"");
 
		user = getenv("USER");
		if(strcmp(user,"root") == 0){
			printf("Sciezka zostala dodana do $PATH\n");
		}
	}
 
	char* checkColons = strstr(complete, "::");
	if(checkColons != NULL){
		int checkingCount = strlen(checkColons);
		int completeCount = strlen(complete);
 
		strncpy(fpath, complete, completeCount - checkingCount);
		strcat(fpath, checkColons + 1);
		strcpy(complete, fpath);
	}
 
	if(complete[(strlen(complete)-1)] == ':'){
		complete[(strlen(complete)-1)] = '\0';
	}
 
	updateFile(fptr, complete);
 
	printf("Czy chcesz teraz zrestartowac system w celu wprowadzenia zmian?(tak\nie) \n");
 
	scanf("%s",&answer);
	if(strcmp(answer,"tak") == 0){
		puts("Za 10 sekund zostaniesz automatycznie wylogowany. Zaloguj sie ponownie i sprawdz zmiany!\n");
		int x = 10;
		printf("Czas pozostaly do wylogowania: ");
		while(x != 0){
			printf("%d",x);
			fflush(stdout);
			if(x >= 10) printf("\b \b");
			if(x >= 1) printf("\b \b");
			delay(1000);
			x--;
		}
		strcpy(command,"pkill -u root");
		system(command);
 
	}
	else{
		printf("PAMIETAJ! Zaloguj sie ponownie w celu zatwierdzenia wszelakich zmian.\n");
	}
	return 1;
}
 
void readPath(FILE *f, char c){
    f = fopen("/etc/environment", "r");// /etc/environment
 
    if (f == NULL){
        printf("Wystapil problem z otwarciem pliku.");
        exit(0);
    }
    c = fgetc(f);
    while (c != EOF){
        strncat(path, &c, 1);
        c = fgetc(f);
    }
    fclose(f);
}
 
void updateFile(FILE *f, char* path){
	f = fopen("/etc/environment", "w");
 
	if (f == NULL){
        printf("Wystapil problem z zapisem pliku!\n");
        exit(0);
    }
    fprintf(f, "%s", path);
	fclose(f);
}
 
void checkUser(char *u){
	u = getenv("USER");
	if(strcmp(u,"root") == 0){
		printf("Aktualny uzytkownik to: ROOT. Zaczynam dzialanie programu.\n");
	}
	else{
		printf("Aby zaczac dzialanie programu musisz byc ROOT'em!\n");
	}
}
 
void delay(int milisec){
    long pause;
    clock_t now;
    clock_t then;
 
    pause = milisec * (CLOCKS_PER_SEC / 1000);
    now = then = clock();
    while((now-then) < pause){
		now = clock();
	}     
}