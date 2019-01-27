#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<random>
__int64 current_state, my_state, opp_state; // 전체 보드상황 ( 있으면 1 없으면 0 ), 내(컴퓨터) 상황, 상대(유저) 상황
__int64 mask1 = 0b111100011110001111000111100011110001111000; // 가로 승리확인 제한 mask
__int64 mask2 = 0b111111111111111111111000000000000000000000; // 세로 승리확인 제한 mask
__int64 mask3 = 0b111100011110001111000000000000000000000000; // 오른쪽 위 승리확인 제한 mask
__int64 mask4 = 0b000111100011110001111000000000000000000000; // 왼쪽 위 승리확인 제한 mask
int bottom[7] = { 41, 40, 39, 38, 37, 36, 35 }; // 보드에 바로 놓을 수 있는 각 위치 별 높이에 대한 left shift value.
int record[44]; // 기보 기록.
int eval_board[42] = { 3, 4, 5, 7, 5, 4, 3,
4, 6, 8, 10, 8, 6, 4,
5, 8, 11, 13, 11, 8, 5,
5, 8, 11, 13, 11, 8, 5,
4, 6, 8, 10, 8, 6, 4,
3, 4, 5, 7, 5, 4, 3 }; // evaluation에 쓰이는 각 칸 별 점수.
void prtboard(__int64 my_state, __int64 opp_state) { // 보드 현황 출력함수
	int i, j, my, opp;//▒●○
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
int getchoice() { // 유저의 입력을 받는 함수
	int input;
	printf("\n 돌을 놓을 위치의 번호를 입력해주세요. (1~7) ");
	scanf("%d", &input);
	while (input < 1 || input>7) { // 입력값의 범위가 1~7을 벗어나면 적절한 값을 입력할 때 까지 무한정 다시 받는다.
		printf("\n 1에서 7사이의 숫자를 입력해주세요.");
		scanf("%d", &input);
	}
	return input;
}
__int64 check(__int64 state) { // 현재 입력받은 state에 가로 세로 대각선으로 연속한 4개의 돌이 놓여있는 부분이 있는지를 확인하는 함수.
	__int64 check;
	check = state & state << 1 & state << 2 & state << 3 & mask1; // 가로
	check |= state & state << 7 & state << 14 & state >> 7 & mask2; // 세로
	check |= state & state << 8 & state << 16 & state >> 8 & mask3; // 오른쪽 위 대각선
	check |= state & state << 6 & state << 12 & state << 18 & mask4; // 왼쪽 위 대각선
	return check;
}
__int64 checkwin() { // 내(컴퓨터)가 이겼는지를 확인한 후 메시지 출력
	__int64 a = check(my_state);
	if (a) printf("하하하. 제가 이겼습니다!");
	return a;
}
__int64 checklose() { // 상대(유저)가 이겼는지 확인한 후 메시지 출력
	__int64 a = check(opp_state);
	if (a) printf("이런.. 당신은 정말 잘 하시는군요.. 제가 졌습니다..");
	return a;
}
int checkwinposition(__int64 my_state, int bot[7]) { // 1~7번에 한 번씩 수를 둔 후 check함수를 통해 승패가 갈리는 지 여부를 판단하여 있으면 0~6(놓을 위치), 없으면 7 리턴.
	__int64 t = 1;
	int i;
	for (i = 0; i < 7; i++) {
		if (bot[i] >= 0) {
			if (check(my_state | t << bot[i])) break;
		}
	}
	return i;
}
int checksecondwin() { // 00110, 01010, 01100 중 한 경우의 수가 존재하는지 판단한다. 이는 두 번의 수 만에 확실히 이길 수 있는 경우에 속한다.
	int i, j, my, opp, choice = -1;
	int b[10][10];
	__int64 temp1 = my_state, temp2 = opp_state;
	for (i = 0; i < 6; i++) { // 쉽게 판단하기 위해 임시로 보드로 변환.
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
	for (j = 0; j < 7; j++) b[7][j] = -1; // 바닥에서 index error를 방지하기 위해 한 줄을 더 두고 -1로 바닥임을 표시한다. !=0인 조건에 걸리지 않게 하기 위함도 있다.
	for (i = 0; i < 6; i++) {
		for (j = 0; j < 7; j++) { // 가로로 5개의 수를 봐야 하므로 중앙을 기준으로 좌 우를 본다고 할 때 j가 2~4 ( 3~5 ) 일 경우만 확인해야 한다.
			if (j >= 2 && j <= 4 && b[i][j] == 0 && b[i + 1][j] != 0 && b[i][j - 1] == 1 && b[i][j + 1] == 1 && b[i][j - 2] == 0 && b[i + 1][j - 2] != 0 && b[i][j + 2] == 0 && b[i + 1][j + 2] != 0) return j;
			if (j >= 2 && j <= 4 && b[i][j + 1] == 0 && b[i + 1][j + 1] != 0 && b[i][j - 1] == 1 && b[i][j] == 1 && b[i][j - 2] == 0 && b[i + 1][j - 2] != 0 && b[i][j + 2] == 0 && b[i + 1][j + 2] != 0) return j + 1;
			if (j >= 2 && j <= 4 && b[i][j - 1] == 0 && b[i + 1][j - 1] != 0 && b[i][j] == 1 && b[i][j + 1] == 1 && b[i][j - 2] == 0 && b[i + 1][j - 2] != 0 && b[i][j + 2] == 0 && b[i + 1][j + 2] != 0) return j - 1;
			if (i >= 2 && i <= 3 && j >= 2 && j <= 4) { // 대각선을 보는 경우는 i가 2~3, j가 2~4안에 있어야 보드를 벗어나지 않고 확인할 수 있다.
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
			} // ==0 임을 확인하는 부분에서 꼭 !=0 인지 여부를 확인해야 한다. 바로 둘 수 있는 경우에만 확실한 승리수라고 생각할 수 있기 때문이다.
		}
	}
	for (i = 0; i < 6; i++) { // 내 확실한 수를 확인한 후 없으면 상대방의 확실한 수를 확인한다. 과정은 동일하다.
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
int evaluation_state_board(__int64 m_state, __int64 o_state) { // eval_board를 기반으로 현재 state의 전세를 score로 변환하여 리턴.
	int i = 0, m_score = 0, o_score = 0;
	for (i = 0; i < 42; i++) {
		m_score += eval_board[i] * (m_state % 2);
		m_state /= 2;
		o_score += eval_board[i] * (o_state % 2);
		o_state /= 2;
	}
	return m_score - o_score;
}
struct ret { // 알파 베타 프루닝에서 값과 선택지를 함께 리턴하기 위해 구조체 선언.
	int value;
	int choice;
};
struct ret search(__int64 state, __int64 m_state, __int64 o_state, int bot[7], int depth, int minmax_state, int alpha, int beta) {
	int i;
	__int64 t = 1, temp;
	struct ret ge, re;
	if (minmax_state == 1) { // 내 차례인 경우 상대방이 이겼다면 더 이상 탐색을 진행하지 않는다.
		temp = check(o_state);
		if (temp) {
			re.value = -99999999;
			re.choice = -1;
			return re;
		}
	}
	else { // 반대로 상대방 차례인 경우 내가 이겼다면 더 이상 탐색을 진행하지 않는다.
		temp = check(m_state);
		if (temp) {
			re.value = 99999999;
			re.choice = -1;
			return re;
		}
	}
	if (depth == 12) { // depth제한을 두어 시간제한을 둔다.
		re.value = evaluation_state_board(m_state, o_state); // 최대 depth에 도달했을때 알파베타 프루닝에 사용할 값을 현재 state를 기준으로 evaluation해 부여한다.
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
			if (minmax_state == 1) { // 내 차례를 의미한다. MINMAX의 MAX에 해당된다.
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
			else { // 상대방 차례를 의미한다. MIN에 해당된다.
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
int randomgo(__int64 state, __int64 m_state, __int64 o_state, int bot[7], int me, int count) { //MCTS의 케이스를 랜덤하게 진행하는 함수.
	int x = (rand() * 10000) % 7, i;
	__int64 t = 1, temp;
	if (check(m_state)) return 999; // 이긴경우
	else if (check(o_state)) return -999; // 진 경우
	if (count >= 42) return 0; // 비긴 경우
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
ret MCTS(__int64 state, __int64 m_state, __int64 o_state, int bot[7], int count) { //MCTS를 진행.
	int i, j, bet[7], val, ct = 0, max = -9999;
	__int64 t = 1, temp;
	ret re;
	for (i = 0; i < 7; i++)
		bet[i] = bot[i];
	for (i = 0; i < 7; i++) { // 7개의 수를 둔 경우를 가정.
		temp = t << bet[i];
		bet[i] -= 7;
		ct = 0;
		for (j = 0; j < 1000000; j++) { // 100만번의 수를 둔다
			val = randomgo(state | temp, m_state | temp, o_state, bet, -1, count + 1);
			if (val >= 0) ct++; // 100만번 중 이기거나 비기는 경우를 카운트 한다. 이상적인 인공지능의 경우 비기는 것이 최대 결과값이므로 포함시켰다.
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
int master2[8] = { 0, 4, 3, 4, 4, 4, 5, 4 }; // master'n'은 n번째 수에 대해 두어야 할 perfect한 수를 pre-based 해 놓은 배열.
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
int rulechoice(int count, int bot[7]) { // rule로 두는 경우에 수행하는 프로세스.
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
int choice(int count, int mode) { // main에서 choice를 요청하면 현재 내(컴퓨터)가 어디에 두어야 할 지를 리턴한다. 룰로 두는 경우도 포함.
	int i, bet[7], temp;
	for (i = 0; i < 7; i++) bet[i] = bottom[i];
	if (count == 1) { // count 1~7은 정해진 퍼펙트 솔루션대로 수를 둔다.
		if (mode == 1) printf("RULE : 정해진 master solution에 의해 둔다.");
		return 2;
	}
	else if (count == 2) {
		if (mode == 1) printf("RULE : 정해진 master solution에 의해 둔다.");
		return master2[record[count - 1]] - 1;
	}
	else if (count == 3) {
		if (mode == 1) printf("RULE : 정해진 master solution에 의해 둔다.");
		return master3[record[count - 1]] - 1;
	}
	else if (count == 4) {
		if (mode == 1) printf("RULE : 정해진 master solution에 의해 둔다.");
		return master4[record[count - 3]][record[count - 1]] - 1;
	}
	else if (count == 5) {
		if (mode == 1) printf("RULE : 정해진 master solution에 의해 둔다.");
		return master5[record[count - 3]][record[count - 1]] - 1;
	}
	else if (count == 6) {
		if (mode == 1) printf("RULE : 정해진 master solution에 의해 둔다.");
		return master6[record[count - 5]][record[count - 3]][record[count - 1]] - 1;
	}
	else if (count == 7) {
		if (mode == 1) printf("RULE : 정해진 master solution에 의해 둔다.");
		return master7[record[count - 5]][record[count - 3]][record[count - 1]] - 1;
	}
	else {
		temp = checkwinposition(my_state, bet); // 퍼펙트 솔루션을 제외하고 먼저 한 수만에 이길 수 있는 수가 있다면 둔다.
		if (temp < 7) {
			if (mode == 1) printf("RULE : 이거 두면 내가 이기지?ㅎㅎ (내가 한 번에 이기는 수를 둔다.)");
			return temp;
		}
		temp = checkwinposition(opp_state, bet); // 상대방이 한 수만에 이길 수 있는 수가 있다면 막는다.
		if (temp < 7) {
			if (mode == 1) printf("RULE : ㅎㅎ내가 모를 줄 알았어? (상대방이 한 번에 이기는 수를 막는다.)");
			return temp;
		}
		temp = checksecondwin(); // 내가 두 수만에 이길 수 있는 수가 있는지, 상대방이 두 수만에 이길 수 있는 수가 있는지 판별하고 공격하거나 방어한다.
		if (temp != -1) {
			if (mode == 1) printf("RULE : 내가 두 번만에 확실히 이기는 수를 둔다. / 상대방이 두 번만에 확실히 이기는 수를 막는다.");
			return temp; // temp 가 -1이면 그런 수가 없다는 것을 의미한다.
		}
		if (mode == 1) {
			printf("RULE : evaluate_board를 기준으로 높은 점수가 있는 곳 ( 중앙에 가까운 곳 ) 에 둔다.");
			return rulechoice(count, bet); // 룰로 두어야 하는 경우 호출하여 결과를 리턴한다.
		}
		else {
			struct ret x;
			x = search(current_state, my_state, opp_state, bottom, 1, 1, -2147483647, 2147483647); // 룰을 적용시키지 않는 경우, 알파베타 프루닝을 진행한다.
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
	printf("먼저 하시겠습니까? 예(1) 아니오(2)\n");
	scanf("%d", &mode);
	while (mode != 1 && mode != 2) {
		printf("\n 예(1) 아니오(2) 중 하나를 입력하세요\n");
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
		printf("룰을 적용할까요? 예(1) 아니오(2)\n");
		scanf("%d", &mode);
		ch = choice(count, mode);
		current_state |= t << bottom[ch];
		my_state |= t << bottom[ch];
		bottom[ch] -= 7;
		prtboard(my_state, opp_state);
		record[count] = ch + 1;
		count++;
		if (count == 43) {
			printf("비겼습니다!");
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
			printf("비겼습니다!");
			break;
		}
		if (checklose()) break;
	}
	printf("\n종료하려면 아무 키나 입력해 주세요.");
	scanf("%d", &mode);
	return 0;
}