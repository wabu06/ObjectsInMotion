#CC = clang++ -std=gnu++17 -Wall
CC = clang++ -std=gnu++20 -Wall
#CC = g++ -std=gnu++20 -Wall

SD = src/

OBJ = objects.o ObjectsInMotion.o main.o
INC = ${SD}objects.hpp ${SD}ObjectsInMotion.hpp

imgui_INC = ${SD}imconfig-SFML.h ${SD}imgui-SFML_export.h ${SD}imgui-SFML.h

build:	oim
	
oim:	${OBJ}
	${CC} ${OBJ} `pkg-config sfml-graphics --libs` -o oim

main.o:	${INC} ${SD}main.cpp
	${CC} -c ${SD}main.cpp `pkg-config imgui --cflags` -I./${SD}

ObjectsInMotion.o:	${INC} ${SD}ObjectsInMotion.cpp
	${CC} -c ${SD}ObjectsInMotion.cpp `pkg-config imgui --cflags` -I./${SD}

objects.o:	${SD}objects.hpp ${SD}objects.cpp
	${CC} -c ${SD}objects.cpp

#`pkg-config imgui --cflags -I./${SD}`
#`pkg-config imgui --libs --cflags`

oim-gui:	imgui-SFML.o ${OBJ}
	${CC} imgui-SFML.o ${OBJ} `pkg-config sfml-graphics imgui gl --libs` -o oim

#${CC} imgui-SFML.o ${OBJ} `pkg-config sfml-graphics --libs` `pkg-config imgui --libs` `pkg-config gl --libs` -o oim

imgui-SFML.o:	${imgui_INC} ${SD}imgui-SFML.cpp
	${CC} -c ${SD}imgui-SFML.cpp `pkg-config imgui --cflags` -I./src

clean:
	rm *.o `if [ -f oim  ]; then rm oim; fi`

