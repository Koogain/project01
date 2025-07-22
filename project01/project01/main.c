#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdbool.h>

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

void gotoxy(int x, int y);

void drawMainFrame(int x, int y, int width, int height);
void drawIDButton(int x, int y);
void drawPWInputBox(int x, int y);
void drawSignupText(int x, int y);

void waitForClick();

bool checkDuplicateID(const char* id);

int main(void)
{
    system("cls");

    drawMainFrame(5, 1, 50, 28);            // 전체 틀
    drawIDButton(15, 4);                // 로그인 버튼
    drawPWInputBox(15, 10);           // 비밀번호 입력
    drawSignupText(20, 16);                // [회원가입]

    waitForClick();

    printf("\n다음 화면으로 넘어갑니다!\n");

    gotoxy(0, 22); // 커서 내리기
    _getch();
    return 0;
}

void SetColor(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void CursorView(char show)
{
    HANDLE hConsole;
    CONSOLE_CURSOR_INFO ConsoleCursor;

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    ConsoleCursor.bVisible = show;
    ConsoleCursor.dwSize = 1;

    SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}

void gotoxy(int x, int y) {
    COORD pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void drawMainFrame(int x, int y, int width, int height) {
    gotoxy(x, y); printf("┌");
    for (int i = 0; i < width - 2; i++) printf("─");
    printf("┐");

    for (int j = 1; j < height - 1; j++) {
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
    int width = 25;

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
    int width = 25;

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

void drawSignupText(int x, int y) {
    gotoxy(x, y);
    printf("[ 회원가입 ]");
}

bool isInside(int x, int y, int left, int top, int right, int bottom)
{
    return (x >= left && x <= right && y >= top && y <= bottom);
}

void getTextInput(int x, int y, char* buffer, bool mask)
{
    int i = 0;
    char ch;
    while (1) {
        ch = _getch();
        if (ch == 13) {
            buffer[i] = '\0';
            break;
        }
        else if (ch == 8) {
            if (i > 0) {
                i--;
                buffer[i] = '\0';
                gotoxy(x + i, y);
                printf(" ");
                gotoxy(x + i, y);
            }
        }
        else if (i < 49 && ch >= 32 && ch <= 126) {
            buffer[i++] = ch;
            gotoxy(x + i - 1, y);
            printf("%c", mask ? '*' : ch);
        }
    }
}

void waitForClick()
{
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD prevMode;
    GetConsoleMode(hInput, &prevMode);
    SetConsoleMode(hInput, ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);

    INPUT_RECORD inputRecord;
    DWORD events;

    char id[50] = "";
    char pw[50] = "";
    bool idDone = false, pwDone = false;

    while (1) {
        ReadConsoleInput(hInput, &inputRecord, 1, &events);

        if (inputRecord.EventType == MOUSE_EVENT) {
            MOUSE_EVENT_RECORD m = inputRecord.Event.MouseEvent;

            if (m.dwEventFlags == 0 && (m.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)) {
                int x = m.dwMousePosition.X;
                int y = m.dwMousePosition.Y;

                if (isInside(x, y, LOGIN_X, LOGIN_Y, LOGIN_X + BTN_WIDTH - 1, LOGIN_Y + BTN_HEIGHT - 1)) {
                    gotoxy(LOGIN_X + 2, LOGIN_Y + 2);
                    printf("                    ");
                    gotoxy(LOGIN_X + 2, LOGIN_Y + 2);
                    getTextInput(LOGIN_X + 2, LOGIN_Y + 2, id, false);

                    if (checkDuplicateID(id)) {
                        gotoxy(0, 22);
                        printf("❌ 이미 존재하는 ID입니다. 다시 입력하세요.");
                        idDone = false;
                        continue; // 다시 클릭 대기
                    }
                    else {
                        idDone = true;
                    }
                }

                else if (isInside(x, y, PASS_X, PASS_Y, PASS_X + BTN_WIDTH - 1, PASS_Y + BTN_HEIGHT - 1)) {
                    gotoxy(PASS_X + 2, PASS_Y + 2);
                    printf("                    ");
                    gotoxy(PASS_X + 2, PASS_Y + 2);
                    getTextInput(PASS_X + 2, PASS_Y + 2, pw, true);
                    pwDone = true;
                }
                else if (isInside(x, y, SIGNUP_X, SIGNUP_Y, SIGNUP_X + (int)strlen("[ 회원가입 ]") - 1, SIGNUP_Y)) {
                    system("cls");
                    printf("회원가입 클릭됨\n");
                    break;
                }

                // ID와 PW 둘 다 입력 완료되었을 때 저장
                if (idDone && pwDone) {
                    FILE* fp = fopen("Member.txt", "a");
                    if (fp) {
                        fprintf(fp, "ID: %s\n", id);
                        fprintf(fp, "PW: %s\n", pw);
                        fprintf(fp, "--------------------\n");
                        fclose(fp);

                        gotoxy(0, 22);
                        printf("✅ Member.txt에 저장 완료!\n");
                    }
                    break;  // 저장 후 종료
                }
            }
        }
    }
}

bool checkDuplicateID(const char* id) 
{
    FILE* fp = fopen("Member.txt", "r");
    if (!fp) return false; // 파일이 없으면 중복 아님

    char line[100];
    while (fgets(line, sizeof(line), fp)) 
    {
        if (strncmp(line, "ID: ", 4) == 0) 
        {
            char storedId[50];
            sscanf(line + 4, "%s", storedId);
            if (strcmp(storedId, id) == 0) 
            {
                fclose(fp);
                return true; // 중복 ID
            }
        }
    }

    fclose(fp);
    return false;
}
