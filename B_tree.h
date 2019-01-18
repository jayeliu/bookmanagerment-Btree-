#ifndef BTREE_H
#define BTREE_H
#include"Books.h"
#include<cstdlib>
#include<queue>
struct Node {
	int n;
	struct Node** child;   //���ӵĵ�ַ
	binarycode* key;    //�ؼ���
	bool leaf;  //�Ƿ�ΪҶ�ӵı��
	Node(int t = 0) { n = t; child = nullptr; key = nullptr; leaf = true; }
	~Node() { n = 0; child = nullptr; key = nullptr; }
};
class BTree {
public:
	BTree(int m = 2) { t = m; root = new Node; node_num = 0; }
	~BTree() { PostOrder_Delete(root); }
	void travel() { travel_special(root, 0); };//���뷨����
	Node* B_Tree_Search(const binarycode& elem) { return search(root, elem); }  //����Ԫ��
	Node* B_Tree_Search_special(const binarycode& elem, int &loc) { return search(root, elem, loc); }
	void B_Tree_Insert(const binarycode& k) { Insert(root, k); }//����Ԫ��k
	void B_Tree_Delete(const binarycode &k);					    //ɾ��Ԫ��k
	void Print();										//�ṹ�����
	int size()const { return node_num; }                        //����Ԫ�صĸ���
private:
	void travel_special(Node *r, int num);//���뷨����
	//�ͷ�����ռ�õĿռ䣬��������������
	void PostOrder_Delete(Node*& cur);  //�������ɾ��

	//���Ҳ���
	Node* search(Node* r, const binarycode& elem);
	Node* search(Node* x, const binarycode& k, int &loc);
	//���벿��
	void B_Tree_Split_Child(Node* x, int i);//x��i�����ѱ�
	void Insert(Node* &r, const binarycode& k);//����һ��Ҫ����ָ������ã�������޷����ʵ����������
	void B_Tree_Insert_NonFull(Node* x, const binarycode& k); //����ǿ�Ԫ��

	//ɾ������
	void Merge_Node(Node*& x, int i, Node*& y, Node*& z);  //�Ͳ�x�ĵ�i���ӽ��y�͵�i+1���ӽ��z����ʱy��z��key��������t-1
	binarycode Search_Predecessor(Node* y); //����ǰ��
	binarycode Search_Successor(Node* z);   //���Һ��
	void Shift_To_Left_Child(Node*& x, int i, Node*& y, Node*& z);//�л���������
	void Shift_To_Right_Child(Node*& x, int i, Node*& y, Node*& z);//�л���������
	void B_Tree_Delete_NoNone(Node* x, const binarycode &k);//ɾ���ڲ����x��k

	//��������
	Node* root;	//�����
	int t;              //������Ĭ����2
	int node_num;       //������
};

Node* BTree::search(Node* x, const binarycode& k) {
	int i = 0;
	while (i<x->n&&k.ISBN>x->key[i].ISBN) {
		++i;
	}
	if (i < x->n&&k.ISBN == x->key[i].ISBN) {   //�ҵ��ˣ����ص�ַ
		return x;
	}
	else if (x->leaf) {        //δ�ҵ������ؿ�
		return nullptr;
	}
	else {
		return search(x->child[i], k);   //β�ݹ�����²���
	}
}

Node* BTree::search(Node* x, const binarycode& k, int &loc) {
	int i = 0;
	while (i<x->n&&k.ISBN>x->key[i].ISBN) {
		++i;
	}
	if (i < x->n&&k.ISBN == x->key[i].ISBN) {   //�ҵ��ˣ����ص�ַ
		loc = i;
		return x;
	}
	else if (x->leaf) {        //δ�ҵ������ؿ�
		return nullptr;
	}
	else {
		return search(x->child[i], k);   //β�ݹ�����²���
	}
}
void BTree::B_Tree_Split_Child(Node* x, int i) { //i��������±�
	Node* y = x->child[i];
	Node*L, *R;   //L R�����ѱ������Ƭ��
	L = new Node;
	R = new Node;
	L->n = R->n = t - 1;
	L->key = new binarycode[t - 1];
	R->key = new binarycode[t - 1];
	L->leaf = R->leaf = x->child[i]->leaf;  //�����˸��ĺ��ӵ�����
	//���Ƽ�ֵ
	for (int j = 0; j <= t - 2; ++j) {
		L->key[j] = y->key[j];
		R->key[j] = y->key[j + t];
	}
	if (!y->leaf) {
		L->child = new Node*[t];
		R->child = new Node*[t];
		for (int j = 0; j < t; ++j) {
			L->child[j] = y->child[j];
			R->child[j] = y->child[j + t];
		}
	}
	//����x
	x->key = (binarycode*)realloc(x->key, (x->n + 1) * sizeof(binarycode));
	x->child = (Node**)realloc(x->child, (x->n + 2) * sizeof(Node*));
	for (int j = x->n - 1; j >= i; --j) { //��ֵ����һλ
		x->key[j + 1] = x->key[j];
	}
	x->key[i] = y->key[t - 1];
	for (int j = x->n; j >= i + 1; --j) {
		x->child[j + 1] = x->child[j];
	}
	x->child[i] = L;
	x->child[i + 1] = R;
	x->n++;
	//����ԭ���Ŀռ�
	delete[] y->child;
	delete[] y->key;
	delete y;
}

void BTree::Insert(Node* &r, const binarycode& k) {
	if (B_Tree_Search(k)) {
		return;
	}
	else if (r != nullptr && r->n == 2 * t - 1) {
		Node* s = new Node;
		s->leaf = false;
		s->child = new Node*[1];
		s->child[0] = r;
		r = s;
		s->n = 0;
		B_Tree_Split_Child(s, 0);
		B_Tree_Insert_NonFull(s, k);
	}
	else {
		B_Tree_Insert_NonFull(r, k);
	}
	node_num++;  //����������
}

void BTree::B_Tree_Insert_NonFull(Node* x, const binarycode& k) {
	int i = x->n - 1;
	if (x->leaf) {  //Ҷ�ӽ��ֱ�Ӳ���
		x->key = (binarycode*)realloc(x->key, (x->n + 1) * sizeof(binarycode));
		while (i >= 0 && k.ISBN < x->key[i].ISBN) {
			x->key[i + 1] = x->key[i];
			--i;
		}
		x->key[i + 1] = k;
		++x->n;
	}
	else {
		while (i >= 0 && k.ISBN < x->key[i].ISBN) {
			--i;
		}
		++i;
		if (x->child[i]->n == 2 * t - 1) {
			B_Tree_Split_Child(x, i);
			if (k.ISBN > x->key[i].ISBN) {
				++i;
			}
		}
		B_Tree_Insert_NonFull(x->child[i], k);  //β�ݹ�
	}
}

void BTree::Merge_Node(Node*& x, int i, Node*& y, Node*& z) {
	y->key = (binarycode*)realloc(y->key, (2 * t - 1) * sizeof(binarycode));
	for (int j = 0; j < t - 1; ++j) {
		y->key[t + j] = z->key[j];
	}
	//����Ҷ�ӽ�������£�����Ҫ���ƺ���
	if (!y->leaf) {
		y->child = (Node**)realloc(y->child, 2 * t * sizeof(Node**));
		for (int j = 0; j < t; ++j) {
			y->child[t + j] = z->child[j];
		}
	}
	y->key[t - 1] = x->key[i];  //�ƶ�x�Ľ�㲹�䵽y
	y->n = 2 * t - 1;
	//ɾ��x�����ƹؼ���
	for (int j = i; j < x->n - 1; ++j) {
		x->key[j] = x->key[j + 1];
	}
	x->key = (binarycode*)realloc(x->key, (x->n - 1) * sizeof(binarycode));
	//ɾ��ԭ��ָ��z��ָ��
	for (int j = i + 1; j <= x->n - 1; ++j) {
		x->child[j] = x->child[j + 1];
	}
	x->child = (Node**)realloc(x->child, (x->n) * sizeof(Node**));
	x->child[i] = y;
	--x->n;
	//�ͷŵ����z
	delete[] z->key;
	delete[] z->child;
	delete z;
	z = nullptr;
}

binarycode BTree::Search_Predecessor(Node* y) {
	Node* x = y;//->child[y->n];
	while (!x->leaf) {
		x = x->child[x->n];
	}
	return x->key[x->n - 1];
}

binarycode BTree::Search_Successor(Node* z) {
	Node* x = z;//->child[0];
	while (!x->leaf) {
		x = x->child[0];
	}
	return x->key[0];
}

void BTree::Shift_To_Left_Child(Node*& x, int i, Node*& y, Node*& z) {
	if (y == nullptr) {  //��ֹ���ֿ�ָ��
		y = new Node;
	}
	++y->n;
	//���Ƽ�ֵ
	y->key = (binarycode*)realloc(y->key, y->n * sizeof(binarycode));
	y->key[y->n - 1] = x->key[i];
	x->key[i] = z->key[0];
	for (int j = 0; j < z->n - 1; ++j) {
		z->key[j] = z->key[j + 1];
	}
	//��Ҷ�ӽ�������¸��ƺ���
	if (!z->leaf) {
		y->child = (Node**)realloc(y->child, (y->n + 1) * sizeof(Node*));
		y->child[y->n] = z->child[0];
		for (int j = 0; j < z->n; ++j) {
			z->child[j] = z->child[j + 1];
		}
		z->child = (Node**)realloc(z->child, z->n * sizeof(Node*));
	}
	--z->n;
	z->key = (binarycode*)realloc(z->key, z->n * sizeof(binarycode));
}

void BTree::Shift_To_Right_Child(Node*& x, int i, Node*& y, Node*& z) {
	if (z == nullptr) { //��ֹ���ֿ�ָ��
		z = new Node;
	}
	++z->n;
	//���Ƽ�ֵ
	z->key = (binarycode*)realloc(z->key, z->n * sizeof(binarycode));
	for (int j = z->n - 1; j > 0; --j) {
		z->key[j] = z->key[j - 1];
	}
	z->key[0] = x->key[i];
	x->key[i] = y->key[y->n - 1];
	//��Ҷ�ӽ�������¸��ƺ���
	if (!z->leaf) {
		z->child = (Node**)realloc(z->child, (z->n + 1) * sizeof(Node*));
		for (int j = 0; j < z->n; ++j) {
			z->child[j + 1] = z->child[j];
		}
		z->child[0] = y->child[y->n];
		y->child = (Node**)realloc(y->child, y->n * sizeof(Node*));
	}
	--y->n;
	y->key = (binarycode*)realloc(y->key, y->n * sizeof(binarycode));
}

void BTree::B_Tree_Delete_NoNone(Node* x, const binarycode &k) {
	//�����ᵽ�ĸ������ȫ�������㷨����
	if (x->leaf) {   //���1
		int i = 0;
		while (i<x->n&&k.ISBN>x->key[i].ISBN) {
			++i;
		}
		for (int j = i + 1; j < x->n; ++j) {
			x->key[j - 1] = x->key[j];
		}
		--x->n;
		x->key = (binarycode*)realloc(x->key, x->n * sizeof(binarycode));
	}
	else {
		int i = 0;
		while (i<x->n&&k.ISBN>x->key[i].ISBN) {
			++i;
		}
		Node *y = x->child[i], *z = nullptr;
		if (i < x->n) {
			z = x->child[i + 1];
		}
		if (k.ISBN == x->key[i].ISBN) { //���2
			if (y->n > t - 1) { //���2a
				binarycode k1 = Search_Predecessor(y);
				B_Tree_Delete_NoNone(y, k1);
				x->key[i] = k1;
			}
			else if (z->n > t - 1) { //���2b
				binarycode k1 = Search_Successor(z);
				B_Tree_Delete_NoNone(z, k1);
				x->key[i] = k1;
			}
			else {  //���2c
				Merge_Node(x, i, y, z);
				B_Tree_Delete_NoNone(y, k);
			}
		}
		else { //���3
			Node* p = nullptr;
			if (i > 0) {
				p = x->child[i - 1];
			}
			if (y->n == t - 1) {
				if (i > 0 && p->n > t - 1) {  //���3a
					Shift_To_Right_Child(x, i - 1, p, y);
				}
				else if (i<x->n&&z->n>t - 1) { //���3a
					Shift_To_Left_Child(x, i, y, z);
				}
				else if (i > 0) { //���3b
					Merge_Node(x, i - 1, p, y);  //�������ϲ�
					y = p;
				}
				else {          //���3b
					Merge_Node(x, i, y, z);      //���Ҳ�ϲ�
				}
			}
			B_Tree_Delete_NoNone(y, k);
		}
	}
}

void BTree::travel_special(Node *r, int num)
{
	int j = num, m = r->n;
	if (r != NULL) {
		for (int i = m; i > 0; i--)
		{
			if (!r->leaf)travel_special(r->child[i], num + 1);
			while (j--) { std::cout << "    "; }
			cout << r->key[i - 1].ISBN << endl;
			j = num;
		}
		if (!r->leaf)travel_special(r->child[0], num + 1);
	}
}
void BTree::B_Tree_Delete(const binarycode &k) {
	Node* r = root;
	if (root->n == 1 && root->child == nullptr) {  //ɾ�����һ��Ԫ��
		delete[] root->key;
		delete root;
		root = nullptr;
	}
	else if (root->n == 1) {  //ѹ�����ڵ�
		Node *y = root->child[0], *z = root->child[1];
		if (y->n == z->n&&y->n == t - 1) {
			Merge_Node(root, 0, y, z);
			root = y;
			delete[] r->child;
			delete[] r->key;
			delete r;
			B_Tree_Delete_NoNone(y, k);
		}
		else {
			B_Tree_Delete_NoNone(root, k);
		}
	}
	else {
		B_Tree_Delete_NoNone(r, k);
	}
	--node_num;  //����������һ��
}

void BTree::Print() {
	Node *last = root;//last��ʾ��ǰ������ҽ��
	Node* p = root;
	std::queue<Node*>Q;
	if (p) {
		Q.push(p);
	}
	while (!Q.empty()) {
		Node* tmp = Q.front();
		Q.pop();
		if (!tmp->leaf) {
			std::cout << "|";
		}
		std::cout << "(";
		for (int i = 0; i < tmp->n; ++i) {
			std::cout << "���:" << tmp->key[i].ISBN << endl;
			std::cout << "����:" << tmp->key[i].ptr->name << endl;
			std::cout << "����:" << tmp->key[i].ptr->author << endl;
			std::cout << "�ִ���:" << tmp->key[i].ptr->currentSize << endl;
			std::cout << "�ܴ���:" << tmp->key[i].ptr->allSize << endl;
			if (tmp->child&&i != tmp->n - 1) {
				std::cout << "|";
			}
		}
		std::cout << ")";
		if (!tmp->leaf) {   //����Ҷ�ӽ����к���
			std::cout << "|";
			for (int i = 0; i < tmp->n + 1; ++i) {
				Q.push(tmp->child[i]);
			}
		}
		if (!last->leaf&&tmp == last) {
			std::cout << std::endl;
			last = last->child[last->n];
		}
	}
	std::cout << std::endl;
}

void BTree::PostOrder_Delete(Node*& cur) {
	if (cur->leaf) {  //ֱ��ɾ��Ҷ�ӽ��
		delete[] cur->key;
		delete cur;
		cur = nullptr;
	}
	else { //�ݹ��ɾ��ÿ������
		for (int i = 0; i < cur->n + 1; ++i) {
			PostOrder_Delete(cur->child[i]);
		}
		delete[] cur->key;
		delete cur;
		cur = nullptr;
	}
}

#endif
