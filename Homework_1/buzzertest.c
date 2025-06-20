/* buzzertest.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include "buzzertest.h"

/* 헤더에 이미 전역 배열이 ‘정의’돼 있으므로 여기서는 참조만 */
extern char gBuzzerBaseSysDir[128];

/* enable 파일을 한 번만 열어 보관 */
static int gEnableFd = -1;          // -1이면 아직 미개방

/* ───────── 버저 ON/OFF ───────── */
void buzzerEnable(int enable)
{
    if (gEnableFd < 0) {
        printf("enable 파일이 열려 있지 않습니다\n");
        return;
    }

    const char *val = enable ? "1" : "0";
    if (write(gEnableFd, val, 1) != 1)
        printf("enable 값 쓰기 실패\n");
}

/* ───────── 주파수 설정 ───────── */
void setFrequency(int frequency)
{
    char path[256];
    snprintf(path, sizeof(path),
             "%s%s", gBuzzerBaseSysDir, BUZZER_FREQUENCY_NAME);

    int fd = open(path, O_WRONLY);
    if (fd < 0) {
        printf("주파수 파일 열기 실패\n");
        return;
    }

    if (dprintf(fd, "%d", frequency) < 0)
        printf("주파수 쓰기 실패\n");

    close(fd);
}

/* ───────── 초기화 ───────── */
int buzzerInit(void)
{
    DIR *dir = opendir(BUZZER_BASE_SYS_PATH);
    if (!dir) {
        printf("버저 sysfs 디렉터리 열기 실패\n");
        return -1;
    }

    int found = 0;
    struct dirent *ent;
    while ((ent = readdir(dir))) {
        if (strncmp(ent->d_name, BUZZER_FILENAME,
                    strlen(BUZZER_FILENAME)) == 0) {
            snprintf(gBuzzerBaseSysDir, sizeof(gBuzzerBaseSysDir),
                     "%s%s/", BUZZER_BASE_SYS_PATH, ent->d_name);
            found = 1;
            break;
        }
    }
    closedir(dir);

    if (!found) {
        printf("버저 디렉터리를 찾을 수 없습니다\n");
        return -1;
    }

    /* enable 파일을 열어 FD 저장 */
    char enPath[256];
    snprintf(enPath, sizeof(enPath),
             "%s%s", gBuzzerBaseSysDir, BUZZER_ENABLE_NAME);

    gEnableFd = open(enPath, O_WRONLY);
    if (gEnableFd < 0) {
        printf("enable 파일 열기 실패\n");
        return -1;
    }

    printf("버저 sysfs 경로: %s\n", gBuzzerBaseSysDir);
    return 0;
}

/* ───────── 종료 ───────── */
void buzzerExit(void)
{
    buzzerEnable(0);               // 소리 OFF
    if (gEnableFd >= 0) {
        close(gEnableFd);          // FD 닫기
        gEnableFd = -1;
    }
}

/* ───────── 외부용 API ───────── */
void buzzerTone(int scale)
{
    setFrequency(scale);
    buzzerEnable(1);
}

void buzzerStop(void)
{
    buzzerEnable(0);
}

void delay_ms(unsigned int ms)
{
    usleep(ms * 1000);
}

