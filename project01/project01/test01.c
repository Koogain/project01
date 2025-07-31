#include <direct.h>
#include <stdio.h>

int main() {
    char buf[256];
    _getcwd(buf, sizeof(buf));
    printf("현재 실행 디렉토리: %s\n", buf);

    FILE* test = fopen("Member.txt", "a");
    if (test) 
    {
        fprintf(test, "⭐ 여기 저장됨!\n");
        fclose(test);
        printf("파일 쓰기 성공!\n");
    }
    else {
        printf("파일 쓰기 실패!\n");
    }

    return 0;
}
