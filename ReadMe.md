# libeventInterface

## 将libevent融合到已有服务
已经在线上跑的服务，修改网络同步接口为异步请求，libevent事件循环与服务事件循环冲突，进行融合。

## 特点
* 对容器和服务进行抽象，方便扩充新的服务，如HTTPServer，TCPServer，TimeoutServer等
* 对链路和请求进行管理
* 在线查看请求响应统计，清除统计
* 最大连接数，最大请求数，日志以及HTTP头信息可配置
* 跨平台，windows、linux均适用
* 方便二次开发

## 编译方式：

1. windows平台
	```
	首先下载libevent最新源码，用vs编译生成可用库；
	然后用vs2010以上版本打开\build\libeventInterface\libeventInterfacesln，设置libevent头文件目录和库目录，然后编译即可；
	将源码目录中的config目录拷贝到工程目录\build\libeventInterface\libeventInterface下运行
	```

2. linux平台
	```
	首先下载libevent最新源码并编译，生成libevent库；
	查看是否平台有cmake工具，如果没有则安装cmake；
	进入libeventInterface源码目录，确认有CMakeLists.txt文件；
	执行:
		mkdir build
		cd build
		cmake ..
		make
		cp -f ../config .
		./libevent_test
	```

