# 项目环境配置
**外部依赖**：
-   Eigen       ：矩阵、向量库
-   rapidjson   ：json文件解析
-   stb         ：图片io

以上三个库都是head only，只需要include即可使用
> 注：stb头文件需要定义对应的宏使用
> #define STB_IMAGE_IMPLEMENTATION
> #include <stb/stb_image.h>
> #define STB_IMAGE_WRITE_IMPLEMENTATION
> #include <stb/stb_image_write.h>

-   assimp      ：模型解析库
这个库的编译过程如下
    1. mkdir build
    2. cd build
    3. cmake .. -DASSIMP_BUILD_ZLIB=ON
    4. cmake --build . --config Release

本项目中链接的库是Release版本（编译时链接extlib\assimp-vc142-mt.lib），运行时将extlib\assimp-vc142-mt.dll拷贝至可执行文件所在目录中
> 注：编译的时候尝试过编译纯静态库，但是还是有问题

-   imgui       ：gui库
该库的下载和编译参考了https://greich.com/projects/ImGui-CMake-Installer/
    1. git clone --recursive https://github.com/giladreich/ImGui-CMake-Installer
    2. cd ImGui-CMake-Installer && mkdir build && cd build
    3. cmake .. -DIMGUI_IMPL_DX11=ON
    4. cmake --build . --config Release --target install

当前项目**目前**中使用了dx11的实现版本，上述指令执行后，dist目录中的include文件即ext/imgui中的文件，生成的imgui.lib放入extlib中，链接时除了imgui库之外需要额外指定d3d11和d3dcompiler
