## S.I.M.P.L.E.
**S**mart **I**ntra-operative **M**essaging **P**latform with **L**ess **E**ffort.         
[![Build Status](https://gitlab.lrz.de/CAMP_IFL/simple/badges/master/build.svg)](https://gitlab.lrz.de/CAMP_IFL/simple/commits/master)
___

S.I.M.P.L.E. (or just *simple*) is a cross-platform C++ library to exchange between devices data across a network, and to do it in an easy way.          
It is built on top of [ZeroMQ](https://github.com/zeromq/libzmq) and uses custom defined [FlatBuffers](https://github.com/google/flatbuffers) messages, it implements a clean interface to the classic **Publisher-Subscriber** and **Request-Reply** patters.         

*simple* can be used on **Windows**, **Linux** and **OSX**. 
___
### Dependencies

- A C++11 compatible compiler (tested on **gcc**, **clang**, **msvc14** and **msvc15**.)
- CMake 3.5
- [ZeroMQ](https://github.com/zeromq/libzmq)
- [FlatBuffers](https://github.com/google/flatbuffers)

___

### simple_msgs
*simple* provides a set of message that can be immediately used to exchange data.        
They represent common information that are usually needed

The following messages are defined:
- Bool, Int, Float, Double, String
- Header
- Position
- Quaternion
- Pose
- Rotation Matrix
- Image

___

### Usage

##### simple - compile

```bash
git clone https://github.com/google/flatbuffers.git
cd flatbuffers && mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j8
make install
```

```bash
git clone https://github.com/zeromq/libzmq.git
cd libzmq && mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j8
make install
```

```bash
git clone https://gitlab.lrz.de/CAMP_IFL/simple.git
cd simple && mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j8
make install
```

##### simple - CMake

You can easily add *simple* to your project via CMake adding:    

```cmake
find_package(simple REQUIRED)      
target_link_libraries(YourTargetName simple::simple)
```

**YourTargetName** would be any library or executable that you previously defined. *That is everything you need!*

A static version of *simple* is also provided, we recommend Windows users to use it!

```cmake
find_package(simple REQUIRED)         
target_link_libraries(YourTargetName simple::simple-static)      
```

##### Publisher / Subscriber

The Publisher / Subscriber pattern is implemented using the classes [*Publisher*](include/simple/publisher.hpp) and [*Subscriber*](include/simple/subscriber.hpp).         
A Publisher simply sends out messages to any Subscriber that is listening to its **ip address** and **port**.         
Here is an example of a Publisher for messages of type **simple_msgs::Point**, it will publish 10 times to any listeners on port **5555**.        

```cpp
int main() {
  simple_msgs::Point my_point(1.0, 2.0, 3.0);
  simple::Publisher<simple_msgs::Point> publisher("tcp://*:5555");   
  for (auto i=0; i < 10; ++i) {
    publisher.publish(my_point);
  }
}
```

A Subscriber listens to messages sent my a Publisher on the given **ip address** and **port**.        
Every message that arrives will be handles by the given callback function. That function could do anything, in this example it just prints out the content of the message.    

```cpp
void example_callback(const simple_msgs::Point& p)
{
  std::cout << p << std::endl;
}

int main()
{
  simple::Subscriber<simple_msgs::Point> subscriber("tcp://IP_ADDRESS:5555", example_callback);
}
```

A full example is available for a [Publisher](examples/publisher.cpp) and a [Subscriber](examples/subscriber.cpp).

##### Request / Reply

The Request / Reply pattern is implemented using the classes [*Client*](include/simple/client.hpp) and [*Server*](include/simple/server.hpp).          
A *Client* sends a request while a *Server* will receive requests and reply with an answer. In simple, both a request and a reply are contained in the same message.      
In this example, a *Client* sends a request with a **simple_msgs::Point** and receive the point being increased by one.       
For a real life example, a good idea is to have a field of the message containing the request and one the reply.      


Here a *Client* will send 10 requests to a *Server* using its **ip address** and **port**.      
```cpp
int main()
{
  simple_msgs::Point p(5.0, 6.0, 7.0);

  simple::Client<simple_msgs::Point> client("tcp://IP_ADDRESS:5555");

  for (auto i = 0; i < 10; ++i)
  {
    if (client.request(p))
    {
      std::cout << "Request successful \n" << p << std::endl;
    }
    else
    {
      std::cerr << "Request to the server failed." << std::endl;
    }
    // Sleep for some time.
  }
}
```

Here a *Server* will wait for requests sent from any *Client* on its **port**.        
Any request is processed using the given callback function, in this case the received point is just increased by one.       

```cpp
void example_callback(simple_msgs::Point& p)
{
  std::cout << "Received a point. Adding 1 to its elements." << std::endl;
  p += 1.0;
}

int main()
{
  simple::Server<simple_msgs::Point> server("tcp://*:5555", example_callback);
}
```

A full example is available for a [Client](examples/client.cpp) and a [Server](examples/server.cpp).

### But it looks like ROS!

Indeed! We are heavy (and happy) ROS users and we wanted to provide a very similar interface.     
___

### Contacts
Salvatore Virga - salvo.virga@tum.de          
Fernanda Levy Langsch - fernanda.langsch@tum.de         
___
### License

*simple* is distributed under the LGPLv3 License. See [LICENSE](LICENSE) for details.