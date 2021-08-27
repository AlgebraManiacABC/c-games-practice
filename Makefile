all: tetris HH

tetris: tetris.c color.h myterm.h
    gcc tetris.c -g -Wall -Werror -lm -std=c11 -o tetris

HH: HH.c select.h color.h myterm.h
    gcc tetris.c -g -Wall -Werror -lm -std=c11 -o Henderson-Hasselbalch
