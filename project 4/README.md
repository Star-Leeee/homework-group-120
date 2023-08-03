# do your best to optimize SM3 implementation (software)

实现方式：利用SIMD并行优化，调整算法优化IO

实现平台<br>
|CPU|RAM|
|---|---|
|Intel Core i7-8550U|hynix DDR4 2400Mhz 8Gib|

SM3主要由消息扩展与迭代压缩构成<br>
![消息扩展](https://github.com/Star-Leeee/homework-group-120/assets/139939885/c8b83aaa-6355-439d-ad48-98138f0979d6)<br>👆消息扩展<br>
![压缩函数](https://github.com/Star-Leeee/homework-group-120/assets/139939885/b2e0e63f-3503-444c-821f-223085e09973)<br>👆压缩函数<br>
具体算法描述参见👉[SM3密码杂凑算法](https://www.oscca.gov.cn/sca/xxgk/2010-12/17/1002389/files/302a3ada057c4a73830536d03e683110.pdf)<br>
此外，我发现了目前公开的标准文档中的示例的一个填充错误![error](https://github.com/Star-Leeee/homework-group-120/assets/139939885/f17ab89e-a2f9-4394-ab31-a1621b28fbbb)<br>👆红线处应为***00***


具体优化方法<br>
1. SIMD优化消息扩展过程
消息扩展具有一定并行性，利于SIMD方法优化，具体使用AVX指令实现<br>，参考👉[Intel® Intrinsics Guide](https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html)实现
![消息扩展SIMD](https://github.com/Star-Leeee/homework-group-120/assets/139939885/7d137ab7-7d51-4f66-a3d1-4436c5f606cf)
2. 优化迭代压缩过程IO
    - 压缩函数的快速实现可以从结构调整、流程变更、常数计算等方面着手。
      压缩函数的结构可以做适当的调整。压缩函数每一轮的最末会执行如下所示的循环右移，A‖B‖C‖D←(A‖B‖C‖D)>>>32,E‖F‖G‖H←(E‖F‖G‖H)>>>32。为了减少循环移位导致的不必要的赋值运算，可以将字的循环右移变更每轮输入字顺序的变动，且这个顺序变动会在4轮后还原，具体情况如下(以下用OneRound(·)表示一轮压缩)：
        ```C
        OneRound(i+0,A,B,C,D,E,F,G,H,W)
        OneRound(i+1,D,A,B,C,H,E,F,G,W)
        OneRound(i+2,C,D,A,B,G,H,E,F,W)
        OneRound(i+3,B,C,D,A,F,G,H,E,W)
        ```
      我在实现时使用了avx指令 ***_mm256_permutevar8x32_epi32***
    - 可以优化压缩函数的中间变量的生成流程。此优化生成流程可以去除不必要的赋值，减少中间变量个数。
    - 利用上述调整以及消息扩展部分的调整可以将原来计算TT1、TT2、D和H的过程进行如下的进一步简化。
    - 预先计算并存储常数ti=Ti<<<i。这可以避免每个消息分组都去计算常数，且占用的存储空间也很少，仅256 Byte。
    
    优化后的算法将消息扩展和压缩函数结合在一起。
    
    具体实现方法参考👉[***SM3杂凑算法的软件快速实现研究***](http://html.rhhz.net/tis/html/20150620.htm#outline_anchor_15)。

4. 优化结果
    |优化前|优化后|提升|
    |:----:|:----:|:----:|
    |220s|26s|88%|
   
    在
    |CPU|RAM|
    |---|---|
    |Intel Core i7-8550U|hynix DDR4 2400Mhz 8Gib|  
吞吐量达到0.59GiB/s
