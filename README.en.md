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

#### Choose the Right One
Most products on the market either charge fees, update slowly, or lack complete functionality. They also lack technical support. With our product, you don't need to worry about these issues at all.  
Our product is completely free and offers both free and paid technical support. It is rich in features and allows for custom development. You can start using our product now and experience the best and open-source time service available.

#### Version difference
V3 version, the protocol is basically reimplemented. It is no longer compatible with the V2 protocol. V3 is basically changed to the HTTP interface for management. The compatibility is better. Please check the documentation for more.  
V2 Version can be update to V3,V3 DB Compatible with V2 Databse

## Software feature
Our feature list not only includes the following but also has many features in development.  
You can refer to this webpage to learn about the features we plan to support in the future: https://github.com/libxengine/XEngine_Authorize/issues  
feature list:  
1. support http.websocket protocols
2. Support Encrypto communication
3. Support User Management
4. Support Serial Management
5. Support Log Management
6. Support Configure Management
7. Support Serical Multie types(second card, day card, frequency card, custom card)
8. Support Try,Support quick verification
9. Support Retrieve password through email
10. Support app program and html web Authorize
11. Support Accesss Control
12. support http management api function,you can write web page to implement network authorize management interface by self
13. support third user verification interface
14. support distributed verification authorize function
15. black list management
16. cdkey support(local, network-number of times, time-user self-registration, system authorization-super multi-functional cdkey)
17. announecement management
18. support dynamic verifaction code
19. support mulit login and merge time

## install

#### XEngine Evn
you must install XEngine,need V8.15 or above,install XEngine can be refer to xengine Readme docment  
GITEE:https://gitee.com/xyry/libxengine  
GITHUB:https://github.com/libxengine/xengine

##### fast to deployment 
git clone https://gitee.com/xyry/libxengine.git or git clone https://github.com/libxengine/xengine.git  
window Exection XEngine_WINEnv.bat   
Linux Exection:sudo ./XEngine_LINEnv.sh -i 3  
Macos Exection:./XEngine_LINEnv.sh -i 3

#### sub module
Due to the dependent sub-modules, after you checkout the warehouse, execute the following command in the warehouse directory to pull the sub-modules  
git submodule init  
git submodule update  

#### Windows
use vs open and compile,suport windows 7sp1 and above  
Just Run it,use XEngine_AuthorizeService  
XEngine_AuthorizeApp is pc management tools

#### Linux
use makefile compile,UBUNTU22.04 x64 or RockyLinux 9 x64  
Run it on the terminal,use XEngine_AuthorizeService

#### Macos
use makefile compile,mac 13 and above  
Run it on the terminal,use XEngine_AuthorizeService

##### compile command
execute command in XEngine_Source path   
make complie  
make FLAGS=InstallAll Install File  
make FLAGS=CleanAll Clear Complie  

## how to use

#### upgrade
If it is an upgrade installation, after downloading the new version and modifying the configuration file, it can be run directly  
database update need to open the database matching fields and tables are consistent
#### server(management)
Windows:complie complete,copy VSCopy_x86 or VSCopy_x64 to your complie dir,should copy depend module when execution complete,and copy XEngine_Release dir all file to complie dir  
Linux or macos:it can be execution....

#### client
client can be refer XEngine_Apps,and can be refer the docment of protocol,The process should be:register->login->pay->timeout->end.  

#### administrator
The program released now comes with an administrator user, user name: admin password: 123123aa, you need to change the password yourself  

## test server
address:app.xyry.org  
port :tcp port 5300,websocket port 5301,http management port 5302,Transmission verification password: no encryption

## Screenshot of the program
![输入图片说明](https://www.xyry.org/XEngine_StructPic/authorize/1.png "在这里输入图片标题")
![输入图片说明](https://www.xyry.org/XEngine_StructPic/authorize/2.png "在这里输入图片标题")
![输入图片说明](https://www.xyry.org/XEngine_StructPic/authorize/3.png "在这里输入图片标题")
![输入图片说明](https://www.xyry.org/XEngine_StructPic/authorize/4.png "在这里输入图片标题")
![输入图片说明](https://www.xyry.org/XEngine_StructPic/authorize/5.png "在这里输入图片标题")
![输入图片说明](https://www.xyry.org/XEngine_StructPic/authorize/6.png "在这里输入图片标题")

## Participate in contribution

1.  Fork this code
2.  Create new Feat_xxx branch
3.  Submit the code
4.  New Pull Request

## development path
The V1 version is an XEngine component. Most of the code is concentrated in XEngine  
The V2 version comes out separately with all related modules and is developed separately  
The V3 version is a new version, developed to meet the requirements of many environments  

## update guide
Database update Use Navicat and other database software to open the judgment table and the different fields for modification  
After modification, directly copy the database to the new version to overwrite, and modify your configuration file  
3.11->3.12 A new time field has been added to the user table, please refer to the SQL file

## Follow us
If you think this software is helpful to you, please give us a START  
and wechat qr scan to follow us  
![qrcode](https://www.xyry.org/qrcode.jpg)

## post issues

if you have eny quest.post issues...