all: build

ifeq ($(OS),Windows_NT)
build:
	@echo "Building on windows:"
	windres resources/installer.rc -O coff -o resources/installer.o
	g++ -static -o dependency_installer.exe dependency_installer.cpp resources/installer.o -lmingw32 -lgdi32
	windres resources/icons.rc -O coff -o resources/icons.o
	i686-w64-mingw32-g++ -static -o customcloud.exe customcloud.cpp resources/icons.o -I src/include src/include/CustomCPP/customCPP.cpp -lmingw32 -lgdi32
else ifeq ($(shell uname),Linux)
build:
	@echo "Building on Linux:"
	g++ -o app main.cpp -I ./ -I /usr/include/freetype2/ CustomCPP/customCPP.cpp CustomSDL/customSDL.cpp -lSDL2 -lfreetype
endif

clean:
	rm -f app
