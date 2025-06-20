#include "../ledtest.h" 
#include "../buttontest.h" 
#include "../buzzertest.h" 
#include "fndtest.h" 
#include "textlcdtest.h" 
#include "colorLEDtest.h" 

#include <stdio.h> 
#include <unistd.h> 
#include <signal.h> 
#include <errno.h> 
#include <linux/input.h>

int qid;
BUTTON_MSG_T m; 
int rgb[3][3] = {{25, 0, 0}, {0, 25, 0}, {0, 0, 25}}; 
const char* name[3] = {"RED", "GREEN", "BLUE"}; 
volatile sig_atomic_t running = 1; 
int i, c, ch; 

static void byeSig(int s) { 
    running = 0; 
    printf(" program_ STOP\n");
}

int main(void) { 
    signal(SIGUSR1, byeSig); 
    
    qid = msgget(MESSAGE_ID, 0); 
    ledLibInit(); 
    buttonInit(); 
    buzzerInit(); 
    textLCDInit(); 
    colorLED_init(); 
    colorLED_pwmEnable();
    
    textLCDWrite("jungro", 1); 
    textLCDWrite("homework", 2); 
    printf("[LCD] jungro / homework\n"); 

    for (i = 0; i <= 1 && running; ++i) { 
        fndInit(i, 0); 
        printf("[FND] %d\n", i); 
        sleep(1);
    } 
    
    while (running) { 
        for (c = 0; c < 3 && running; ++c) { 
            for (ch = 0; ch < 3; ++ch) 
                colorLED_serPercent(rgb[c][ch], ch); 
            
            textLCDWrite(name[c], 1); 
            printf("[LED] %s\n", name[c]); 

            ssize_t n = msgrcv(qid, &m, sizeof(m) - sizeof(long), 0, IPC_NOWAIT); 
            if (n > 0) { 
                if (m.pressed && m.type == EV_KEY && m.keyInput == KEY_HOME) { 
                    usleep(150 * 1000);  
                    while (msgrcv(qid, &m, sizeof(m) - sizeof(long), 0, IPC_NOWAIT) > 0);
                    if (m.pressed && m.keyInput == KEY_HOME) { 
                        kill(getpid(), SIGUSR1); 
                    }
                } 
            } else if (n < 0 && errno != ENOMSG) { 
                perror("msgrcv"); 
            } 

            sleep(1); 
        } 
    }

    buzzerStop(); 
    buttonExit(); 
    ledLibExit(); 
    buzzerExit(); 
    fndExit(); 
    textLCDExit(); 
    return 0; 
}

