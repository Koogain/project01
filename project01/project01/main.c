#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <time.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdbool.h>
#include "struct.h"

HANDLE hConsole;

// 전처리 변수
#define BTN_WIDTH 20
#define BTN_HEIGHT 3
#define FRAME_WIDTH 40
#define FRAME_HEIGHT 20

#define LOGIN_X 10
#define LOGIN_Y 3

#define PASS_X 10
#define PASS_Y 8

#define SIGNUP_X 14
#define SIGNUP_Y 14

// 함수 전방 선언
bool isInside(int x, int y, int left, int top, int right, int bottom);
void gotoxy(int x, int y);
void trimAll(char* str);
void setColor(WORD color);
int getRandomColor();
void drawBox(int x, int y, int width, int height);
void getTextInput(int x, int y, char* buffer, int maxLen);

void handleLoginScreen(const char* userName, const char* userID, const char* userPW);
void handleSignupScreen(const char* userName, const char* userID, const char* userPW);

void handleMainMenuScreen(const char* userName, const char* userID, const char* userPW);

int getDayIndex(const char* day);
int getStartHour(const char* timeStr);

void handleTimetableScreen(const char* userID);
void handleSubjectList(const char* userID);

void handleFriendsListScreen(const char* userName, const char* userID, const char* userPW);
void handleAddFriendScreen(const char* userName, const char* userID);

void handleFriendPin();

// 15 : 하얀색, 13 : 밝은 자주색

// 전역 변수
char userName[50] = { 0 };
char userID[50] = { 0 };
char userPW[50] = { 0 };

// 메인함수
int main()
{
    system("MODE CON:COLS=100 LINES=50");      // 콘솔 창 크기 설정
    system("cls");

    drawBox(4, 1, 92, 47);      // 전체 틀

    handleLoginScreen(userName, userID, userPW);

    gotoxy(0, 22); // 커서 내리기
    _getch();
    return 0;
}

// --------------
// 기본 기능 함수
// --------------

// 클릭 좌표 참거짓 함수
bool isInside(int clickX, int clickY, int left, int top, int right, int bottom)
{
    return (clickX >= left && clickX <= right && clickY >= top && clickY <= bottom);
}  

// 좌표 지정 함수
void gotoxy(int x, int y) 
{
    COORD pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}


// 공백 텍스트 제거 함수
void trimAll(char* str) 
{
    char* p = str;
    char* q = str;
    while (*q) 
    {
        if (*q != '\n' && *q != '\r' && *q != ' ' && *q != '\t')
            *p++ = *q;
        q++;
    }
    *p = '\0';
}

// 색깔 설정 함수
void setColor(WORD color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// 랜덤 색깔 지정 함수
int getRandomColor() 
{
    
}

// 박스 그리기 함수
void drawBox(int x, int y, int width, int height)
{
    // 윗줄
    gotoxy(x, y);
    printf("┌");
    for (int i = 0; i < width - 2; i++) printf("─");
    printf("┐");

    // 중간줄
    for (int i = 0; i < height - 2; i++)
    {
        gotoxy(x, y + 1 + i);
        printf("│");
        for (int j = 0; j < width - 2; j++) printf(" ");
        printf("│");
    }

    // 아랫줄
    gotoxy(x, y + height - 1);
    printf("└");
    for (int i = 0; i < width - 2; i++) printf("─");
    printf("┘");
}

// 텍스트 입력 함수
void getTextInput(int x, int y, char* buffer, int maxLen)
{
    int i = 0;
    char ch;

    gotoxy(x, y);  // 입력 시작 위치로 커서 이동

    while (1)
    {
        ch = _getch();

        if (ch == 13)  // Enter
        {
            buffer[i] = '\0';
            break;
        }
        else if (ch == 8 && i > 0)  // Backspace
        {
            i--;
            buffer[i] = '\0';
            gotoxy(x + i, y);
            printf(" ");
            gotoxy(x + i, y);
        }
        else if (i < maxLen && ch >= 32 && ch <= 126)  // 입력 가능한 문자
        {
            buffer[i++] = ch;
            gotoxy(x + i - 1, y);
            printf("%c", ch);
        }
    }
    buffer[i] = '\0';
}

// ------------------------------------------
// 메인 활용 기능 부가 함수 (시간표 부가 함수)
// ------------------------------------------

// 요일 함수
int getDayIndex(const char* day)
{
    if (strstr(day, "월")) return 0;
    if (strstr(day, "화")) return 1;
    if (strstr(day, "수")) return 2;
    if (strstr(day, "목")) return 3;
    if (strstr(day, "금")) return 4;
    return -1;
}

// 시간 함수
int getStartHour(const char* timeStr)
{
    int hour = 0;
    sscanf(timeStr, " %d", &hour); // 첫 숫자만 추출
    return hour;
}

// --------------
// 활용 기능 함수 (중기능)
// --------------

// 로그인 함수
void handleLoginScreen(char* userName, char* userID, char* userPW)
{
    system("cls");
    drawBox(4, 1, 92, 47);

    gotoxy(38, 4);  printf("===== 로그인 창 =====");

    // 아이디 입력창
    gotoxy(31, 9);  printf("아이디 : ");
    drawBox(31, 11, 35, 3);  

    // 비밀번호 입력창
    gotoxy(31, 15); printf("비밀번호 : ");
    drawBox(31, 17, 35, 3); 

    // 버튼
    gotoxy(33, 24); printf("[  로그인  ]");
    gotoxy(50, 24); printf("[ 회원가입 ]");

    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    SetConsoleMode(hInput, ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);

    INPUT_RECORD record;
    DWORD events;

    while (1)
    {
        ReadConsoleInput(hInput, &record, 1, &events);

        if (record.EventType == MOUSE_EVENT)
        {
            MOUSE_EVENT_RECORD mouse = record.Event.MouseEvent;
            int clickX = mouse.dwMousePosition.X;
            int clickY = mouse.dwMousePosition.Y;

            // 커서가 위치할 때
            if (mouse.dwEventFlags == MOUSE_MOVED)
            {
                // 로그인 커서
                if (isInside(clickX, clickY, 33, 24, 44, 24))
                {
                    setColor(13);
                    gotoxy(33, 24); printf("[  로그인  ]");
                }
                else
                {
                    setColor(15); // 흰색
                    gotoxy(33, 24); printf("[  로그인  ]");
                }

                // 회원가입 커서
                if (isInside(clickX, clickY, 50, 24, 61, 24))
                {
                    setColor(13);
                    gotoxy(50, 24); printf("[ 회원가입 ]");
                }
                else
                {
                    setColor(15);
                    gotoxy(50, 24); printf("[ 회원가입 ]");
                }

                setColor(15); // 기본 색상 복원
            }

            // 클릭을 했을 때
            if ((mouse.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) && mouse.dwEventFlags == 0)
            {
                // [회원가입] 버튼 
                if (isInside(clickX, clickY, 50, 24, 61, 24))
                {
                    handleSignupScreen(userName, userID, userPW);
                    break;
                }

                // [로그인] 버튼 
                else if (isInside(clickX, clickY, 33, 24, 44, 24))
                {
                    FILE* file = fopen("C:\\Users\\Koomy\\source\\repos\\project01\\Member.txt", "r");
                    if (!file)
                    {
                        gotoxy(31, 28);
                        printf("파일을 열 수 없습니다.");
                        continue;
                    }

                    char line[200];
                    Member loginUser = { 0 };
                    int isMatch = 0;

                    while (fgets(line, sizeof(line), file))
                    {
                        if (sscanf(line, "userNAME: %49[^/]/ userID: %49[^/]/ userPW: %49[^\n]",
                            loginUser.userName, loginUser.userID, loginUser.userPW) == 3)
                        {
                            trimAll(loginUser.userID);
                            trimAll(loginUser.userPW);

                            if (strcmp(loginUser.userID, userID) == 0 && strcmp(loginUser.userPW, userPW) == 0)
                            {
                                strncpy(userName, loginUser.userName, sizeof(loginUser.userName) - 1);
                                userName[sizeof(loginUser.userName) - 1] = '\0';

                                gotoxy(31, 28);
                                printf(" 【  회원가입 성공!, 환영합니다, %s님  】 ", userName);
                                Sleep(1000);
                                fclose(file);
                                system("cls");
                                handleMainMenuScreen(userName, userID, userPW);
                                return;
                            }
                        }
                    }
                    fclose(file);
                    gotoxy(31, 28);
                    printf(" 로그인 실패! 아이디 또는 비밀번호 확인");
                }

                // ID 입력 박스 (좌표: 31~65, 11~13)
                else if (isInside(clickX, clickY, 31, 11, 65, 13))
                {
                    gotoxy(32, 12);
                    printf("                                ");
                    gotoxy(32, 12);
                    getTextInput(32, 12, userID, 49);
                }

                // PW 입력 박스 (좌표: 31~65, 17~19)
                else if (isInside(clickX, clickY, 31, 17, 65, 19))
                {
                    gotoxy(32, 18);
                    printf("                                ");
                    gotoxy(32, 18);
                    getTextInput(32, 18, userPW, 49);
                }
            }
        }
    }
}

// 회원가입 함수
void handleSignupScreen()
{
    system("cls");

    Member user;  // 구조체 사용

    drawBox(4, 1, 92, 47);

    gotoxy(38, 4);  printf("===== 회원가입 창 =====");

    // 입력창
    gotoxy(31, 9);  printf("이름: ");
    drawBox(31, 11, 30, 3);

    gotoxy(31, 15); printf("아이디: ");
    drawBox(31, 17, 30, 3);

    gotoxy(31, 21); printf("비밀번호: ");
    drawBox(31, 23, 30, 3);

    gotoxy(33, 30); printf("[  확인  ]");
    gotoxy(50, 30); printf("[ 로그인 화면 ]");

    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD prevMode;
    GetConsoleMode(hInput, &prevMode);
    SetConsoleMode(hInput, ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);

    INPUT_RECORD record;
    DWORD events;

    while (1)
    {
        ReadConsoleInput(hInput, &record, 1, &events);

        if (record.EventType == MOUSE_EVENT)
        {
            MOUSE_EVENT_RECORD mouse = record.Event.MouseEvent;
            int clickX = mouse.dwMousePosition.X;
            int clickY = mouse.dwMousePosition.Y;

            if (mouse.dwEventFlags == MOUSE_MOVED)
            {
                // 확인 버튼 hover
                if (isInside(clickX, clickY, 33, 30, 41, 30))
                {
                    setColor(13);
                    gotoxy(33, 30); printf("[  확인  ]");
                }
                else
                {
                    setColor(15); 
                    gotoxy(33, 30); printf("[  확인  ]");
                }

                // 로그인 화면 버튼 hover
                if (isInside(clickX, clickY, 50, 30, 65, 30))
                {
                    setColor(13);
                    gotoxy(50, 30); printf("[ 로그인 화면 ]");
                }
                else
                {
                    setColor(15);
                    gotoxy(50, 30); printf("[ 로그인 화면 ]");
                }

                setColor(15); // 기본 색상 복원
            }

            if ((mouse.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) && mouse.dwEventFlags == 0)
            {
                // 이름 입력 박스
                if (isInside(clickX, clickY, 31, 11, 61, 13))
                {
                    gotoxy(32, 12);
                    printf("                         ");
                    gotoxy(32, 12);
                    getTextInput(32, 12, user.userName, sizeof(user.userName) - 1);
                }
                // 아이디 입력 박스
                else if (isInside(clickX, clickY, 31, 17, 61, 19))
                {
                    gotoxy(32, 18);
                    printf("                         ");
                    gotoxy(32, 18);
                    getTextInput(32, 18, user.userID, sizeof(user.userID) - 1);
                }
                // 비밀번호 입력 박스
                else if (isInside(clickX, clickY, 31, 23, 61, 25))
                {
                    gotoxy(32, 24);
                    printf("                         ");
                    gotoxy(32, 24);
                    getTextInput(32, 24, user.userPW, sizeof(user.userPW) - 1);
                }
                // 확인 버튼 클릭
                else if (isInside(clickX, clickY, 33, 30, 41, 30))
                {
                    // 입력값 검증
                    if (strlen(user.userName) == 0 || strlen(user.userID) == 0 || strlen(user.userPW) == 0)
                    {
                        gotoxy(33, 32);
                        printf("회원가입 실패! (모든 칸을 입력하세요)");
                        Sleep(1500);
                        gotoxy(33, 32);
                        printf("                                     "); // 메시지 지우기
                        continue; // 다시 입력 대기
                    }

                    // 중복 ID 검사
                    int isDuplicate = 0;
                    FILE* checkFile = fopen("C:\\Users\\Koomy\\source\\repos\\project01\\Member.txt", "r");
                    char line[150], existingID[50];

                    if (checkFile)
                    {
                        while (fgets(line, sizeof(line), checkFile))
                        {
                            if (sscanf(line, "userNAME: %*[^/] / userID: %49s", existingID) == 1)
                            {
                                if (strcmp(existingID, user.userID) == 0)
                                {
                                    isDuplicate = 1;
                                    break;
                                }
                            }
                        }
                        fclose(checkFile);
                    }

                    if (isDuplicate)
                    {
                        gotoxy(33, 32);
                        printf("회원가입 실패! (이미 존재하는 아이디)");
                        Sleep(1500);
                        gotoxy(33, 32);
                        printf("                                     ");
                        continue;
                    }

                    // 신규 계정 저장
                    FILE* file = fopen("C:\\Users\\Koomy\\source\\repos\\project01\\Member.txt", "a");
                    if (file)
                    {
                        fprintf(file, "userNAME: %s / userID: %s / userPW: %s\n",
                            user.userName, user.userID, user.userPW);
                        fclose(file);

                        gotoxy(33, 32);
                        printf(" 【  회원가입 성공!  】 ");
                        Sleep(1000);

                        SetConsoleMode(hInput, prevMode);
                        system("cls");
                        handleLoginScreen(user.userName, user.userID, user.userPW);
                        return;
                    }
                    else
                    {
                        gotoxy(33, 32);
                        printf("파일 오류! 회원가입 실패");
                        Sleep(1500);
                    }
                }
                // 로그인 화면 버튼 클릭
                else if (isInside(clickX, clickY, 50, 30, 65, 30))
                {
                    SetConsoleMode(hInput, prevMode);
                    system("cls");
                    handleLoginScreen(user.userName, user.userID, user.userPW);
                    return;
                }
            }
        }
    }


    // 기존 콘솔 모드 복원
    SetConsoleMode(hInput, prevMode);

    // 회원가입 후 로그인 화면으로 이동
    system("cls");
    handleLoginScreen(user.userName, user.userID, user.userPW);
}

// 메인메뉴 함수
void handleMainMenuScreen(const char* userID, const char* userName, const char* userPW)
{
    system("cls");

    drawBox(4, 1, 92, 47);
    gotoxy(32, 4);
    printf("===== 메인 메뉴 =====");

    gotoxy(36, 6);
    printf("환영합니다!");

    drawBox(27, 9, 30, 3);
    gotoxy(29, 10);  printf("1. 나의 시간표 보기 ");

    drawBox(27, 13, 30, 3);
    gotoxy(29, 14);  printf("2. 친구 목록 보기 ");

    drawBox(27, 17, 30, 3);
    gotoxy(29, 18);  printf("3. 로그아웃 ");

    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD prevMode;
    GetConsoleMode(hInput, &prevMode);
    SetConsoleMode(hInput, ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);

    INPUT_RECORD record;
    DWORD events;

    while (1)
    {
        ReadConsoleInput(hInput, &record, 1, &events);

        if (record.EventType == MOUSE_EVENT)
        {
            MOUSE_EVENT_RECORD mouse = record.Event.MouseEvent;
            int clickX = mouse.dwMousePosition.X;
            int clickY = mouse.dwMousePosition.Y;

            if (mouse.dwEventFlags == MOUSE_MOVED)
            {
                // 나의 시간표 보기
                if (isInside(clickX, clickY, 27, 9, 56, 11))
                {
                    setColor(13);
                    drawBox(27, 9, 30, 3);
                    gotoxy(29, 10);  printf("1. 나의 시간표 보기 ");
                }
                else
                {
                    setColor(15);
                    drawBox(27, 9, 30, 3);
                    gotoxy(29, 10);  printf("1. 나의 시간표 보기 ");
                }

                // 친구 목록 보기
                if (isInside(clickX, clickY, 27, 13, 56, 15))
                {
                    setColor(13);
                    drawBox(27, 13, 30, 3);
                    gotoxy(29, 14);  printf("2. 친구 목록 보기 ");
                }
                else
                {
                    setColor(15);
                    drawBox(27, 13, 30, 3);
                    gotoxy(29, 14);  printf("2. 친구 목록 보기 ");
                }

                // 로그아웃
                if (isInside(clickX, clickY, 27, 17, 56, 19))
                {
                    setColor(13);
                    drawBox(27, 17, 30, 3);
                    gotoxy(29, 18);  printf("3. 로그아웃 ");
                }
                else
                {
                    setColor(15);
                    drawBox(27, 17, 30, 3);
                    gotoxy(29, 18);  printf("3. 로그아웃 ");
                }

                setColor(15); // 기본 색상 복원
            }

            if (mouse.dwEventFlags == 0 && (mouse.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED))
            {
                if (isInside(clickX, clickY, 27, 9, 56, 11))  
                {
                    handleTimetableScreen(userID);
                }
                else if (isInside(clickX, clickY, 27, 13, 56, 15))  
                {
                    handleFriendsListScreen(userName, userID, userPW);
                }
                else if (isInside(clickX, clickY, 27, 17, 56, 19)) 
                {
                    handleLoginScreen(userName, userID, userPW);
                    break;
                }
            }
        }
    }
}

// 친구 목록 함수
void handleFriendsListScreen(const char* userID, const char* userName, const char* userPW)
{
    system("cls");
    drawBox(4, 1, 92, 47);

    gotoxy(32, 4);
    printf("===== 친구 목록 =====");

    FILE* file = fopen("C:\\Users\\Koomy\\source\\repos\\project01\\Friends.txt", "r");
    if (!file)
    {
        gotoxy(30, 10);
        printf(" 친구 목록을 불러올 수 없습니다.");
        _getch();
        return;
    }

    Friends friendData;
    char line[256];
    int yPos = 8;
    int friendCount = 0;

    while (fgets(line, sizeof(line), file))
    {
        if (sscanf(line,
            "userID: %49[^/] / friendName: %49[^/] / friendID: %49[^/] / friendPin: %9s",
            friendData.userID, friendData.friendName, friendData.friendID, friendData.friendPin) == 4)
        {
            trimAll(friendData.userID);
            trimAll(friendData.friendName);
            trimAll(friendData.friendID);

            if (strcmp(friendData.userID, userID) == 0)
            {
                gotoxy(20, yPos);
                printf("친구 이름 : %-12s ( ID : %-12s)", friendData.friendName, friendData.friendID);
                yPos += 2;
                friendCount++;
            }
        }
    }
    fclose(file);

    if (friendCount == 0)
    {
        gotoxy(30, 10);
        printf("친구가 없습니다.");
    }

    // 버튼 표시
    gotoxy(27, 30);
    printf("[ 친구 추가 ]");
    /*
    gotoxy(40, 30);
    printf("[ 즐겨찾기 ]")
    */
    gotoxy(53, 30);
    printf("[ 뒤로가기 ]");

    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD prevMode;
    GetConsoleMode(hInput, &prevMode);
    SetConsoleMode(hInput, ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);

    INPUT_RECORD record;
    DWORD events;

    while (1)
    {
        ReadConsoleInput(hInput, &record, 1, &events);

        if (record.EventType == MOUSE_EVENT)
        {
            MOUSE_EVENT_RECORD m = record.Event.MouseEvent;

            if (m.dwEventFlags == 0 && (m.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED))
            {
                MOUSE_EVENT_RECORD mouse = record.Event.MouseEvent;
                int clickX = mouse.dwMousePosition.X;
                int clickY = mouse.dwMousePosition.Y;

                // [ 친구 추가 ] 클릭
                if (isInside(clickX, clickY, 27, 30, 38, 30))
                {
                    handleAddFriendScreen(userName, userID);
                    system("cls");
                    handleFriendsListScreen(userID, userName, userPW);
                    break;
                }
                // [ 즐겨찾기 ] 클릭
                /*
                else if (isInside(clickX, clickY, 40, 30, 50, 30))
                {
                    handleFriendPin();
                    break;
                }
                */
                // [ 뒤로가기 ] 클릭
                else if (isInside(clickX, clickY, 53, 30, 63, 30))
                {
                    handleMainMenuScreen(userID, userName, userPW);
                    break;
                }
            }
        }
    }
}

// 친구 추가 함수
void handleAddFriendScreen(const char* userName, const char* userID)
{
    system("cls");
    drawBox(4, 1, 92, 47);

    Friends newFriend;
    int isAlreadyFriend = 0;

    gotoxy(32, 4);
    printf("===== 친구 추가 =====");

    // 입력창
    gotoxy(31, 9);  printf("친구 이름 : ");
    drawBox(31, 11, 30, 3);

    gotoxy(31, 15); printf("친구 아이디 : ");
    drawBox(31, 17, 30, 3);

    gotoxy(33, 30); printf("[ 친구 추가 ]");
    gotoxy(50, 30); printf("[ 친구 목록 ]");

    trimAll(newFriend.friendName);
    trimAll(newFriend.friendID);

    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD prevMode;
    GetConsoleMode(hInput, &prevMode);
    SetConsoleMode(hInput, ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);

    INPUT_RECORD record;
    DWORD events;

    while (1)
    {
        ReadConsoleInput(hInput, &record, 1, &events);

        if (record.EventType == MOUSE_EVENT)
        {
            MOUSE_EVENT_RECORD mouse = record.Event.MouseEvent;
            int clickX = mouse.dwMousePosition.X;
            int clickY = mouse.dwMousePosition.Y;

            if (mouse.dwEventFlags == MOUSE_MOVED)
            {
                // 친구 추가 버튼 hover
                if (isInside(clickX, clickY, 33, 30, 41, 30))
                {
                    setColor(10); // 초록색
                    gotoxy(33, 30); printf("[ 친구 추가 ]");
                }
                else
                {
                    setColor(15); // 흰색
                    gotoxy(33, 30); printf("[ 친구 추가 ]");
                }

                // 친구 목록 화면 이동 버튼 hover
                if (isInside(clickX, clickY, 50, 30, 65, 30))
                {
                    setColor(10);
                    gotoxy(50, 30); printf("[ 친구 목록 ]");
                    return;
                }
                else
                {
                    setColor(15);
                    gotoxy(50, 30); printf("[ 친구 목록 ]");
                }

                setColor(15); // 기본 색상 복원
            }

            if ((mouse.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) && mouse.dwEventFlags == 0)
            {
                // 친구 이름 입력 박스
                if (isInside(clickX, clickY, 31, 11, 61, 13))
                {
                    gotoxy(32, 12);
                    printf("                         ");
                    gotoxy(32, 12);
                    getTextInput(32, 12, newFriend.friendName, sizeof(newFriend.friendName) - 1);
                }
                // 친구 아이디 입력 박스
                else if (isInside(clickX, clickY, 31, 17, 61, 19))
                {
                    gotoxy(32, 18);
                    printf("                         ");
                    gotoxy(32, 18);
                    getTextInput(32, 18, newFriend.friendID, sizeof(newFriend.friendID) - 1);
                }
                // 확인 버튼 클릭
                else if (isInside(clickX, clickY, 33, 30, 42, 30))
                {
                    // 입력값 검증
                    if (strlen(newFriend.friendName) == 0 || strlen(newFriend.friendID) == 0)
                    {
                        gotoxy(33, 32);
                        printf("친구 추가 실패! (모든 칸을 입력하세요)");
                        Sleep(1500);
                        gotoxy(33, 32);
                        printf("                                     "); // 메시지 지우기
                        continue; // 다시 입력 대기
                    }

                    // 중복 친구 여부 확인
                    FILE* checkFile = fopen("C:\\Users\\Koomy\\source\\repos\\project01\\Friends.txt", "r");
                    char line[256];
                    Friends friendData;

                    if (checkFile)
                    {
                        while (fgets(line, sizeof(line), checkFile))
                        {
                            if (sscanf(line, "userID: %49[^/] / friendName: %49[^/] / friendID: %49[^/] / friendPin: %9s",
                                friendData.userID, friendData.friendName, friendData.friendID, friendData.friendPin) == 4)
                            {
                                trimAll(friendData.userID);
                                trimAll(friendData.friendID);

                                if (strcmp(friendData.userID, userID) == 0 &&
                                    strcmp(friendData.friendID, newFriend.friendID) == 0)
                                {
                                    isAlreadyFriend = 1;
                                    break;
                                }
                            }
                        }
                        fclose(checkFile);
                    }

                    // 결과 처리
                    if (isAlreadyFriend)
                    {
                        gotoxy(28, 23);
                        printf(" 이미 등록된 친구입니다.");
                    }
                    else
                    {
                        FILE* file = fopen("C:\\Users\\Koomy\\source\\repos\\project01\\Friends.txt", "a");
                        if (file)
                        {
                            fprintf(file, "userID: %s / friendName: %s / friendID: %s / friendPin: 0\n",
                                userID, newFriend.friendName, newFriend.friendID);
                            fclose(file);
                        }
                        gotoxy(28, 23);
                        printf(" 친구가 성공적으로 추가되었습니다!");
                        handleFriendsListScreen(userID, userName, userPW);
                    }
                }
                // 친구 목록 화면 이동 
                else if (isInside(clickX, clickY, 50, 30, 59, 30))
                {
                    SetConsoleMode(hInput, prevMode);
                    system("cls");
                    handleFriendsListScreen(userID, userName, userPW);
                    return;
                }
            }
        }
    }
    // 기존 콘솔 모드 복원
    SetConsoleMode(hInput, prevMode);
}

// 시간표 함수
void handleTimetableScreen(const char* userID)
{
    system("cls");
    drawBox(4, 1, 92, 47);

    int cellWidth = 15;      
    int cellHeight = 3;     
    int dayCellHeight = 3; 
    int timeCellWidth = 7;  

    int startX = 16;        
    int startY = 7;       

    const char* days[5] = { "월", "화", "수", "목", "금" };

    // 화면 제목
    gotoxy(39, 1);
    printf(" %s님의 시간표", userID);

    gotoxy(28, 45); printf("[ 수업추가 ]");
    gotoxy(42, 45); printf("[ 수업삭제 ]");
    gotoxy(56, 45); printf("[ 메인메뉴 ]");

    // 요일 칸
    for (int d = 0; d < 5; d++)
    {
        int x = startX + d * cellWidth;
        int y = startY - dayCellHeight;
        drawBox(x, y, cellWidth, dayCellHeight);
        gotoxy(x + (cellWidth / 2) - 1, y + 1);
        printf("%s", days[d]);
    }

    // 시간 칸
    for (int h = 0; h < 11; h++)
    {
        int posY = startY + h * cellHeight;
        drawBox(startX - timeCellWidth - 1, posY, timeCellWidth, cellHeight);
        gotoxy(startX - timeCellWidth + 1, posY + (cellHeight / 2));
        printf("%02d시", 9 + h);
    }

    // 수업 칸
    for (int row = 0; row < 11; row++)
    {
        int posY = startY + row * cellHeight;
        for (int col = 0; col < 5; col++)
        {
            drawBox(startX + col * cellWidth, posY, cellWidth, cellHeight);
        }
    }

    FILE* TimetableFile = fopen("C:\\Users\\Koomy\\source\\repos\\project01\\Timetable.txt", "r");

    if (!TimetableFile)
    {
        gotoxy(28, 45);
        printf("시간표 데이터(Timetable.txt)를 열 수 없습니다.");
        getchar();
        return;
    }

    char line[256], codes[200] = { 0 };
    while (fgets(line, sizeof(line), TimetableFile))
    {
        char fileUser[50];
        if (sscanf(line, "userID: %49[^/] / subjectCODE: %[^\n]", fileUser, codes) == 2) 
        {
            if (strcmp(fileUser, userID) == 0) 
                break;
        }
    }
    fclose(TimetableFile);

    if (strlen(codes) == 0) 
    {
        gotoxy(28, 45);
        printf("시간표 데이터가 없습니다.");
        getchar();
        return;
    }

    int codesArr[50], codeCount = 0;
    char* token = strtok(codes, ",");
    while (token) 
    {
        codesArr[codeCount++] = atoi(token);
        token = strtok(NULL, ",");
    }

    // 칸 사용 여부 체크
    int occupied[5][13] = { 0 };

    FILE* file = fopen("C:\\Users\\Koomy\\source\\repos\\project01\\Subject.txt", "r");
    if (!file) 
    {
        gotoxy(28, 45);
        printf("과목 데이터(Subject.txt)를 열 수 없습니다.");
        getchar();
        return;
    }

    while (fgets(line, sizeof(line), file))
    {
        SubjectList sub;
        if (sscanf(line,
            "subjectCODE: %9[^/] / subjectNAME: %49[^/] / professorNAME: %49[^/] / subjectCATE: %19[^/] / WEEK: %19[^/] / TIME: %[^\n]",
            sub.subjectCode, sub.subjectName, sub.professorName, sub.subCategory, sub.week, sub.time) == 6)
        {
            int subjectCodeInt = atoi(sub.subjectCode);
            for (int i = 0; i < codeCount; i++) 
            {
                if (codesArr[i] == subjectCodeInt) 
                {
                    int dayIdx = getDayIndex(sub.week);

                    int startHour = 0, endHour = 0;
                    sscanf(sub.time, "%d : %*d ~ %d : %*d", &startHour, &endHour);

                    int hourIdx = startHour - 9;
                    int duration = endHour - startHour;
                    if (duration <= 0) duration = 1;

                    // 중복 체크
                    int canPlace = 1;
                    for (int h = hourIdx; h < hourIdx + duration && h < 13; h++) 
                    {
                        if (occupied[dayIdx][h] == 1) 
                        {
                            canPlace = 0;
                            break;
                        }
                    }

                    // 빈칸일 때만 표시
                    if (canPlace && dayIdx >= 0 && hourIdx >= 0 && hourIdx < 13) 
                    {
                        int boxY = startY + hourIdx * cellHeight;
                        int boxHeight = cellHeight * duration;

                        int randColor = getRandomColor();
                        setColor(randColor);

                        drawBox(startX + dayIdx * cellWidth, boxY, cellWidth, boxHeight);
                        gotoxy(startX + dayIdx * cellWidth + 1, boxY + 1);
                        printf("%s", sub.subjectName);

                        setColor(7); // 7(흰색)

                        for (int h = hourIdx; h < hourIdx + duration && h < 13; h++) 
                        {
                            occupied[dayIdx][h] = 1;
                        }
                    }
                }
            }
        }
    }
    fclose(file);

    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD prevMode;
    GetConsoleMode(hInput, &prevMode);
    SetConsoleMode(hInput, ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);

    INPUT_RECORD record;
    DWORD events;

    while (1)
    {
        ReadConsoleInput(hInput, &record, 1, &events);

        if (record.EventType == MOUSE_EVENT)
        {
            MOUSE_EVENT_RECORD mouse = record.Event.MouseEvent;
            int clickX = mouse.dwMousePosition.X;
            int clickY = mouse.dwMousePosition.Y;

            if ((mouse.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) && mouse.dwEventFlags == 0)
            {
                // 수업추가 버튼
                if (isInside(clickX, clickY, 28, 45, 28 + 7, 45))
                {
                    handleSubjectList(userID);
                }

                // 수업삭제 버튼 
                else if (isInside(clickX, clickY, 42, 45, 42 + 7, 45))
                {
                    // 수업삭제 동작
                }

                // 메인메뉴 버튼 
                else if (isInside(clickX, clickY, 56, 45, 56 + 7, 45))
                {
                    handleMainMenuScreen(userID, userName, userPW);
                        break;
                }

            }
        }
    }
}

// 강의 목록 함수
void handleSubjectList(const char* userID)
{
    system("cls");
    SubjectList subjectdata;
    SubjectList allSubjects[300];

    char line[256];
    int totalSubjects = 0;
    int itemsPerPage = 8;
    int page = 1;
    int currentIndex = 0;

    FILE* file = fopen("C:\\Users\\Koomy\\source\\repos\\project01\\Subject.txt", "r");
    if (!file)
    {
        gotoxy(30, 10);
        printf(" 강의 목록이 열리지 않습니다. ");
        _getch();
        return;
    }

    // 강의 전체 불러오기
    while (fgets(line, sizeof(line), file))
    {
        if (sscanf(line,
            "subjectCODE: %49[^/] / subjectNAME: %49[^/] / professorNAME: %49[^/] / subjectCATE: %49[^/] / WEEK: %49[^/] / TIME: %[^\n]",
            subjectdata.subjectCode, subjectdata.subjectName, subjectdata.professorName,
            subjectdata.subCategory, subjectdata.week, subjectdata.time) == 6)
        {
            trimAll(subjectdata.subjectCode);
            trimAll(subjectdata.subjectName);
            trimAll(subjectdata.subCategory);
            trimAll(subjectdata.professorName);
            trimAll(subjectdata.week);
            trimAll(subjectdata.time);

            allSubjects[totalSubjects++] = subjectdata;
        }
    }
    fclose(file);

    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD prevMode;
    GetConsoleMode(hInput, &prevMode);
    SetConsoleMode(hInput, ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);

    INPUT_RECORD record;
    DWORD events;

    while (1)
    {
        system("cls");

        // 총 페이지 계산
        int totalPages = (totalSubjects + itemsPerPage - 1) / itemsPerPage;
        gotoxy(30, 5);
        printf(" ▶-----   강의 목록 (페이지 %d/%d)   -----◀", page, totalPages);

        // 강의 목록 출력 (박스는 강의 수만큼)
        int yPos = 8;
        int subjectCount = 0;
        for (int i = currentIndex; i < totalSubjects && subjectCount < itemsPerPage; i++)
        {
            drawBox(20, yPos, 70, 3);
            gotoxy(22, yPos + 1);
            printf("%-8s | %-10s | %-8s | %-10s | %-4s | %s",
                allSubjects[i].subjectCode,
                allSubjects[i].subjectName,
                allSubjects[i].subCategory,
                allSubjects[i].professorName,
                allSubjects[i].week,
                allSubjects[i].time);

            yPos += 4;
            subjectCount++;
        }

        // 페이지 이동 버튼
        gotoxy(20, 45); printf("[ 이전 페이지 ]");
        gotoxy(40, 45); printf("[ 다음 페이지 ]");
        gotoxy(60, 45); printf("[ 시간표 ]");

        // 마우스 입력 감지
        ReadConsoleInput(hInput, &record, 1, &events);

        if (record.EventType == MOUSE_EVENT)
        {
            MOUSE_EVENT_RECORD mouse = record.Event.MouseEvent;
            int clickX = mouse.dwMousePosition.X;
            int clickY = mouse.dwMousePosition.Y;

            if ((mouse.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) && mouse.dwEventFlags == 0)
            {
                // 이전 페이지 버튼
                if (isInside(clickX, clickY, 20, 45, 35, 45))
                {
                    if (currentIndex - itemsPerPage >= 0)
                    {
                        currentIndex -= itemsPerPage;
                        page--;
                    }
                }
                // 다음 페이지 버튼
                else if (isInside(clickX, clickY, 40, 45, 55, 45))
                {
                    if (currentIndex + itemsPerPage < totalSubjects)
                    {
                        currentIndex += itemsPerPage;
                        page++;
                    }
                }
                // 시간표로 돌아가기 버튼
                else if (isInside(clickX, clickY, 60, 45, 75, 45))
                {
                    SetConsoleMode(hInput, prevMode);
                    handleTimetableScreen(userID);
                    break;
                }
            }
        }
    }

    // 콘솔 모드 복원
    SetConsoleMode(hInput, prevMode);
}

// 즐겨찾기 함수
void handleFriendPin()
{

}

