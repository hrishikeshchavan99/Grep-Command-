#include<stdio.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<getopt.h>
#include <dirent.h>
#include"queue.h"
#include"list.h"
void tostring(char str[], int num);
int CheckBinary(char str[]);
void GetFilesFromFolder(queue *q,char str[]);
int binarygrep(char str[], char str2[], int setvalue[], list *l);
void CheckForRegEx(char str[]);
int max = -1;
int setflag = 0;
int setreg[8];
int grep(char s[], char s1[], int setvalue[], list *l);
int main( int argc, char *argv[]) {
	int setvalue[16], i = 0, j = 0, k = 0, index, len, fd, x, binaryflag = 0, n1 = 0, n2 = 0, n3 = 0;
	queue q, q2;
	qinit(&q);
	qinit(&q2);
	list l;
	init(&l);
	char  ch, str[256], filename[64], temp[64], temp1[64], temp2[64];
	char *s1, *s2;
	if(argc < 3) {
		errno = EINVAL;
		perror("usage: ./mygrep <option if any><pattern> <file>");
		return errno;
	}
	int option = 0;
	while ((option = getopt(argc, argv,"Hhbwm:cqvif:re:")) != -1) {
		switch (option) {
			case 'H' :
				setvalue[1] = 1;
				break;
			case 'h' : 
				if (setvalue[1] == 1)
					setvalue[1] = 0;
				setvalue[2] = 2;
				break;
			case 'b' :
				setvalue[3] = 3;
				break;
			case 'w' : 
				setvalue[4] = 4;
				break;
			case 'm' :
				setvalue[5] = 5;
				max = atoi(optarg);
				if(argc == 3) {
					errno = EINVAL;
					perror("usage: ./mygrep<-m><maxcount><pattern> <file>");
					return errno;
				}
				break;
			case 'c' :
				setvalue[6] = 6;
				break;
			case 'q' : 
				exit(1);
				break;
			case 'v' : 
				setvalue[8] = 8;
				break;
			case 'i' : 
				setvalue[9] = 9;
				break;
			case 'f' : 
				setvalue[10] = 10;
				strcpy(filename, optarg);
				break;
			case 'r' : 
				setvalue[11] = 11;
				break;
			case 'e' : 
				CheckForRegEx(optarg);
				if (setreg[3] == 1){
					strcpy(temp, optarg);
					for (n1 = 0; n1 < strlen(temp); n1++){
						if (temp[n1] != '?'){
							temp1[n2] = temp[n1];
							temp2[n3] = temp[n1];
							n2++;
							n3++;
						}
						else {
							temp2[n3 - 1] = '\0';
							n3--;
						}
					}
					temp1[n2] = '\0';
					temp2[n3] = '\0';
					enq(&q2, temp1);
					j++;
					enq(&q2, temp2);
					j++;
				}
				else {
					enq(&q2, optarg);
					j++;
				}
				setvalue[12] = 12;
				setreg[3] = 0;
				break;
			
		}
	}
	//Normal command with only one pattern
	if (setvalue[10] != 10 && setvalue[12] != 12) {
		strcpy(str, argv[optind]);
		optind++;
	}//Patterns are in a file 
	else if (setvalue[10] == 10){
		fd = open( filename, O_RDONLY);
		if ( fd == -1) {
			printf("%s:\n", filename);
			perror("Open Failed:\n");
			return errno;
		}
		x = read(fd, &ch, 1);
		while (x) {
			while (ch != '\n') {
				str[i] = ch;
				x = read(fd, &ch, 1);
				i++;
			}
			str[i] = '\0';
			i = 0;
			enq(&q2, str);
			j++;
			x = read(fd, &ch, 1);
		}	
	}
	//to grep in a directory
	if (setvalue[11] == 11){
		for (index = optind; index != argc; index++) {
			GetFilesFromFolder(&q, argv[index]);
			setflag = 1;
		}
	}//to grep in files
	else {
		for (index = optind; index != argc; index++) {
			if(!qfull(&q))
			enq(&q, argv[index]);
			k++;
		}
	}
	//if multiple files are there, set a flag.
	if (k > 1)
		setflag = 1;
	while(!qempty(&q)) {
        	s1 = deq(&q);
        	// If file is binary
        	if (CheckBinary(s1))
        		binaryflag = 1;
        	init(&l);
        	// To grep multiple patterns
        	if (setvalue[10] == 10 || setvalue[12] == 12) {
        		for (index = 0; index < j; index++){
				s2 = deq(&q2);
				grep(s2, s1, setvalue, &l);
				enq(&q2, s2);
			}
			if (setvalue[8] == 8) {
				//if -v option is used with multiple patterns
				setvalue[13] = 13;
				for (index = 0; index < j; index++){
					s2 = deq(&q2);
					grep(s2, s1, setvalue, &l);
					enq(&q2, s2);
				}
			}
				len = length(&l);
				if (setvalue[6] == 6 && setvalue[8] != 8) {
					if (setvalue[1] == 1 || (setvalue[2] != 2 && setflag == 1))
						printf("%s:", s1);
					printf("%d\n", len);
				}
				else if (setvalue[6] == 6 && setvalue[8] == 8) {
					if (setvalue[1] == 1 || (setvalue[2] != 2 && setflag == 1))
						printf("%s:", s1);
					printf("%d\n", len);
				}
				else if (binaryflag != 1)
					print(l);
				else if (binaryflag == 1 && len > 0) 
					printf("Binary file %s matches\n", s1);
			
        	}// to grep a single pattern.
        	else{
			grep(str, s1, setvalue, &l);
			len = length(&l);
			if (setvalue[6] == 6 && setvalue[8] != 8) {
				if (setvalue[1] == 1 || (setvalue[2] != 2 && setflag == 1))
					printf("%s:", s1);
				printf("%d\n", len);
			}
			else if (setvalue[6] == 6 && setvalue[8] == 8) {
				if (setvalue[1] == 1 || (setvalue[2] != 2 && setflag == 1))
					printf("%s:", s1);
				printf("%d\n", len);
			}
			else if (binaryflag != 1)
				print(l);
			else if (binaryflag == 1 && len > 0)
				printf("Binary file %s matches\n", s1);
		}
		setvalue[13] = 0;
		binaryflag = 0;
	}
	return 0;
}
int grep(char str[], char str2[], int setvalue[], list *l){
	int fd, i = 0, j = 0, count = 0, matchcount = 0,dismatchcount = 0, maxcount = max, x, l1 = 0, b = 0, c, flagv = 0, entry = 0, l2, lineno = 0;
	int m = 0, n = 0, len, binaryflag = 0;
	char  ch, s[1024], line[2048], colon[] = {':', '\0'}, offset[64], temp[256], temp1[256];
	data d;
	DIR *dr = opendir(str2);
	if (dr != NULL){
		return 0;
	}
	line[0] = '\0';
	setreg[1] = 0;
	setreg[2] = 0;
	setreg[3] = 0;
	setreg[4] = 0;
	if (CheckBinary(str2)){
		binaryflag = 1;
	}
	// If -e is used , check for reg. exp.
	if (setvalue[12] == 12)
		CheckForRegEx(str);
	// if '^' is used, remove it from string
	if (setreg[1] == 1){
		strcpy(temp1, str);
		len = strlen(str);
		for (m = 1; m < len; m++){
			temp[n] = str[m];
			n++;
		}
		temp[n] = '\0';
		strcpy(str, temp);
	}
	// if '$' is used, remove it .
	if (setreg[2] == 1){
		strcpy(temp1, str);
		str[strlen(str) - 1] = '\0';
	}
	// if '.' is present at initial position, remove it.
	if (setreg[4] == 1){
		if (str[0] == '.') {
			strcpy(temp1, str);
			len = strlen(str);
			for (m = 1; m < len; m++){
				temp[n] = str[m];
				n++;
			}
			temp[n] = '\0';
			strcpy(str, temp);
		}
	}
	fd = open( str2, O_RDONLY);
	if ( fd == -1) {
		printf("%s:\n", str2);
		perror("Open Failed:\n");
		return errno;
	}
	x = read(fd, &ch, 1);
	b++;
	while (x) {
		if (binaryflag == 1){
			while (ch != '\n' && ch != '\0') {
				s[i] = ch;
				x = read(fd, &ch, 1);
				b++;
				i++;
			}
			s[i] = '\0';
			l1 = i;
			i = 0;
		
		}
		else {
			while (ch != '\n') {
				s[i] = ch;
				x = read(fd, &ch, 1);
				b++;
				i++;
			}
			s[i] = '\0';
			i = 0;
			l1 = strlen(s);
		}
			lineno++;
			d.lineno = lineno;
			entry = 0;
			if (str[0] == '\0' && setvalue[8] != 8)
				entry = 1;
			
			if (setreg[2] == 1){
				i = l1 - strlen(str);
			}
			while (i != l1)  {
				// If -w is used and -i is not used.
				if (setvalue[9] != 9){	
				if (str[j] == s[i]) {
					if (setvalue[4] == 4){
						if (s[i - 1] == 96 || (s[i - 1] < 95 && s[i - 1] > 90) || s[i - 1] < 65 || s[i - 1] > 122)
							while (str[j] == s[i]) { 
								count++;
								j++;
								i++;
								if (setreg[4] == 1)
									while (str[j] == '.'){
										i++;
										j++;
										count++;
									}//ignore '.' in pattern
								if (str[j] == '\0')
									break;
							}
						if ((count == strlen(str)) && (s[i] == 96 || (s[i] < 95 && s[i] > 90) || s[i] < 65 || s[i] > 122)){
							c = b - strlen(s) - 1;
							if (setvalue[8] != 8) {
								entry = 1;
							}
							flagv = 1;
							matchcount++;
							if (setreg[1] == 1){
								if (i != count){
									entry = 0;
									flagv = 0;
								}
							}
							break;
						}
						else if (s[i] != '\0'){
							i = i - j + 1;
							j = 0;
						}
						else 
							break;
					}// both -w and -i not used.
					else {
						while (str[j] == s[i]) { 
							count++;
							j++;
							i++;
							if (setreg[4] == 1)
								while (str[j] == '.'){
									i++;
									j++;
									count++;
								}
							if (str[j] == '\0')
								break;
						}
						if (count == strlen(str)){	
							c = b - strlen(s) - 1;
							if (setvalue[8] != 8) {
								entry = 1;
							}
							flagv = 1;
							matchcount++;
							if (setreg[1] == 1){
								if (i != count){
									entry = 0;
									flagv = 0;
								}
							}
							break;
						}
						else if (s[i] != '\0'){
							i = i - j + 1;
							j = 0;
						}
						else 
							break;
					}
				}
				else {
					i++;
					j = 0;
					count = 0;
				}	
			}// If both -i and -w are used.
			else {	
				if ((str[j] == s[i])|| (str[j] + 32 == s[i]) || (str[j] - 32 == s[i])) {
					if (setvalue[4] == 4){
						if (s[i - 1] == 96 || (s[i - 1] < 95 && s[i - 1] > 90) || s[i - 1] < 65 || s[i - 1] > 122)
							while ( ((str[j] == s[i])|| (str[j] + 32 == s[i]) || (str[j] - 32 == s[i]))) { 
								count++;
								j++;
								i++;
								if (setreg[4] == 1)
									while (str[j] == '.'){
										i++;
										j++;
										count++;
									}
								if (str[j] == '\0')
									break;
							}
						if ((count == strlen(str)) && (s[i] == 96 || (s[i] < 95 && s[i] > 90) || s[i] < 65 || s[i] > 122)){
							c = b - strlen(s) - 1;
							if (setvalue[8] != 8) {
								entry = 1;
							}
							flagv = 1;
							matchcount++;
							if (setreg[1] == 1){
								if (i != count){
									entry = 0;
									flagv = 0;
								}
							}
							break;
						}
						else if (s[i] != '\0'){
							i = i - j + 1;
							j = 0;
						}
						else 
							break;
					}// -i is used and -w is not used.
					else {
						while (( ((str[j] == s[i])|| (str[j] + 32 == s[i]) || (str[j] - 32 == s[i])))) { 
							count++;
							j++;
							i++;
							if (setreg[4] == 1)
								while (str[j] == '.'){
									i++;
									j++;
									count++;
								}
							if (str[j] == '\0')
								break;
						}
						if (count == strlen(str)){
							c = b - strlen(s) - 1;
							if (setvalue[8] != 8) {
								entry = 1;							}
							flagv = 1;
							matchcount++;
							if (setreg[1] == 1){
								if (i != count){
									entry = 0;
									flagv = 0;
								}
							}
							break;
						}
						else if (s[i] != '\0'){
							i = i - j + 1;
							j = 0;
						}
						else 
							break;
					}
				}
				else {
					i++;
					j = 0;
					count = 0;
				}	
			}
			}
			// To remove lines extra lines in case of multiple patterns.
			if (setvalue[8] == 8 && flagv == 1)
				remov(l, lineno);
			// In case of -v option, set entry to 1 for non matching lines.
			if (setvalue[8] == 8 && flagv == 0) {
				c = b - strlen(s) - 1;
				entry = 1;
				dismatchcount++;	
			}
			// insert printable line in list.
			if (entry == 1 && setvalue[13] != 13){
				if (setvalue[1] == 1 && maxcount != 0) { 
					strcpy(line, str2);
					strcat(line, colon );
				}
				else if (setflag == 1 && setvalue[2] != 2 && maxcount != 0){ 
					strcpy(line, str2);
					strcat(line, colon );
				}
				if (setvalue[3] == 3 ){
					tostring(offset, c);
					if (line[0] == '\0')
						strcpy(line, offset);
					else
						strcat(line, offset);
					strcat(line, colon );
				}
				if (maxcount != 0) {
					strcat(line, s);
					d.str = (char *)malloc(strlen(line) + 1);
					strcpy(d.str, line);
					l2 = strlen(line);
					d.str[l2] = '\0';
					insert(l, d);
					maxcount--;
				}
			}
			flagv = 0;
			i = 0;
			j = 0;
			line[0] = '\0';
			count = 0;
			x = read(fd, &ch, 1);
			b++;
	}
	if (setvalue[5] == 5 && matchcount > max){
		matchcount = max;
	}
	if (setvalue[5] == 5 && dismatchcount > max){
		dismatchcount = max;
	}	
	// insert ^ and $ again in pattern.
	if (setreg[1] == 1 || setreg[2] == 1)
		strcpy(str, temp1);
	setreg[1] = 0;
	setreg[2] = 0;
	setreg[3] = 0;
	setreg[4] = 0;
	close(fd);
	return 0;
}
// To insert binary offset in list as a string , convert no to string. 
void tostring(char str[], int num) {
	int i, rem, len = 0, n;
	n = num;
	if (n == 0) {
		str[0] = '0';
		str[1] = '\0';
		return;
	}
	while (n != 0) {
		len++;
		n /= 10;
	}
	for (i = 0; i < len; i++) {
        	rem = num % 10;
        	num = num / 10;
        	str[len - (i + 1)] = rem + '0';
	}
	str[len] = '\0';
}
int CheckBinary(char str[]) {
	int fd, flag;
	char ch;
	DIR *dr = opendir(str);
	if (dr != NULL){
		printf("%s is a directory\n", str);
		return 0;
	}
	fd = open(str, O_RDONLY);
	if (fd == -1)
		return 0;
	while (read(fd, &ch, 1)){
		if(ch < 128 && ch >= 0) {
			flag = 0;
		}
		else {
			flag = 1;
			break;
		}
	}
	return flag;
}
// to get all files in a folder recursively
void GetFilesFromFolder(queue *q,char str[]){
	char p[1024], slash[2] = {'/', '\0' }, r[1024];
	struct dirent *de;
	DIR *dr = opendir(str);
	if (dr == NULL){ 
			enq(q, str);
	}
	else {
		strcpy(p, str);
		strcat(p, slash);
		strcpy(r, p);
		while ((de = readdir(dr)) != NULL) { 
			if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0)
				continue;
			strcat(p, de->d_name);
			GetFilesFromFolder(q, p);
			strcpy(p, r);
		}
	}
}
void CheckForRegEx(char str[]){
	int i, len = strlen(str);
	if (str[0] == '^'){
		setreg[1] = 1;
	}
	if (str[len - 1] == '$')
		setreg[2] = 1;
	for (i = 0; i < len; i++){
		if (str[i] == '?')
			setreg[3] = 1;
		if (str[i] == '.')
			setreg[4] = 1;
	}	
}
