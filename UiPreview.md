# QCustomUi

[TOC]

## 控件介绍

### 1. 基本窗口

#### QCtmWindow

QCtmWindow 是一个顶层窗口类，默认带有自定义的标题栏，支持在标题栏上添加菜单，显示图标，添加控件，系统右键菜单等，另外支持windows窗口的屏幕边缘拖拽效果：

![image-20220519144218268](UiPreview.assets/image-20220519144218268.png)

![image-20220519145002696](UiPreview.assets/image-20220519145002696.png)

![165294264943820225191524305](UiPreview.assets/165294264943820225191524305.gif)

#### QCtmDialog

与QCtmWindow相似，为Dialog类型窗口的封装，支持windows窗口特效：

![165294336692220225191522254](UiPreview.assets/165294336692220225191522254.gif)

### 2. 按钮

#### QCtmSwitchButton

状态切换按钮

![165294410414120225191518431](UiPreview.assets/165294410414120225191518431.gif)

#### QCtmSelectionButtonBox

选择按钮组合组件，支持多选，单选，单选（可取消选中），水平排布，垂直排布，可以替代radiobox，checkbox

![QQ20231110-163619-HD202311101638541](UiPreview.assets/QQ20231110-163619-HD202311101638541.gif)

### 3. 进度条

#### QCtmStageProgressBar

节点进度条

![165294524356320225191528126](UiPreview.assets/165294524356320225191528126.gif)

#### QCtmCircleProgressBar

圆形进度条

![image-20220519153100472](UiPreview.assets/image-20220519153100472.png)

![165294551010920225191532407](UiPreview.assets/165294551010920225191532407.gif)

### 4. 输入框

#### QCtmInputDialog

输入窗口

与QInputDialog功能相同，为风格统一实现

![image-20220519153539266](UiPreview.assets/image-20220519153539266.png)![image-20220519153556457](UiPreview.assets/image-20220519153556457.png)![image-20220519153609328](UiPreview.assets/image-20220519153609328.png)![image-20220519153617953](UiPreview.assets/image-20220519153617953.png)![image-20220519153626148](UiPreview.assets/image-20220519153626148.png)

#### QCtmDigitKeyboard

输入数字键盘窗口

方便为带单位输入和触摸屏输入

![image-20220519153749963](UiPreview.assets/image-20220519153749963.png)

#### QCtmIPAddressEdit

IP地址输入框

与windows的IP地址输入框类似

![image-20220519153904665](UiPreview.assets/image-20220519153904665.png)

#### QCtmMultiComboBox

可多选的combobox

![image-20220519154416408](UiPreview.assets/image-20220519154416408.png)

#### QCtmPathBrowser

路径浏览器

![image-20220519154531989](UiPreview.assets/image-20220519154531989.png)

![image-20220519154603715](UiPreview.assets/image-20220519154603715.png)

![image-20220519154616413](UiPreview.assets/image-20220519154616413.png)

#### QCtmLongLongSpinBox

#### QCtmULongLongSpinBox

提供64位长整型的数字输入框

![image-20240621150955630](UiPreview.assets/image-20240621150955630.png)

#### QCtmHexLineEdit

提供16进制单行输入功能

![image-20240801154148218](D:\project\QCustomUI\UiPreview.assets\image-20240801154148218.png)

#### QCtmSerialPortComboBox

点击时自动刷新窗口列表的选择框

![image-20240801154356539](D:\project\QCustomUI\UiPreview.assets\image-20240801154356539.png)

### 5.消息窗口

#### QCtmMessageBox

行为与QMessageBox一致，并提供倒计时自动关闭的功能

![image-20220519155120304](UiPreview.assets/image-20220519155120304.png)![image-20220519155131669](UiPreview.assets/image-20220519155131669.png)![image-20220519155138884](UiPreview.assets/image-20220519155138884.png)![image-20220519155150157](UiPreview.assets/image-20220519155150157.png)

#### QCtmMessageTipButton

#### QCtmMessageTipView

消息提示按钮和列表

![image-20220519160009386](UiPreview.assets/image-20220519160009386.png)

### 6.容器部件

#### QCtmToolBox

工具窗口

![image-20220519160512835](UiPreview.assets/image-20220519160512835.png)

#### QCtmDrawerWidget

抽屉窗口

![16529476089682022519167348](UiPreview.assets/16529476089682022519167348.gif)

### 7.加载窗口

#### QCtmLoadingDialog

![165294782053120225191610569](UiPreview.assets/165294782053120225191610569.gif)

### 8. 视图

#### QCtmClassifyTreeView

分类视图

![image-20220519161302386](UiPreview.assets/image-20220519161302386.png)

#### QCtmAbstractMultiPageItemModel

#### QCtmAbstractMultiPageTableModel

#### QCtmMultiPageStringListModel

#### QCtmMultiPageButtonBox

分页视图系列

![16698569751572022121911412](UiPreview.assets/16698569751572022121911412.gif)

#### QCtmRecentView

#### QCtmRecentModel

#### QCtmRecentViewDelegate

最近使用的文件/项目视图

![QQ录屏2023052616574620235261658311](UiPreview.assets/QQ录屏2023052616574620235261658311.gif)

#### QCtmTableViewButtonsDelegate

QCtmTableView的按钮委托

![image-20250620163140158](UiPreview.assets\image-20250620163140158.png)

### 9. 滑动条

#### QCtmRangeSlider

范围滑动条

![image-20241104100818611](UiPreview.assets/image-20241104100818611.png)
