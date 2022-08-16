[中文](README.md) ||  [English](README.en.md)  
# XEngine_Authorize
This repository has a development and master branch. If you want to use it, please use the master branch  
As long as the repository is not in a suspended state, someone will maintain and develop it all the time, please use it with confidence  

## Introduction
c c++网络验证服务器 网络授权服务器  
c c++network Authorize service  
the server is network authorize verification service code library,This service mainly provides network authorization verification function.support local and network verification mode  
network verification support tcp and websocket protocol verification.it is means:support app program and web  
first open source C/C++ network authentication server, supporting various languages ​​and platforms for network authentication and authorization
This is a cross-platform simple high-performance network authorization server that provides a complete demo code  

## Software feature
A simple and high-performance cross-platform network authentication and authorization server developed and implemented based on XEngine  
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
10. Support app program and html web Authorize
11. Support Accesss Control
12. support http management api function,you can write web page to implement network authorize management interface by self

## install

#### XEngine Evn
you must install XEngine,need V7.38 or above,install XEngine can be refer to xengine Readme docment  
GITEE:https://gitee.com/xyry/libxengine  
GITHUB:https://github.com/libxengine/xengine

##### fast to deployment 
git clone https://gitee.com/xyry/libxengine.git or git clone https://github.com/libxengine/xengine.git  
window Exection XEngine_WINEnv.bat   
Linux Exection:sudo ./XEngine_LINEnv.sh -i 3  
Macos Exection:./XEngine_LINEnv.sh -i 3

#### Windows
use vs open and compile,suport windows 7sp1 and above  
Just Run it,use XEngine_AuthorizeService  
XEngine_AuthorizeApp is pc management tools

#### Linux
use makefile compile,UBUNTU20.04 x64 or CENTOS8 x64  
Run it on the terminal,use XEngine_AuthorizeService

#### Macos
use makefile compile,mac 12 and above  
Run it on the terminal,use XEngine_AuthorizeService

##### compile command
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
![输入图片说明](https://www.xyry.org/authorize/5.png "在这里输入图片标题")

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