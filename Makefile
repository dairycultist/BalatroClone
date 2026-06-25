UNAME := $(shell uname)

ifeq ($(UNAME), Darwin) # macOS
	GCCFLAGS = -lGLEW -framework OpenGL
else ifeq ($(UNAME), Linux)
	GCCFLAGS = -lGLEW -lGL -lm
else
	$(error Unsupported OS: $(UNAME))
endif

build.out: src/*
	@gcc -o build.out src/*.c $(GCCFLAGS) $(shell pkg-config --cflags --libs sdl2 SDL2_image SDL2_ttf)

run: build.out
	@./build.out

clean:
	rm build.out