#include<stdio.h>
long long C1 = 187341129, C2 = 1212049520, q = 1605333871, a = 43, Ya = 22;
long long Xa, M, K, k, inverse_K_mod_q;
long long addmod(long long a, long long b, long long c)
{
	long long x = a % c;
	long long y = b % c;
	if (x == 0) return y;
	if (y == 0) return x;
	if (x + y <= x) return (x - (c - y)) % c;
	return (x + y) % c;
}
long long mulmod(long long a, long long b, long long c)
{
	long long x = a % c, y = b % c, d;
	if (x == 0 || y == 0) return 0;
	if (x == 1) return y;
	if (y == 1) return x;
	d = mulmod(x, y / 2, c);
	if (y % 2 == 0) return addmod(d, d, c);
	return addmod(x, addmod(d, d, c), c);
}
long long exmod(long long a, long long b, long long c) {
	if (a == 0) return 0;
	if (b == 0) return 1;
	if (a == 1) return 1;
	if (b == 1) return a;
	long long d;
	d = exmod(a, b / 2, c);
	if (b % 2 == 0) return mulmod(d, d, c);
	else return mulmod(mulmod(d, d, c), a, c);
}
long long ex_euclid(long long E, long long PI) {
	long long a1, a2, a3, b1, b2, b3, q, temp, ch = 1;
	a1 = 1;
	a2 = 0;
	a3 = PI;
	b1 = 0;
	b2 = 1;
	b3 = E;
	while (1) {
		q = a3 / b3;
		temp = a1;
		a1 = b1;
		b1 = temp - (b1*q);
		temp = a2;
		a2 = b2;
		b2 = temp - (b2*q);
		temp = a3;
		a3 = b3;
		b3 = temp - (b3*q);
		ch *= -1;
		if (b3 == 1) {
			if (ch == -1) return b2 + PI;
			else return b2;
		}
		if (b3 == 0) return 0;
	}
	return 0;
}
long long find_exmodulo(long long res, long long CT, long long x) {
	long long pw, result;
	result = CT;
	if (result%x == res) return 1;
	for (pw = 2; pw <= q - 1; pw++) {
		result *= CT;
		result %= x;
		if (result == res) return pw;
	}
	return 0;
}
int main() {
	long long i, check_C1, check_C2;
	Xa = find_exmodulo(Ya, a, q);
	printf("Xa = %lld\n", Xa);
	K = exmod(C1, Xa, q);
	printf("K = %lld\n", K);
	inverse_K_mod_q = ex_euclid(K, q);
	printf("inverse of K mod q = %lld\n", inverse_K_mod_q);
	M = mulmod(C2, inverse_K_mod_q, q);
	printf("Plaintext = %lld\n", M);
	scanf_s("%lld", &i);
}