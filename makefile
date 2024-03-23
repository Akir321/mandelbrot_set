CXX = g++
CXX_FLAGS = -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal\
 -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum\
 -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual\
 -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2\
 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers -Wnon-virtual-dtor\
 -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing\
 -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG\
 -D_EJUDGE_CLIENT_SIDE 

SRC_DIR = source/


all: mandelbrot

mandelbrot:
	$(CXX) $(SRC_DIR)main.cpp $(SRC_DIR)work_with_SDL.cpp $(SRC_DIR)mandelbrot.cpp -o $@ -L/usr/lib -lSDL2

clean:
	rm mandelbrot
