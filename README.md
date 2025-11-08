# Usage
This project can be run in Visual Studio, Visual Studio Code or any editor that supports C++ and cmake  
Simply open folder in that editor. Project is configured to auto-compile on any changes in Visual Studio  
If you wish to compile by hand -> open terminal -> ```cmake --build``` -> output is in out folder

# Adding libraries (vcpkg)
+ Add library to vcpkg.json -> check here if library exists, and its name -> [Link](https://vcpkg.io/en/)
+ Update [CMakeLists.txt](CMakeLists.txt)
	- Add dependency (#Add dependecy in file) -> find_package(blablabla)
	- Target link libraries (#Target link libraries in file) -> Add BLA::BLA
	- Where to search what should you add -> [Vcpkg github](https://github.com/microsoft/vcpkg) -> Use "Go to file", type your library and find "usage" file

# Adding libraries (libs folder)
+ Put library folder into libs folder
+ In [CMakeLists.txt](CMakeLists.txt)
	- find ```target_link_directories``` and add here your directory with library (example should be here)
	- Target link libraries (#Target link libraries in file) -> Add BLA::BLA (You have to check what you have to add here, but i cannot help you with custom libraries where to look)
