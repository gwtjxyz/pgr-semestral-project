# pgr-semestral-project
Repo for my semestral project for my university's OpenGL graphics course. I tried to code in as much of a "C-style" 
as possible - meaning I avoided unnecessary classes or STL usage as much as I could, which is not to say that I completely succeeded of course.
If I had a bit more time then I'd cut down on what little of it I do use for sure (and maybe I will at some point in the future).
I also tried to **not** make a game engine, as my philosophy generally speaking is to just make whatever is the most obvious thing to make
before the need for something more complex arises. I figured I wouldn't need an entity component system, crazy inheritance or model hot-loading
to make a simple test scene for an OpenGL class. Once again, I know my code isn't ideal, but at the very least it's functional 
and doesn't do too many unnecessary things (I hope so anyway).

The project uses OpenGL as the rendering API, GLFW as its windowing system, GLAD as the extension loader/manager, 
Assimp (badly) for loading in models and Nanogui for managing user interface. Most of the codebase was developed on Linux.

# Building and running
Clone with `--recursive`, build with CMake. The project uses GLAD, you should be able to build and run without any modifications on both
Windows and Linux, as I just forced GLAD's usage regardless of the platform. No clue about Mac as I don't have one. Whatever you do,
DON'T run this with Visual Studio, as you'll need either to do some magic with its configuration settings to make it work with relative paths,
or to Ctrl+F and change every single relative path I use in the code.

If you're building on Windows you might run into issues with Assimp missing some DLLs, there's probably come CMake setting I'm unaware of to 
fix the issue (this is my first project using CMake!), the workaround is to manually copy the generated libraries from Assimp's build folders
and into the directory with the resulting executable and everything should work just fine.
