/* Solution to comp10002 Assignment 1, 2019 semester 2.

Authorship Declaration:

(1) I certify that the program contained in this submission is completely
my own individual work, except where explicitly noted by comments that
provide details otherwise.  I understand that work that has been developed
by another student, or by me in collaboration with other students,
or by non-students as a result of request, solicitation, or payment,
may not be submitted for assessment in this subject.  I understand that
submitting for assessment work developed by or in collaboration with
other students or non-students constitutes Academic Misconduct, and
may be penalized by mark deductions, or by other penalties determined
via the University of Melbourne Academic Honesty Policy, as described
at https://academicintegrity.unimelb.edu.au.

(2) I also certify that I have not provided a copy of this work in either
softcopy or hardcopy or any other form to any other student, and nor will
I do so until after the marks are released. I understand that providing
my work to other students, regardless of my intention or any undertakings
made to me by that other student, is also Academic Misconduct.

(3) I further understand that providing a copy of the assignment
specification to any form of code authoring or assignment tutoring
service, or drawing the attention of others to such services and code
that may have been made available via such a service, may be regarded
as Student General Misconduct (interfering with the teaching activities
of the University and/or inciting others to commit Academic Misconduct).
I understand that an allegation of Student General Misconduct may arise
regardless of whether or not I personally make use of such solutions
or sought benefit from such actions.

Signed by: Khant Thurein Han 1028138
Dated:     10/9/2019

*/

/* Libraries that are used in the assigment*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

/* Constants*/
#define PIE 3.1415926
#define MAX_CHAR 999
#define LINE_LEN 50
#define RES_LEN 9000
#define IN_LINE 0
#define IN_BREAK 1
#define IN_NEW_PARA 2
#define MAX_HEADINGS 5
#define DEFAULT_MARGIN 4


typedef char word_t[MAX_CHAR +1];

/* Declared functions*/
void newpara(char result[], int *reslen, int *spacelen, int *clinelen);
void newbreak(char result[], int *reslen, int *spacelen, int *clinelen);
int mygetchar();
int getword(char line[], char word[], int *len, int *linei, int linelen);
int buildline(char result[], int *reslen);
int wordfit(int *len, int remain);
int readline(char line[], int *linelen);
void formatcom(char result[], char line[], char word[], int *reslen,
	int *spacelen, int *clinelen, int *linei,int linelength, int *charperline,
	int *breaktrue, int *cursor_s, char heading[]);
void addtostr(char result[], char word[], int wordlen, int *reslen, int *clinelen);
void centerline(char centered[], char line[],int *cenlen, int charperline);
void drawline(char result[], int *resi, int *spacelength, int *clinelength,
	int cpl, int cursor_state, int num);
void createheader(char result[],char line[],int num,int lineindex,int *resi,
	int *spacelength,int *clinelength, char heading[]);

/* driver code*/
int main(int argc, char* argv[]) {
	char sentence[RES_LEN];
	int reslen = 0;
	buildline(sentence, &reslen);
	printf("%s", sentence);
return 0;
}
/* Gets the next word in a given line starting from a specific
line index*/
int getword(char line[], char word[], int *len, int *linei, int linelen){
	int wordi = 0;
	int lineindex = *linei, wordlen = 0;
	char c;
	/*ignore everything until the beginning of a word is found*/
	while(lineindex != linelen && (c=line[lineindex])!='\0' &&
			(isspace(c) || c == '\t')){
			lineindex++;
	}
	/*creates a word*/
	while(!isspace(c=line[lineindex]) && c != '\0'){
			lineindex++;
			word[wordi++] = c;
			wordlen++;
	}

	word[wordi] = '\0';
	*len = wordlen;
	*linei = lineindex;
	if (c == '\0'){
			return 1;
	}

	return 0;
}
/* Adds a string to another string while updating index values of the result
and current line position*/
void addtostr(char result[], char word[], int wordlen, int *reslen, int *clinelen){
	int resi = *reslen;
	int clinei = *clinelen;
	strncpy(&result[resi], word, wordlen);
	resi += wordlen;
	result[resi++] = ' ';
	result[resi] = '\0';
	*reslen = resi;
	*clinelen = clinei + wordlen + 1;
	}
/* Create margin of specified length without updating current line position*/
void addspace(char result[], int spacelen, int *reslen){
	int resi = *reslen,i;
	char space[spacelen+1];
	for (i=0; i<spacelen;i++){
			space[i] = ' ';
	}
	strncpy(&result[resi], space, spacelen);
	resi += spacelen;
	result[resi] = '\0';
	*reslen = resi;
}

/* Builds the whole result based on input*/
int buildline(char result[], int *reslen){
	word_t line, word;
	int resi=0;
	int wordlen = 0, linei = 0, clinelen=0, endline, linelength = 0, end,
	spacelen = DEFAULT_MARGIN, charperline = LINE_LEN, breaktrue = 0, cursor_state = IN_BREAK;
	char heading[MAX_HEADINGS + 1] = {'0','0','0','0','0','\0'};
	addspace(result, spacelen, &resi);
	do{
			/* Get a single line from input*/
			end = readline(line, &linelength);
			linei = 0;
			do{		/* break the line down into words*/
					endline = getword(line, word, &wordlen, &linei, linelength);
					//printf(":::::::: %s  ::: %d\n", word, wordlen);
					//printf("::::::::CWCW %d\n", (clinelen + wordlen));
					if (wordlen == 0){
							break;
					}
					/* handles all commands or invalid sentences*/
					else if (line[0] == '.'){
							formatcom(result, line, word, &resi, &spacelen,&clinelen,
									&linei, linelength, &charperline, &breaktrue, &cursor_state,
									heading);
							if (breaktrue){
									breaktrue = 0;
									break;
							}
					}
					/*Word lengths that exceed text width get to exceed the text width*/
					else if (wordlen > charperline){
							newbreak(result, &resi, &spacelen, &clinelen);
							addtostr(result, word, wordlen, &resi, &clinelen);
							newbreak(result, &resi, &spacelen, &clinelen);
							cursor_state = IN_LINE;
					}
					/* Words that fit into the text width gets added*/
					else if((clinelen + wordlen)<= charperline){
							addtostr(result, word, wordlen, &resi, &clinelen);
							cursor_state = IN_LINE;
					}
					/* Words that do not fit into the remaining space starts
					on a new line*/
					else{
							newbreak(result, &resi, &spacelen, &clinelen);
							addtostr(result, word, wordlen, &resi, &clinelen);
							cursor_state = IN_LINE;
					}
			}while(endline == 0);
	}while(end != EOF);
	/*remove trailing margins or spaces*/
	if(cursor_state == IN_BREAK){
		int i;
		for (i = 0; i < spacelen; i++){
			resi--;
		}
		resi--;
		result[resi] = '\0';
	}
	else if(cursor_state == IN_LINE){
		resi--;
		result[resi]='\0';
	}
	*reslen = resi;

	return 0;
}

/* Gets a single line from input*/
int readline(char line[], int *linelen){
	char c;
	int i = 0;
	while((c = mygetchar()) != EOF && c != '\n'){
			line[i++] = c;
	}
	line[i] = '\0';
	if(c == EOF){
			return EOF;
	}
	*linelen = i;
	return 0;
}
/* Starts a new paragraph with margin*/
void newpara(char result[], int *reslen, int *spacelen, int *clinelen){
	int resi = *reslen, spacelength = *spacelen, clinelength = *clinelen;
	resi--;
	result[resi++] = '\n';
	result[resi++] = '\n';
	clinelength = 0;
	addspace(result, spacelength, &resi);
	*reslen = resi;
	*spacelen = spacelength;
	*clinelen = clinelength;
}
/* Starts a new line with margin*/
void newbreak(char result[], int *reslen, int *spacelen, int *clinelen){
	int resi = *reslen, spacelength = *spacelen, clinelength = *clinelen;
	resi--;
	result[resi++] = '\n';
	clinelength = 0;
	addspace(result, spacelength, &resi);
	*reslen = resi;
	*spacelen = spacelength;
	*clinelen = clinelength;
}
/* Handles all commands found in text*/
void formatcom(char result[], char line[], char word[], int *reslen,
	int *spacelen, int *clinelen, int *linei,int linelength, int *charperline,
	int *breaktrue, int *cursor_s, char heading[]){
	int resi = *reslen, clinelength = *clinelen, spacelength = *spacelen,
	cpl = *charperline, breakt = *breaktrue, cursor_state= *cursor_s, lineindex =*linei;
	/*handles break commands*/
	if (strcmp(word, ".b")==0){
			if(cursor_state == IN_LINE){
					newbreak(result, &resi, &spacelength, &clinelength);
			}
			cursor_state = IN_BREAK;
	}
	/*handles new paragraph commands*/
	else if (strcmp(word, ".p")==0){
			if(cursor_state == IN_BREAK){
				int i;	
				for (i = 0; i < spacelength; i++){
						resi--;
					}
					resi--;
					result[resi++] = ' ';
					result[resi] = '\0';
					newpara(result, &resi, &spacelength, &clinelength);
			}
			else if(cursor_state ==IN_LINE){
					newpara(result, &resi, &spacelength, &clinelength);
			}
			cursor_state = IN_NEW_PARA;
	}
	/* handles margin spacing commands*/
	else if (strcmp(word, ".l")==0){
			int wordlen, num;
			getword(line, word, &wordlen, &lineindex, linelength);
			num = atoi(word);
			if (cursor_state != IN_LINE){
					resi -= spacelength + 1;
			}
			spacelength = num;
			newpara(result, &resi, &spacelength, &clinelength);
			cursor_state = IN_NEW_PARA;
	}
	/*handles text width commands*/
	else if (strcmp(word, ".w")==0){
			int wordlen, num;
			getword(line, word, &wordlen, &lineindex, linelength);
			num = atoi(word);
			cpl = num;
			if (cursor_state == IN_LINE || cursor_state == IN_BREAK){
					newpara(result, &resi, &spacelength, &clinelength);
			}
			cursor_state = IN_NEW_PARA;
	}
	/* handles center commands*/
	else if (strcmp(word, ".c")==0){
			char centered[MAX_CHAR];
			int cenlen;
			centerline(centered, line, &cenlen, cpl);
			//printf(":::::::CCCC%d...  \n", cenlen);
			if (cursor_state == IN_LINE){
					newbreak(result, &resi, &spacelength, &clinelength);
			}
			addtostr(result, centered, cenlen, &resi, &clinelength);
			newbreak(result, &resi, &spacelength, &clinelength);
			cursor_state = IN_BREAK;
	}
	/* handles header commands*/
	else if (strcmp(word, ".h")==0){
			int wordlen,num;
			getword(line, word, &wordlen, &lineindex, linelength);
			num = atoi(word);
			drawline(result, &resi, &spacelength, &clinelength, cpl, cursor_state,
					num);
			createheader(result,line, num, lineindex, &resi,&spacelength,
					&clinelength, heading);
			cursor_state = IN_NEW_PARA;
	}
	else{
			breakt = 1;
	}
	*reslen = resi;
	*clinelen = clinelength;                      
	*spacelen = spacelength;
	*charperline = cpl;
	*linei = lineindex;
	*breaktrue = breakt;
	*cursor_s = cursor_state;
}

/* creates a centered string .wbased on the remaining contents*/
void centerline(char centered[], char line[], int *cenlen, int charperline){
	int linecen, maxlinecen, starti, clinelen = 0, cremainlen=0, linelen;
	int endline, remaini = 0, first = 0;
	linelen = strlen(line);
	int wordlen, lineindex = 0;
	word_t word;
	
	/* Form a string of remaining contents*/
	char remain[MAX_CHAR];
	do{                                                
			endline = getword(line, word, &wordlen, &lineindex, linelen);
			/* Check if its the first word as we dont want to include the 
			command into the remaining characters.*/
			if (first==0){
				first++;
			}
			else{
				addtostr(remain, word, wordlen, &remaini, &cremainlen);
			}
	}while (endline == 0);
	remaini--;
	remain[remaini++]='\0';

	/* Calculate the number of spaces needed to center*/
	linelen = strlen(remain);
	maxlinecen = charperline/2;
	linecen = (linelen)/2;
	starti = maxlinecen - linecen;
	if((linelen-1) > charperline){
			starti = 1;
	}
	/*Create a centered string*/
	addspace(centered, starti, &clinelen);
	strcpy(&centered[clinelen], remain);
	*cenlen = linelen + starti;
}
/* Creates a dashline of current value of character per line*/
void drawline(char result[], int *resi, int *spacelength, int *clinelength,
	int cpl, int cursor_state, int num){
	int i=0, reslen = *resi, spacelen = *spacelength, clinelen = *clinelength;
	/* Make sure that the line is drawn in a new paragraph*/
	if (cursor_state == IN_LINE){
					newpara(result, &reslen, &spacelen, &clinelen);
			}
			else if(cursor_state == IN_BREAK){
				int i;
				for (i = 0; i < spacelen; i++){
					reslen--;
				}
				reslen--;
				result[reslen++] = ' ';
				result[reslen] = '\0';
				newpara(result, &reslen, &spacelen, &clinelen);
			}
			else{

			}
			if (num == 1){
					char dashline[cpl];
					for (;i<cpl;i++){
							dashline[i]= '-';
					}
					dashline[i] = '\0';
					addtostr(result, dashline, cpl,&reslen,&clinelen);
					newbreak(result, &reslen,&spacelen, &clinelen);
					cursor_state = IN_BREAK;
			}
	*resi = reslen;
	*spacelength = spacelen;
	*clinelength = clinelen;
}
/* Creates a header */
void createheader(char result[],char line[],int num,int lineindex,int *resi,
	int *spacelength,int *clinelength, char heading[]){
	int headeri=0, headingi= 4, endline, remaini = 0;
	int wordlen = 0, clinelen=0, linelength = strlen(line);
	word_t word;
	static int prevnum = 0;
	char remain[MAX_CHAR], header[MAX_CHAR];
	do{
			endline = getword(line, word, &wordlen, &lineindex, linelength);

			addtostr(remain, word, wordlen, &remaini, &clinelen);
	}while (endline == 0);
	remaini--;
	clinelen--;
	remain[remaini++]='\0';
	if (prevnum > num){
			for(;(headingi+1) > num; headingi--){
					heading[headingi] = '0';
			}
			heading[num-1] += 1;
	}
	else if (prevnum == num){
			heading[num-1] += 1;
	}
	else {
		heading[num-1] += 1;
	}
	headingi = 0;
	for (; headingi < num;){
			if (headingi != 0){
					header[headeri++] = '.';
			}
			header[headeri++] = heading[headingi++];
	}
	header[headeri++] = ' ';
	header[headeri] = '\0';
	strcat(header,remain);
	//printf("HEre   %s\n", header);
	addtostr(result, header, strlen(header), resi, clinelength);
	newpara(result, resi, spacelength, clinelength);
	prevnum = num;

}
/* getchar() code obtained from Assigment1 FAQ to avoid potential erros*/
int mygetchar() {
	int c;
	while ((c=getchar())=='\r') {
	}
	return c;
}
