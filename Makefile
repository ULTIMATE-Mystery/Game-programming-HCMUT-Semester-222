SOURCE: entity.cpp ball.cpp renderwindow.cpp main.cpp
all:
	g++ -Isrc/include -Lsrc/lib -o main entity.cpp ball.cpp renderwindow.cpp main.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer