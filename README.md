# Zero
Prototype renderer

## 进行环境测试
cmake version >= 3.12
-   mkdir build
-   cd build
-   cmake ..
-   cmake --build .

将extlib中的assimp-vc142-mt.dll拷贝至可执行文件app.exe所在目录中（最好拷贝至常用动态库所在目录，我测试时拷到了C:\Windows\System32），然后执行app.exe

>app.exe 会生成到 target 文件夹下，运行 RunDebug.bat，可直接拷贝 assimp-vc142-mt.dll 到 target/Debug/ 下。Release 版本需要修改一下脚本里的路径。

