#=====================================================#
#--------------#
EXE_NAME = UltraSandbox
#--------------#
#=====================================================#
#--------------#
INCLUDE_DIRS = -I"C:\libs\sfml\include" -I"C:\libs\box2d\include" -I"C:\libs\tweeny\include" -I"include"
#~~~~~~#
LIB_DIRS = -L"C:\libs\sfml\lib" -L"C:\libs\box2d\lib"
#~~~~~~#
LIBS = -lsfml-audio-s -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lfreetype -lwinmm -lgdi32 -lopenal32 -lflac -lvorbisenc -lvorbisfile -lvorbis -logg -lbox2d
#~~~~~~#
DebugLIBS = -lsfml-audio-s-d -lsfml-graphics-s-d -lsfml-window-s-d -lsfml-system-s-d -lopengl32 -lfreetype -lwinmm -lgdi32 -lopenal32 -lflac -lvorbisenc -lvorbisfile -lvorbis -logg -lbox2d-d
#--------------#
#=====================================================#
#--------------#
RES_FILE = res/gcc/RC.res
SRCS = $(wildcard src/*.cpp) $(wildcard src/states/*.cpp) $(wildcard src/sw/*.cpp)
OBJS = $(patsubst src/%.cpp, src/obj/%.o, $(wildcard src/*.cpp)) $(patsubst src/%.cpp, src/obj/%.o, $(wildcard src/states/*.cpp)) $(patsubst src/%.cpp, src/obj/%.o, $(wildcard src/sw/*.cpp))
COMPILE_FLAGS =
#--------------#
#=====================================================#
#--------------#
all: d
#~~~~~~#
r: link_rel
	.\$(EXE_NAME).exe
#~~~~~~#
d: link_dbg
	.\$(EXE_NAME).exe
#--------------#
src/obj/%.o: src/%.cpp
	g++ -c $< -o $@ $(INCLUDE_DIRS) -DSFML_STATIC $(COMPILE_FLAGS)
#~~~~~~#
src/obj/%.o: src/states/%.cpp
	g++ -c $< -o $@ $(INCLUDE_DIRS) -DSFML_STATIC $(COMPILE_FLAGS)
#--------------#
link_rel: COMPILE_FLAGS = -O3
link_rel: $(OBJS) $(RES_FILE)
	g++ $^ -o $(EXE_NAME) $(LIB_DIRS) $(LIBS) -mwindows -O3
#~~~~~~#
link_dbg: COMPILE_FLAGS = -Og
link_dbg : $(OBJS) $(RES_FILE)
	g++ $^ -o $(EXE_NAME) $(LIB_DIRS) $(DebugLIBS)
#--------------#
$(RES_FILE): res/gcc/RC.rc
	windres res/gcc/RC.rc -O coff -o res/gcc/RC.res
#--------------#
#=====================================================#
#--------------#
clean : cleanO cleanE
#~~~~~~#
cleanO: cleanS
	rm -f src/obj/*.o
#~~~~~~#
cleanS:
	rm -f src/obj/states/*.o
#~~~~~~#
cleanE:
	rm -f ./*.exe
#--------------#
#=====================================================#