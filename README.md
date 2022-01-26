# QCustomUi

QCustomUi是一个自定义UI库，主要提供无边框窗口和一些自定义控件，方便Qt开发。
当前库于Qt5.15.2和Qt6下编译通过。

编译完成后，点击安装，会自动安装设计器插件到QtDesigner和QtCreator下。
>注意：如果没有将Qt的运行库设置到PATH环境变量中，QtCreator可能缺乏某些QCustomUi依赖的Qt库而无法加载设计器插件。

文档：https://llongww.gitee.io/qcustomuidocument

**[1.8.0]**
```
1.添加QCtmPathBrowser；
2.添加QCtmDigitKeyboard；
3.优化部分代码，优化文档。
```

**[1.7.0]**
```
1.QCtmNavigationBar添加ActionPosition::Center选项，可以将Action添加到中央位置；
2.优化dialog最大化，关闭，打开，恢复正常大小时，大小依然为最大化大小的问题；
3.重新实现QCtmLoadingDialog，优化流程较短时，loading窗口法起到阻塞效果的问题；
4.MessageTip优化对触摸屏的显示，添加Clear All按钮，添加删除/清空消息的确认API, 优化背景色；
5.优化QCtmToolBox样式，添加QCtmToolBox示例；
6.bugfix：解决添加到QCtnNavigationBar的QWidgetAction为不显示时，添加后依然显示的问题；
7.bugfix：解决dialog左右停靠时，关闭再打开失去交互的问题；
8.bugfix：带native子窗口的QCtmWindow，点击任务栏图标显示/隐藏窗口时，显示异常；
9.bugfix：native子窗口hover事件失效；
10.bugfix：QCtmNavigationSidePane Left停靠时，阴影显示异常；
11.bugfix：QCtmMessageTipButton 清空消息时未刷新；
12.bugfix：QCtmInputDialog 垂直方向放大时，标题栏位移，现在使其与QInputDialog放大行为一致；
13.bugfix：解决无边框窗口在双屏幕下的显示/拖拽问题。
```

**[1.6.1]**

```
1.bugfix：QCtmDialog默认显示位置不正确；
2.bugfix：解决窗口被ALT+F4快捷键关闭后，再次打开显示不正常的问题。
```



**[1.6.0]**

```
1.QCtmMessageTipModel默认最新消息显示在前面;
2.新增QCtmInputDialog，该弹窗继承于QInputDialog，实现了QCustomUi的窗口风格；
3.删除冗余样式表；
4.解决windows下的无边框窗口，在子窗口具备句柄的情况下无法拖拽的问题；
5.QCtmApplication添加是否加载默认样式表的选项；
6.解决QCtmDialog最大化后关闭，再次打开时窗口状态异常的问题；
7.修改QCtmDialog的默认行为，不再默认具备最大化和最小化按钮；
8.优化QCtmNavigationBar的Logo图标自适应；
9.适配Qt6。
```
**[1.5.2]**
```
1.增加部分控件的iconSize属性和接口;
2.修复字体改变后，QCtmMessageTipButton显示的数字不全的问题；
3.修复QCtmIPAddressEdit字体大小设置无效的问题；
4.修复QCtmTitleBar字体调整时，标题文字部分被覆盖的问题；
```
**[1.5.1]**
```
1.解决IP地址输入框在触摸屏下无法用软键盘输入的问题；
2.添加QCtmComboBox下拉框弹出前的信号；
3.优化CMake文件；
4.解决MessageTip系列控件样式bug；
5.解决QCtmNavigationSidePane在窗口最大化时，位置大小计算错误问题；
6.添加MessageTipExample。
```

**[1.5.0]**
```
1.添加部分常用控件的设计器插件，可以在设计器中直接拖拽和编辑这些控件，而不必使用“提升为”，提升开发效率；
2.解决QCtmStageProgressBar最大值最小值和当前值可设置为不正确的范围的问题。
```

**[1.4.1]**
```
1.解决QCtmMultiComboBox在disabled状态下依旧能够点击弹出下拉框的问题。
```

**[1.4.0]**
```
1.添加QCtmFile工具类，提供文件读写便捷函数.
```

**[1.3.1]**
```
1.去掉样式表中对于输入框等类型控件的高度限制，让其可以根据字体大小调整；
2.将QSpinBox左右箭头恢复为上下箭头，因为QSpinBox的大小调整后，左右箭头的高度在样式表中难以控制;
3.安装时不再复制resource.h.
```
**[1.3.0]**
```
1.变更工程配置为cmake.
```

**[1.2.1]**
```
1.用户图标无法隐藏bugfix.
```

**[1.2.0]**
```
1.添加QCtmLoadingDialog。
```

**[1.1.3]**
```
1.QCtmStageProgressBar bugfix：设置文字隐藏无效，
  文字隐藏后水平的进度条最小高度不正确，垂直的进度条显示异常。
```
**[1.1.2]**
```
1.添加QCtmClassifyTreeView，显示类似于设置了
  IconMode的QListView，但添加了类似树形结构的分组功能。
```