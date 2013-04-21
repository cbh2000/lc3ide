lc3ide: The cross platform LC-3 Integrated Development Environment.

At the moment, only a Linux and Qt 5 target are supported.  All paths are hard-coded and the code is VERY rough right now.  This program is still in the conceptualization stage.

To compile, you need Qt 5, which can be obtained from http://qt-project.org/downloads.  Once installed, open the "lc3sim_noqml.pro" file from QtCreator.  The full path to QtCreator should be like "/opt/qt5/bin/qtcreator" if you choose to install it to the default directory.  Then, hopefully, you can compile the project.  The program will not run, however, until you adapt the hard-coded paths.

Since all paths are hard-coded and the actual assembling of .asm files is being passed on to the the Unix LC-3 simulator created by Steve Lumetta, (C) 2003, which can be obtained from http://highered.mcgraw-hill.com/sites/dl/free/0072467509/104652/lc3tools_v12.zip.  All code paths that point to "lc3as" or "lc3sim" inside of "widget.cpp" in this project will need to be redirected to wherever you choose to extract your simulator.  I will not provide instructions on how to compile Lumetta's simulator, because there are instructions provided in the download.  (Please note, however, that a label cannot be immediately followed by a ";"--I will publish a fix for that later.)

Then, the path to the temporary file in widget.cpp ("/home/bryan/.lc3sim.asm") will need to be redirected to a folder you have access to, unless your username happens to be "bryan" already!  ;-)

Once the program's UI stabilizes, a new C library will be written to replace the functionality Lumetta's program currently provides.  I have chosen to rewrite the program instead of adapting it because his source is not cross-platform, uses Fortran, and may need an extensive update should the LC-# specification change in the future.

Until Lumetta's program is replaced it will be used to do the heavy lifting, including assembling and simulating the LC-3 source code and .obj files.

Enjoy!  If you have questions, don't be afraid to email me.  I'm a nice person, really!
