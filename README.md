# 使用说明

###程序说明
###滤波`smooth.cpp`
共有三种滤波方式，算术、几何、谐波均值滤波 </br>
####入口
- 直接双击程序，对文件夹下`template.jpg`进行处理//在经椒盐噪声后，一次用三种方法滤波
- 在cmd中运行，格式为

    > sztx.exe [img --default template.jpg] [option] </br>
  
  其中`option`选项含义如下:

    -a : 或-arithmetric 算术平均值滤波
    -g : 或-geometry 几何均值滤波
    -h : 或-harmonic 谐波均值滤波
    -e : 或-example 示例程序，三种滤波效果对比

####出口
- 算术平均滤波，由于运算速度较快，选择按任意键开始每隔对滤波kernal放大进行再次滤波，按空格键暂停，`esc`	退出
- 几何均值滤波和谐波均值滤波，均有一个关于kernal的track bar用于调整
- 示例程序的输出结果：</br>
在原图片分别被，椒盐，盐，椒噪声干扰后进行滤波后的结果
![smooth](http://i.imgur.com/4u7zHYI.jpg)

###小波变换`dwt.cpp`
####入口：
- 直接双击对文件下的`template.jpg`进行一次小波变换
- 在cmd中运行，格式为
	>sztx.exe [img --default template.jpg] [times]</br>
	//即对某图片进行多次小波变换

####出口
对样本图片进行两次小波变换结果：
![dwt](final_result/dwt.png)
###低通滤波器	`lpf.cpp`
####入口:
- 直接双击会显示`help`函数，声明该程序如何使用
- 在cmd中运行，其格式为
	>sztx.exe [option][img --default template,jpg]

######option共有如下选项：

	--e : 或--example，示例程序，当没有图片参数时，默认调用template.jpg
 	--i : 或--ideal low pass filter 理想低通滤波器
 	--g : 或--gaussian LPF 高斯低通滤波器
 	--b : 或--butterworth LPF 巴特沃斯低通滤波器
#####理想低通滤波器
程序要求输入一个参数，其值建议 < `min(src.rows,src.cols)/2`
#####高斯低通滤波器
程序要求输入两个参数，分别为横向与纵向的高斯sigma参数，此参数越小
模糊效果月明显
#####巴特沃斯低通滤波器
程序要求输入两个参数，分别为与截止频率有关的半径，以及阶数
#####示例程序
示例程序默认对`template.jpg`进行处理,</br>也可在结尾输入想要处理的图片位置
</br>处理结果放置到一副图片中，分别为

|b分量|被理想滤波后的频域下图片|被理想低通滤波器滤波后的图像|
|:--|:--|:--|
|g分量|被高斯低通滤波后频域下图片|高斯滤波后的空间域图像|
|r分量|被巴特沃斯滤波后的频域下图片|巴特沃斯滤波后的空间域图像|
####出口：
####示例程序结果范例：
![低通滤波范例](http://i.imgur.com/uVE4lKT.jpg)
</br>
</br>


###简单图像处理，二值化，直方图等功能	`sztx.cpp`
####入口：

- 直接双击，默认调用文件夹下的`template.jpg	`作为源图像
- 在cmd中运行，添加一个图片作为源图像如 

	> sztx.exe template2.jpg </br>
	> sztx.exe d:\tmp\2.jpg
	
####出口:

- 将源图片保存为bmp格式，输出为`dst.bmp	`
- 生成一副图片包含9个部分，依次为：

|R分量|源图片|二值图片|
|:--|:--|:--|
|G分量|灰度图|直方图均衡化后图片|
|B分量|灰度图直方图|直方图均衡化后直方图|

####结果范例：
![结果样例](http://i.imgur.com/76Fq5mW.jpg)


