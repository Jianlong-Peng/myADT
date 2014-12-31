@if NOT EXIST build mkdir build
@cd build
@if EXIST CMakeCache.txt del CMakeCache.txt
@rem cmake .. -DCMAKE_INSTALL_PREFIX=D:\myscript\template.void -DDO_CHECK=ON 
cmake .. -DCMAKE_INSTALL_PREFIX=D:\clibrary\template.void -DBUILD_SHARED=OFF
@cd ..
