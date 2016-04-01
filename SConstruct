import os

env = Environment(ENV=os.environ, tools=['mingw'],
                  CPPPATH=['C:\\SDL2-2.0.4\\include',
                           'C:\\SDL2_ttf-2.0.14\\i686-w64-mingw32\\include\\SDL2'],
                  CCFLAGS=["-Dmain=SDL_main","-ggdb"])
env.Program(Glob('*.c'),
            LIBPATH=['C:\\SDL2-2.0.4\\lib\\x86',
                     'C:\\SDL2_ttf-2.0.14\\i686-w64-mingw32\\lib'],
            LIBS=['SDL2Main','SDL2','mingw32','SDL2_ttf'])
