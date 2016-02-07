import os

env = Environment(ENV=os.environ, tools=['mingw'], CPPPATH="C:\\SDL2-2.0.4\\include", CCFLAGS="-Dmain=SDL_main")
env.Program('cvinvaders.c', LIBPATH="C:\\SDL2-2.0.4\\lib\\x86", LIBS=['SDL2Main','SDL2', 'mingw32'])
