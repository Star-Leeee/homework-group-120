# Project16: implement sm2 2P decrypt with real network communication

![sm2——dec](https://github.com/Star-Leeee/homework-group-120/assets/139939885/95562289-357f-48bd-9bf1-10a7a32a3fc8)
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

实验结果

![server](https://github.com/Star-Leeee/homework-group-120/assets/139939885/989550ad-5820-45ef-96d7-837479c69d23)
![client](https://github.com/Star-Leeee/homework-group-120/assets/139939885/aaba4c70-a349-4fbf-b80e-87c81b784f8a)

