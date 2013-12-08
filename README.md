lc3ide: The cross platform LC-3 Integrated Development Environment.
-------------
**WARNING:** The current build is broken and won't compile under Qt Creator.  (There are several sub-projects being developed independently of the Qt project.)
-------------
At the moment, only a Linux and Qt 5 target are supported.  The code is rough right now.  This program is still in the conceptualization stage.

To compile, you need Qt 5, which can be obtained from http://qt-project.org/downloads.  Once installed, open the "lc3sim_noqml.pro" file from QtCreator.  The full path to QtCreator should be something like "/opt/qt5/bin/qtcreator" if you choose to install it to the default directory.  Then, hopefully, you can compile the project.  The program will not run, however, until you adapt the build directories from within Qt Creator.

Setup:
* Clone the directory into your chosen directory:

        git clone https://github.com/cbh2000/lc3ide.git

* Enter the lc3ide subdirectory, which was created by Git:

        cd lc3ide

* Tell Git to ignore any changes Qt Creator will make to the .pro.user files:

         git update-index --assume-unchanged ./*.pro.user ./*/*.pro.user

* Open the projects in Qt Creator:

         qtcreator ./*.pro ./*/*.pro

* Tell Qt Creator that you wish to import the .user settings files for each project.  (These files contain custom build instructions and are required to build the projects.)
* From Qt Creator's menu, select Build → Build All.
* From Qt Creator's dark sidebar on the right, click the computer icon and select "lc3sim_noqml".
* Click on the green arrow just beneath the computer icon to run the project (or simply press Ctrl+R or from the menu, Build → Run).

Credits
-------------
Lumetta's simulator is (C) 2003, Steve Lumetta, and can be obtained from either this repository or http://highered.mcgraw-hill.com/sites/dl/free/0072467509/104652/lc3tools_v12.zip.  I would recommend using the version in this repository, since it contains bug fixes.

Once the program's UI stabilizes, lc3assembler will replace the functionality Lumetta's program currently provides.  I have chosen to rewrite Lumetta's program instead of adapting it because his source is not cross-platform, uses Fortran, and may need an extensive update should the LC-# specification change in the future.

lc3assembler and lc3ide are MIT licensed, though I have neglected to prefix the notice at the beginning of each file I created.  lc3tools is GPL licensed.  I chose to use the MIT over the GPL because the GPL is incompatible with the iOS App Store terms (http://michelf.ca/blog/2011/gpl-ios-app-store/).  This sucks because to add support for iOS, I must completely rewrite two projects: Lumetta's LC-3 tools and the C compiler.  I have created issues for this, so check them out for my solutions.

If you have questions, I am available via email at cbh2000@gmail.com.
