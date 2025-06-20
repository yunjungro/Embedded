LID_DIR = ..

all : main.elf
	scp main.elf ecube@192.168.50.206:/home/ecube/homework_2
	echo "[send main.elf]"
	rm -f *.o *.a
	echo "[remove files]"

ledtest.o : $(LID_DIR)/ledtest.h $(LID_DIR)/ledtest.c
	arm-linux-gnueabi-gcc -c $(LID_DIR)/ledtest.c -o ledtest.o
	echo "1. make ledtest.o"

buttontest.o : $(LID_DIR)/buttontest.h $(LID_DIR)/buttontest.c
	arm-linux-gnueabi-gcc -c $(LID_DIR)/buttontest.c -o buttontest.o
	echo "2. make buttontest.o"

buzzertest.o : $(LID_DIR)/buzzertest.h $(LID_DIR)/buzzertest.c
	arm-linux-gnueabi-gcc -c $(LID_DIR)/buzzertest.c -o buzzertest.o
	echo "3. make buzzertest.o"

# ── 현재 디렉터리에 있는 모듈 ― LID_DIR 접두어 제거 ──
fndtest.o : fndtest.h fndtest.c
	arm-linux-gnueabi-gcc -c fndtest.c -o fndtest.o
	echo "4. make fndtest.o"

textlcdtest.o : textlcdtest.h textlcdtest.c   # ← 여기 철자 주의
	arm-linux-gnueabi-gcc -c textlcdtest.c -o textlcdtest.o
	echo "5. make texttest.o"



colorLEDtest.o : colorLEDtest.h colorLEDtest.c
	arm-linux-gnueabi-gcc -c colorLEDtest.c -o colorLEDtest.o
	echo "6. make colorLEDtest.o"
# ──────────────────────────────────────────────────────

libMyPeri.a : ledtest.o buttontest.o buzzertest.o fndtest.o textlcdtest.o colorLEDtest.o
	arm-linux-gnueabi-ar rc libMyPeri.a ledtest.o buttontest.o buzzertest.o fndtest.o textlcdtest.o colorLEDtest.o
	echo "[make libMyPeri.a]"

main.elf : main.c libMyPeri.a
	arm-linux-gnueabi-gcc -o main.elf main.c -lMyPeri -L. -lpthread
	echo "[make main.elf]"
