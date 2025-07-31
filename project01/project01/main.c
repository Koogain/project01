#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdbool.h>

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
void gotoxy(int x, int y);

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



void handleMainMenuScreen(const char* userName);
void handleFriendsListScreen();

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
    drawSignupText(45, 20);    // [회원가입]

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
    char fileId[50], filePw[50], fileName[50];
    int isMatch = 0;

    if (file)
    {
        while (fgets(line, sizeof(line), file))
        {
            if (sscanf(line, "userNAME: %[^/] / userID: %s / userPW: %s", fileName, fileId, filePw) == 3)
            {
                if (strcmp(fileId, id) == 0 && strcmp(filePw, pw) == 0)
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
        printf("✅ 로그인 성공! 환영합니다, %s님", fileName);
        Sleep(1000);  // 메시지 잠깐 보여주기
        system("cls"); // 화면 지우기
        handleMainMenuScreen(fileName); //  로그인 성공 - 시간표 메뉴 화면으로 이동
    }
    else
    {
        printf("❌ 로그인 실패! 아이디 또는 비밀번호 확인");
    }
}

void handleSignupScreen()
{
    system("cls");

    char name[50], id[50], pw[50];

    drawMainFrame(5, 1, 80, 35);

    gotoxy(32, 4);  printf("===== 회원가입 창 =====");

    // 입력창
    gotoxy(25, 8);  printf("이름: ");
    drawInputBox(35, 7, 30);
    getTextInput(36, 8, name, 49);

    gotoxy(25, 11); printf("아이디: ");
    drawInputBox(35, 10, 30);
    getTextInput(36, 11, id, 49);

    gotoxy(25, 14); printf("비밀번호: ");
    drawInputBox(35, 13, 30);
    getTextInput(36, 14, pw, 49);

    gotoxy(36, 19); printf("[ 확인 ]");

    // 마우스 입력 설정
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
                    getTextInput(36, 8, name, 49);
                else if (x >= 35 && x <= 64 && y == 11)
                    getTextInput(36, 11, id, 49);
                else if (x >= 35 && x <= 64 && y == 14)
                    getTextInput(36, 14, pw, 49);
                else if (x >= 36 && x <= 43 && y == 19)  // 확인 버튼 클릭
                    break;
            }
        }
    }

    // 중복 ID 검사
    FILE* file = fopen("C:\\Users\\Koomy\\source\\repos\\project01\\Member.txt", "r");
    char line[150], checkId[50];
    int isDuplicate = 0;

    if (file)
    {
        while (fgets(line, sizeof(line), file))
        {
            if (sscanf(line, "userNAME: %*[^/] / userID: %s", checkId) == 1)
            {
                if (strcmp(checkId, id) == 0)
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
        printf("❌ 이미 존재하는 아이디입니다.");

        gotoxy(25, 23);
        printf("Enter 키를 누르면 로그인 화면으로 이동합니다.");
        _getch();

        system("cls");
        main();  // 로그인 화면 복귀
        return;
    }

    // 저장
    file = fopen("C:\\Users\\Koomy\\source\\repos\\project01\\Member.txt", "a");
    if (file)
    {
        fprintf(file, "userNAME: %s / userID: %s / userPW: %s\n", name, id, pw);
        fclose(file);

        gotoxy(30, 23);
        printf("✅ 회원가입 성공!");

        gotoxy(25, 35);
        printf("Enter 키를 누르면 로그인 화면으로 이동합니다.");
        _getch();

        system("cls");
        main();  // 로그인 화면 복귀
    }
    else
    {
        gotoxy(32, 23);
        printf("⚠️ 회원가입 실패!");
        _getch();
        system("cls");
        main();
    }
}

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
void handleMainMenuScreen(const char* userName)
{
    system("cls");

    drawMainFrame(5, 1, 80, 35);

    gotoxy(32, 4);
    printf("===== 메인 메뉴 =====");

    gotoxy(30, 6);
    printf(" %s님 환영합니다!", userName);

    // 메뉴 버튼 좌표 및 크기
    int btnWidth = 25;
    int btnHeight = 3;
    int startX = 27;
    int startY = 10;

    // 3개 버튼 그리기
    char* menuText[3] = { "1. 나의 시간표 보기", "2. 친구 목록 보기", "3. 로그아웃" };
    int buttonY[3];

    for (int i = 0; i < 3; i++)
    {
        buttonY[i] = startY + (i * (btnHeight + 2));

        // 상단
        gotoxy(startX, buttonY[i]);
        printf("┌");
        for (int j = 0; j < btnWidth - 2; j++) printf("─");
        printf("┐");

        // 중간
        gotoxy(startX, buttonY[i] + 1);
        printf("│ %-20s │", menuText[i]);

        // 하단
        gotoxy(startX, buttonY[i] + 2);
        printf("└");
        for (int j = 0; j < btnWidth - 2; j++) printf("─");
        printf("┘");
    }

    // 마우스 입력 모드 활성화
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
                int mx = m.dwMousePosition.X;
                int my = m.dwMousePosition.Y;

                // 1. 나의 시간표 보기
                if (mx >= startX && mx <= startX + btnWidth &&
                    my >= buttonY[0] && my <= buttonY[0] + btnHeight)
                {
                    system("cls");
                    gotoxy(30, 20);
                    printf(" [나의 시간표 보기] 기능 실행");
                    _getch();
                    return;
                }

                // 2. 친구 목록 보기
                if (mx >= startX && mx <= startX + btnWidth &&
                    my >= buttonY[1] && my <= buttonY[1] + btnHeight)
                {
                    handleFriendsListScreen();  // 친구 리스트 화면으로 이동
                    return;
                }

                // 3. 로그아웃
                if (mx >= startX && mx <= startX + btnWidth &&
                    my >= buttonY[2] && my <= buttonY[2] + btnHeight)
                {
                    system("cls");
                    gotoxy(30, 20);
                    printf(" 로그아웃되었습니다.");
                    _getch();
                    main();  // 로그인 화면으로 돌아감
                    return;
                }
            }
        }
    }
}

void handleFriendsListScreen()
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

    char userID[50], friendName[50], friendID[50], favorite[10];
    int yPos = 8; // 버튼 시작 Y좌표
    int friendCount = 0;

    // 버튼 정보를 저장할 배열
    typedef struct {
        int x, y, width, height;
        char name[50];
        char id[50];
    } FriendButton;

    FriendButton buttons[50];

    while (fscanf(file, "%s %s %s %s", userID, friendName, friendID, favorite) == 4)
    {
        // 버튼 정보 저장
        buttons[friendCount].x = 20;
        buttons[friendCount].y = yPos;
        buttons[friendCount].width = 40;
        buttons[friendCount].height = 3;
        strcpy(buttons[friendCount].name, friendName);
        strcpy(buttons[friendCount].id, friendID);

        // 버튼 출력
        gotoxy(buttons[friendCount].x, buttons[friendCount].y);
        printf("┌");
        for (int i = 0; i < buttons[friendCount].width - 2; i++) printf("─");
        printf("┐");

        gotoxy(buttons[friendCount].x, buttons[friendCount].y + 1);
        printf("│ %-10s (ID: %s) │", friendName, friendID);
        for (int i = strlen(friendName) + strlen(friendID) + 9; i < buttons[friendCount].width - 2; i++) printf(" ");

        gotoxy(buttons[friendCount].x, buttons[friendCount].y + 2);
        printf("└");
        for (int i = 0; i < buttons[friendCount].width - 2; i++) printf("─");
        printf("┘");

        yPos += 4;
        friendCount++;
    }
    fclose(file);

    gotoxy(32, yPos + 2);
    printf("[ 뒤로가기 ]");

    // 마우스 입력 설정
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
                int mx = m.dwMousePosition.X;
                int my = m.dwMousePosition.Y;

                // 친구 버튼 클릭 감지
                for (int i = 0; i < friendCount; i++)
                {
                    if (mx >= buttons[i].x && mx <= buttons[i].x + buttons[i].width &&
                        my >= buttons[i].y && my <= buttons[i].y + buttons[i].height)
                    {
                        system("cls");
                        gotoxy(30, 15);
                        printf(" '%s' (%s) 친구를 선택했습니다.", buttons[i].name, buttons[i].id);
                        _getch();
                        return;
                    }
                }

                // 뒤로가기 버튼
                if (mx >= 32 && mx <= 43 && my == yPos + 2)
                {
                    return;
                }
            }
        }
    }
}

