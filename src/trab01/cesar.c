/*
 * cesar.c
 *
 * 	Decifra a Cifra de César através da análise de frequência alfabética
 *
 *      Authors: João Pedro Finoto Martins    <joao.finoto.martins@usp.br>  n� USP 8549938
 *               Marcos Túlio Campos Cândido  <marcos.tulio.candido@usp.br> n� USP 8549917
 *
 *        Turma: Mecatrônica
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

/**
 * A sequencia de letras esperadas de um texto em portugues
 */
const char expectedDistribution[] = { 'a', 'e', 'o', 's', 'r', 'i', 'n', 'd',
		'm', 't', 'u', 'c', 'l', 'p', 'v', 'g', 'h', 'q', 'b', 'f', 'z', 'j',
		'x', 'k', 'w', 'y' };

/**
 * Transforma uma única letra. C deve ser um número entre 0 e 25.
 * Isso pode ser feito fazendo (c - 'a').
 */
char getTransform(char c, int shift) {
	while (shift < 0) {
		shift += 26;
	}
	return abs((c + shift) % 26);
}

/**
 * Realiza o deslocamento de n em todas as letras de input
 * e armazena o resultado em output.
 */
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

/**
 * Calcula a frequencia de cada letra e armazena em order e f,
 * da maior para a menor frequencia.
 * Order contém as letras, e f a frequencia.
 */
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

/**
 * Calcula a distância entre a distribuição dada
 * e a distribuição esperada para um texto em
 * português
 */
int getDeviation(char *order, int *freq) {
	int i, j, sum = 0, span = 26;

	for (i = 0; i < span && freq[i]; i++) {
		for (j = i; expectedDistribution[j] != order[i]; j++) {
			if (j >= 26) {
				//retornar para a primeira letra
				j = -1;
			}
		}
		sum += (i - j);
	}

	return sum;
}

/**
 * Verifica alguns traços linguísticos para saber
 * se o texto tem chance de ser válido
 */
int isValid(char *m) {
	int countConsonant = 0, countVowels = 0, currentCount = 0;
	int i;
	int tamanho;

	for(tamanho = 0; m[tamanho]; tamanho++);

	for (i = 0; m[i]; i++) {
		char c = m[i];
		//final de palavra ou final de frase
		if (c == ' ' || i == tamanho - 1) {
			if (currentCount == 1) {
				//letras sozinhas devem ser 'a', 'e' ou 'o'
				switch (m[i - 1]) {
				case 'a':
				case 'o':
				case 'e':
					break;
				default:
					return 0;
				}
			}
#ifndef ESTRANGEIRISMOS
			else {
				//última letra não pode ser uma dessas
				switch (c == ' ' ? m[i - 1] : c) {
				case 'h':
				case 'c':
				case 'n':
				case 'p':
				case 't':
				case 'q':
				case 'v':
				case 'd':
				case 'f':
					return 0;
				}
			}
#endif
			currentCount = 0;
			countConsonant = 0;
			countVowels = 0;
		} else {
			currentCount++;
			switch (c) {
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

			//m antes de p e b
			if (c == 'p' || c == 'b') {
				if (m[i - 1] == 'n') {
					return 0;
				}
			}

			//maximo número de consoantes e vogais
			if (countVowels > 3 || countConsonant > 4) {
				return 0;
			}

		}
	}

	return 1;
}

void decifrar(char * mensagem, char * resultado) {
	int i, minDev, minI, dev;
	char order[26]; //vetor para a sequencia de frequencia
	int freq[26]; //vetor para a quantidade de cada letra

	char *temp = malloc((strlen(mensagem) + 1) * sizeof(char));

	//testar todas as possibilidades
	for (i = 0; i < 26; i++) {
		//decifrar usando a chave atual
		transform(mensagem, temp, i);
		//calcular a frequencia de letras
		calculateFrequency(temp, order, freq);
		//calcular o desvio com relação à distribuição esperada
		dev = abs(getDeviation(order, freq));
		if (!i || (dev < minDev && isValid(temp))) {
			//armazenar o menor desvio
			minDev = dev;
			minI = i;
		}
		if (!minDev) {
			//se a distancia for zero não é necessário
			//continuar a verificação
			break;
		}
	}

	free(temp);

	//usando o menor desvio, traduza a mensagem
	transform(mensagem, resultado, minI);
}
