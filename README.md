## S.I.M.P.L.E.
**S**mart **I**ntuitive **M**essaging **P**latform with **L**ess **E**ffort.         
___

S.I.M.P.L.E. (or just *simple*) is a cross-platform C++ library to **easily** exchange data across a network.          
It is built on top of [ZeroMQ](https://github.com/zeromq/libzmq) and uses custom [FlatBuffers](https://github.com/google/flatbuffers) messages.       
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

*simple* offers a light interface to [FlatBuffers](https://github.com/google/flatbuffers) messages.          
[Common messages are provided and live under *simple_msgs*](https://github.com/IFL-CAMP/simple/wiki#simple_msgs)        
New messages can be defined - in the *simple_msgs* format - and sent/received using the same interface.

### Usage

**Build and Install from source:** on [Windows](https://github.com/IFL-CAMP/simple/wiki/install:-windows) or [Linux/Mac OSX](https://github.com/IFL-CAMP/simple/wiki/install:-linux).       
**Use in your C++ project:** [via CMake](https://github.com/IFL-CAMP/simple/wiki#cmake). We suggest Windows users to link to the static version of *simple*.     

#### Publisher / Subscriber 
- A Publisher sends (publishes) data to any Subscriber that is listening to its IP address/port.
- A Subscriber waits for new data from a Publisher and asynchrounsly receives it.      
An example of the Publisher/Subscriber pattern in C++ is available [here](https://github.com/IFL-CAMP/simple/wiki/example:-pubsub).

#### Request / Reply
- A Client sends a request to an IP address/port of a Server and expects a reply from it.           
- A Server waits for requests and asynchrounsly replies to a Client.         
An example of the Client/Server pattern in C++ is available [here](https://github.com/IFL-CAMP/simple/wiki/example:-reqrep).

#### Inspiration from ROS

We are heavy (and happy) [ROS](http://www.ros.org/) users and we aim to provide a very similar interface.     
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
