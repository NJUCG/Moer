# Zero
Prototype renderer

## 进行环境测试
cmake version >= 3.12
-   mkdir build
-   cd build
-   cmake ..
-   cmake --build .

将extlib中的assimp-vc142-mt.dll拷贝至可执行文件app.exe所在目录中（最好拷贝至常用动态库所在目录，我测试时拷到了C:\Windows\System32），然后执行app.exe