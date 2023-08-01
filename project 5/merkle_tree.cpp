// ����ʵ��
// ���� Merkle Tree, ��ʵ�ֶ�һ�λ��Ĵ洢��
// �� Hello, This Is Cielle.
// һ����`Hello` + `,` + `This` + `Is` + `Cielle` + `.` Ϊ�������ִ���     

#include <stdio.h>
#include <stdlib.h>
#include"sha256.h"


// Merkle Tree �ṹ�嶨�� 
typedef struct MerkleTreeNode {
	struct MerkleTreeNode* left;		// ���ӽڵ� 
	struct MerkleTreeNode* right;		// ���ӽڵ� 
	struct MerkleTreeNode* parent;		// ���ڵ� 
	uint_32 level;							// ��ǰ����ײ�Ϊ0 
	unsigned int data[8];							// ��ǰ�ڵ��ֵ����ײ�Ϊԭʼ���ݣ����Ͼ�Ϊ����Ĺ�ϣֵ 
}MerkleTree;

// ����꺯��������һ�� Merkle Tree�ڵ�
#define New_Merkle_Node(mt, tree_depth) {	\
	mt = (MerkleTree *)malloc(sizeof(MerkleTree)); \
	mt->left = NULL; \
	mt->right = NULL; \
	mt->parent = NULL; \
	mt->level = (uint_32)tree_depth; \
	mt->data[0] = 0;	\
	mt->data[1] = 0;	\
	mt->data[2] = 0;	\
	mt->data[3] = 0;	\
	mt->data[4] = 0;	\
	mt->data[5] = 0;	\
	mt->data[6] = 0;	\
	mt->data[7] = 0;	\
	}

// ��ӡ Merkle tree������������ʽ 
void Print_Merkle_Tree(MerkleTree* mt, int high)
{
	MerkleTree* p = mt;
	int i;

	if (p == NULL)
		return;

	if (p->left == NULL && p->right == NULL) {
		for (i = 0; i < high - p->level; i++)
			printf("           ");

		printf("------>%-6d\n", p->data);
	}
	else {
		Print_Merkle_Tree(mt->left, high);
		printf("\n");
		for (i = 0; i < high - p->level; i++)
			printf("           ");

		printf("------>%-6d\n", p->data);

		Print_Merkle_Tree(mt->right, high);
	}
}


// ��������������hash ֵ 
void hash_two_node(uint_256 num1, uint_256 num2,uint_256 data)
{
	uint_8 tmp[128];

	if (num1 == 0 && num2 != 0)//01
	{
		for (int i = 0; i < 64; i++)
		{
			tmp[i] = 0;
		}
		memcpy(&(tmp[64]), num2, 8 * (sizeof(unsigned int)));
	}
	else if (num1 != 0 && num2 == 0)//10
	{
		memcpy(&(tmp[0]), num1, 8 * (sizeof(unsigned int)));
		for (int i = 64; i < 128; i++)
		{
			tmp[i] = 0;
		}
	}
	else if (num1 == 0 && num2 == 0)//00
	{
		for (int i = 0; i < 128; i++)
		{
			tmp[i] = 0;
		}
	}
	else//11
	{
		memcpy(&(tmp[0]), num1, 8 * (sizeof(unsigned int)));
		memcpy(&(tmp[64]), num2, 8 * (sizeof(unsigned int)));
	}
	
	sha256_do(tmp, data, 1024);
}

// �������������ҵ����һ��Ҷ�ӽڵ㣬������ʽΪǰ��������ʽ 
MerkleTree* Find_Last_Node(MerkleTree* mt) {
	MerkleTree* p = mt, * tmp;

	// ��������ӽڵ㶼��NULL���϶���Ҷ�ӽڵ㣬ֱ�ӷ��� 
	if (p->left == NULL && p->right == NULL)
		return p;
	// ��� ���ӽڵ�Ϊ�գ����ӽڵ㲻Ϊ�գ������������ӽڵ㣬ֱ��Ҷ�ӽڵ� 
	else if (p->right == NULL && p->left != NULL)
		return Find_Last_Node(p->left);
	// ��� ���ӽڵ㲻Ϊ�գ��������ӽڵ� 
	else if (p->right != NULL)
		return Find_Last_Node(p->right);
}

// �������һ���ڵ㣬�ҵ������λ�� 
MerkleTree* Find_Empty_Node(MerkleTree* mt) {
	MerkleTree* p = mt->parent;

	// �����ҵ���һ�����ڿ�ȱ�Ľڵ㣬ֱ�����ڵ� 
	while (p->left != NULL && p->right != NULL && p->parent != NULL)  p = p->parent;

	// ��� p ָ�򶥽ڵ㣬˵����ǰ���������� ������ NULL 
	if (p->parent == NULL && p->left != NULL && p->right != NULL)
		return NULL;
	// ����ǰ�ڵ����Ҫ����ڵ��λ�� 
	else
		return p;
}

// Merkle tree ��ʼ�� (�ݹ�ʵ��)
MerkleTree* Creat_Merkle_Tree(MerkleTree* mt,unsigned int** arr, int nums)
{
	MerkleTree* node, * tmp, * p;
	int i;

	// 1. nums ����0ʱ�����������ϣ���ʱ����merkle treeͷ��� 
	if (nums == 0) {

		printf("Merkle Tree ������ϣ�����\n");
		return mt;
	}
	else {

		// 2. ����һ��Ҷ�ڵ� 
		New_Merkle_Node(node, 0);
		memcpy(node->data , arr[nums-1],32);
		printf("�½�Ҷ�ڵ� [%d] tree_depth=%d, level=%d, data=%-6d, nums=%d, \n", __LINE__, mt == NULL ? 0 : mt->level, node->level, node->data, nums);

		// 3. ��� mt Ϊ�գ�˵����ǰû��������Ҫ�½�һ��ͷ��� 
		if (mt == NULL) {

			// 3.1 ����ͷ���
			New_Merkle_Node(mt, 1);
			mt->left = node;
			node->parent = mt;
			printf("�½�ͷ�ڵ� [%d] tree_depth=%d, level=%d, data=%-6d \n", __LINE__, mt->level, mt->level, mt->data);

			// 3.2 ����ͷ����ϣֵ 
			hash_two_node(mt->left->data, mt->right == NULL ? 0 : mt->right->data, mt->data);

			// 3.3 �ݹ������һ��ֵ 
			mt = Creat_Merkle_Tree(mt, &arr[1], nums - 1);
		}
		// 4.��� mt ��Ϊ�գ�˵����ǰ�Ѿ������� 
		else
		{
			// 5. ������ǰ�����ҵ�һ���յ�Ҷ�ӽڵ㣬��������ʱ����NULL 
			p = Find_Empty_Node(Find_Last_Node(mt));

			// 6. �������ֵ��Ϊ NULL, ˵���Ѿ��ҵ���Ҫ����Ľڵ� 
			if (p != NULL) {
				// 6.1 �������¾���Ҷ�ӽڵ㣬��ֱ�Ӹ�ֵ right
				if (p->left->left == NULL && p->right == NULL)
				{
					p->right = node;
					node->parent = p;
					// ���¹�ϣֵ 
					hash_two_node(p->left->data, p->right == NULL ? 0 : p->right->data, p->data);
				}
				// 6.2 ������ص��е����м�ڵ� 
				else
				{
					// 6.2.1 �����м�ڵ㣬�½�һ�� right �ӽڵ�         
					i = p->level - 1;
					New_Merkle_Node(tmp, i);
					p->right = tmp;
					tmp->parent = p;
					printf("���м�ڵ� [%d] tree_depth=%d, level=%d, data=%-6d \n", __LINE__, mt->level, tmp->level, tmp->data);

					// 6.2.2 ����ָ��p��ָ��ǰ�½��� right �ӽڵ㣬�Ӵ˿�ʼ��ֻ��Ҫѭ����� left�ӽڵ�Ϳ����� 
					p = p->right;
					i = p->level - 1;		// ����level - 1 

					while (i > 0) {
						// �����м�ڵ� 
						New_Merkle_Node(tmp, i);
						p->left = tmp;
						tmp->parent = p;
						printf("���м�ڵ� [%d] tree_depth=%d, level=%d, data=%-6d \n", __LINE__, mt->level, tmp->level, tmp->data);

						p = p->left;
						i--;
					}

					// 6.2.3 ��ǰ p �ڵ�ָ����� leven=1����ʱ�����Ҷ�ӽڵ��� 
					p->left = node;
					node->parent = p;

					// 6.2.4 �Ե����ϸ��¹�ϣֵ
					while (p != mt) {
						hash_two_node(p->left->data, p->right == NULL ? 0 : p->right->data, p->data);
						p = p->parent;
					}
					// 6.2.5 ���¸��ڵ��ϣֵ  
					hash_two_node(p->left->data, p->right == NULL ? 0 : p->right->data, p->data);

				}
				// 6.3 �ڵ����ɹ����ݹ������һ��ֵ  
				mt = Creat_Merkle_Tree(mt, &arr[1], nums - 1);
			}
			// 7. ���û�пյĽڵ㣬˵����ǰ��������������Ҫ����ͷ�ڵ��ˣ�levelҲҪ��1 
			else
			{
				tmp = mt;				// ���浱ǰͷ���

				// 7.1 ����һ���µ�ͷ�ڵ㣬 ���߶� +1 
				New_Merkle_Node(mt, tmp->level + 1);
				mt->left = tmp; 		// ͷ�ڵ㸳ֵ 
				tmp->parent = mt;
				printf("�½�ͷ�ڵ� [%d] tree_depth=%d, level=%d, data=%-6d \n", __LINE__, mt->level, mt->level, mt->data);

				// 7.2 ����ͷ�ڵ��µĵ�һ�� right �ӽڵ� 
				New_Merkle_Node(tmp, mt->level - 1);
				mt->right = tmp;
				tmp->parent = mt;
				printf("���м�ڵ� [%d] tree_depth=%d, level=%d, data=%-6d \n", __LINE__, mt->level, tmp->level, tmp->data);

				p = mt->right;
				i = p->level - 1;

				// ����������ȴ���ͬ����ȵ����� 
				while (i > 0) {
					// �������
					New_Merkle_Node(tmp, i);
					p->left = tmp;
					tmp->parent = p;
					printf("���м�ڵ� [%d] tree_depth=%d, level=%d, data=%-6d \n", __LINE__, mt->level, tmp->level, tmp->data);

					p = p->left;
					i--;
				}
				// Ҷ�ӽڵ㸳ֵ 
				p->left = node;
				node->parent = p;

				// �Ե����ϸ��¹�ϣֵ
				while (p != mt) {
					hash_two_node(p->left->data, p->right == NULL ? 0 : p->right->data, p->data);
					p = p->parent;
				}
				// ���¸��ڵ��ϣֵ  
				hash_two_node(p->left->data, p->right == NULL ? 0 : p->right->data, p->data);

				// �ݹ���� 
				mt = Creat_Merkle_Tree(mt, &arr[1], nums - 1);
			}
		}
	}
}

// ��� Merkle tree, �������з�����ڴ� 
void Delete_Merkle_Tree(MerkleTree* mt)
{
	// �����Ҷ�ӽڵ㣬ֱ�ӻ��� 
	if (mt->level == 0) {
		printf("����Ҷ�ӽڵ㣬level=%d, data=%d \n", mt->level, mt->data);
		free(mt);
	}
	else {
		if (mt->left != NULL) {
			Delete_Merkle_Tree(mt->left);
		}
		if (mt->right != NULL) {
			Delete_Merkle_Tree(mt->right);
		}
		// �ͷŵ�ǰ�ڵ�
		printf("�����м�ڵ㣬level=%d, data=%d \n", mt->level, mt->data);
		free(mt);
	}
}

int main(void)
{
	unsigned int** array = (unsigned int**)malloc(12 * sizeof(unsigned int*));
	for (int i = 0; i < 12; i++)
	{
		array[i] = (unsigned int*)malloc(32);
		array[i][0] = array[i][1] = array[i][2] = array[i][3] = array[i][4] = array[i][5] = array[i][6] = array[i][7] = i;
	}
	MerkleTree* mt = NULL;
	// �������鶯̬���� Merkle Tree 
	mt = Creat_Merkle_Tree(mt, array, 12);

	// ��ӡ��ǰmerkle Tree
	if (mt != NULL) {
		printf("\n��ʼ��ӡ��ǰ Merkle ��:\n");
		Print_Merkle_Tree(mt, mt->level);
		printf("\n\n");
	}

	// �����ڴ�
	Delete_Merkle_Tree(mt);

	return 0;
}

