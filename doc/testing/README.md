# 测试模块使用方法

## 添加一个测试模块

### step 1 新建测试单元

在`test`目录下新建一个目录，假设为`test-env`。该名称表示将要测试的内容。

### step 2 编写cmake

使用`AddTestModule`函数设置可执行文件。

```cmake
set(TEST_NAME xx)
AddTestModule(
    ModuleName ${TEST_NAME} # 测试单元名称
    Src xx.cpp yy.cpp zz.cpp # 可执行文件
)
```

编译出的可执行文件名为`${PROJECT_NAME}_${TEST_NAME}.exe`，在`test\target`目录下

### step 3 添加该测试单元

1. 在主目录的`CMakeLists.txt`中，`Testing()`函数中依次添加`${TEST_NAME}`。

```cmake
Testing(
    example-test1
    example-test2
    test-env
)
```

2. 在`test`目录下的`CMakeLists.txt`中添加：

   ```cmake
   add_subdirectory(test-env) # 与测试单元的目录名一致
   ```

### step 4

在主目录下运行`RunTesting.bat`可查看测试结果。

## 选择某些模块进行测试

直接修改主目录下的`CMakeLists.txt`

```cmake
Testing(
    # example-test1
    example-test2
    # test-env
)
```

`Testing()`中保留希望测试的模块名即可。

## 具体测试问题查看

### 方法一

```bash
.\>cd build
.\build>ctest -C DEBUG -V
```

### 方法二（推荐）

直接进入`test\target`目录下运行对应的`exe`文件。

