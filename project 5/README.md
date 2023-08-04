# Project5：Impl Merkle Tree following RFC6962  

## 实现方式
根据RFC6962对于Merkle Tree的设计，实现了Merkle Tree的构建，利用***Evidence***和***Verify***实现Inclusion proof and Exclusion proof for specified element

特别的，RFC6962要求构建从Root开始递归构建二叉树  
<img width="449" alt="RFC" src="https://github.com/Star-Leeee/homework-group-120/assets/139939885/dda1d0ca-d48e-41aa-846e-eb88e8c49b32">

初始化一个二维列表用于存放我们的Merkel tree，计算树的深度和叶子节点的个数，接着计算数据哈希值并写入叶子节点；每两个子节点计算相加后的哈希值并写入父节点列表。 而对于同一层的节点可以重复调用这个function（过程），生成下一层（父节点层）Merkle树的节点；每层向上生成父节点的时候，需要讨论对于节点数为奇数的层的最后一个节点，直接写入下一层（父节点层）；节点数为偶数则正好配对完全，进行递归步骤(3)和(4)的过程，循环步骤(1)计算的树的深度，完成Merkle树的生成过程；

## 具体实现
```python
def Evidence(m,Tree):
    h = (hashlib.sha256(m.encode())).hexdigest()
    try:
        n=Tree[0].index(h)
    except:
        print("The leafnode is not in the tree")

    Depth = len(Tree)
    Evidence = []
    for d in range(0,Depth):
        if n%2==0:
            if n == len(Tree[d]) - 1:
                pass
            else:
                Evidence.append([Tree[d][n],Tree[d][n+1]])
        else:
            Evidence.append([Tree[d][n-1], Tree[d][n]])

        n = math.floor(n/2)

    Evidence.append([Tree[-1][0]])

    return Evidence

def Verify(m,Evidence,Top):
    h = (hashlib.sha256(m.encode())).hexdigest()
    if h != Evidence[0][0] and h != Evidence[0][1]:
        return False

    if Evidence[-1][0] != Top:
        return False

    Depth = len(Evidence)
    for i in range(0,Depth-1):
        node = (hashlib.sha256(Evidence[i][0].encode() + Evidence[i][1].encode())).hexdigest()
        if node != Evidence[i+1][0] and node != Evidence[i+1][1]:
            return False

    if (hashlib.sha256(Evidence[-2][0].encode() + Evidence[-2][1].encode())).hexdigest() != Evidence[-1][0]:
        return False

    return True
```
 
## 运行结果
添加十万个结点后进行InclusionProof：  
![evidence](https://github.com/Star-Leeee/homework-group-120/assets/139939885/97480e0c-f599-43e6-be63-159663c983be)

