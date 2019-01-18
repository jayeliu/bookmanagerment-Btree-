#ifndef BTREE_H
#define BTREE_H
#include"Books.h"
#include<cstdlib>
#include<queue>
struct Node {
	int n;
	struct Node** child;   //孩子的地址
	binarycode* key;    //关键字
	bool leaf;  //是否为叶子的标记
	Node(int t = 0) { n = t; child = nullptr; key = nullptr; leaf = true; }
	~Node() { n = 0; child = nullptr; key = nullptr; }
};
class BTree {
public:
	BTree(int m = 2) { t = m; root = new Node; node_num = 0; }
	~BTree() { PostOrder_Delete(root); }
	void travel() { travel_special(root, 0); };//凹入法便历
	Node* B_Tree_Search(const binarycode& elem) { return search(root, elem); }  //查找元素
	Node* B_Tree_Search_special(const binarycode& elem, int &loc) { return search(root, elem, loc); }
	void B_Tree_Insert(const binarycode& k) { Insert(root, k); }//插入元素k
	void B_Tree_Delete(const binarycode &k);					    //删除元素k
	void Print();										//结构化输出
	int size()const { return node_num; }                        //返回元素的个数
private:
	void travel_special(Node *r, int num);//凹入法便历
	//释放整个占用的空间，给析构函数调用
	void PostOrder_Delete(Node*& cur);  //后根次序删除

	//查找部分
	Node* search(Node* r, const binarycode& elem);
	Node* search(Node* x, const binarycode& k, int &loc);
	//插入部分
	void B_Tree_Split_Child(Node* x, int i);//x的i孩子裂变
	void Insert(Node* &r, const binarycode& k);//这里一点要传入指针的引用，否则会无法完成实际树的增长
	void B_Tree_Insert_NonFull(Node* x, const binarycode& k); //插入非空元素

	//删除部分
	void Merge_Node(Node*& x, int i, Node*& y, Node*& z);  //和并x的第i个子结点y和第i+1个子结点z，此时y和z的key个数都是t-1
	binarycode Search_Predecessor(Node* y); //查找前驱
	binarycode Search_Successor(Node* z);   //查找后继
	void Shift_To_Left_Child(Node*& x, int i, Node*& y, Node*& z);//切换到左子树
	void Shift_To_Right_Child(Node*& x, int i, Node*& y, Node*& z);//切换到右子树
	void B_Tree_Delete_NoNone(Node* x, const binarycode &k);//删除内部结点x的k

	//基础数据
	Node* root;	//根结点
	int t;              //度数，默认是2
	int node_num;       //结点个数
};

Node* BTree::search(Node* x, const binarycode& k) {
	int i = 0;
	while (i<x->n&&k.ISBN>x->key[i].ISBN) {
		++i;
	}
	if (i < x->n&&k.ISBN == x->key[i].ISBN) {   //找到了，返回地址
		return x;
	}
	else if (x->leaf) {        //未找到，返回空
		return nullptr;
	}
	else {
		return search(x->child[i], k);   //尾递归的向下查找
	}
}

Node* BTree::search(Node* x, const binarycode& k, int &loc) {
	int i = 0;
	while (i<x->n&&k.ISBN>x->key[i].ISBN) {
		++i;
	}
	if (i < x->n&&k.ISBN == x->key[i].ISBN) {   //找到了，返回地址
		loc = i;
		return x;
	}
	else if (x->leaf) {        //未找到，返回空
		return nullptr;
	}
	else {
		return search(x->child[i], k);   //尾递归的向下查找
	}
}
void BTree::B_Tree_Split_Child(Node* x, int i) { //i是数组的下标
	Node* y = x->child[i];
	Node*L, *R;   //L R是新裂变的两个片段
	L = new Node;
	R = new Node;
	L->n = R->n = t - 1;
	L->key = new binarycode[t - 1];
	R->key = new binarycode[t - 1];
	L->leaf = R->leaf = x->child[i]->leaf;  //别忘了更改孩子的属性
	//复制键值
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
	//处理x
	x->key = (binarycode*)realloc(x->key, (x->n + 1) * sizeof(binarycode));
	x->child = (Node**)realloc(x->child, (x->n + 2) * sizeof(Node*));
	for (int j = x->n - 1; j >= i; --j) { //键值后移一位
		x->key[j + 1] = x->key[j];
	}
	x->key[i] = y->key[t - 1];
	for (int j = x->n; j >= i + 1; --j) {
		x->child[j + 1] = x->child[j];
	}
	x->child[i] = L;
	x->child[i + 1] = R;
	x->n++;
	//销毁原来的空间
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
	node_num++;  //结点个数增加
}

void BTree::B_Tree_Insert_NonFull(Node* x, const binarycode& k) {
	int i = x->n - 1;
	if (x->leaf) {  //叶子结点直接插入
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
		B_Tree_Insert_NonFull(x->child[i], k);  //尾递归
	}
}

void BTree::Merge_Node(Node*& x, int i, Node*& y, Node*& z) {
	y->key = (binarycode*)realloc(y->key, (2 * t - 1) * sizeof(binarycode));
	for (int j = 0; j < t - 1; ++j) {
		y->key[t + j] = z->key[j];
	}
	//不是叶子结点的情况下，还需要复制孩子
	if (!y->leaf) {
		y->child = (Node**)realloc(y->child, 2 * t * sizeof(Node**));
		for (int j = 0; j < t; ++j) {
			y->child[t + j] = z->child[j];
		}
	}
	y->key[t - 1] = x->key[i];  //移动x的结点补充到y
	y->n = 2 * t - 1;
	//删除x的下移关键字
	for (int j = i; j < x->n - 1; ++j) {
		x->key[j] = x->key[j + 1];
	}
	x->key = (binarycode*)realloc(x->key, (x->n - 1) * sizeof(binarycode));
	//删除原来指向z的指针
	for (int j = i + 1; j <= x->n - 1; ++j) {
		x->child[j] = x->child[j + 1];
	}
	x->child = (Node**)realloc(x->child, (x->n) * sizeof(Node**));
	x->child[i] = y;
	--x->n;
	//释放掉结点z
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
	if (y == nullptr) {  //防止出现空指针
		y = new Node;
	}
	++y->n;
	//复制键值
	y->key = (binarycode*)realloc(y->key, y->n * sizeof(binarycode));
	y->key[y->n - 1] = x->key[i];
	x->key[i] = z->key[0];
	for (int j = 0; j < z->n - 1; ++j) {
		z->key[j] = z->key[j + 1];
	}
	//非叶子结点的情况下复制孩子
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
	if (z == nullptr) { //防止出现空指针
		z = new Node;
	}
	++z->n;
	//复制键值
	z->key = (binarycode*)realloc(z->key, z->n * sizeof(binarycode));
	for (int j = z->n - 1; j > 0; --j) {
		z->key[j] = z->key[j - 1];
	}
	z->key[0] = x->key[i];
	x->key[i] = y->key[y->n - 1];
	//非叶子结点的情况下复制孩子
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
	//下面提到的各种情况全部参照算法导论
	if (x->leaf) {   //情况1
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
		if (k.ISBN == x->key[i].ISBN) { //情况2
			if (y->n > t - 1) { //情况2a
				binarycode k1 = Search_Predecessor(y);
				B_Tree_Delete_NoNone(y, k1);
				x->key[i] = k1;
			}
			else if (z->n > t - 1) { //情况2b
				binarycode k1 = Search_Successor(z);
				B_Tree_Delete_NoNone(z, k1);
				x->key[i] = k1;
			}
			else {  //情况2c
				Merge_Node(x, i, y, z);
				B_Tree_Delete_NoNone(y, k);
			}
		}
		else { //情况3
			Node* p = nullptr;
			if (i > 0) {
				p = x->child[i - 1];
			}
			if (y->n == t - 1) {
				if (i > 0 && p->n > t - 1) {  //情况3a
					Shift_To_Right_Child(x, i - 1, p, y);
				}
				else if (i<x->n&&z->n>t - 1) { //情况3a
					Shift_To_Left_Child(x, i, y, z);
				}
				else if (i > 0) { //情况3b
					Merge_Node(x, i - 1, p, y);  //向左侧结点合并
					y = p;
				}
				else {          //情况3b
					Merge_Node(x, i, y, z);      //向右侧合并
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
	if (root->n == 1 && root->child == nullptr) {  //删除最后一个元素
		delete[] root->key;
		delete root;
		root = nullptr;
	}
	else if (root->n == 1) {  //压缩根节点
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
	--node_num;  //结点个数减少一个
}

void BTree::Print() {
	Node *last = root;//last表示当前层的最右结点
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
			std::cout << "书号:" << tmp->key[i].ISBN << endl;
			std::cout << "书名:" << tmp->key[i].ptr->name << endl;
			std::cout << "作者:" << tmp->key[i].ptr->author << endl;
			std::cout << "现存量:" << tmp->key[i].ptr->currentSize << endl;
			std::cout << "总存量:" << tmp->key[i].ptr->allSize << endl;
			if (tmp->child&&i != tmp->n - 1) {
				std::cout << "|";
			}
		}
		std::cout << ")";
		if (!tmp->leaf) {   //不是叶子结点才有孩子
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
	if (cur->leaf) {  //直接删除叶子结点
		delete[] cur->key;
		delete cur;
		cur = nullptr;
	}
	else { //递归地删除每个孩子
		for (int i = 0; i < cur->n + 1; ++i) {
			PostOrder_Delete(cur->child[i]);
		}
		delete[] cur->key;
		delete cur;
		cur = nullptr;
	}
}

#endif
