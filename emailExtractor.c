#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>
#define BUFFERLENGTH 100000
/*
Simple e-mail extractor of file in C
*/
void findAllEmail(char *str, FILE *fp);
void clear(char *buffer);

int main(int argc, char **argv)
{
	if(argc != 3){
		printf("Usage: %s <input file> <output file>\n", argv[0]);
		return 0;
	}
	char buffer[BUFFERLENGTH];
	FILE *outputFile = fopen(argv[2], "a+");
	FILE *inputFile = fopen(argv[1], "r");

	while(!feof(inputFile)){
		fgets(buffer, BUFFERLENGTH, inputFile);
		clear(buffer);
		findAllEmail(buffer, outputFile);
	}

	fclose(outputFile);
	fclose(inputFile);
	return 0;
}

void findAllEmail(char *str, FILE *fp)
{
	int start = 0;
	int error;
	char email[BUFFERLENGTH];
	regmatch_t match;
	regex_t reg;

	regcomp(&reg , "[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,4}", REG_EXTENDED);

	error = regexec(&reg, str, 1, &match, 0);
	while(error == 0) {
		memset(email, '\0', BUFFERLENGTH);
		memcpy(email, &str[match.rm_so + start], match.rm_eo - match.rm_so);
		email[strlen(email)] = '\n';
		fputs(email, fp);
		start +=match.rm_eo;
		error = regexec(&reg, str+start, 1, &match, REG_NOTBOL);
	}
	regfree(&reg);
}

void clear(char *buffer){
	char *posChar;
	if ((posChar = strchr(buffer, '\n')) != NULL){
		*posChar = '\0';
	}
	if ((posChar = strchr(buffer, '\r')) != NULL){
		*posChar = '\0';
	}
	if ((posChar = strchr(buffer, '"')) != NULL){
		*posChar = ' ';
	}
	if ((posChar = strchr(buffer, ',')) != NULL){
		*posChar = ' ';
	}
	if ((posChar = strchr(buffer, '\'')) != NULL){
		*posChar = ' ';
	}

}
