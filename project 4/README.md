# do your best to optimize SM3 implementation (software)

实现方式：利用SIMD并行优化，调整算法优化IO

实现平台<br>
|CPU|RAM|
|---|---|
|Intel Core i7-8550U|hynix DDR4 2400Mhz 8Gib|

SM3主要由消息扩展与迭代压缩构成<br>
![消息扩展](https://github.com/Star-Leeee/homework-group-120/assets/139939885/c8b83aaa-6355-439d-ad48-98138f0979d6)<br>消息扩展<br>
![压缩函数](https://github.com/Star-Leeee/homework-group-120/assets/139939885/b2e0e63f-3503-444c-821f-223085e09973)<br>压缩函数<br>
具体算法描述参见[SM3密码杂凑算法](https://www.oscca.gov.cn/sca/xxgk/2010-12/17/1002389/files/302a3ada057c4a73830536d03e683110.pdf)

1. SIMD优化消息扩展过程
消息扩展具有一定并行性，利于SIMD方法优化，具体使用AVX指令实现<br>
![消息扩展SIMD](https://github.com/Star-Leeee/homework-group-120/assets/139939885/7d137ab7-7d51-4f66-a3d1-4436c5f606cf)
2. 优化迭代压缩过程IO
压缩函数的优化可以从结构调整、流程变更、常数计算等方面着手。譬如不再W′i则使用W′i=Wi⊕Wi+4代替，同时在迭代压缩过程中再计算Wi，调整压缩过程以减少中间量个数，赋值、异或等操作次数，都将优化IO与计算效率，但优化IO的提升更为明显，具体方法参考[SM3杂凑算法的软件快速实现研究](http://html.rhhz.net/tis/html/20150620.htm#outline_anchor_15)。
3. 优化结果
    |优化前|优化后|提升|
    |:----:|:----:|:----:|
    |220s|26s|88%|
