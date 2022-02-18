[中文](README.md) ||  [English](README.en.md)  
# XEngine_Authorize
This repository has a development and master branch. If you want to use it, please use the master branch  

## Introduction
c c++网络验证服务器 网络授权服务器  
c c++network Authorize service  
the server is network authorize verification service code library,This service mainly provides network authorization verification function.support local and network verification mode  
network verification support tcp and websocket protocol verification.it is means:support app program and web  
This is a cross-platform simple high-performance network authorization server that provides a complete demo code  

## Software feature
The purpose of development and implementation based on XEngine is a cross-platform network storage service  
feature list:  
1. use standard private protocol to handle network communication
2. Support Encrypto communication
3. Support User Management
4. Support Serial Management
5. Support Log Management
6. Support Configure Management
7. Support Serical Multie types(Minute card, day card, frequency card, custom card)
8. Support Try,Support quick verification
9. Support Retrieve password through email
10. Support Verification for Local CDKey
11. Support app program and html web Authorize

## install

#### Requirements
support system above windows 7sp1 and linux(ubuntu20.04,centos8)  
you must install XEngine,need V7.28 or above  

#### Windows
Just Run it.,use XEngine_AuthorizeApp

#### Linux
Run it on the terminal,use XEngine_AuthorizeService

## complie
install XEngine can be refer to xengine Readme docment  
GITEE:https://gitee.com/xyry/libxengine  
GITHUB:https://github.com/libxengine/xengine

#### Windows
use vs open and complie  
Please use git tool to download the code, otherwise it may not compile

#### Linux
Linux use Makefile complie  
UBUNTU20.04 x64 or CENTOS8 x64  

##### Screenshot of the program
execute command in XEngine_Source path   
make complie  
make FLAGS=InstallAll Install File  
make FLAGS=CleanAll Clear Complie  

## test server
address:app.xyry.org or 159.75.200.173  
port :tcp port 5300,websocket port 5301,Transmission verification password: no encryption

## Screenshot of the program
![输入图片说明](https://www.xyry.org/authorize/1.png "在这里输入图片标题")
![输入图片说明](https://www.xyry.org/authorize/2.png "在这里输入图片标题")
![输入图片说明](https://www.xyry.org/authorize/3.png "在这里输入图片标题")
![输入图片说明](https://www.xyry.org/authorize/4.png "在这里输入图片标题")

## Participate in contribution

1.  Fork this code
2.  Create new Feat_xxx branch
3.  Submit the code
4.  New Pull Request

## Authorization instructions
Use this software must be open source and indicate the source,If you want closed source and commercial,Then need to be authorized by the author.

## Follow us
If you think this software is helpful to you, please give us a START

## post issues

if you have eny quest.post issues...