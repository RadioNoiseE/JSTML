# *J*ing's *S*tructured *T*ext *M*arkup *L*anguage

## 这是什么
一个简单的parser，将特定的（当然、超简单［甚至都没有嵌套］）的文本文件转义至TeX，并定义一个超简单的TeX Style Class来处理它。<br/>
基于最简单的下推自动机（对、甚至都不是递归下降、因为实在太简单了没这个必要）。<br/>
用我蹩脚的C实现，用Lua来完成自动化处理全班的文本信息。<br/>

## 开发状态
主体算是完成了，把项目clone下来make一下就行了（默认用的是clang、gcc的话改一下makefile）。<br/>
文档在写（其实还没开始），Lua的自动小脚本写完了（怎么用往后看）。<br/>
不积极开发，看心情（呸、是时间）。<br/>

## 使用方法
目前就是make完之后只要没出大问题总归会生成一个可执行文件是吧，运行它就好。<br/>
语法就是`./jstml [-o 输出文件名] 输入文件名`，如果不填输入文件名就会有个默认的与输入同名但扩展名是`.tex`的东西。<br/>
至于这个孤寡TeX文件怎么处理，就得等到我写完TeX的样式文件了。<br/>
顺便说一句，这个parser处理的文件照理来说扩展名应该是`.jtm`，但这无所谓了，高兴就好。<br/>
语法等文档，但其实真的很简单，大概五句话就结束了；但是希望我的同学能会用、老师能会用？<br/>

## 自动化小脚本使用指北
用法很简单，也没有与用户的交互，只需要创建一个文件然后运行它就好。<br/>
你需要创建一个叫`jsindex.ind`的文件，里面要写你需要处理的文件（带后缀名）。<br/>
照理来说一行写一个，然后换行；但是你要用逗号分隔也没问题：因为原理就是把换行符`\n`映射到`,`，再添上头和尾变成Lua的表格。<br/>
然后运行就好了。等我把TeX Style Driver文件写完了那么运行完应该就会出现你要的PDF文件了。<br/>
很遗憾我还没写完，所以现在运行只会报错。

## 小心黑魔法
呃，我已经尽力了，处理函数大概用的都是`strn…()`系列的，但处理文件扩展名的用的就是邪恶的不带`n`的系列，如果可能会报「分段错误」。<br/>
这个parser不具备任何教育价值，参考价值；或者说，除了能让他们写同学录没别的价值。<br/>
你去看看源文件就会发现各种随意且风骚的函数、变量、宏……<br/>
指针乱指、数组乱搞、地址引用都出现了，完全就是瞎写一通然后根据编译器报错乱改出来的东西。<br/>
~~而且一个这么小的东西我分成了7、8个源文件，作用域、生存期这些不要去深究。~~<bf/>
第二版本将其合并为一个文件，详细原因见该[issue#657: Clang hangs after "segmentation fault"](https://github.com/holzschu/a-shell/issues/657)（主要是iOS没有`os.fork()`函数导致clang在编译多文件项目时会挂）。：

## 关于可执行文件
克隆仓库后会发现里面有一个叫`jstml.wasm`的可执行文件。<br/>
这个当然是web assembly的格式了，是我用iPad编译所得。<br/>
编译器是：`clang -cc1 version 14.0.0 based upon LLVM 14.0.0git default target arm64-apple-darwin22.2.0`。<br/>
那链接器当然就是wasm32了，参数如下：
```
-cc1 -triple wasm32-unknown-wasi -emit-obj -mrelax-all --mrelax-relocations -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name JSTML-WASM.c -mrelocation-model static -mframe-pointer=none -ffp-contract=on -fno-rounding-math -mconstructor-aliases -target-cpu generic -fvisibility hidden -debugger-tuning=gdb -target-linker-version 14.0.0
```

## `\expandafter`
```shell
clang tokbuf.c creattok.c scantok.c asrktp.c jstparse.c textokenize.c jstml.c -o jstml.wasm
segmentation fault
segmentation fault
segmentation fault
segmentation fault
segmentation fault
segmentation fault
segmentation fault
*** Error code 1
Stop.
```
