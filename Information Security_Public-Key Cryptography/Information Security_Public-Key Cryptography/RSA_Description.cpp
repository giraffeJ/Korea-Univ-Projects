#include<stdio.h>
unsigned long long n = 18444164967047483891, e = 29, C = 21;
int k;
unsigned long long addmod(unsigned long long a, unsigned long long b, unsigned long long c)
{
	unsigned long long x = a % c;
	unsigned long long y = b % c;
	if (x == 0) return y;
	if (y == 0) return x;
	if (x + y <= x) return (x - (c - y)) % c;
	return (x + y) % c;
}
unsigned long long mulmod(unsigned long long a, unsigned long long b, unsigned long long c)
{
	unsigned long long x = a % c, y = b % c, d;
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
unsigned long long factoring(unsigned long long x) {
	unsigned long long k;
	for (k = 2; k <= x; k++) {
		if (x%k == 0) return k;
	}
	return 0;
}
unsigned long long ex_euclid(unsigned long long E, unsigned long long PI) {
	unsigned long long a1, a2, a3, b1, b2, b3, q, temp, ch = 1;
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
int main() {
	unsigned long long p, q, pie, d, dmodp, dmodq, res_p, res_q, inverse_p, inverse_q, Plaintext;
	p = factoring(n);
	q = n / p;
	printf("factors : p=%llu, q=%llu\n", p, q);
	pie = (p - 1)*(q - 1);
	printf("pie : %llu\n", pie);
	d = ex_euclid(e, pie);
	printf("decryption key : %llu\n\n", d);

	dmodp = d % (p - 1);
	printf("d mod (p-1) = %llu\n", dmodp);
	res_p = exmod(C, dmodp, p);
	printf("C^d mod p = %llu\n", res_p);

	inverse_q = ex_euclid(q, p);
	printf("q^-1 mod p = %llu\n\n", inverse_q);

	dmodq = d % (q - 1);
	printf("d mod (q-1) = %llu\n", dmodq);
	res_q = exmod(C, dmodq, q);
	printf("C^d mod q = %llu\n", res_q);
	inverse_p = ex_euclid(p, q);
	printf("p^-1 mod q = %llu\n", inverse_p);
	Plaintext = addmod(mulmod(mulmod(p, inverse_p, n), res_q, n), \
		mulmod(mulmod(q, inverse_q, n), res_p, n), n);
	printf("Plaintext = %llu\n", Plaintext);
	scanf_s("%d", &k);
}