## S.I.M.P.L.E.
A Smart Intra-operative Messaging Platform with Less Effort.         
[![Build Status](https://gitlab.lrz.de/CAMP_IFL/simple/badges/master/build.svg)](https://gitlab.lrz.de/CAMP_IFL/simple/commits/master)
___

S.I.M.P.L.E. (or just simple) is a cross-platform C++ library to exchange data across devices, and to do it in an easy way.          
It is built on top of ZeroMQ and uses custom defined Flatbuffers messages, it implements a clean interface to the classic Publisher-Subscriber and Request-Reply patters.         

Simple can be used on Windows, Linux and MacOS X platforms and it is tested on gcc, clang, msvc14 and msvc15.          

### SIMPLE_MSGS
The following messages are defined and immediately available using simple:
- Bool, Int, Float, Double, String
- Header
- Position
- Quaternion
- Pose
- Rotation Matrix
- Image

### Usage

#### simple CMake

You can easily add simple to your project via CMake adding:    

```cmake
find_package(simple REQUIRED)      
target_link_libraries(YourTargetName simple::simple)
```

YourTargetName would be any library or executable that you previously defined. That is everything you need.

A static version of simple is also provided, that can be used with:

```cmake
find_package(simple REQUIRED)         
target_link_libraries(YourTargetName simple::simple-static)      
```

We recommend Windows users to link to simple-static! 

#### PUB / SUB

#### REQ / RES

### But it looks like ROS!

Indeed! We are heavy (and happy) ROS users and we wanted to provide a very similar interface.     
___

### Contacts
Salvatore Virga - salvo.virga@tum.de          
Fernanda Levy Langsch - fernanda.langsch@tum.de         