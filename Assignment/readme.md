Use Visual Studio and not VSCode. I used Visual Studio 2022. In the CMakeLists.txt file, in add-executable line, type :-

add_executable (CMakeProject1 "/Assignment/tripView.c", "/Assignment/WGraph.c", "/Assignment/WGraph.h", "/Assignment/Queue.c", "/Assignment/Queue.h")

Donot mention the tripViewscratch file. That is just for detailed learning.
Ofcourse, the Assignment folder must be inside the CMakeProject1 folder. The double quotes is necessary. What we are doing above is linking all these C files and Header files to Build an object file which is executable.

Also, the order of the files above indicates the order in which the above files are linked. I didn't think in my program that the order mattered. 

First after writing code in all these files, compile it. Then type the above line in CMakeLists.txt file, and then run "Build" command, which will be in the Build menu.

Then you can Run or "Play" the object file created. 

So, basically, for the assignment, I used Weighted Graph and Queue. Its not perfect, but through this assignment, I learned C.
