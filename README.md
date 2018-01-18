## S.I.M.P.L.E.
**S**mart **I**ntra-operative **M**essaging **P**latform with **L**ess **E**ffort.         
[![Build Status](https://gitlab.lrz.de/CAMP_IFL/simple/badges/master/build.svg)](https://gitlab.lrz.de/CAMP_IFL/simple/commits/master)
___

S.I.M.P.L.E. (or just *simple*) is a cross-platform C++ library to exchange data across a network, and to do it in an easy way.          
It is built on top of [ZeroMQ](https://github.com/zeromq/libzmq) and uses custom defined [FlatBuffers](https://github.com/google/flatbuffers) messages.
It provides a clean interface to the classic **Publisher-Subscriber** and **Request-Reply** patterns.         

*simple* can be used on **Windows**, **Linux** and **OSX**. 
___
### Dependencies

- A C++11 compiler (tested on **gcc**, **clang**, **msvc14** and **msvc15**.)
- CMake 3.5+
- [ZeroMQ](https://github.com/zeromq/libzmq)
- [FlatBuffers](https://github.com/google/flatbuffers)

___

### simple_msgs

The following messages are provided:
- Bool, Int, Float, Double, String
- Header
- Position
- Quaternion
- Pose
- Rotation Matrix
- Image

___

### Usage

You can build it.
You can use it via CMake.
You have some messages already defined.
You have publisher/subscriber and request/reply

#### Ehi...it looks like ROS!

Indeed! We are heavy (and happy) ROS users and we wanted to provide a very similar interface.     
___

### Contacts

[![TUM](http://campar.in.tum.de/files/goeblr/TUM_Web_Logo_blau.png "TUM Logo")](http://tum.de)        
[Chair for Computer Aided Medical Procedures](http://campar.in.tum.de/)        
[Technical University of Munich](www.tum.de), Germany.       

Salvatore Virga : [salvo.virga@tum.de][salvo_email]  
Fernanda Levy Langsch : [fernanda.langsch@tum.de][fernanda_email]

[salvo_email]: salvo.virga@tum.de
[fernanda_email]: fernanda.langsch@tum.de       
___
### License

*simple* is distributed under the LGPLv3 License. See [LICENSE](LICENSE) for details.