[中文](README.md) ||  [English](README.en.md)  
# XEngine_Authorize
本仓库有开发和主分支,如果要使用,请使用master分支下的代码  

## 介绍
c c++网络验证服务器 网络授权服务器  
c c++network Authorize service  
这是网络授权验证服务器的代码库,这个服务主要提供网络授权验证功能.支持本地和网络两种验证模式  
网络验证支持TCP和WEBSOCKET协议验证,也就是说,支持APP程序和WEB网页端  
这是一个跨平台简单高性能网络授权服务器,提供了完整的演示代码  

## 软件特性
基于XEngine开发并实现的一套简洁高性能跨平台网络验证授权服务器  
软件特性:  
1. 采用标准私协议来处理网络通信
2. 支持加密通信
3. 支持用户管理
4. 支持序列卡管理
5. 支持日志管理
6. 支持配置管理
7. 序列卡多种类型支持(分钟卡,天数卡,次数卡,自定义卡)
8. 支持试用,支持快速验证
9. 支持通过邮件找回密码
10. 支持本地CDKEY注册验证
11. 支持APP程序和HTML网页授权验证
12. 支持权限控制

## 安装教程

#### 版本需求
支持WINDOWS 7SP1和LINUX(UBUNT20.04,CENTOS8)以上系统  
必须安装XEngine,版本需要V7.28或者以上版本    

#### Windows
直接运行即可,使用XEngine_AuthorizeApp

#### Linux
在控制台运行,使用XEngine_AuthorizeService

## 编译
安装XEngine可以参考其Readme文档  
GITEE:https://gitee.com/xyry/libxengine  
GITHUB:https://github.com/libxengine/xengine

#### Windows
使用VS打开并且编译  

#### Linux
Linux使用Makefile编译  
UBUNTU20.04 x64或者CENTOS8 x64均可  

##### 编译命令
在XEngine_Source目录下执行命令  
make 编译  
make FLAGS=InstallAll 安装库程序  
make FLAGS=CleanAll 清理编译  

## 测试服务器
地址:app.xyry.org 或者 159.75.200.173  
端口:tcp端口 5300,websocket端口 5301,传输验证密码:无加密

## 程序截图
![输入图片说明](https://www.xyry.org/authorize/1.png "在这里输入图片标题")
![输入图片说明](https://www.xyry.org/authorize/2.png "在这里输入图片标题")
![输入图片说明](https://www.xyry.org/authorize/3.png "在这里输入图片标题")
![输入图片说明](https://www.xyry.org/authorize/4.png "在这里输入图片标题")
![输入图片说明](https://www.xyry.org/authorize/5.png "在这里输入图片标题")

## 参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request 

## 授权说明
使用此软件必须开源并且注明出处,如果想闭源商用,那么需要经过作者授权.

## 关注我们
如果你觉得这个软件对你有帮助,请你给我们一个START吧

## 提交问题

如果你有问题,可以在issues中提交

## 开发计划 
生成会话TOKEN分布式鉴权  
mysql支持  
支持管理接口，用于网页版本  
支持第三方用户密码验证  
支持用户和硬件绑定同时验证  
支持自定义序列号插入导入  
统计用户所有在线时间  