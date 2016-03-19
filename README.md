LibCore
=======

##<a name="index"/>目录
* [Libcore简介](#Introduction )
* [网络库](#netlib)
* [数据库](#db)
* [计时器](#timer) 
* [线程池](#threads)
* [序列化](#marshal)
* [参数化](#param)
* [csv转化](#csv)
* [文件路径说明](#files) 

<a name="Introduction"/>
#Libcore简介
    是一款跨平台的游戏服务器底层框架,包含基于reactor的网络库，易使用的rpc，基于时间轮或者最小堆实现的计时器，基于log4plus的日志系统,基于orm的数据库，底层采用leveldb或者rocksdb,以及其他编码转化，随机数，测试用例，常用函数,线程池等。

<a name="netlib"/>
#网络库：
	采用reactor实现的高性能网络库，包含如下功能:
		epollex:
			底层采用腾讯开源的libco协程对epoll进行hook,合理使用协程，避免cpu在epoll时处于iowait状态，浪费cpu使用。
		zmq:
			基于第三方库的消息队列简单实现消息收发的。
		unix-socket:
			用于将从客户端的套接字传递给同主机的其他网络进程，以减少一个进程的压力。
		其他reactor实现：包括udp，select，wsa，raknet，epoll ,iocp
	http:
		简单的Http解析以及包含数字签名.
		
<a name="db"/>
#数据库
	单线程版本基于leveldb实现，多线程compact基于rocksdb,默认为leveldb,因为是Key-Value数据库,所以数据格式采用了mongo库中mongobson，在游戏中可以很灵活增加数据，而不用担心版本问题.
	在数据格式之上采用了redis的常用数据结构，例如:hash，zset,ztop等，也可自定义拓展一些数据结构。在这层数据结构之上搭建了一层orm，来使数据对象化。同时，数据库也支持主从，热备份。
    
<a name="db"/>
#计时器
	计时器采用两种方式，时间轮和最小堆实现。
    
<a name="threads"/>
#线程池
	根据每个线程的优先级选择对应优先级的task执行。
    
<a name="marshal"/>
#序列化:
	可以将任意数据格式或者容器转化为2进制数据。同时也可以将这些数据存入数据库。
    
<a name="param"/>
#参数化
	基于序列化可以将数据打包成一个参数类，有点类似auto不过是动态的。
    
<a name="csv"/>
#csv转化
	可以将excel通过python转化为csv格式，用于程序读取，并生成对应读取的cpp文件，目前支持的格式包括int,int64,double,string,支持将这些数据的数组形式。未来将支持结构体类型。
	
<a name="files"/>
#文件路径说明
```
	/trunk/bin/						程序生成的二进制可执行文件以及程序执行相关的依赖库.
	/trunk/build					程序生成跨平台的编译文件.采用premake5.
	/trunk/configs/msg				使用protobuf时生成对应CPP文件的工具.
	/trunk/deps/					libcore可能使用到的一些第三方库
	/trunk/deps/boost				主要使用到的智能指针,和侵入式指针.
	/trunk/deps/CityHash			Google开源的Hash库.性能非常高.
	/trunk/deps/expat				xml解析库
	/trunk/deps/glog				Google开源的Log库.轻量级,未使用.
	/trunk/deps/gperftools			Google开源的tcmalloc库.性能非常高.
	/trunk/deps/jsoncpp				Json解析工具.使用简单.
	/trunk/deps/rapidjson			腾讯开源的库.据说解析很快.可用于需要动态解析的时候.目前未用到.
	/trunk/deps/LevelDB				Google开源的数据库引擎.单线程,使用简单.性能很高.易上手.
	/trunk/deps/Libbson				bson的开源库.不是很好用.
	/trunk/deps/Libco				腾讯开源的协程库.据说用于整个微信架构.确实很好用.
	/trunk/deps/Libiconv			字符编码转换.
	/trunk/deps/Log4cplus			C++中比较好用的日志工具.
	/trunk/deps/Log4cpp				比Log4cplus稍微难用点.感觉对64位支持不太好.目前未用到.
	/trunk/deps/Lua					未来会基于Lua做一套C++和Lua相互调用的接口,目前未用到.
	/trunk/deps/MongoBson			mongoDB的Bson库.从Mongo中扒出来的.内存管理很好.支持引用.
	/trunk/deps/Mysql				关系型数据库,用于游戏需要写内存缓存池,未用到.
	/trunk/deps/openssl				目前用于Http协议解析中的数字签名.
	/trunk/deps/protobuf			Google开源的数据序列化库.主要用于网络通信中.
	/trunk/deps/pthread				开源的跨平台线程库.
	/trunk/deps/Raknet				将UDP改编为可靠传输的网络库.也可打洞.
	/trunk/deps/Re2					Google开源的比较强大的正则表达式.
	/trunk/deps/Rocksdb				Facebook开源的基于LevelDB的数据库.
	/trunk/deps/Snappy				Google开源的压缩工具.追求的是压缩速度.不追求压缩率.
	/trunk/deps/tbb					Intel开源的并行开发库.包含多线程安全的map和set.以及自旋锁等.
	/trunk/deps/Unittest++			C++代码测试工具.简单易上手.
	/trunk/deps/VLD					WIndows下的内存检测工具.简单易上手.
	/trunk/deps/zeromq				号称宇宙最快的消息队列.使用简单.
	/trunk/LibCore/					Libcore的源代码.
	/trunk/LibCore/Coroutine		基于腾讯开源的协程库.实现一个简单的跨平台协程库.
	/trunk/LibCore/Cutil			Libcore一些基础的函数实现最小堆,字符编码转换,随机数等等.
	/trunk/LibCore/GameDB			数据库的功能实现.
	/trunk/LibCore/LogLib			用桥接模式实现了Log4Cpp,Log4Cplus,Glog.默认采用Log4Cplus输出.
	/trunk/LibCore/Marshal			序列化库.可以将任何数据或者容器进行序列化.
	/trunk/LibCore/MsgLib			基于网络库实现的易使用的RPC库.
	/trunk/LibCore/NetLib			基于Reactor实现的网络库.
	/trunk/LibCore/Test				Libcore的测试用例.
	/trunk/LibCore/Threadpool		线程池
	/trunk/LibCore/Timer			需要用到时间的库
	/trunk/LibCore/XML				基于expat解析的xml库.
	/trunk/shared					测试文件
	/trunk/Tools/					Libcore用到的工具
	/trunk/Tools/bin/				用到的工具的二进制执行文件夹.
	/trunk/Tools/bin/GeneratorCSV	生成CSV的工具.
	/trunk/Tools/bin/GeneratorDB	生成ORM相关的数据.
	/trunk/Tools/bin/GeneratorRPC	生成RPC的工具.
	/trunk/Tools/src				二进制工具的源代码路径.
	/trunk/Tools/GameDBClient		游戏数据库的客户端.
	/trunk/Tools/GameDBCommon		游戏数据库贡献的一些代码.
	/trunk/Tools/GameDBServer		游戏数据库的服务器端.
	/trunk/Tools/GenProtobufFiles	生成Protobuf的CPP文件.
	/trunk/Tools/GenRPCS			增加在VS中方便生成RPC的项目文件.
	/trunk/vsproject				windows下的项目文件管理.
```
