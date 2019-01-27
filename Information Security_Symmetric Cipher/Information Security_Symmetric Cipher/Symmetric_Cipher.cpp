#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
FILE *in, *out;
int keys[11][34], Plaintext[65], Result[12][65], ciphertext[65];
int Sbox1[17] = { 14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7 };
int Sbox2[17] = { 5, 6, 12, 15, 8, 10, 0, 4, 11, 3, 7, 13, 14, 1, 2, 9 };
int Permute[32] = { 29, 1, 17, 8, 30, 22, 28, 6, 18, 4, 12, 19, 21, 26, 2, 20, 31, 10, 9, 25, 13, 0, 23, 15, 3, 27, 5, 11, 7, 14, 24, 16 };
int Left[33], Right[33], XOR[33], Sb[9], Bp[33], Ap[33];
int check = 0;
void GetKey() { // KEY 입력
	int i, j, n;
	for (i = 1; i <= 4; i++) {
		fscanf(in, "%X", &n);
		for (j = 1; j <= 8; j++) {
			keys[0][i * 8 - j + 1] = n % 2;
			n = n / 2;
		}
	}
}
void KeyGenerate() { // 10개의 Subkey 생성
	int i, j;
	for (i = 1; i <= 10; i++) {
		for (j = 1; j <= 32; j++) {
			if (j == 16) keys[i][j] = keys[i - 1][1];
			else if (j == 32) keys[i][j] = keys[i - 1][17];
			else keys[i][j] = keys[i - 1][j + 1];
		}
	}
}
void GetPT() { // Plaintext 입력
	int i, j, n;
	for (i = 1; i <= 8; i++) {
		fscanf(in, "%X", &n);
		for (j = 1; j <= 8; j++) {
			Plaintext[i * 8 - j + 1] = n % 2;
			n /= 2;
		}
	}
}
void GetCT() { //Ciphertext 입력
	int i, j, n;
	for (i = 1; i <= 8; i++) {
		fscanf(in, "%X", &n);
		for (j = 1; j <= 8; j++) {
			ciphertext[i * 8 - j + 1] = n % 2;
			n /= 2;
		}
	}
}
void Encryption() { // Encryption Process
	int i, j, sum, x;
	for (i = 1; i <= 64; i++) {
		Result[0][i] = Plaintext[i];
	}
	for (i = 1; i <= 10; i++) {
		for (j = 1; j <= 32; j++) { // Left / Right 분할
			Left[j] = Result[i - 1][j];
			Result[i][j] = Right[j] = Result[i - 1][j + 32]; // 이전 결과의 Right를 이번 결과의 Left에 입력
			XOR[j] = Right[j] ^ keys[i][j]; // XOR연산
		}
		sum = 0;
		x = 1;
		for (j = 32; j >= 1; j--) { // S-box를 사용하기 위한 16진수화.
			sum += x * XOR[j];
			x *= 2;
			if ((j - 1) % 4 == 0) {
				Sb[j / 4 + 1] = sum;
				sum = 0;
				x = 1;
			}
		}
		for (j = 1; j <= 8; j++) { // S-box연산
			if (j % 2 == 1) Sb[j] = Sbox1[Sb[j]];
			else Sb[j] = Sbox2[Sb[j]];
		}
		x = 8;
		for (j = 32; j >= 1; j--) { // Permutation을 사용하기 위한 2진수화.
			Bp[j] = Sb[x] % 2;
			Sb[x] /= 2;
			if ((j - 1) % 4 == 0) x--;
		}
		for (j = 1; j <= 32; j++) { // Permutation Function.
			Ap[Permute[j - 1] + 1] = Bp[j];
		}
		for (j = 1; j <= 32; j++) { // Function의 결과와 Left의 XOR 연산
			Result[i][j + 32] = Ap[j] ^ Left[j];
		}
		x = 8;
		sum = 0;
	}
	for (i = 1; i <= 32; i++) { // 마지막 Left와 Right의 Position Change
		Result[11][i] = Result[10][i + 32];
		Result[11][i + 32] = Result[10][i];
	}

}
void GuessKey(int i) { // Key를 찾기 위한 Brute-Force Attack
	if (i == 33 && check == 0) {
		KeyGenerate();
		Encryption();
		int j;
		for (j = 1; j <= 64; j++) {
			if (ciphertext[j] != Result[11][j]) break;
		}
		if (j == 65) {
			int k, sum, x;
			sum = 0;
			x = 8;
			for (k = 1; k <= 32; k++) {
				sum += x * keys[0][k];
				x /= 2;
				if (x == 0) {
					fprintf(out, "%X", sum);
					sum = 0;
					x = 8;
				}
				if (k % 8 == 0) fprintf(out, " ");
			}
			check = 1; // Key를 찾았음을 표시.
		}
		else return;
	}
	else if (check == 0) {
		keys[0][i] = 0;
		GuessKey(i + 1);
		keys[0][i] = 1;
		GuessKey(i + 1);
	}
	else {
		return;
	}
}
int main() {
	in = fopen("INPUT.TXT", "r");
	out = fopen("OUTPUT.TXT", "w");
	int z;
	fscanf(in, "%d", &z);
	if (z == 1) { // 실행목적이 Key와 Plaintext가 주어질 때 Encryption을 수행하기 위함인지 판별.
		int i, x, sum;
		GetKey();
		KeyGenerate();
		GetPT();
		Encryption();
		x = 8;
		sum = 0;
		for (i = 1; i <= 64; i++) {
			sum += Result[11][i] * x;
			x /= 2;
			if (x == 0) {
				fprintf(out, "%X", sum);
				x = 8;
				sum = 0;
			}
			if (i % 8 == 0) fprintf(out, " ");
		}
	}
	else { // 실행목적이 Plaintext와 Ciphertext가 주어질 때 Key를 찾기 위함인지 판별.
		GetPT();
		GetCT();
		keys[0][1] = 1;
		keys[0][2] = 1;
		keys[0][3] = 1;
		keys[0][4] = 1;
		keys[0][5] = 0;
		keys[0][6] = 0;
		keys[0][7] = 1;
		keys[0][8] = 0; // 주어진 힌트 Key의 앞 8비트가 F2이므로 기본값 입력.
		GuessKey(9);
	}
}