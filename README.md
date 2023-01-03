# QCustomUi

[TOC]

## 一、简介

QCustomUi是一个自定义UI库，主要提供无边框窗口和一些自定义控件，方便Qt开发。
当前库于Qt5.15.2和Qt6下编译通过。

编译完成后，点击安装，会自动安装设计器插件到QtDesigner和QtCreator下。
>注意：如果没有将Qt的运行库设置到PATH环境变量中，QtCreator可能缺乏某些QCustomUi依赖的Qt库而无法加载设计器插件。

## 二、编译

现在QCustomUi默认仅编译QCustomUi本身，若需要编译Examples，则在CMake中将**BUILD_EXAMPLES**选项激活，若要编译Tests，则在CMake中将**BUILD_TESTS**选项激活，若要编译Qt设计器插件，则在CMake中将**BUILD_DESIGNER**选项激活。

## 三、控件介绍

### 1. 基本窗口

#### QCtmWindow

QCtmWindow 是一个顶层窗口类，默认带有自定义的标题栏，支持在标题栏上添加菜单，显示图标，添加控件，系统右键菜单等，另外支持windows窗口的屏幕边缘拖拽效果：

![image-20220519144218268](README.assets/image-20220519144218268.png)

![image-20220519145002696](README.assets/image-20220519145002696.png)

![165294264943820225191524305](README.assets/165294264943820225191524305.gif)

#### QCtmDialog

与QCtmWindow相似，为Dialog类型窗口的封装，支持windows窗口特效：

![165294336692220225191522254](README.assets/165294336692220225191522254.gif)

### 2. 按钮

#### QCtmSwitchButton

状态切换按钮

![165294410414120225191518431](README.assets/165294410414120225191518431.gif)

### 3. 进度条

#### QCtmStageProgressBar

节点进度条

![165294524356320225191528126](README.assets/165294524356320225191528126.gif)

#### QCtmCircleProgressBar

圆形进度条

![image-20220519153100472](README.assets/image-20220519153100472.png)

![165294551010920225191532407](README.assets/165294551010920225191532407.gif)

### 4. 输入框

#### QCtmInputDialog

输入窗口

与QInputDialog功能相同，为风格统一实现

![image-20220519153539266](README.assets/image-20220519153539266.png)![image-20220519153556457](README.assets/image-20220519153556457.png)![image-20220519153609328](README.assets/image-20220519153609328.png)![image-20220519153617953](README.assets/image-20220519153617953.png)![image-20220519153626148](README.assets/image-20220519153626148.png)

#### QCtmDigitKeyboard

输入数字键盘窗口

方便为带单位输入和触摸屏输入

![image-20220519153749963](README.assets/image-20220519153749963.png)

#### QCtmIPAddressEdit

IP地址输入框

与windows的IP地址输入框类似

![image-20220519153904665](README.assets/image-20220519153904665.png)

#### QCtmMultiComboBox

可多选的combobox

![image-20220519154416408](README.assets/image-20220519154416408.png)

#### QCtmPathBrowser

路径浏览器

![image-20220519154531989](README.assets/image-20220519154531989.png)

![image-20220519154603715](README.assets/image-20220519154603715.png)

![image-20220519154616413](README.assets/image-20220519154616413.png)

### 5.消息窗口

#### QCtmMessageBox

行为与QMessageBox一致

![image-20220519155120304](README.assets/image-20220519155120304.png)![image-20220519155131669](README.assets/image-20220519155131669.png)![image-20220519155138884](README.assets/image-20220519155138884.png)![image-20220519155150157](README.assets/image-20220519155150157.png)

#### QCtmMessageTipButton

#### QCtmMessageTipView

消息提示按钮和列表

![image-20220519160009386](README.assets/image-20220519160009386.png)

### 6.容器部件

#### QCtmToolBox

工具窗口

![image-20220519160512835](README.assets/image-20220519160512835.png)

#### QCtmDrawerWidget

抽屉窗口

![16529476089682022519167348](README.assets/16529476089682022519167348.gif)

### 7.加载窗口

#### QCtmLoadingDialog

![165294782053120225191610569](README.assets/165294782053120225191610569.gif)

### 8. 视图

#### QCtmClassifyTreeView

分类视图

![image-20220519161302386](README.assets/image-20220519161302386.png)

#### QCtmAbstractMultiPageItemModel

#### QCtmAbstractMultiPageTableModel

#### QCtmMultiPageStringListModel

#### QCtmMultiPageButtonBox

分页视图系列

![16698569751572022121911412](README.assets/16698569751572022121911412.gif)
