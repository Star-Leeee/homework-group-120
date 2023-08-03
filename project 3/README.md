# PROJECT 3 implement length extension attack for SM3, SHA256, etc.

实现方式：C语言
 
实现平台<br>
|CPU|RAM|
|---|---|
|Intel Core i7-8550U|hynix DDR4 2400Mhz 8Gib|

攻击原理<br>
一篇详细的阐述👉[Everything you need to know about hash length extension attacks](https://www.skullsecurity.org/2012/everything-you-need-to-know-about-hash-length-extension-attacks)<br>
MD结构的哈希函数，当message经过padding后，再按一定***长度***迭代加密，譬如512bit，如此在***message||padding***后添加***faker***消息，得到***messa||padding||faker***，这样的结构将会发生加密faker段时，输入IV为Hash(message_padding),而这是我们可知的。需要**注意**的是，***faker***不仅仅包含我们的***faker_message***，也包含一个***fake_padding***,此***fake_padding***是由***messa||padding||faker_message***计算得到的。<br>

攻击结果👇
![攻击结果——扩展](https://github.com/Star-Leeee/homework-group-120/assets/139939885/147d1f35-0d60-4a81-8160-906b58146f4e)<br>
在随机生成的**message**后扩展了**ihateyou**

SHA256与SM3的***padding***完全一致，因此长度扩展攻击的实现也完全一样，尤其是我按照相同接口实现了两个密码算法。
