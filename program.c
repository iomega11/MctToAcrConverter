#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_INPUT_FILENAME "original.mct"
#define DEFAULT_OUTPUT_FILENAME "converted.dump"

#define USAGE "\
Ce programme sert a convertir le fichier cree par l'application MIFARE Classic Tool \
au format attendu par le logiciel Windows du lecteur ACR122U.\n \
Pour lancer le programme, faire un glisser-deposer du fichier a convertir sur le programme. Le fichier \"converted.dump\" sera alors cree.\n \
Il est aussi possible de lancer le programme via l'invite de commandes ou le PowerShell :\n \
Convertisseur.exe <fichierSource>\n \
Ou encore :\n \
Convertisseur.exe <fichierSource> <fichierDestination>\n \
Appuyer sur Entree pour fermer le programme\n"

void convertLine(char* input, char* res)  {
	sscanf(input, "%2x%2x%2x%2x%2x%2x%2x%2x%2x%2x%2x%2x%2x%2x%2x%2x", 
	&res[0], &res[1], &res[2], &res[3], &res[4], &res[5], &res[6], &res[7], 
	&res[8], &res[9], &res[10], &res[11], &res[12], &res[13], &res[14], &res[15]);
}

void printString(char* string, int length){
	for(int i = 0; i < length; i++) {
		printf("%c",string[i]);
	}
}

void convertFile(char* inputFilename, char* outputFilename){
	FILE *input, *output;
	char line[32];
	char res[16];
	int nbReadChars;
	input = fopen (inputFilename, "r");
	output = fopen (outputFilename, "wb");
	while(fscanf(input, "%s\n", line) > 0) {
		nbReadChars = strlen(line);
		if(nbReadChars == 32) { // Skipping lines with "+Sector: X"
			printf("%s\n", line);
			convertLine(line, res);
			//printf("Converted: '");
			//printString(res, 16);
			//printf("'\n");
			fwrite(res, sizeof(char), 16, output);
		}
	}
	fclose(input);
	fclose(output);	
	printf("=> Enregistré dans %s\nAppuyer sur Entree pour fermer le programme", outputFilename);
	getchar(); // Pause before exiting
}

int main(int argc, char** argv) {
	char *inputFilename = DEFAULT_INPUT_FILENAME, *outputFilename = DEFAULT_OUTPUT_FILENAME;
	if(argc == 3) { // Input & output files given as parameters via command line
		inputFilename = argv[1];
		outputFilename = argv[2];
	} else if(argc == 2) { // Input file given as parameter via command line or drag & drop
		inputFilename = argv[1];
	} else { // No argument or too much arguments, showing usage
		printf(USAGE);
		getchar(); // Pause so that user can see Usage
		return(1);
	} 
	convertFile(inputFilename, outputFilename);
	return(0);
}