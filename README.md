## EventServer
libevent开发server端

1.handler 仿android实现

2.threadpool实现

3.libevent 多线程io

## 框架层次设计
payload protocol(json or other) (未开发)

mqtt (未开发)

TLS(未开发)

1个acceptor thread

8 个线程 负责 8个 event_base loop, write 和 read分别用不同的event

24个线程进行读写操作


## 读写及缓存设计：
接收：

自定义读读缓存设计

写数据：

自定义写缓存设计
  
## 编译
./autogen.sh

./configure

make

## clean
make clean

如使用以下命令，则需要重新执行 autogen.sh 以及 configure
make distclean

## 执行
./src/server/EventServer

./src/client/EventClient

目前在同一台主机上测试，由于端口分配的限制，只能测试5万个链接，5个client线程分别轮询1万个socket 进行发送数据，服务端会原样返回client发过去的数据。

说明：

server负责监听链接，以及在收到client数据时，将数据重新发回client

每个client 创建一万个链接，并用一个线程轮流给1万个socket发送数据，

author：afreeliyunfeil@163.com
