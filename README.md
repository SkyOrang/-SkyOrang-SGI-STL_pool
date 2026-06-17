# SGI-STL_pool个人实现版本
语言 / Language: [中文](README.md) | [English](README.en.md)
#### 介绍
仿SGI-STL二级分配器，实现高性能内存池
封装为独立库，提供PoolMalloc/PoolFree/PoolNew/PoolDelete接口

作用

1.解决内存碎片的问题

2.更高效率的申请和释放内存，减少直接向操作系统申请和释放内存的时间消耗

3.提升缓存局部性，让数据更加集中以便CPU更快访问

#### 软件架构
1.仅依靠C++标准库实现，没有任何第3方依赖

2.该项目设计仅为单线程的环境提供稳定支持

#### 安装教程
(1)使用源代码

直接克隆下载包含对应头文件即可使用

(2)使用静态库（在Release中可以下载Windows版）

以VS为例：

1.在VS项目属性页C++常规栏目附加包含库目录加入头文件路径

2.在VS项目属性页链接器常规栏目附加包含库目录加入库文件路径

3.在VS项目属性页链接器输入栏目附加依赖项加入要包含的库文件

#### 使用说明
具体详见wiki

#### 贡献
(1)提供建议：

如果各位对本项目有各种建议，欢迎通过759792938@qq.com邮箱来联系我

(2)提供代码：

1.复刻仓库

2.创建功能分支

3.提交代码

4.创建拉取请求

#### 成果展示
在Release条件下：

<img width="692" height="551" alt="image" src="https://github.com/user-attachments/assets/80f0926e-86a1-45f8-a53b-7af34950b03b" />

<img width="692" height="414" alt="image" src="https://github.com/user-attachments/assets/99ae79b7-77f5-4ebc-979e-080a588c2ce7" />

结论：

我为内存池做了全面的性能测试，在 Release 模式下：

核心数据：

8 字节小对象分配释放比系统快 7.4倍

对象生命周期管理（PoolNew/PoolDelete）比系统快 10倍

批量分配接近零开销

100 万次操作后内存增长仅 80 KB左右，无明显泄漏。

这些数据充分证明了内存池在小对象高频分配场景下的性能优势。

测试代码：详见Wiki



