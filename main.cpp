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
	cout << "========B-Trees �������ʵ��========" << endl;
	cout << "����˵��:" << endl;
	cout << "1.�������B-Tree" << endl;
	cout << "2.������" << endl;
	cout << "3.�ɱ����" << endl;
	cout << "4.����" << endl;
	cout << "5.�黹" << endl;
	cout << "6.������ʾ" << endl;
	cout << "7.�˳�" << endl;
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
		cout << "������������:";
		int n;
		cin.clear(); cin.sync();
		cin >> n;
		switch (n) {
		case 1:
			if (bt == nullptr) {
				cout << "��ǰû�н���B��!" << endl;
			}
			else if (bt->size() <= 0) {
				cout << "B������Ԫ��!" << endl;
			}
			else {
				bt->Print();
			}
			break;
		case 2:
			if (bt == nullptr) {
				cout << "��ǰû�н���B��!" << endl;
			}
			else if (bt->size() <= 0) {
				cout << "B������Ԫ��!" << endl;
			}
			else {
				cout << "������Ҫɾ����Ԫ��:";
				binarycode k;
				cin >> k.ISBN;
				if (bt->B_Tree_Search(k) == nullptr) { cout << "���鲻���ڣ�" << endl; break; }
				bt->B_Tree_Delete(k);
			}
			break;
		case 3:
			if (bt != nullptr) {
				cout << "����������Ԫ��:" << endl;
				cout << "��ţ�"; cin >> ISBN; cout << endl;
				cout << "������"; cin >> name; cout << endl;
				cout << "���ߣ�"; cin >> author; cout << endl;
				cout << "�ִ�����"; cin >> currentSize; cout << endl;
				cout << "�ܴ�����"; cin >> allsize; cout << endl;
				binarycode *k = new binarycode;
				k->ptr = new Books(ISBN, name, author, currentSize, allsize);
				k->ISBN = ISBN;
				binarycode &c = *k;
				if (bt->B_Tree_Search(c) != nullptr) { cout << "�����Ѵ��ڣ�" << endl; break; }
				bt->B_Tree_Insert(c);
			}
			else {
				cout << "B��δ������" << endl;
			}
			; break;
		case 4: {
			binarycode c; int loc = 0, currentsize = 0;
			cout << "������ţ�" << endl;
			cin >> c.ISBN;
			Node *p = bt->B_Tree_Search_special(c, loc);
			if (p != nullptr) {
				currentSize = p->key[loc].ptr->currentSize;
				if (currentSize) { p->key[loc].ptr->currentSize--; cout << "���ĳɹ���" << endl; }
				else { cout << "��治�㣬�޷����ģ�"; }
			}
			else {
				cout << "ͼ���δ���и��飡" << endl;
			}
		}; break;
		case 5: {
			binarycode c; int loc = 0, currentsize = 0, allsize = 0;
			cout << "�黹��ţ�" << endl;
			cin >> c.ISBN;
			Node *p = bt->B_Tree_Search_special(c, loc);
			if (p != nullptr) {
				p->key[loc].ptr->currentSize++;
				currentSize = p->key[loc].ptr->currentSize;
				allsize = p->key[loc].ptr->allSize;
				cout << "�黹�ɹ���" << endl;
				if (currentSize > allsize) { p->key[loc].ptr->allSize = currentSize; }
			}
			else {
				cout << "ͼ���δ���и��飡" << endl;
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
			cout << "������Ч!" << endl;
			cin.clear();            //clear bad input flag
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  //��Ӧgnu
			cin.sync();     //��Ӧmsvc
		}
	}
	system("pause");
	return 0;
}