CXX = g++
CXX_FLAGS = -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal\
 -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum\
 -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual\
 -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2\
 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers -Wnon-virtual-dtor\
 -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing\
 -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG\
 -D_EJUDGE_CLIENT_SIDE

VERSION = #-D TIME_MEASURE -D BASELINE


SRC_DIR = source/

OBJ_DIR = object/

OBJECTS = $(OBJ_DIR)main.o          \
          $(OBJ_DIR)work_with_SDL.o \
		  $(OBJ_DIR)mandelbrot.o    \
		  #$(OBJ_DIR)html_logfile.o

INCLUDE = $(SRC_DIR)mandelbrot.h

SIMD    = $(OBJ_DIR)set_calc_simd.o

NAIVE   = $(OBJ_DIR)set_calc_naive.o

EXEC    = mandelbrot_naive \
          mandelbrot_simd 

		  
all: mandelbrot_naive


mandelbrot_simd: $(OBJECTS) $(SIMD)
	$(CXX)       $(OBJECTS) $(SIMD)   -o $@ -L/usr/lib -lSDL2

mandelbrot_naive: $(OBJECTS) $(NAIVE)
	$(CXX)        $(OBJECTS) $(NAIVE) -o $@ -L/usr/lib -lSDL2



$(OBJ_DIR)set_calc_simd.o:  $(SRC_DIR)set_calc_simd.cpp  $(INCLUDE)
	$(CXX) -O3 -c $< -o $@ $(CXX_FLAGS) $(VERSION) -mavx2 

$(OBJ_DIR)set_calc_naive.o: $(SRC_DIR)set_calc_naive.cpp $(INCLUDE)
	$(CXX) -O3 -c $< -o $@ $(CXX_FLAGS) $(VERSION)

$(OBJ_DIR)main.o:           $(SRC_DIR)main.cpp           $(INCLUDE)
	$(CXX) -c $< -o $@ $(CXX_FLAGS)

$(OBJ_DIR)work_with_SDL.o:  $(SRC_DIR)work_with_SDL.cpp  $(SRC_DIR)work_with_SDL.h
	$(CXX) -c $< -o $@ $(CXX_FLAGS)

$(OBJ_DIR)mandelbrot.o:     $(SRC_DIR)mandelbrot.cpp     $(INCLUDE)
	$(CXX) -c $< -o $@ $(CXX_FLAGS)      $(VERSION)

$(OBJ_DIR)html_logfile.o:   $(SRC_DIR)html_logfile.cpp   $(SRC_DIR)html_logfile.h
	$(CXX) -c $< -o $@ $(CXX_FLAGS)

clean:
	rm $(EXEC) $(OBJ_DIR)*.o
