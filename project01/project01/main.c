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
void setColor(int color);
int getRandomColor();
void drawMainFrame(int x, int y, int width, int height);
void drawButtonBox(int x, int y, int width, int height);
void getTextInput(int x, int y, char* buffer, int maxLen);

void handleLoginScreen(const char* userName, const char* userID, const char* userPW);
void handleSignupScreen(const char* userName, const char* userID, const char* userPW);

void handleMainMenuScreen(const char* userName, const char* userID, const char* userPW);

int getDayIndex(const char* day);
int getStartHour(const char* timeStr);

void handleTimetableScreen(const char* userID);

void handleFriendsListScreen(const char* userName, const char* userID, const char* userPW);
void handleAddFriendScreen(const char* userName, const char* userID);

// 전역 변수
char userName[50] = { 0 };
char userID[50] = { 0 };
char userPW[50] = { 0 };

// 메인함수
int main(const char* userName, const char* userID, const char* userPW)
{
    system("MODE CON:COLS=100 LINES=50");      // 콘솔 창 크기 설정
    system("cls");

    drawMainFrame(4, 1, 92, 47);      // 전체 틀

    handleLoginScreen(userName, userID, userPW);

    gotoxy(0, 22); // 커서 내리기
    _getch();
    return 0;
}

// 기본 기능 함수
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

void setColor(int color) 
{
    SetConsoleTextAttribute(hConsole, color);
}

int getRandomColor() 
{
    int color;
    do 
    {
        color = 1 + rand() % 15;
    } while (color == 7);
    return color;
}

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

// 활용 기능 함수
void handleLoginScreen(char* userName, char* userID, char* userPW)
{
    system("cls");
    drawMainFrame(4, 1, 92, 47);

    gotoxy(32, 4);  printf("===== 로그인 창 =====");

    gotoxy(25, 6);  printf("아이디 : ");
    drawButtonBox(25, 8, 35, 3);

    gotoxy(25, 12); printf("비밀번호 : ");
    drawButtonBox(25, 14, 35, 3);

    gotoxy(27, 20); printf("[  로그인  ]");
    gotoxy(45, 20); printf("[ 회원가입 ]");

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

                // 회원가입 버튼
                if (isInside(clickX, clickY, 45, 20, 45 + strlen("[ 회원가입 ]") - 1, 20))
                {
                    handleSignupScreen(userName, userID, userPW);
                    break;
                }

                // 로그인 버튼
                else if (isInside(clickX, clickY, 27, 20, 37, 20))
                {
                    FILE* file = fopen("C:\\Users\\Koomy\\source\\repos\\project01\\Member.txt", "r");
                    if (!file)
                    {
                        gotoxy(26, 23);
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
                                userName[sizeof(loginUser.userName) - 1] = '\0';  // 문자열 끝 보장

                                gotoxy(26, 23);
                                printf(" 로그인 성공! 환영합니다, %s님", userName);
                                Sleep(1000);
                                fclose(file);
                                system("cls");
                                handleMainMenuScreen(userName, userID, userPW);
                                return;  // 함수 종료
                            }
                        }
                    }
                    fclose(file);

                    // 로그인 실패 시
                    gotoxy(26, 23);
                    printf(" 로그인 실패! 아이디 또는 비밀번호 확인");
                }

                // ID 입력 박스 클릭 시
                else if (isInside(clickX, clickY, 26, 9, 60, 9))
                {
                    gotoxy(26, 9);
                    printf("                                        ");
                    gotoxy(26, 9);
                    getTextInput(26, 9, userID, 49);
                }

                // PW 입력 박스 클릭 시
                else if (isInside(clickX, clickY, 26, 15, 60, 15))
                {
                    gotoxy(26, 15);
                    printf("                                        ");
                    gotoxy(26, 15);
                    getTextInput(26, 15, userPW, 49);
                }
            }
        }
    }
}

void handleSignupScreen()
{
    system("cls");

    Member user;  // 구조체 사용

    drawMainFrame(5, 1, 85, 38);

    gotoxy(32, 4);  printf("===== 회원가입 창 =====");

    // 입력창
    gotoxy(25, 8);  printf("이름: ");
    drawButtonBox(35, 7, 30, 3);
    getTextInput(36, 8, user.userName, sizeof(user.userName) - 1);

    gotoxy(25, 11); printf("아이디: ");
    drawButtonBox(35, 10, 30, 3);
    getTextInput(36, 11, user.userID, sizeof(user.userID) - 1);

    gotoxy(25, 14); printf("비밀번호: ");
    drawButtonBox(35, 13, 30, 3);
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

void handleMainMenuScreen(const char* userID, const char* userName, const char* userPW)
{
    system("cls");

    drawMainFrame(4, 1, 92, 47);
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
                    handleTimetableScreen(userID);
                }
                else if (x >= 29 && x <= 57 && y >= 13 && y <= 15)
                {
                    handleFriendsListScreen(userName, userID, userPW);
                }
                else if (x >= 29 && x <= 57 && y >= 17 && y <= 19)
                {
                    handleLoginScreen(userName, userID, userPW);
                    break;
                }
            }
        }
    }
}

void handleFriendsListScreen(const char* userID, const char* userName, const char* userPW)
{
    system("cls");
    drawMainFrame(4, 1, 92, 47);

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
                    handleAddFriendScreen(userName, userID);
                    system("cls");
                    handleFriendsListScreen(userID, userName, userPW);
                    break;
                }
                // [뒤로가기] 클릭
                else if (x >= 43 && x <= 53 && y == 30)
                {
                    handleMainMenuScreen(userID, userName, userPW);
                    break;
                }
            }
        }
    }
}

void handleAddFriendScreen(const char* userName, const char* userID)
{
    system("cls");
    drawMainFrame(4, 1, 92, 47);

    Friends newFriend;
    int isAlreadyFriend = 0;

    gotoxy(32, 4);
    printf("===== 친구 추가 =====");

    // 입력창
    gotoxy(25, 8);  printf("친구 이름: ");
    drawButtonBox(40, 7, 10, 25);
    getTextInput(41, 8, newFriend.friendName, sizeof(newFriend.friendName));

    gotoxy(25, 11); printf("친구 ID: ");
    drawButtonBox(40, 10, 10, 25);
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

int getDayIndex(const char* day)
{
    if (strstr(day, "월")) return 0;
    if (strstr(day, "화")) return 1;
    if (strstr(day, "수")) return 2;
    if (strstr(day, "목")) return 3;
    if (strstr(day, "금")) return 4;
    return -1;
}

int getStartHour(const char* timeStr)
{
    int hour = 0;
    sscanf(timeStr, " %d", &hour); // 첫 숫자만 추출
    return hour;
}

void handleTimetableScreen(const char* userID)
{
    system("cls");
    drawMainFrame(4, 1, 92, 47);

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

    // 요일 칸
    for (int d = 0; d < 5; d++)
    {
        int x = startX + d * cellWidth;
        int y = startY - dayCellHeight;
        drawButtonBox(x, y, cellWidth, dayCellHeight);
        gotoxy(x + (cellWidth / 2) - 1, y + 1);
        printf("%s", days[d]);
    }

    // 시간 칸
    for (int h = 0; h < 11; h++)
    {
        int posY = startY + h * cellHeight;
        drawButtonBox(startX - timeCellWidth - 1, posY, timeCellWidth, cellHeight);
        gotoxy(startX - timeCellWidth + 1, posY + (cellHeight / 2));
        printf("%02d시", 9 + h);
    }

    // 수업 칸
    for (int row = 0; row < 11; row++)
    {
        int posY = startY + row * cellHeight;
        for (int col = 0; col < 5; col++)
        {
            drawButtonBox(startX + col * cellWidth, posY, cellWidth, cellHeight);
        }
    }

    FILE* timetableFile = fopen("Timetable.txt", "r");
    if (!timetableFile) 
    {
        gotoxy(28, 45);
        printf("시간표 데이터(Timetable.txt)를 열 수 없습니다.");
        getchar();
        return;
    }

    char line[256], codes[200] = { 0 };
    while (fgets(line, sizeof(line), timetableFile)) 
    {
        char fileUser[50];
        if (sscanf(line, "userID: %49[^/] / subjectCODE: %[^\n]", fileUser, codes) == 2) 
        {
            if (strcmp(fileUser, userID) == 0) 
                break;
        }
    }
    fclose(timetableFile);

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

    FILE* subjectFile = fopen("Subject.txt", "r");
    if (!subjectFile) 
    {
        gotoxy(28, 45);
        printf("과목 데이터(Subject.txt)를 열 수 없습니다.");
        getchar();
        return;
    }

    while (fgets(line, sizeof(line), subjectFile)) 
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

                        drawButtonBox(startX + dayIdx * cellWidth, boxY, cellWidth, boxHeight);
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
    fclose(subjectFile);

    gotoxy(28, 45);
    printf("아무 키나 누르면 돌아갑니다...");
    getchar();
}

