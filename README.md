## S.I.M.P.L.E.
A Smart Intra-operative Messaging Platform with Less Effort.         
[![Build Status](https://gitlab.lrz.de/CAMP_IFL/simple/badges/master/build.svg)](https://gitlab.lrz.de/CAMP_IFL/simple/commits/master)
___

S.I.M.P.L.E. (or just simple) is a cross-platform C++ library to exchange between devices data across a network, and to do it in an easy way.          
It is built on top of [ZeroMQ](https://github.com/zeromq/libzmq) and uses custom defined [FlatBuffers](https://github.com/google/flatbuffers) messages, it implements a clean interface to the classic **Publisher-Subscriber** and **Request-Reply** patters.         

Simple can be used on **Windows**, **Linux** and **OSX** platforms and it is tested on **gcc**, **clang**, **msvc14** and **msvc15**.

### Dependencies

- A C++11 compatible compiler
- CMake 3.5
- [ZeroMQ](https://github.com/zeromq/libzmq)
- [FlatBuffers](https://github.com/google/flatbuffers)


### simple_msgs
Simple provides a set of message that can be immediately used to exchange data.        
They represent common information that are usually needed

The following messages are defined simple:
- Bool, Int, Float, Double, String
- Header
- Position
- Quaternion
- Pose
- Rotation Matrix
- Image

### Usage

##### simple CMake

You can easily add simple to your project via CMake adding:    

```cmake
find_package(simple REQUIRED)      
target_link_libraries(YourTargetName simple::simple)
```

**YourTargetName** would be any library or executable that you previously defined. That is everything you need!

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
___
### License

*simple* is distributed under the LGPLv3 License. See [LICENSE](LICENSE) for details.