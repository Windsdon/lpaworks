/*
 * cesar.c
 *
 * 	Decifra a Cifra de C�sar atrav�s da an�lise de frequ�ncia alfab�tica
 *
 *      Authors: Jo�o Pedro Finoto Martins    <joao.finoto.martins@usp.br>  n� USP 8549938
 *               Marcos T�lio Campos C�ndido  <marcos.tulio.candido@usp.br> n� USP 8549917
 *
 *        Turma: Mecatr�nica
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

const char expectedDistribution[] = { 'a', 'e', 'o', 's', 'r', 'i', 'n', 'd',
		'm', 't', 'u', 'c', 'l', 'p', 'v', 'g', 'h', 'q', 'b', 'f', 'z', 'j',
		'x', 'k', 'w', 'y' };

char getTransform(char c, int shift) {
	return abs((c + shift) % 26);
}

void transform(const char *input, char *output, int n) {
	int i;
	char c;

	for (i = 0; input[i]; i++) {
		c = input[i];
		if (c < 'a' || c > 'z') {
			output[i] = c;
			continue;
		}
		output[i] = 'a' + getTransform(c - 'a', n);
	}

	output[i] = 0;
}

void calculateFrequency(char *text, char *order, int *f) {
	int i, changed = 0, skip = 1;
	for (i = 0; i < 26; i++) {
		f[i] = 0;
		order[i] = 'a' + i;
	}
	for (i = 0; text[i]; i++) {
		int c = text[i] - 'a';
		if (c >= 0 && c < 26) {
			f[c]++;
		}
	}

	do {
		changed = 0;
		for (i = 0; i < 26 - skip; i++) {
			if ((f[i] < f[i + 1])) {
				int ftemp = f[i];
				char ctemp = order[i];
				f[i] = f[i + 1];
				f[i + 1] = ftemp;
				order[i] = order[i + 1];
				order[i + 1] = ctemp;
				changed = 1;
			}
		}
		skip++;
	} while (changed);
}

int getDeviation(char *order, int *freq) {
	int i, j, sum = 0, span = 26;

	for (i = 0; i < span && freq[i]; i++) {
		for (j = i; expectedDistribution[j] != order[i]; j++) {
			if (j >= 26) {
				j = -1;
			}
		}
		sum += (i - j);
	}

	return sum;
}

int isValid(char *m) {
	int countConsonant = 0, countVowels = 0, currentCount = 0;
	int i;
	for(i = 0; m[i]; i++){
		char c = m[i];
		if(c == ' '){
			if(currentCount == 1){
				//stand alone letters must be a, e or o
				switch(m[i - 1]){
				case 'a':
				case 'o':
				case 'e':
					break;
				default:
					return 0;
				}
			}
			currentCount = 0;
			countConsonant = 0;
			countVowels = 0;
		}else{
			currentCount++;
			switch(c){
			case 'a':
			case 'e':
			case 'i':
			case 'o':
			case 'u':
				countVowels++;
				countConsonant = 0;
				break;
			default:
				countConsonant++;
				countVowels = 0;
			}

			//maximum number of vowels and consonants
			if(countVowels > 3 || countConsonant > 4){
				return 0;
			}

		}
	}

	return 1;
}

void decifrar(char * mensagem, char * resultado) {
	int length = strlen(mensagem);
	int i, minDev, minI, dev;
	char order[26];
	int freq[26];

	for (i = 0; i < 26; i++) {
		transform(mensagem, resultado, i);
		calculateFrequency(resultado, order, freq);
		dev = abs(getDeviation(order, freq));
		if (!i  || (dev < minDev && isValid(resultado))) {
			minDev = dev;
			minI = i;
		}
		if (!minDev) {
			break;
		}
	}

	transform(mensagem, resultado, minI);

	printf("Probable shift: %d, dev: %d\nString: ", minI, minDev);
	puts(resultado);
}

int main(int argc, char **argv) {
	char teste[] = "g lxkwaktiog jk rkzxgy ks as zkdzu zks "
			"yoju lxkwaktzksktzk kyzajgjg vgxg ayu ks "
			"ixovzumxglog k gtgroyk jk lxkwaktiog ks "
			"vgxzoiargx";
	char teste2[] = "i am immune to your stratagems";

	char *testef = malloc(100 * (sizeof(char)));


	transform(teste2, teste2, 5);

	puts(teste2);

	decifrar(teste, testef);

	return 0;
}

