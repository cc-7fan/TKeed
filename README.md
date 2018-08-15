# TKeed WebServer

**TKeed is a high performance HTTP WebServer uses the Reactor model. Code normative and functional scalability are close to the industry level. The project will be updated until feature have been completed. Have a fun. SYSU, TeeKee.**

![info](./datum/info.png)

## Dev Document

| Part Ⅰ | Part Ⅱ | Part Ⅲ | Part Ⅳ | Part Ⅴ | Part Ⅵ | Part Ⅷ | Part Ⅸ |
| :--------: | :---------: | :---------: | :---------: | :---------: | :---------: |:--------:| :--------:|:--------:|
---
## Dev Environment

**Dev Tool**

- 操作系统：Ubuntu 16.04

- 编辑器：Sublime + Vim

- 编译器：gcc 5.4.0

- 单元测试：~~gtest~~

- 版本控制：git

- 代码结构：[Understand](https://scitools.com/) + [callgraph](http://blog.csdn.net/solstice/article/details/488865)

- 集成环境：[Clion](https://www.jetbrains.com/clion/)

**Other**

- 自动化构建：[Travis CI]

- 压测工具：[WebBench](https://github.com/EZLippi/WebBench)

--- 

## Timeline

**Now**

- v1.0已经完成，本地已调试通过。提交到GitHub上的代码会由Travis自动构建。

    特性：

    - 添加Timer定时器，定时回调handler处理超时请求

        - 高效的小根堆结构

        - 惰性删除方式

    - 实现了HTTP长连接传输数据

        - 非阻塞I/O

        - epoll边缘触发模式（ET）

    - 线程池操作及其同步互斥管理

        - 调度选项

            - 队列式FIFO调度模式

            - 加入优先级的优先队列 (+)

    - 使用状态机解析HTTP协议，非简单字符串匹配方式解析请求

- v1.1修改了CPU负载较高问题，修改后1000并发各线程（4worker）CPU使用率10%左右。

---

[![Build Status](https://travis-ci.org/linw7/TKeed.svg?branch=master)](https://travis-ci.org/linw7/TKeed)
[![license](https://img.shields.io/github/license/mashape/apistatus.svg)](https://opensource.org/licenses/MIT)

---