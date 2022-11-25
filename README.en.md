[中文](README.md) ||  [English](README.en.md)  
# XEngine_Authorize
This repository has a development and master branch. If you want to use it, please use the master branch  
As long as the repository is not in a suspended state, someone will maintain and develop it all the time, please use it with confidence  

## Introduction
c c++网络验证服务器 网络授权服务器  
c c++network Authorize service  
the server is network authorize verification service code library,This service mainly provides network authorization verification function.support local and network verification mode  
network verification support tcp and websocket and http protocol verification.it is means:support app program and web  
first open source C/C++ network authentication server, supporting various languages ​​and platforms for network authentication and authorization
This is a cross-platform simple high-performance network authorization server that provides a complete demo code  

#### why choose us
Fast iteration: feature updates are timely  
Technical support: complete technical documentation and technical support, quick response to your questions  
Unlimited language: don't care about the language your client uses, you can choose your own appropriate communication method  
Stable and reliable: based on C/C++, the core framework has been verified for 10 years. It is stable and compatible with high performance  
Flexible authentication: supports third-party authentication and authorization, supports distributed authentication and authorization, and can use its own user password system  
more functions: support various types of authorization  

#### Version difference
V3 version, the protocol is basically reimplemented. It is no longer compatible with the V2 protocol. V3 is basically changed to the HTTP interface for management. The compatibility is better. Please check the documentation for more.  
V2 Version can be update to V3,V3 DB Compatible with V2 Databse

## Software feature
A simple and high-performance cross-platform network authentication and authorization server developed and implemented based on XEngine  
feature list:  
1. support tcp,http.websocket protocols
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
13. support third user verification interface
14. support distributed verification authorize function

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

## how to use
#### server(management)
Windows:complie complete,copy VSCopy_x86 or VSCopy_x64 to your complie dir,should copy depend module when execution complete,and copy XEngine_Release dir all file to complie dir  
Linux:it can be execution....

#### client
client can be refer XEngine_Apps,and can be refer the docment of protocol,The process should be:register->login->pay->timeout->end.  

## test server
address:app.xyry.org  
port :tcp port 5300,websocket port 5301,http management port 5302,Transmission verification password: no encryption

## Screenshot of the program
![输入图片说明](https://www.xyry.org/XEngine_StructPic/authorize/1.png "在这里输入图片标题")
![输入图片说明](https://www.xyry.org/XEngine_StructPic/authorize/2.png "在这里输入图片标题")
![输入图片说明](https://www.xyry.org/XEngine_StructPic/authorize/3.png "在这里输入图片标题")
![输入图片说明](https://www.xyry.org/XEngine_StructPic/authorize/4.png "在这里输入图片标题")
![输入图片说明](https://www.xyry.org/XEngine_StructPic/authorize/5.png "在这里输入图片标题")

## Participate in contribution

1.  Fork this code
2.  Create new Feat_xxx branch
3.  Submit the code
4.  New Pull Request

## development path
The V1 version is an XEngine component. Most of the code is concentrated in XEngine  
The V2 version comes out separately with all related modules and is developed separately  
The V3 version is a new version, developed to meet the requirements of many environments  

## Authorization instructions
Use this software must be open source and indicate the source,If you want closed source and commercial,Then need to be authorized by the author.

## Follow us
If you think this software is helpful to you, please give us a START

## post issues

if you have eny quest.post issues...