#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<random>
__int64 current_state, my_state, opp_state; // ��ü �����Ȳ ( ������ 1 ������ 0 ), ��(��ǻ��) ��Ȳ, ���(����) ��Ȳ
__int64 mask1 = 0b111100011110001111000111100011110001111000; // ���� �¸�Ȯ�� ���� mask
__int64 mask2 = 0b111111111111111111111000000000000000000000; // ���� �¸�Ȯ�� ���� mask
__int64 mask3 = 0b111100011110001111000000000000000000000000; // ������ �� �¸�Ȯ�� ���� mask
__int64 mask4 = 0b000111100011110001111000000000000000000000; // ���� �� �¸�Ȯ�� ���� mask
int bottom[7] = { 41, 40, 39, 38, 37, 36, 35 }; // ���忡 �ٷ� ���� �� �ִ� �� ��ġ �� ���̿� ���� left shift value.
int record[44]; // �⺸ ���.
int eval_board[42] = { 3, 4, 5, 7, 5, 4, 3,
4, 6, 8, 10, 8, 6, 4,
5, 8, 11, 13, 11, 8, 5,
5, 8, 11, 13, 11, 8, 5,
4, 6, 8, 10, 8, 6, 4,
3, 4, 5, 7, 5, 4, 3 }; // evaluation�� ���̴� �� ĭ �� ����.
void prtboard(__int64 my_state, __int64 opp_state) { // ���� ��Ȳ ����Լ�
	int i, j, my, opp;//�ơܡ�
	int board[6][7];
	__int64 temp1 = my_state, temp2 = opp_state;
	for (i = 0; i < 6; i++) {
		for (j = 6; j >= 0; j--) {
			my = temp1 % 2;
			temp1 /= 2;
			opp = temp2 % 2;
			temp2 /= 2;
			if (my == 1) board[i][j] = 1;
			if (opp == 1) board[i][j] = 2;
			if (my == 0 && opp == 0) board[i][j] = 0;
		}
	}
	printf("\n");
	for (i = 0; i < 6; i++) {
		for (j = 0; j < 7; j++) {
			if (board[i][j] == 1) printf("[O] ");
			else if (board[i][j] == 2) printf("[X] ");
			else printf("[ ] ");
		}
		printf("\n");
	}
	for (i = 0; i < 7; i++)
		printf(" %d  ", i + 1);
	printf("\n");
}
int getchoice() { // ������ �Է��� �޴� �Լ�
	int input;
	printf("\n ���� ���� ��ġ�� ��ȣ�� �Է����ּ���. (1~7) ");
	scanf("%d", &input);
	while (input < 1 || input>7) { // �Է°��� ������ 1~7�� ����� ������ ���� �Է��� �� ���� ������ �ٽ� �޴´�.
		printf("\n 1���� 7������ ���ڸ� �Է����ּ���.");
		scanf("%d", &input);
	}
	return input;
}
__int64 check(__int64 state) { // ���� �Է¹��� state�� ���� ���� �밢������ ������ 4���� ���� �����ִ� �κ��� �ִ����� Ȯ���ϴ� �Լ�.
	__int64 check;
	check = state & state << 1 & state << 2 & state << 3 & mask1; // ����
	check |= state & state << 7 & state << 14 & state >> 7 & mask2; // ����
	check |= state & state << 8 & state << 16 & state >> 8 & mask3; // ������ �� �밢��
	check |= state & state << 6 & state << 12 & state << 18 & mask4; // ���� �� �밢��
	return check;
}
__int64 checkwin() { // ��(��ǻ��)�� �̰������ Ȯ���� �� �޽��� ���
	__int64 a = check(my_state);
	if (a) printf("������. ���� �̰���ϴ�!");
	return a;
}
__int64 checklose() { // ���(����)�� �̰���� Ȯ���� �� �޽��� ���
	__int64 a = check(opp_state);
	if (a) printf("�̷�.. ����� ���� �� �Ͻô±���.. ���� �����ϴ�..");
	return a;
}
int checkwinposition(__int64 my_state, int bot[7]) { // 1~7���� �� ���� ���� �� �� check�Լ��� ���� ���а� ������ �� ���θ� �Ǵ��Ͽ� ������ 0~6(���� ��ġ), ������ 7 ����.
	__int64 t = 1;
	int i;
	for (i = 0; i < 7; i++) {
		if (bot[i] >= 0) {
			if (check(my_state | t << bot[i])) break;
		}
	}
	return i;
}
int checksecondwin() { // 00110, 01010, 01100 �� �� ����� ���� �����ϴ��� �Ǵ��Ѵ�. �̴� �� ���� �� ���� Ȯ���� �̱� �� �ִ� ��쿡 ���Ѵ�.
	int i, j, my, opp, choice = -1;
	int b[10][10];
	__int64 temp1 = my_state, temp2 = opp_state;
	for (i = 0; i < 6; i++) { // ���� �Ǵ��ϱ� ���� �ӽ÷� ����� ��ȯ.
		for (j = 6; j >= 0; j--) {
			my = temp1 % 2;
			temp1 /= 2;
			opp = temp2 % 2;
			temp2 /= 2;
			if (my == 1) b[i][j] = 1;
			if (opp == 1) b[i][j] = 2;
			if (my == 0 && opp == 0) b[i][j] = 0;
		}
	}
	for (j = 0; j < 7; j++) b[7][j] = -1; // �ٴڿ��� index error�� �����ϱ� ���� �� ���� �� �ΰ� -1�� �ٴ����� ǥ���Ѵ�. !=0�� ���ǿ� �ɸ��� �ʰ� �ϱ� ���Ե� �ִ�.
	for (i = 0; i < 6; i++) {
		for (j = 0; j < 7; j++) { // ���η� 5���� ���� ���� �ϹǷ� �߾��� �������� �� �츦 ���ٰ� �� �� j�� 2~4 ( 3~5 ) �� ��츸 Ȯ���ؾ� �Ѵ�.
			if (j >= 2 && j <= 4 && b[i][j] == 0 && b[i + 1][j] != 0 && b[i][j - 1] == 1 && b[i][j + 1] == 1 && b[i][j - 2] == 0 && b[i + 1][j - 2] != 0 && b[i][j + 2] == 0 && b[i + 1][j + 2] != 0) return j;
			if (j >= 2 && j <= 4 && b[i][j + 1] == 0 && b[i + 1][j + 1] != 0 && b[i][j - 1] == 1 && b[i][j] == 1 && b[i][j - 2] == 0 && b[i + 1][j - 2] != 0 && b[i][j + 2] == 0 && b[i + 1][j + 2] != 0) return j + 1;
			if (j >= 2 && j <= 4 && b[i][j - 1] == 0 && b[i + 1][j - 1] != 0 && b[i][j] == 1 && b[i][j + 1] == 1 && b[i][j - 2] == 0 && b[i + 1][j - 2] != 0 && b[i][j + 2] == 0 && b[i + 1][j + 2] != 0) return j - 1;
			if (i >= 2 && i <= 3 && j >= 2 && j <= 4) { // �밢���� ���� ���� i�� 2~3, j�� 2~4�ȿ� �־�� ���带 ����� �ʰ� Ȯ���� �� �ִ�.
				if (b[i + 2][j - 2] == 0 && b[i + 3][j - 2] != 0 && b[i - 2][j + 2] == 0 && b[i - 1][j + 2] != 0) {
					if (b[i][j] == 0 && b[i + 1][j] != 0 && b[i + 1][j - 1] == 1 && b[i - 1][j + 1] == 1) return j;
					if (b[i][j] == 1 && b[i + 1][j - 1] == 1 && b[i - 1][j + 1] == 0 && b[i][j + 1] != 0) return j + 1;
					if (b[i][j] == 1 && b[i + 1][j - 1] == 0 && b[i + 2][j - 1] != 0 && b[i - 1][j + 1] == 1) return j - 1;
				}
				if (b[i + 2][j + 2] == 0 && b[i + 3][j + 2] != 0 && b[i - 2][j - 2] == 0 && b[i - 1][j - 2] != 0) {
					if (b[i][j] == 0 && b[i + 1][j] != 0 && b[i - 1][j - 1] == 1 && b[i + 1][j + 1] == 1) return j;
					if (b[i][j] == 1 && b[i - 1][j - 1] == 1 && b[i + 1][j + 1] == 0 && b[i + 2][j + 1] != 0) return j + 1;
					if (b[i][j] == 1 && b[i - 1][j - 1] == 0 && b[i][j - 1] != 0 && b[i + 1][j + 1] == 1) return j - 1;
				}
			} // ==0 ���� Ȯ���ϴ� �κп��� �� !=0 ���� ���θ� Ȯ���ؾ� �Ѵ�. �ٷ� �� �� �ִ� ��쿡�� Ȯ���� �¸������ ������ �� �ֱ� �����̴�.
		}
	}
	for (i = 0; i < 6; i++) { // �� Ȯ���� ���� Ȯ���� �� ������ ������ Ȯ���� ���� Ȯ���Ѵ�. ������ �����ϴ�.
		for (j = 0; j < 7; j++) {
			if (j >= 2 && j <= 4 && b[i][j] == 0 && b[i + 1][j] != 0 && b[i][j - 1] == 2 && b[i][j + 1] == 2 && b[i][j - 2] == 0 && b[i + 1][j - 2] != 0 && b[i][j + 2] == 0 && b[i + 1][j + 2] != 0) return j;
			if (j >= 2 && j <= 4 && b[i][j + 1] == 0 && b[i + 1][j + 1] != 0 && b[i][j - 1] == 2 && b[i][j] == 2 && b[i][j - 2] == 0 && b[i + 1][j - 2] != 0 && b[i][j + 2] == 0 && b[i + 1][j + 2] != 0) return j + 1;
			if (j >= 2 && j <= 4 && b[i][j - 1] == 0 && b[i + 1][j - 1] != 0 && b[i][j] == 2 && b[i][j + 1] == 2 && b[i][j - 2] == 0 && b[i + 1][j - 2] != 0 && b[i][j + 2] == 0 && b[i + 1][j + 2] != 0) return j - 1;
			if (i >= 2 && i <= 3 && j >= 2 && j <= 4) {
				if (b[i + 2][j - 2] == 0 && b[i + 3][j - 2] != 0 && b[i - 2][j + 2] == 0 && b[i - 1][j + 2] != 0) {
					if (b[i][j] == 0 && b[i + 1][j] != 0 && b[i + 1][j - 1] == 2 && b[i - 1][j + 1] == 2) return j;
					if (b[i][j] == 2 && b[i + 1][j - 1] == 2 && b[i - 1][j + 1] == 0 && b[i][j + 1] != 0) return j + 1;
					if (b[i][j] == 2 && b[i + 1][j - 1] == 0 && b[i + 2][j - 1] != 0 && b[i - 1][j + 1] == 2) return j - 1;
				}
				if (b[i + 2][j + 2] == 0 && b[i + 3][j + 2] != 0 && b[i - 2][j - 2] == 0 && b[i - 1][j - 2] != 0) {
					if (b[i][j] == 0 && b[i + 1][j] != 0 && b[i - 1][j - 1] == 2 && b[i + 1][j + 1] == 2) return j;
					if (b[i][j] == 2 && b[i - 1][j - 1] == 2 && b[i + 1][j + 1] == 0 && b[i + 2][j + 1] != 0) return j + 1;
					if (b[i][j] == 2 && b[i - 1][j - 1] == 0 && b[i][j - 1] != 0 && b[i + 1][j + 1] == 2) return j - 1;
				}
			}
		}
	}
	return choice;
}
int evaluation_state_board(__int64 m_state, __int64 o_state) { // eval_board�� ������� ���� state�� ������ score�� ��ȯ�Ͽ� ����.
	int i = 0, m_score = 0, o_score = 0;
	for (i = 0; i < 42; i++) {
		m_score += eval_board[i] * (m_state % 2);
		m_state /= 2;
		o_score += eval_board[i] * (o_state % 2);
		o_state /= 2;
	}
	return m_score - o_score;
}
struct ret { // ���� ��Ÿ ����׿��� ���� �������� �Բ� �����ϱ� ���� ����ü ����.
	int value;
	int choice;
};
struct ret search(__int64 state, __int64 m_state, __int64 o_state, int bot[7], int depth, int minmax_state, int alpha, int beta) {
	int i;
	__int64 t = 1, temp;
	struct ret ge, re;
	if (minmax_state == 1) { // �� ������ ��� ������ �̰�ٸ� �� �̻� Ž���� �������� �ʴ´�.
		temp = check(o_state);
		if (temp) {
			re.value = -99999999;
			re.choice = -1;
			return re;
		}
	}
	else { // �ݴ�� ���� ������ ��� ���� �̰�ٸ� �� �̻� Ž���� �������� �ʴ´�.
		temp = check(m_state);
		if (temp) {
			re.value = 99999999;
			re.choice = -1;
			return re;
		}
	}
	if (depth == 12) { // depth������ �ξ� �ð������� �д�.
		re.value = evaluation_state_board(m_state, o_state); // �ִ� depth�� ���������� ���ĺ�Ÿ ����׿� ����� ���� ���� state�� �������� evaluation�� �ο��Ѵ�.
		re.choice = -1;
		return re;
	}
	re.value = -2100000000;
	re.value *= minmax_state;
	re.choice = -1;
	for (i = 0; i < 7; i++) {
		if (bot[i] >= 0) {
			temp = t << bot[i];
			bot[i] -= 7;
			if (minmax_state == 1) { // �� ���ʸ� �ǹ��Ѵ�. MINMAX�� MAX�� �ش�ȴ�.
				ge = search((state | temp), (m_state | temp), o_state, bot, depth + 1, minmax_state*(-1), alpha, beta);
				bot[i] += 7;
				if (ge.value == 99999999) {
					re.value = 99999999;
					re.choice = i;
					return re;
				}
				if (ge.value > re.value) {
					re.value = ge.value;
					re.choice = i;
				}
				if (ge.value > alpha) alpha = ge.value;
				if (alpha >= beta) break;
			}
			else { // ���� ���ʸ� �ǹ��Ѵ�. MIN�� �ش�ȴ�.
				ge = search((state | temp), m_state, (o_state | temp), bot, depth + 1, minmax_state*(-1), alpha, beta);
				bot[i] += 7;
				if (ge.value == -99999999) {
					re.value = -99999999;
					re.choice = i;
					return re;
				}
				if (ge.value < re.value) {
					re.value = ge.value;
					re.choice = i;
				}
				if (ge.value < beta) beta = ge.value;
				if (alpha >= beta) break;
			}
		}
	}
	return re;
}
int randomgo(__int64 state, __int64 m_state, __int64 o_state, int bot[7], int me, int count) { //MCTS�� ���̽��� �����ϰ� �����ϴ� �Լ�.
	int x = (rand() * 10000) % 7, i;
	__int64 t = 1, temp;
	if (check(m_state)) return 999; // �̱���
	else if (check(o_state)) return -999; // �� ���
	if (count >= 42) return 0; // ��� ���
	for (i = 0; i < 7; i++) {
		if (bot[x] > 0) break;
		x = (x + 1) % 7;
	}
	if (i == 7) return 0;
	temp = t << bot[x];
	bot[x] -= 7;
	if (me == 1) return randomgo(state | temp, m_state | temp, o_state, bot, me*(-1), count + 1);
	else return randomgo(state | temp, m_state, o_state | temp, bot, me*(-1), count + 1);
}
ret MCTS(__int64 state, __int64 m_state, __int64 o_state, int bot[7], int count) { //MCTS�� ����.
	int i, j, bet[7], val, ct = 0, max = -9999;
	__int64 t = 1, temp;
	ret re;
	for (i = 0; i < 7; i++)
		bet[i] = bot[i];
	for (i = 0; i < 7; i++) { // 7���� ���� �� ��츦 ����.
		temp = t << bet[i];
		bet[i] -= 7;
		ct = 0;
		for (j = 0; j < 1000000; j++) { // 100������ ���� �д�
			val = randomgo(state | temp, m_state | temp, o_state, bet, -1, count + 1);
			if (val >= 0) ct++; // 100���� �� �̱�ų� ���� ��츦 ī��Ʈ �Ѵ�. �̻����� �ΰ������� ��� ���� ���� �ִ� ������̹Ƿ� ���Խ��״�.
		}
		if (ct > max) {
			max = ct;
			re.choice = i;
		}
		bet[i] += 7;
	}
	re.value = 0;
	return re;
}
int master2[8] = { 0, 4, 3, 4, 4, 4, 5, 4 }; // master'n'�� n��° ���� ���� �ξ�� �� perfect�� ���� pre-based �� ���� �迭.
int master3[8] = { 0, 4, 6, 2, 3, 3, 2, 3 };
int master4[8][8] = { { 0,0,0,0,0,0,0,0 },{ 0,4,4,4,4,4,4,3 },{ 0,3,2,3,2,3,3,3 },{ 0,4,4,3,4,3,4,4 },{ 0,3,3,2,4,3,5,4 },{ 0,4,4,4,4,5,4,4 },{ 0,5,5,5,4,5,6,5 },{ 0,3,4,4,4,4,4,4 } };
int master5[8][8] = { { 0,0,0,0,0,0,0,0 },{ 0,5,2,5,5,5,2,5 },{ 0,3,6,3,6,3,6,2 },{ 0,3,4,4,4,3,4,4 },{ 0,3,3,4,3,3,3,3 },{ 0,3,3,5,3,3,3,3 },{ 0,2,4,4,4,4,4,4 },{ 0,3,3,4,3,3,3,3 } };
int master6[8][8][8] = { { { 0,0,0,0,0,0,0,0 },{ 0,0,0,0,0,0,0,0 },{ 0,0,0,0,0,0,0,0 },{ 0,0,0,0,0,0,0,0 },{ 0,0,0,0,0,0,0,0 },{ 0,0,0,0,0,0,0,0 },{ 0,0,0,0,0,0,0,0 } },
{ { 0,0,0,0,0,0,0,0 },{ 0,1,4,4,5,4,5,4 },{ 0,4,4,4,2,4,4,4 },{ 0,4,4,4,3,4,4,4 },{ 0,4,4,3,6,4,4,4 },{ 0,4,4,4,5,4,4,4 },{ 0,5,4,4,4,4,4,4 },{ 0,5,4,5,5,3,4,5 } },
{ { 0,0,0,0,0,0,0,0 },{ 0,3,3,3,3,3,3,3 },{ 0,3,3,3,3,3,3,3 },{ 0,3,3,2,3,3,3,3 },{ 0,4,4,2,4,3,3,3 },{ 0,3,3,3,4,5,5,3 },{ 0,3,3,3,4,3,6,3 },{ 0,3,3,3,4,3,3,3 } },
{ { 0,0,0,0,0,0,0,0 },{ 0,4,4,4,3,4,4,4 },{ 0,4,4,4,4,4,4,4 },{ 0,4,4,4,4,4,4,4 },{ 0,3,4,4,4,4,4,4 },{ 0,4,4,4,4,4,4,4 },{ 0,4,4,4,4,4,4,4 },{ 0,4,4,4,3,4,4,4 } },
{ { 0,0,0,0,0,0,0,0 },{ 0,0,0,0,0,0,0,0 },{ 0,0,0,0,0,0,0,0 },{ 0,0,0,0,0,0,0,0 },{ 0,0,0,0,0,0,0,0 },{ 0,0,0,0,0,0,0,0 },{ 0,0,0,0,0,0,0,0 },{ 0,0,0,0,0,0,0,0 } },
{ { 0,0,0,0,0,0,0,0 },{ 0,4,4,4,5,4,4,4 },{ 0,4,4,4,4,4,4,4 },{ 0,4,4,4,4,4,4,4 },{ 0,4,4,4,4,4,4,5 },{ 0,4,4,4,4,4,4,4 },{ 0,4,4,4,4,4,4,4 },{ 0,4,4,4,5,4,4,4 } },
{ { 0,0,0,0,0,0,0,0 },{ 0,5,4,5,4,5,5,5 },{ 0,4,2,3,4,5,5,5 },{ 0,3,3,3,4,5,5,5 },{ 0,4,4,4,5,4,4,4 },{ 0,5,4,5,5,6,5,5 },{ 0,4,3,5,4,5,5,5 },{ 0,5,5,5,5,5,5,5 } },
{ { 0,0,0,0,0,0,0,0 },{ 0,5,4,5,5,3,4,5 },{ 0,4,4,4,4,4,4,3 },{ 0,4,4,4,3,4,4,4 },{ 0,4,4,4,2,5,4,4 },{ 0,4,4,4,5,4,4,4 },{ 0,4,4,4,6,4,4,4 },{ 0,4,3,4,3,4,4,7 } } };
int master7[8][8][8] = { { { 0,0,0,0,0,0,0,0 },{ 0,0,0,0,0,0,0,0 },{ 0,0,0,0,0,0,0,0 },{ 0,0,0,0,0,0,0,0 },{ 0,0,0,0,0,0,0,0 },{ 0,0,0,0,0,0,0,0 },{ 0,0,0,0,0,0,0,0 } },
{ { 0,0,0,0,0,0,0,0 },{ 0,2,6,6,6,6,2,6 },{ 0,3,4,3,4,4,2,4 },{ 0,2,6,2,2,2,2,2 },{ 0,2,6,2,2,2,2,2 },{ 0,3,5,5,4,3,3,3 },{ 0,5,5,5,5,2,5,5 },{ 0,2,6,2,2,2,2,2 } },
{ { 0,0,0,0,0,0,0,0 },{ 0,4,4,2,3,6,4,4 },{ 0,5,2,3,4,6,5,5 },{ 0,3,3,5,6,5,5,3 },{ 0,4,4,3,4,4,4,6 },{ 0,6,3,5,4,3,3,6 },{ 0,4,5,4,4,3,4,3 },{ 0,3,5,3,6,5,3,4 } },
{ { 0,0,0,0,0,0,0,0 },{ 0,4,5,4,4,2,4,4 },{ 0,5,5,5,5,1,5,5 },{ 0,5,5,5,5,1,5,5 },{ 0,4,4,3,4,4,4,4 },{ 0,2,4,4,4,4,4,4 },{ 0,5,5,5,5,1,5,5 },{ 0,5,5,5,5,1,5,5 } },
{ { 0,0,0,0,0,0,0,0 },{ 0,3,3,2,3,3,3,3 },{ 0,3,3,3,3,3,3,3 },{ 0,4,4,4,4,4,4,4 },{ 0,3,3,4,3,3,3,3 },{ 0,3,3,3,3,3,3,3 },{ 0,3,3,6,3,3,3,3 },{ 0,3,3,2,3,3,3,3 } },
{ { 0,0,0,0,0,0,0,0 },{ 0,3,3,4,3,3,3,3 },{ 0,3,3,6,3,3,3,3 },{ 0,3,2,3,3,5,7,6 },{ 0,3,3,3,3,3,3,3 },{ 0,3,3,5,3,3,3,3 },{ 0,3,3,4,3,3,3,3 },{ 0,3,3,4,3,3,3,3 } },
{ { 0,0,0,0,0,0,0,0 },{ 0,4,4,4,3,4,4,4 },{ 0,5,1,1,1,1,1,1 },{ 0,5,1,1,1,1,1,1 },{ 0,4,4,4,4,7,4,5 },{ 0,2,1,1,1,1,1,1 },{ 0,5,1,1,1,1,1,1 },{ 0,5,1,1,1,1,1,1 } },
{ { 0,0,0,0,0,0,0,0 },{ 0,3,3,4,3,3,3,3 },{ 0,3,3,2,3,3,3,3 },{ 0,5,4,5,5,4,2,5 },{ 0,3,3,5,3,3,3,3 },{ 0,3,3,4,3,3,3,3 },{ 0,3,3,4,3,3,3,3 },{ 0,3,3,4,3,3,3,3 } } };
int rulechoice(int count, int bot[7]) { // rule�� �δ� ��쿡 �����ϴ� ���μ���.
	int i, max = -2147483647, maxindex = -1, temp;
	__int64 t = 1;
	for (i = 0; i < 7; i++) {
		if (bot[i] >= 0) {
			if (maxindex == -1) maxindex = i;
			temp = evaluation_state_board(my_state | t << bot[i], opp_state);
			if (temp > max) {
				max = temp;
				maxindex = i;
			}
		}
	}
	return maxindex;
}
int choice(int count, int mode) { // main���� choice�� ��û�ϸ� ���� ��(��ǻ��)�� ��� �ξ�� �� ���� �����Ѵ�. ��� �δ� ��쵵 ����.
	int i, bet[7], temp;
	for (i = 0; i < 7; i++) bet[i] = bottom[i];
	if (count == 1) { // count 1~7�� ������ ����Ʈ �ַ�Ǵ�� ���� �д�.
		if (mode == 1) printf("RULE : ������ master solution�� ���� �д�.");
		return 2;
	}
	else if (count == 2) {
		if (mode == 1) printf("RULE : ������ master solution�� ���� �д�.");
		return master2[record[count - 1]] - 1;
	}
	else if (count == 3) {
		if (mode == 1) printf("RULE : ������ master solution�� ���� �д�.");
		return master3[record[count - 1]] - 1;
	}
	else if (count == 4) {
		if (mode == 1) printf("RULE : ������ master solution�� ���� �д�.");
		return master4[record[count - 3]][record[count - 1]] - 1;
	}
	else if (count == 5) {
		if (mode == 1) printf("RULE : ������ master solution�� ���� �д�.");
		return master5[record[count - 3]][record[count - 1]] - 1;
	}
	else if (count == 6) {
		if (mode == 1) printf("RULE : ������ master solution�� ���� �д�.");
		return master6[record[count - 5]][record[count - 3]][record[count - 1]] - 1;
	}
	else if (count == 7) {
		if (mode == 1) printf("RULE : ������ master solution�� ���� �д�.");
		return master7[record[count - 5]][record[count - 3]][record[count - 1]] - 1;
	}
	else {
		temp = checkwinposition(my_state, bet); // ����Ʈ �ַ���� �����ϰ� ���� �� ������ �̱� �� �ִ� ���� �ִٸ� �д�.
		if (temp < 7) {
			if (mode == 1) printf("RULE : �̰� �θ� ���� �̱���?���� (���� �� ���� �̱�� ���� �д�.)");
			return temp;
		}
		temp = checkwinposition(opp_state, bet); // ������ �� ������ �̱� �� �ִ� ���� �ִٸ� ���´�.
		if (temp < 7) {
			if (mode == 1) printf("RULE : �������� �� �� �˾Ҿ�? (������ �� ���� �̱�� ���� ���´�.)");
			return temp;
		}
		temp = checksecondwin(); // ���� �� ������ �̱� �� �ִ� ���� �ִ���, ������ �� ������ �̱� �� �ִ� ���� �ִ��� �Ǻ��ϰ� �����ϰų� ����Ѵ�.
		if (temp != -1) {
			if (mode == 1) printf("RULE : ���� �� ������ Ȯ���� �̱�� ���� �д�. / ������ �� ������ Ȯ���� �̱�� ���� ���´�.");
			return temp; // temp �� -1�̸� �׷� ���� ���ٴ� ���� �ǹ��Ѵ�.
		}
		if (mode == 1) {
			printf("RULE : evaluate_board�� �������� ���� ������ �ִ� �� ( �߾ӿ� ����� �� ) �� �д�.");
			return rulechoice(count, bet); // ��� �ξ�� �ϴ� ��� ȣ���Ͽ� ����� �����Ѵ�.
		}
		else {
			struct ret x;
			x = search(current_state, my_state, opp_state, bottom, 1, 1, -2147483647, 2147483647); // ���� �����Ű�� �ʴ� ���, ���ĺ�Ÿ ������� �����Ѵ�.
																								   //x = MCTS(current_state, my_state, opp_state, bottom, count);
			if (x.choice == -1) {
				for (i = 0; i < 7; i++) {
					if (bottom[i] >= 0) return i;
				}
			}
			return x.choice;
		}
	}
}
int main() {
	int mode, ch, count = 1;
	__int64 t = 1;
	mode = 0;
	printf("���� �Ͻðڽ��ϱ�? ��(1) �ƴϿ�(2)\n");
	scanf("%d", &mode);
	while (mode != 1 && mode != 2) {
		printf("\n ��(1) �ƴϿ�(2) �� �ϳ��� �Է��ϼ���\n");
		scanf("%d", &mode);
	}
	prtboard(my_state, opp_state);
	if (mode == 1) {
		ch = getchoice();
		record[count] = ch;
		ch -= 1;
		current_state |= t << bottom[ch];
		opp_state |= t << bottom[ch];
		bottom[ch] -= 7;
		prtboard(my_state, opp_state);
		count++;
	}
	while (1) {
		printf("���� �����ұ��? ��(1) �ƴϿ�(2)\n");
		scanf("%d", &mode);
		ch = choice(count, mode);
		current_state |= t << bottom[ch];
		my_state |= t << bottom[ch];
		bottom[ch] -= 7;
		prtboard(my_state, opp_state);
		record[count] = ch + 1;
		count++;
		if (count == 43) {
			printf("�����ϴ�!");
			break;
		}
		if (checkwin()) break;
		ch = getchoice();
		ch -= 1;
		current_state |= t << bottom[ch];
		opp_state |= t << bottom[ch];
		bottom[ch] -= 7;
		prtboard(my_state, opp_state);
		record[count] = ch + 1;
		count++;
		if (count == 43) {
			printf("�����ϴ�!");
			break;
		}
		if (checklose()) break;
	}
	printf("\n�����Ϸ��� �ƹ� Ű�� �Է��� �ּ���.");
	scanf("%d", &mode);
	return 0;
}