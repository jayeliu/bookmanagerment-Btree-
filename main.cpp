#include <iostream>
#include<fstream>
#include <cstring>
#include <limits>
#include "B_tree.h"
#include"Books.h"
using namespace std;
void input(BTree *bt) {
	ifstream file("data.txt");
	int ISBN, currentSize, allsize;
	char name[20], author[20];
	if (file.is_open()) {
		while (file.peek() != EOF)
		{
			file >> ISBN >> name >> author >> currentSize >> allsize;
			binarycode *c = new binarycode;
			c->ISBN = ISBN;
			c->ptr = new Books(ISBN, name, author, currentSize, allsize);
			binarycode &k = *c;
			bt->B_Tree_Insert(k);
		}
	}
}
void Menu() {
	cout << "========B-Trees 的设计与实现========" << endl;
	cout << "操作说明:" << endl;
	cout << "1.遍历输出B-Tree" << endl;
	cout << "2.清除库存" << endl;
	cout << "3.采编入库" << endl;
	cout << "4.借阅" << endl;
	cout << "5.归还" << endl;
	cout << "6.凹入显示" << endl;
	cout << "7.退出" << endl;
}
int main()
{
	Menu();
	int ISBN, currentSize, allsize;
	char name[20], author[20];
	BTree* bt = nullptr;
	bool flag = true;
	bt = new BTree;
	input(bt);
	while (flag) {
		cout << "请输入操作编号:";
		int n;
		cin.clear(); cin.sync();
		cin >> n;
		switch (n) {
		case 1:
			if (bt == nullptr) {
				cout << "当前没有建立B树!" << endl;
			}
			else if (bt->size() <= 0) {
				cout << "B树中无元素!" << endl;
			}
			else {
				bt->Print();
			}
			break;
		case 2:
			if (bt == nullptr) {
				cout << "当前没有建立B树!" << endl;
			}
			else if (bt->size() <= 0) {
				cout << "B树中无元素!" << endl;
			}
			else {
				cout << "请输入要删除的元素:";
				binarycode k;
				cin >> k.ISBN;
				if (bt->B_Tree_Search(k) == nullptr) { cout << "该书不存在！" << endl; break; }
				bt->B_Tree_Delete(k);
			}
			break;
		case 3:
			if (bt != nullptr) {
				cout << "请输出插入的元素:" << endl;
				cout << "书号："; cin >> ISBN; cout << endl;
				cout << "书名："; cin >> name; cout << endl;
				cout << "作者："; cin >> author; cout << endl;
				cout << "现存量："; cin >> currentSize; cout << endl;
				cout << "总存量："; cin >> allsize; cout << endl;
				binarycode *k = new binarycode;
				k->ptr = new Books(ISBN, name, author, currentSize, allsize);
				k->ISBN = ISBN;
				binarycode &c = *k;
				if (bt->B_Tree_Search(c) != nullptr) { cout << "该书已存在！" << endl; break; }
				bt->B_Tree_Insert(c);
			}
			else {
				cout << "B树未建立！" << endl;
			}
			; break;
		case 4: {
			binarycode c; int loc = 0, currentsize = 0;
			cout << "借阅书号：" << endl;
			cin >> c.ISBN;
			Node *p = bt->B_Tree_Search_special(c, loc);
			if (p != nullptr) {
				currentSize = p->key[loc].ptr->currentSize;
				if (currentSize) { p->key[loc].ptr->currentSize--; cout << "借阅成功！" << endl; }
				else { cout << "库存不足，无法借阅！"; }
			}
			else {
				cout << "图书馆未藏有该书！" << endl;
			}
		}; break;
		case 5: {
			binarycode c; int loc = 0, currentsize = 0, allsize = 0;
			cout << "归还书号：" << endl;
			cin >> c.ISBN;
			Node *p = bt->B_Tree_Search_special(c, loc);
			if (p != nullptr) {
				p->key[loc].ptr->currentSize++;
				currentSize = p->key[loc].ptr->currentSize;
				allsize = p->key[loc].ptr->allSize;
				cout << "归还成功！" << endl;
				if (currentSize > allsize) { p->key[loc].ptr->allSize = currentSize; }
			}
			else {
				cout << "图书馆未藏有该书！" << endl;
			}
		}; break;
		case 6: {
			bt->travel();
		}; break;
		case 7:
			delete bt;
			bt = nullptr;
			flag = false;
			break;
		default:
			cout << "输入无效!" << endl;
			cin.clear();            //clear bad input flag
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  //适应gnu
			cin.sync();     //适应msvc
		}
	}
	system("pause");
	return 0;
}