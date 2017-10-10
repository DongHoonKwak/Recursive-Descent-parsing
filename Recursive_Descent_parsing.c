// 재귀 하강 파싱
// 곽동훈

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define ADD_OP 21
#define SUB_OP 22
#define SEMI 23
#define EQUAL 24
#define LP_CODE 25
#define RP_CODE 26
#define  EOP -1
#define INT_CODE 10
#define ID_CODE 11
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 2

void addChar();
void getChar();
void getNonBlank();
int lookup(char ch);
int lex();

void error();
void program();
void stmts();
void stmt();
void expr();
void term();
void var();

int nextChar;
char charClass;
char nextToken;
char lexeme[98];
int lexLen = 0;

FILE *in_fp = NULL;

void getChar() {

	if ((nextChar = getc(in_fp)) != EOF) // 파일에서 받아온 문자가 문장의 끝이 아니라면
	{
		if (isalpha(nextChar)) // 만약 다음문자로 알파벳이 나온다면
			charClass = LETTER; // charClass 에 알파벳을 저장

		else if (isdigit(nextChar)) //만약 다음문자로 상수가 나온다면
			charClass = DIGIT; // charClass 에 상수를 저장

		else charClass = UNKNOWN; // 알파벳,숫자둘다 아닐경우 UNKNOWN
	}

	else
		charClass = EOF; //문장의 끝이면 끝냄.
}
void getNonBlank()
{
	while (isspace(nextChar)) //Space공간이 안나올때까지
		getChar(); // 다음 문자를 가져옴
}
void addChar()
{
	if (lexLen <= 98) //만약 lexLen이 98이하라면
	{
		lexeme[lexLen++] = nextChar; //첫번째 배열부터 문자를 채워나감
		lexeme[lexLen] = 0; // 다시 0으로 초기화
	}

	else
		printf("Error – lexeme is too long"); // 아니면 에러출력
}
int lookup(char ch)
{
	switch (ch) //ch를 받음
	{
	case '(': // ( 이면
		addChar(); // 문자열에 추가하고
		nextToken = LP_CODE; //nextToken에 LP_CODE값 저장
		break; //브레이크

	case ')': // )이면
		addChar();  // 문자열에 추가하고
		nextToken = RP_CODE;//nextToken에 RP_CODE값 저장
		break; //브레이크

	case '+': // +이면
		addChar();  // 문자열에 추가하고
		nextToken = ADD_OP;//nextToken에 ADD_OP값 저장
		break; //브레이크

	case '-': // -이면
		addChar();  // 문자열에 추가하고
		nextToken = SUB_OP;//nextToken에 SUB_OP값 저장
		break; //브레이크

	case '=': // =이면
		addChar();  // 문자열에 추가하고
		nextToken = EQUAL;//nextToken에 EQUAL값 저장
		break; //브레이크

	case ';': // ;이면
		addChar();  // 문자열에 추가하고
		nextToken = SEMI;//nextToken에 SEMI값 저장
		break; //브레이크

	default: //실패시
		addChar();  // 문자열에 추가하고
		nextToken = EOP;//nextToken에 EOP값 저장
		break; //브레이크
	}

	return nextToken; //nextToken값을 반환함
}
int lex()
{
	lexLen = 0; //lexLen를 0으로 초기화
	getNonBlank(); //space들을 넘김(?) 무시함(?)

	switch (charClass) //charClass값을 받아서 switch문을 돌림
	{

	case LETTER: //알파벳이면
		addChar(); // 문자열에 추가하고
		getChar(); // 다음문자를 가져옴

		while (charClass == LETTER || charClass == DIGIT) //그 문자가 알파벳이거나 숫자이면
		{
			addChar(); //문자열에 추가하고
			getChar(); // 다음 문자를 가져옴
		}
		nextToken = ID_CODE; //nextToken에 ID_CODE값 저장
		break; //브레이크

	case DIGIT: //숫자이면
		addChar(); // 문자열에 추가하고
		getChar(); // 다음 문자를 가져옴

		while (charClass == DIGIT) //그 문자가 숫자면
		{
			addChar(); //문자열에 추가하고 
			getChar(); //다음 문자를 가져옴
		}
		nextToken = INT_CODE;//nextToken에 INT_CODE값 저장
		break; //브레이크

	case UNKNOWN: //문자숫자 둘다 아니면
		lookup(nextChar); //lookup 함수에서 정의한 기호인지 확인하고
		getChar(); //다음 문자를 가져옴
		break; //브레이크

	case EOF: //오류라면
		nextToken = EOF; // nextToken에 EOF값을 저장하고
		lexeme[0] = 'E'; // lexeme배열에 EOF 저장
		lexeme[1] = 'O';
		lexeme[2] = 'F';
		lexeme[3] = 0;
		break; //브레이크
	}

	printf("Next token is : %d, Next lexeme is %s\n", nextToken, lexeme); // 다음 토큰과 다음 어휘 출력

	return nextToken; //nextToken값을 반환
}

void program()
{
	printf("Enter <program>\n"); //program으로 들어감
	stmts(); //stmts 함수로 감
	printf("Exit <program>\n"); // program을 나옴
}
void stmts()
{
	printf("Enter <stmts>\n"); //stmts로 들어감
	stmt(); //stmt함수로 들어감

	while (nextToken == SEMI) //세미콜론이 안나올때까지 반복
	{
		if (nextToken == SEMI) // ;이면
		{
			lex(); //알파벳,숫자,기호,오류 여부를 판단해서
			stmts(); //stmts함수로 들어감
		}
		printf("Exit <stmts>\n"); //stmts를 나감
	}
}
void stmt()
{
	printf("Enter <stmt>\n"); // stmt로 들어감
	var(); // var함수로 들어감

	if (nextToken == EQUAL) //다음 토큰이 =이라면
	{
		lex(); //lex함수로 들어감
		expr(); //expr함수로 들어감
	}
	printf("Exit <stmt>\n"); //stmt를 나감
}
void expr()
{
	printf("Enter <expr>\n"); //expr로 들어감
	term(); //term함수로 들어감

	if (nextToken == ADD_OP || nextToken == SUB_OP) // 만약 다음문자가 + 나 - 라면
	{
		lex(); //lex함수로 들어감
		term(); //term함수로 들어감
	}
	printf("Exit <expr>\n"); //expr을 나감
}
void term()
{
	printf("Enter <term>\n"); // term에 들어감

	if (nextToken == ID_CODE || nextToken == INT_CODE) // 왼쪽 조건을 만족할 시 if문 들어감
		lex(); //lex함수로 들어감
	else//아니면
		error(); //비승인출력

	printf("Exit <term>\n"); //term함수 나옴
}
void var()
{
	printf("Enter <var>\n"); //var로 들어감

	if (nextToken == ID_CODE) //다음 토큰이 ID_CODE라면
		lex(); //lex함수로 들어감
	else //아니면 
		error(); //비승인출력

	printf("Exit <var>\n"); //var를 나감
}

void error() //비승인 출력함수
{
	printf("비승인입니다.\n"); //비승인 출력
}

void main()
{
	if ((in_fp = fopen("front.txt", "r")) == NULL) //front파일이 비어있으면
		printf("ERROR – cannot open front.txt \n"); // 열수없다는 경고문 출력

	else //파일안에 쓰여있으면
	{
		getChar(); //문자하나를 받아서
		lex();
		program(); //program하나만 실행시키면 이안에서 다 돌고 나옴
	}
}