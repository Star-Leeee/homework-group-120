# Project5：Impl Merkle Tree following RFC6962  
**主要做的工作**：根据RFC6962对于Merkle Tree的设计，实现了Merkle Tree的构建，利用Merkle Audit Paths实现Inclusion proof for specified element和Inclusion proof for specified element;实现Merkle Consistency Proofs；测试程序在进行Inclusion Proof时的效率。   
***
## 实验说明
### 构建方式：
不同于普遍的Merkle Tree设计从叶子结点开始两两组合生成Merkle Root，RFC6962要求构建从Root开始递归构建二叉树  
<img width="449" alt="image" src="https://github.com/Dianyudengdeng/homework-group-113/assets/93588357/450525be-db20-43b5-af64-21e92c02819a">  
例如添加五个结点：  
<img width="295" alt="image" src="https://github.com/Dianyudengdeng/homework-group-113/assets/93588357/8641d4d4-c5c5-419d-9dfd-5ff1b99fbc16">
<img width="723" alt="image" src="https://github.com/Dianyudengdeng/homework-group-113/assets/93588357/13ba8d8a-2fc0-4a86-b45a-ddb7e7ee8763">  
从计算hash值的过程中可以看出构建的树结构是相同的。  
### 数据存储：
在生成树的过程中，使用散列表MTH以存储内部结点的Hash值，注意这里为了演示存储了叶子结点的Hash值，实际运行中并不会存储。因为在进行Inclusion/Exclusion proof证明时，只会用到已经获取的叶子结点的Hash值，所以并不会产生影响。  
### 具体实现：
根据RFC6962，调整索引设计得到的的Audit Path与Subproof： 
#### Audit Path: 
```c++
string tree::Audit_Path(int index, int start, int end) {//递归构建审计路径
	if (start == end - 1) {
		string concatenated = to_string(start) + "-" + to_string(end);
		cout << "审计路径包含的内容:" << concatenated << endl;
		return MTH[concatenated];
	}
	else {
		int k = 1;
		while (k < end - start) {
			k <<= 1; 
		}
		k >>= 1;
		if (index < k) {
			string concatenated =to_string(start + k) + "-" + to_string(end-start);
			cout << "审计路径包含的内容:" << concatenated << endl;
			return sha256("0x01" + Audit_Path(index, start, start+k)+ MTH[concatenated]);
		}
		else {
			string concatenated = to_string(start) + "-" + to_string(start+k);
			cout << "审计路径包含的内容:" << concatenated << endl;
			return sha256("0x01"+MTH[concatenated]+ Audit_Path(index, start + k, end));
		}
	}
}
```
#### Subproof:
```c++
string tree::SUBPROOF(int index,int start,int end,bool TF) {
	int n = end - start;
	if (index == n) {
		if (TF)
			return this->merkleRoot->getHash();//根哈希已知
		else {
			string concatenated = to_string(0) + "-" + to_string(index);
			return MTH[concatenated];
		}
	}
	else {
		int k = 1;
		while (k < n) {
			k <<= 1;
		}
		k >>= 1;
		if (index <= k) {
			string concatenated = to_string(index) + "-" + to_string(n);
			return sha256("0x01" + SUBPROOF(index, 0, k, TF) + MTH[concatenated]);
		}
		else {
			string concatenated = to_string(0) + "-" + to_string(k);
			return sha256("0x01" + SUBPROOF(index-k, k, n, TF) + MTH[concatenated]);
		}
	}

}
```
而后的Inclusion/Exclusion proof都是基于Audit Path计算根节点索引值与MerkleRoot得到的结果相比进行判断。    
## 运行结果
添加十万个结点后进行InclusionProof：  
<img width="722" alt="image" src="https://github.com/Dianyudengdeng/homework-group-113/assets/93588357/80c29ab5-7c97-42d0-b5cf-2d312e7a22fb">  
<img width="722" alt="image" src="https://github.com/Dianyudengdeng/homework-group-113/assets/93588357/c26c387b-911d-4215-9d72-4f48a6e59e74">  
对比可知，由于Merkle Tree的设计，InclusionProof的时间复杂度为O(log n)，所以在结点数量差异较大的情况下，尽管可以直观感受到树构建过程的时间差异，InclusionProof的时间开销并没有较大的差异。
