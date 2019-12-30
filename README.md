从零开始的简单函数式 C++
========================================

[![CC-BY-NC-SA-4.0](https://i.creativecommons.org/l/by-nc-sa/4.0/88x31.png)](https://creativecommons.org/licenses/by-nc-sa/4.0/)

## 标题

如标题所示，这个教程主要是讲解如何用现代的 C++ 以及所配套的 STL 来“函数式”地编程。具体而言，本教程会讲解以下内容：

* STL 中的主要工具：元组 `pair`，`tuple`，向量 `vector`
* C++11 引入的 lambda 函数
* 假装在函数式编程的三板斧：`filter`，`map`，`reduce`
* 是否需要引入 lazy 求值
* 简单的函数操作
* 递归与状态变换
* 基础的模板元编程（编译时计算）

当然，如果真的执着于函数式编程的话，C++ 是显然不能满足绝大多数人的要求的，比如不能轻松地进行模式匹配，也没有较为简单地惰性求值。这时候不如推荐一些纯粹的函数式语言例如 Haskell 或者 scheme。

## 章节说明

* [前言](Chapters/chapter-00-Introduction.md)
* [STL 中的主要工具](Chapters/chapter-01-STL-Tools.md)
* [C++11 引入的 lambda 函数](Chapters/chapter-02-lambda.md)
* [函数式编程工具箱](Chapters/chapter-03-Functional-Tools.md)
* [更多的函数](Chapters/chapter-04-More-Functional.md)
* [C++ 的元编程](Chapters/chapter-05-CPP-Metaprogram.md)
* [杂项](Chapters/chapter-06-Misc.md)

## 其他

* 本教程的目的在于理解并思考一种可能的 C++ 的函数式编程实现，并非劝诫大家**使用**函数式 C++
* 本文同时发布在知乎上，也欢迎大家评论

## License

This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License. To view a copy of this license, visit <http://creativecommons.org/licenses/by-nc-sa/4.0/> or send a letter to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.