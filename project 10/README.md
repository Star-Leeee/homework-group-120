# Project10: report on the application of this deduce technique in Ethereum with ECDSA
## ECDSA算法描述
### 算法概述
ECDSA即椭圆曲线数字签名算法，是使用椭圆曲线密码（ECC）对数字签名算法（DSA）的模拟。ECDSA于1999年成为ANSI标准，并于2000年成为IEEE和NIST标准。

与普通的离散对数问题（discrete logarithm problem DLP）和大数分解问题（integer factorization problem IFP）不同，椭圆曲线离散对数问题（elliptic curve discrete logarithm problem ECDLP）没有亚指数时间的解决方法。因此椭圆曲线密码的单位比特强度要高于其他公钥体制。

椭圆曲线密码体制的安全性基于椭圆曲线离散对数问题（ECDLP）的难解性。椭圆曲线离散对数问题远难于离散对数问题，椭圆曲线密码系统的单位比特强度要远高于传统的离散对数系统。因此在使用较短的密钥的情况下，ECC可以达到于DL系统相同的安全级别。这带来的好处就是计算参数更小，密钥更短，运算速度更快，签名也更加短小。因此椭圆曲线密码尤其适用于处理能力、存储空间、带宽及功耗受限的场合。

### 优势
1. 通过公钥推导技术，以太坊交易中包含签名和原始交易数据，但不包含公钥信息。可以在验证交易时只传输签名和原始数据，而无需传输公钥，从而减少交易数据量，节省网络带宽和存储空间。
2. 以太坊是一个高交易量的区块链网络，传统的公钥验证需要从区块链中查询相应的公钥信息，使用公钥推导技术验证方可以通过签名和原始数据来推导出签名者的公钥，无需额外的公钥查询操作，减少了交易验证的时间复杂度，同时节省了区块链访问带来的计算开销。
3. 以太坊交易中的公钥是与账户地址相关联的，公钥推导技术可以在验证交易的过程中避免公开传输公钥信息，从而提高交易的隐私性和匿名性。
