C := gcc 

PROJECT := emu.dll
OPT_F := -O2 -Os -s
SOR := cpu.c gpu.c emu.c

all: $(PROJECT) test1

$(PROJECT):
	$(C) --shared $(SOR) -o build/$(PROJECT) $(OPT_F)

test1:
	gcc test/main1.c -o build/main1.exe -Lbuild -lemu -O2 -Os -s 
	gcc test/main2.c -o build/main2.exe -O2 -Os -s
	gcc test/main3.c -o build/main3.exe -Lbuild -lemu -O2 -Os -s 
	gcc test/main4.c -o build/main4.exe -Lbuild -lemu -O2 -Os -s 

