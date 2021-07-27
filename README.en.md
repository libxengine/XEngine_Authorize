# XEngine_Authorize

## Introduction
c c++网络验证服务器 网络授权服务器  
c c++network Authorize service  
the server is network authorize verification service code library,This service mainly provides network authorization verification function.support local and network verification mode  
This is a cross-platform simple high-performance network authorization server that provides a complete demo code  

## Software feature
The purpose of development and implementation based on libXEngine is a cross-platform network storage service  
This repository has a development and master branch. If you want to use it, please use the master branch  
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

## install

#### Requirements
support system above windows 7sp1 and linux(ubuntu20.04,centos8)  
XEngine need V7.17 or above  

#### Windows
Just Run it.


#### Linux
Run it on the terminal

## complie

#### Windows
use vs2019 x86(debug or release) x64(only release) open and complie  
You need to configure the environment in the following way, otherwise you may need to set the library directory in the project yourself  

##### XEngine
XEngine can be download with mine repository,whe you downloaded xengine,you have to add value to you user environment  
- XEngine_Include header file path
- XEngine_Library library file path

#### Linux
Linux use Makefile complie  
UBUNTU20.04 x64 or CENTOS8 x64  

##### XEngine环境
you can install xengine env to your system by shell  
like this:sudo XEngine_RunEnv.sh -i 3

##### Screenshot of the program
execute command in XEngine_Source path   
make complie  
make FLAGS=InstallAll Install File  
make FLAGS=CleanAll Clear Complie  

## Participate in contribution

1.  Fork this code
2.  Create new Feat_xxx branch
3.  Submit the code
4.  New Pull Request

## post issues

if you have eny quest.post issues...