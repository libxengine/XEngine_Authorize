# XEngine_Authorize

## 介绍
c c++网络验证服务器 网络授权服务器  
c c++network Authorize service  
这是网络授权验证服务器的代码库,这个服务主要提供网络授权验证功能.支持本地和网络两种验证模式  
这是一个跨平台简单高性能网络授权服务器,提供了完整的演示代码  

## 软件特性
基于libXEngine开发并实现的一套简洁高性能跨平台网络存储服务  
本仓库有开发和主分支,如果要使用,请使用master分支下的代码  
软件特性:  
1. 采用标准私有协议来处理网络通信
2. 支持加密通信
3. 支持用户管理
4. 支持序列卡管理
5. 支持日志管理
6. 支持配置管理
7. 序列卡多种类型支持(分钟卡,天数卡,次数卡,自定义卡)
8. 支持试用,支持快速验证
9. 支持通过邮件找回密码
10. 支持本地CDKEY注册验证

## 安装教程

#### 版本需求
支持WINDOWS 7SP1和LINUX(UBUNT20.04,CENTOS8)以上系统  
XEngine版本需要V7.17或者以上版本    

#### Windows
直接运行即可,使用XEngine_AuthorizeApp


#### Linux
在控制台运行,使用XEngine_AuthorizeService

## 编译

#### Windows
使用VS2019 x86(debug release)打开并且编译 你需要按照下面的方式配置环境,不然你可能需要自己在项目中设置库目录

##### XEngine环境
XEngine可以直接下载,下载完毕后添加用户环境变量,需要下面两个  
- XEngine_Include 头文件目录地址
- XEngine_Library 库文件目录地址

#### Linux
Linux使用Makefile编译  
UBUNTU20.04 x64或者CENTOS8 x64均可  

##### XEngine环境
XEngine可以通过脚本文件安装sudo XEngine_RunEnv.sh -i 3

##### 编译命令
在XEngine_Source目录下执行命令  
make 编译  
make FLAGS=InstallAll 安装库程序  
make FLAGS=CleanAll 清理编译  

## 程序截图
![输入图片说明](https://www.xyry.org/authorize/1.png "在这里输入图片标题")
![输入图片说明](https://www.xyry.org/authorize/2.png "在这里输入图片标题")
![输入图片说明](https://www.xyry.org/authorize/3.png "在这里输入图片标题")
![输入图片说明](https://www.xyry.org/authorize/4.png "在这里输入图片标题")

## 参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request 

## 提交问题

如果你有问题,可以在issues中提交