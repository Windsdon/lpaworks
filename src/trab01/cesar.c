/*
 * cesar.c
 *
 * 	Decifra a Cifra de César através da análise de frequência
 *
 *      Authors: João Pedro Finoto Martins    <joao.finoto.martins@usp.br>  nº USP 8549938
 *               Marcos Túlio Campos Cândido  <marcos.tulio.candido@usp.br> nº USP 8549917
 *
 *        Turma: Mecatrônica
 */

#include <stdio.h>
#include <math.h>

const char expectedDistribution[] = { 'a', 'e', 'o', 's', 'r', 'i', 'n', 'd',
		'm', 't', 'u', 'c', 'l', 'p', 'v', 'g', 'h', 'q', 'b', 'f', 'z', 'j',
		'x', 'k', 'w', 'y' };

char getTransform(char c, int shift) {
	return abs((c + shift) % 26);
}

int find(const char *h, char n, int c) {
	int i;

	for (i = 0; i < c; i++) {
		if (h[i] == n) {
			return i;
		}
	}
	return -1;
}

void calculateFrequence(char *text, char *order, int *f) {
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
			if ((f[i] < f[i + 1])
					|| (f[i] == f[i + 1]
							&& (find(expectedDistribution, order[i], 26)
									> find(expectedDistribution, order[i + 1], 26)))) {
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

void decifrar(char * mensagem, char * resultado) {

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

	return sum / span;
}

int main(int argc, char **argv) {
	char teste[] = "a frequencia de letras em um texto tem sido frequentemente "
			"estudada para uso em criptografia e analise de frequencia "
			"em particular. ";
	char teste2[] = "g lxkwaktiog jk rkzxgy ks as zkdzu zks yoju lxkwaktzksktzk kyzajgjg vgxg ayu ks ixovzumxglog k gtgroyk jk lxkwaktiog ks vgxzoiargx";
	int i;
	char order[26];
	int freq[26];

	calculateFrequence(teste2, order, freq);

	printf("Analyzing: %s\n", teste2);

	for (i = 0; i < 26; i++) {
		printf("%c (%c) - %d\n", order[i], expectedDistribution[i], freq[i]);
	}

	printf("Median deviation: %d\n", getDeviation(order, freq));

	return 0;
}

