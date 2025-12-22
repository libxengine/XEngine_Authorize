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

## Notice
the compiled release version will only be released on github.

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
20. user bind hardware code

## install

#### XEngine Evn
you must install XEngine,need V9.x or above,install XEngine can be refer to xengine Readme docment  
GITEE:https://gitee.com/libxengine/libxengine  
GITHUB:https://github.com/libxengine/libxengine

##### fast to deployment 
git clone https://gitee.com/libxengine/libxengine.git or git clone https://github.com/libxengine/libxengine.git  
window Exection XEngine_WINEnv.bat   
Linux Exection:sudo ./XEngine_LINEnv.sh -i 3  
Macos Exection:./XEngine_LINEnv.sh -i 3

#### sub module
Due to the dependent sub-modules, after you checkout the resprepository, execute the following command in the resprepository directory to pull the sub-modules  
git submodule init  
git submodule update  

#### Windows
use vs open and compile,suport windows 7sp1 and above  
Just Run it,use XEngine_AuthorizeService  
XEngine_AuthorizeApp is pc management tools

#### Linux
use makefile compile,Run it on the terminal,use XEngine_AuthorizeService

#### Macos
use makefile compile,Run it on the terminal,use XEngine_AuthorizeService

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
![输入图片说明](./XEngine_Picture/authorize_1.png "在这里输入图片标题")
![输入图片说明](./XEngine_Picture/authorize_2.png "在这里输入图片标题")
![输入图片说明](./XEngine_Picture/authorize_3.png "在这里输入图片标题")
![输入图片说明](./XEngine_Picture/authorize_4.png "在这里输入图片标题")
![输入图片说明](./XEngine_Picture/authorize_5.png "在这里输入图片标题")
![输入图片说明](./XEngine_Picture/authorize_6.png "在这里输入图片标题")
![输入图片说明](./XEngine_Picture/authorize_7.png "在这里输入图片标题")
![输入图片说明](./XEngine_Picture/authorize_8.png "在这里输入图片标题")

## No-Code License Authorization & Verification Support
#### app packet service
If you don't have software development capabilities, don't want to implement it yourself, or want to add authorization verification to other software, we also provide an authorization packaging service, which allows your program to directly package our verification.  
Program Packaging Service is a paid service, and you can package and add verification to any program. You can integrate our verification service without requiring source code or modifying your program. After packaging is complete, when it is run next time, our verification client will run first, and then your provided program will run after verification is successful, which is very convenient to use.  
This service is paid, priced at 200 per package. It supports both x86 and x64 programs, and only requires you to provide your server address, port, and verification mode. The process is as follows:  
Payment → Send the program to us → We package it → Verification testing → Send the packaged program to you → Completed  
After packaging, when the program is launched, the following window will open first. Only after a successful login will your program start. As shown below, once launched, the main interface will be displayed. After login, the main interface will close and your program will then start.  
![Main Interface](./XEngine_Picture/packet_1.png "Main Interface")  
![CDKEY Login](./XEngine_Picture/packet_2.png "CDKEY Login")  
![User Registration](./XEngine_Picture/packet_3.png "User Registration")  
![User Recharge](./XEngine_Picture/packet_4.png "User Recharge")  
![Password Recovery](./XEngine_Picture/packet_5.png "Password Recovery")  
![Remaining Time](./XEngine_Picture/packet_6.png "Remaining Time")  

The verification timer will automatically stop. Supported features include login, local CDKEY verification, registration, recharge, password recovery, and remaining time query.  
Note: This feature is only a packaging replacement. If you require full encryption, shell protection, and higher security, you must implement it yourself through code development.  
#### Packager
We can also provide the original packager if you have more packaging needs. The packager is currently in a promotional campaign, 500 yuan.  
You can choose our verification program, then select the program you want to package. Then select the configuration file, and that's it. It supports automatically updating the program icon to your software icon.  
Configuration file supports configuring encryption and decryption communication and authentication mode  
![Main Interface](./XEngine_Picture/packer_1.png "Main Interface")

## Participate in contribution

1.  Fork this code
2.  Create new Feat_xxx branch
3.  Submit the code
4.  New Pull Request
5.  merge to develop branch

## development path
The V1 version is an XEngine component. Most of the code is concentrated in XEngine  
The V2 version comes out separately with all related modules and is developed separately  
The V3 version is a new version, developed to meet the requirements of many environments  

## update guide
Database update Use Navicat and other database software to open the judgment table and the different fields for modification  
After modification, directly copy the database to the new version to overwrite, and modify your configuration file  
new db field for serial table by V3.19

## Follow us
If you think this software is helpful to you, please give us a START  
and wechat qr scan to follow us  
![qrcode](https://www.xyry.org/qrcode.jpg)

## post issues

if you have eny quest.post issues...