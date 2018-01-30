## S.I.M.P.L.E.
**S**mart **I**ntuitive **M**essaging **P**latform with **L**ess **E**ffort.         
[![Build Status](https://gitlab.lrz.de/CAMP_IFL/simple/badges/master/build.svg)](https://gitlab.lrz.de/CAMP_IFL/simple/commits/master)
___

S.I.M.P.L.E. (or just *simple*) is a cross-platform C++ library to exchange data across a network between C++ application, and to do it in an easy way.          
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

### What can I send/receive?

*simple* uses [FlatBuffers](https://github.com/google/flatbuffers) messages, plus a small interface on top of those.       
Some common messages are already provided and live under *simple_msgs*:
- Bool, Int, Float, Double, String
- Header
- Position
- Quaternion
- Pose
- Rotation Matrix
- Image

Need another type of message? It can be defined - following the way *simple_msgs* are designed - and sent/received using the interface provided by *simple*.

### Usage

**Build and Install from source:** on [Windows](https://gitlab.lrz.de/CAMP_IFL/simple/wikis/install:-windows) or [Linux/Mac OSX](https://gitlab.lrz.de/CAMP_IFL/simple/wikis/install:-linux).       
**Use in your C++ project:** via [CMake](https://gitlab.lrz.de/CAMP_IFL/simple/wikis/home#cmake). We suggest Windows users to link to the static version of *simple*.     

#### Publisher / Subscriber 
- A Publisher sends (publishes) data to any Subscriber that is listening to its IP address / port.
- A Subscriber waits for new data from a Publisher and asynchrounsly elaborates it when it arrives.      
An example of the implementation of a Publisher and a Subscriber in C++ is available [here](https://gitlab.lrz.de/CAMP_IFL/simple/wikis/example:-pubsub).

#### Request / Reply
- A Client sends a request to a Server (knowing its IP address / port) and expects a reply from it.           
- A Server waits for requests and asynchrounsly elaborates a reply that is sent back to the Client.         
An example of the implementation of a Client and a Server in C++ is available [here](https://gitlab.lrz.de/CAMP_IFL/simple/wikis/example:-reqrep).

#### Inspiration from ROS

We are heavy (and happy) ROS users and we aim to provide a very similar interface to the one available in [ROS](http://www.ros.org/).     
___

### Contacts

[![TUM](http://campar.in.tum.de/files/goeblr/TUM_Web_Logo_blau.png "TUM Logo")](http://tum.de)        
[Chair for Computer Aided Medical Procedures](http://campar.in.tum.de/)        
[Technical University of Munich](www.tum.de), Germany.       

**Salvatore Virga** : [salvo.virga@tum.de][salvo_email]  
**Fernanda Levy Langsch** : [fernanda.langsch@tum.de][fernanda_email]

[salvo_email]: salvo.virga@tum.de
[fernanda_email]: fernanda.langsch@tum.de       
___
### License

*simple* is distributed under the LGPLv3 License. See [LICENSE](LICENSE) for details.