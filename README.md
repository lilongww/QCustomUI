# QCustomUi

## 一、简介

QCustomUi是一个自定义UI库，主要提供无边框窗口和一些自定义控件，方便Qt开发。
当前库于Qt5.15.2和Qt6下编译通过。

编译完成后，点击安装，会自动安装设计器插件到QtDesigner和QtCreator下。

>注意：如果没有将Qt的运行库设置到PATH环境变量中，QtCreator可能缺乏某些QCustomUi依赖的Qt库而无法加载设计器插件。

## 二、编译

现在QCustomUi默认仅编译QCustomUi本身，若需要编译Examples，则在CMake中将**BUILD_EXAMPLES**选项激活，若要编译Tests，则在CMake中将**BUILD_TESTS**选项激活，若要编译Qt设计器插件，则在CMake中将**BUILD_DESIGNER**选项激活。

现在QCustomUi引用了第三方库**pegtl**, 在CMake生成阶段将自动从指定地址下载，**下载过程可能需要一点时间，请耐心等待**由于国内对github访问速度缓慢的问题，在CMake中默认是从gitee的镜像仓库下载的，若需要从原地址下载，请将**src/QCustomUi/CMakeLists.txt**中下列内容修改为注释掉的github地址：

```cmake
FetchContent_Declare(
  pegtl
  #GIT_REPOSITORY https://github.com/taocpp/PEGTL.git #github 原仓库地址
  GIT_REPOSITORY https://gitee.com/llongww/PEGTL.git #gitee镜像地址
  GIT_TAG        main
  FIND_PACKAGE_ARGS NAMES pegtl
)
```

当然，下载**pegtl**是在当前环境中并没有pegtl库的情况下，如果你已经下载了pegtl库，并且引入其**pegtl-config.cmake**到CMAKE_PREFIX_PATH中，或者使用其它库管理工具，如vcpkg等，也可以在vcpkg中安装pegtl库，这样在编译过程中会自动找到pegtl，而不会执行下载过程。

## 三、UI界面预览

界面预览请查看[界面预览](UiPreview.md)页面