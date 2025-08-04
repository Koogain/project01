#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdbool.h>
#include "struct.h"

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

// 전역 변수
char globalID[50] = "";
char globalPW[50] = "";

// 함수 전방 선언
bool isInside(int x, int y, int left, int top, int right, int bottom);
void gotoxy(int x, int y);
void trimAll(char* str);

void drawMainFrame(int x, int y, int width, int height);
void drawIDButton(int x, int y);
void drawPWInputBox(int x, int y);
void drawLoginButton(int x, int y);
void drawSignupText(int x, int y);

void listenForClick();
void handleLogin(const char* id, const char* pw);
void handleSignupScreen();
void drawInputBox(int x, int y, int width);
void getTextInput(int x, int y, char* buffer, int maxLen);
void waitForSignupClick(char* name, char* id, char* pw);

void handleMainMenuScreen(const char* userID, const char* userName);
void handleFriendsListScreen(const char* userID, const char* userName);
void handleAddFriendScreen(const char* userID);

bool isInside(int x, int y, int left, int top, int right, int bottom);

// 메인함수
int main(void)
{
    system("MODE CON:COLS=90 LINES=40");
    system("cls");

    drawMainFrame(5, 1, 80, 35);      // 전체 틀
    drawIDButton(25, 6);             // 로그인 버튼
    drawPWInputBox(25, 12);         // 비밀번호 입력
    drawLoginButton(27, 20);  // [ 로그인 버튼 ]
    drawSignupText(45, 20);    // [ 회원가입 ]

    listenForClick();   // 클릭 대기

    gotoxy(0, 22); // 커서 내리기
    _getch();
    return 0;
}

// 기본 함수
bool isInside(int x, int y, int left, int top, int right, int bottom)
{
    return (x >= left && x <= right && y >= top && y <= bottom);
}

void gotoxy(int x, int y) 
{
    COORD pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

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

// 기능 함수
void drawMainFrame(int x, int y, int width, int height) 
{
    gotoxy(x, y); printf("┌");
    for (int i = 0; i < width - 2; i++) printf("─");
    printf("┐");

    for (int j = 1; j < height - 1; j++) 
    {
        gotoxy(x, y + j);
        printf("│");
        for (int i = 0; i < width - 2; i++) printf(" ");
        printf("│");
    }

    gotoxy(x, y + height - 1); printf("└");
    for (int i = 0; i < width - 2; i++) printf("─");
    printf("┘");
}

void drawIDButton(int x, int y)
{
    int width = 35;

    gotoxy(x, y);
    printf("로그인: ");

    // 박스를 아래에 그림 (y+1부터)
    gotoxy(x, y + 1);
    printf("┌");
    for (int i = 0; i < width - 2; i++) printf("─");
    printf("┐");

    // 중간 줄 (입력창)
    gotoxy(x, y + 2);
    printf("│");
    for (int i = 0; i < width - 2; i++) printf(" ");
    printf("│");

    // 아래 줄
    gotoxy(x, y + 3);
    printf("└");
    for (int i = 0; i < width - 2; i++) printf("─");
    printf("┘");
}

void drawPWInputBox(int x, int y)
{
    int width = 35;

    gotoxy(x, y);
    printf("비밀번호: ");

    // 박스를 아래에 그림 (y+1부터)
    gotoxy(x, y + 1);
    printf("┌");
    for (int i = 0; i < width - 2; i++) printf("─");
    printf("┐");

    // 중간 줄 (입력창)
    gotoxy(x, y + 2);
    printf("│");
    for (int i = 0; i < width - 2; i++) printf(" ");
    printf("│");

    // 아래 줄
    gotoxy(x, y + 3);
    printf("└");
    for (int i = 0; i < width - 2; i++) printf("─");
    printf("┘");
}

void drawLoginButton(int x, int y) 
{
    gotoxy(x, y);
    printf("[  로그인  ]");
}

void drawSignupText(int x, int y) 
{
    gotoxy(x, y);
    printf("[ 회원가입 ]");
}

// 버튼 박스 그리는 함수
void drawButtonBox(int x, int y, int width, int height) 
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

// 텍스트 입력 박스 그리는 함수
void drawInputBox(int x, int y, int width)
{
    gotoxy(x, y);     printf("┌");
    for (int i = 0; i < width - 2; i++) printf("─");
    printf("┐");

    gotoxy(x, y + 1); printf("│");
    for (int i = 0; i < width - 2; i++) printf(" ");
    printf("│");

    gotoxy(x, y + 2); printf("└");
    for (int i = 0; i < width - 2; i++) printf("─");
    printf("┘");
}

//
void listenForClick()
{
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

            if ((mouse.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) && mouse.dwEventFlags == 0)
            {
                int clickX = mouse.dwMousePosition.X;
                int clickY = mouse.dwMousePosition.Y;

                // [회원가입]
                if (isInside(clickX, clickY, 45, 20, 45 + strlen("[ 회원가입 ]") - 1, 20)) 
                {
                    handleSignupScreen();
                    break;
                }

                // [로그인]
                else if (isInside(clickX, clickY, 27, 20, 37, 20)) 
                {
                    handleLogin(globalID, globalPW); 
                }

                // ID 입력 박스
                else if (isInside(clickX, clickY, 26, 8, 60, 8)) 
                {
                    gotoxy(26, 8);
                    getTextInput(26, 8, globalID, 49);  
                }

                // PW 입력 박스
                else if (isInside(clickX, clickY, 26, 14, 60, 14)) 
                {
                    gotoxy(26, 14);
                    getTextInput(26, 14, globalPW, 49);  
                }
            }
        }
    }
}

void handleLogin(const char* id, const char* pw)
{
    FILE* file = fopen("C:\\Users\\Koomy\\source\\repos\\project01\\Member.txt", "r");
    char line[150];
    Member user; 
    int isMatch = 0;

    if (file)
    {
        while (fgets(line, sizeof(line), file))
        {
            if (sscanf(line, "userNAME: %[^/] / userID: %s / userPW: %s",
                user.userName, user.userID, user.userPW) == 3)
            {
                if (strcmp(user.userID, id) == 0 && strcmp(user.userPW, pw) == 0)
                {
                    isMatch = 1;
                    break;
                }
            }
        }
        fclose(file);
    }

    gotoxy(26, 23);
    if (isMatch)
    {
        printf(" 로그인 성공! 환영합니다, %s님", user.userName);
        Sleep(1000);
        system("cls");
        handleMainMenuScreen(user.userID, user.userName);
    }
    else
    {
        printf(" 로그인 실패! 아이디 또는 비밀번호 확인");
    }
}

void handleSignupScreen()
{
    system("cls");

    Member user;  // 구조체 사용

    drawMainFrame(5, 1, 80, 35);

    gotoxy(32, 4);  printf("===== 회원가입 창 =====");

    // 입력창
    gotoxy(25, 8);  printf("이름: ");
    drawInputBox(35, 7, 30);
    getTextInput(36, 8, user.userName, sizeof(user.userName) - 1);

    gotoxy(25, 11); printf("아이디: ");
    drawInputBox(35, 10, 30);
    getTextInput(36, 11, user.userID, sizeof(user.userID) - 1);

    gotoxy(25, 14); printf("비밀번호: ");
    drawInputBox(35, 13, 30);
    getTextInput(36, 14, user.userPW, sizeof(user.userPW) - 1);

    gotoxy(36, 19); printf("[ 확인 ]");

    // 마우스 입력
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD prevMode;
    GetConsoleMode(hInput, &prevMode);
    SetConsoleMode(hInput, ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);

    INPUT_RECORD inputRecord;
    DWORD events;

    while (1)
    {
        ReadConsoleInput(hInput, &inputRecord, 1, &events);

        if (inputRecord.EventType == MOUSE_EVENT)
        {
            MOUSE_EVENT_RECORD m = inputRecord.Event.MouseEvent;

            if (m.dwEventFlags == 0 && (m.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED))
            {
                int x = m.dwMousePosition.X;
                int y = m.dwMousePosition.Y;

                if (x >= 35 && x <= 64 && y == 8)
                    getTextInput(36, 8, user.userName, sizeof(user.userName) - 1);
                else if (x >= 35 && x <= 64 && y == 11)
                    getTextInput(36, 11, user.userID, sizeof(user.userID) - 1);
                else if (x >= 35 && x <= 64 && y == 14)
                    getTextInput(36, 14, user.userPW, sizeof(user.userPW) - 1);
                else if (x >= 36 && x <= 43 && y == 19)  // 확인 버튼 클릭
                    break;
            }
        }
    }

    // 중복 ID 검사
    FILE* file = fopen("C:\\Users\\Koomy\\source\\repos\\project01\\Member.txt", "r");
    char line[150];
    Member existingUser;
    int isDuplicate = 0;

    if (file)
    {
        while (fgets(line, sizeof(line), file))
        {
            if (sscanf(line, "userNAME: %*[^/] / userID: %s", existingUser.userID) == 1)
            {
                if (strcmp(existingUser.userID, user.userID) == 0)
                {
                    isDuplicate = 1;
                    break;
                }
            }
        }
        fclose(file);
    }

    if (isDuplicate)
    {
        gotoxy(30, 23);
        printf(" 이미 존재하는 아이디입니다.");
        _getch();
        return;
    }

    // 신규 계정 저장
    file = fopen("C:\\Users\\Koomy\\source\\repos\\project01\\Member.txt", "a");
    if (file)
    {
        fprintf(file, "userNAME: %s / userID: %s / userPW: %s\n",
            user.userName, user.userID, user.userPW);
        fclose(file);

        gotoxy(30, 23);
        printf(" 회원가입 성공!");
        _getch();
    }
    else
    {
        gotoxy(32, 23);
        printf(" 회원가입 실패!");
        _getch();
    }
}

void getTextInput(int x, int y, char* buffer, int maxLen) 
{
    int i = 0;
    char ch;
    while (1) 
    {
        ch = _getch();
        if (ch == 13) 
        { // Enter
            buffer[i] = '\0';
            break;
        }
        else if (ch == 8 && i > 0) 
        { // Backspace
            i--;
            buffer[i] = '\0';
            gotoxy(x + i, y);
            printf(" ");
            gotoxy(x + i, y);
        }
        else if (i < maxLen && ch >= 32 && ch <= 126) 
        {
            buffer[i++] = ch;
            gotoxy(x + i - 1, y);
            printf("%c", ch);
        }
    }
}

//
void waitForSignupClick(char* name, char* id, char* pw)
{
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD prevMode;
    GetConsoleMode(hInput, &prevMode);
    SetConsoleMode(hInput, ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);

    INPUT_RECORD inputRecord;
    DWORD events;

    while (1) 
    {
        ReadConsoleInput(hInput, &inputRecord, 1, &events);

        if (inputRecord.EventType == MOUSE_EVENT) 
        {
            MOUSE_EVENT_RECORD m = inputRecord.Event.MouseEvent;

            if (m.dwEventFlags == 0 && (m.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)) 
            {
                int x = m.dwMousePosition.X;
                int y = m.dwMousePosition.Y;

                // 이름 박스
                if (x >= 30 && x <= 59 && y == 8) 
                {
                    gotoxy(31, 8);
                    getTextInput(31, 8, name, false);
                }

                // 아이디 박스
                else if (x >= 30 && x <= 59 && y == 11) 
                {
                    gotoxy(31, 11);
                    getTextInput(31, 11, id, false);
                }

                // 비밀번호 박스
                else if (x >= 30 && x <= 59 && y == 14) 
                {
                    gotoxy(31, 14);
                    getTextInput(31, 14, pw, true);
                }

                // [확인]을 클릭했다고 가정하는 y좌표 처리 (예시)
                else if (x >= 20 && x <= 35 && y == 17) 
                {
                    break;  // 입력 완료
                }
            }
        }
    }
}

//
void handleMainMenuScreen(const char* userID, const char* userName)
{
    system("cls");

    drawMainFrame(5, 1, 80, 35);
    gotoxy(32, 4);
    printf("===== 메인 메뉴 =====");

    gotoxy(36, 6);
    printf("환영합니다!");

    drawButtonBox(27, 9, 30, 3);
    drawButtonBox(27, 13, 30, 3);
    drawButtonBox(27, 17, 30, 3);

    gotoxy(29, 10);  printf("1. 나의 시간표 보기 ");
    gotoxy(29, 14);  printf("2. 친구 목록 보기 ");
    gotoxy(29, 18);  printf("3. 로그아웃 ");

    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD prevMode;
    GetConsoleMode(hInput, &prevMode);
    SetConsoleMode(hInput, ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);

    INPUT_RECORD inputRecord;
    DWORD events;

    while (1)
    {
        ReadConsoleInput(hInput, &inputRecord, 1, &events);

        if (inputRecord.EventType == MOUSE_EVENT)
        {
            MOUSE_EVENT_RECORD m = inputRecord.Event.MouseEvent;

            if (m.dwEventFlags == 0 && (m.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED))
            {
                int x = m.dwMousePosition.X;
                int y = m.dwMousePosition.Y;

                if (x >= 29 && x <= 57 && y >= 9 && y <= 11)
                {
                    // 나의 시간표 보기
                }
                else if (x >= 29 && x <= 57 && y >= 13 && y <= 15)
                {
                    handleFriendsListScreen(userID, userName); 
                }
                else if (x >= 29 && x <= 57 && y >= 17 && y <= 19)
                {
                    main();
                    break;
                }
            }
        }
    }
}

void handleFriendsListScreen(const char* userID, const char* userName)
{
    system("cls");
    drawMainFrame(5, 1, 80, 35);

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
    gotoxy(43, 30);
    printf("[ 뒤로가기 ]");

    // 마우스 이벤트 처리
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD prevMode;
    GetConsoleMode(hInput, &prevMode);
    SetConsoleMode(hInput, ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);

    INPUT_RECORD inputRecord;
    DWORD events;

    while (1)
    {
        ReadConsoleInput(hInput, &inputRecord, 1, &events);

        if (inputRecord.EventType == MOUSE_EVENT)
        {
            MOUSE_EVENT_RECORD m = inputRecord.Event.MouseEvent;

            if (m.dwEventFlags == 0 && (m.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED))
            {
                int x = m.dwMousePosition.X;
                int y = m.dwMousePosition.Y;

                // [친구 추가] 클릭
                if (x >= 27 && x <= 38 && y == 30)
                {
                    handleAddFriendScreen(userID);
                    system("cls");
                    handleFriendsListScreen(userID, userName);
                    break;
                }
                // [뒤로가기] 클릭
                else if (x >= 43 && x <= 53 && y == 30)
                {
                    handleMainMenuScreen(userID, userName);
                    break;
                }
            }
        }
    }
}

void handleAddFriendScreen(const char* userID)
{
    system("cls");
    drawMainFrame(5, 1, 80, 35);

    Friends newFriend;
    int isAlreadyFriend = 0;

    gotoxy(32, 4);
    printf("===== 친구 추가 =====");

    // 입력창
    gotoxy(25, 8);  printf("친구 이름: ");
    drawInputBox(40, 7, 25);
    getTextInput(41, 8, newFriend.friendName, sizeof(newFriend.friendName));

    gotoxy(25, 11); printf("친구 ID: ");
    drawInputBox(40, 10, 25);
    getTextInput(41, 11, newFriend.friendID, sizeof(newFriend.friendID));

    trimAll(newFriend.friendName);
    trimAll(newFriend.friendID);

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
    }

    _getch();
}

// 

