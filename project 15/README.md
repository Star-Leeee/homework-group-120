# Project15: implement sm2 2P sign with real network communication

![SM2 SIGN](https://github.com/Star-Leeee/homework-group-120/assets/139939885/6701093f-f31b-4f1f-b01f-2c1f85501285)

左边为client端，右边为server端

client端进行补步骤为:

- 建立连接
- Gen_Key()
- Gen_Q1_e(Z, M)
- Sign(d1, k1, r, s2, s3)

server端进行的步骤为:

- 建立连接
- Gen_Key(P1)
- Gen_r_s2_s3(d2, Q1, e)
