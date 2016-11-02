# 2DCharacterAnimMaker
2D character animation based on standing foot to avoid sliping.
This is the source project of paper [基于分层图像的建模与漫游] ({{site.url}}/Resources/2007LayerImage.pdf) in Chinese.
You can also find the information of this paper in the `publications` page [here](https://hanhonglei.github.io/publications/)
- More information, please feel free to visit my personal website [here](https://hanhonglei.github.io/).
**本程序实现的功能是** 利用用户交互来得到比较真实的动画。
**本程序所使用的技术有** spline曲线拟合等。
本软件使用了Win32控制台程序结构，使用命令行来传递程序参数。
需要首先设定2个程序运行参数：第一是所要调入`Gif`图像的文件名（如果不再当前文件夹下，需要指定详细路径）；第二是所调入角色运动的朝向（`r`表示向右，`l`表示向左）。
比如，用户需要处理名为brachio的gif角色动画，则需要在`bat`文件中输入必要参数如下：
```javascript
AnmTest.exe brachio.gif r
```
