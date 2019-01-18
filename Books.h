#ifndef BOOKS_H
#include<iostream>
#include<cstring>
#define BOOKS_H
using namespace std;
struct Books {
	friend struct binarycode;
	Books(int nISBN, char *nname, char *nauthor, int ncurrentSize, int nallSize);
	Books();
	~Books();
	int ISBN;
	char name[20];
	char author[20];
	int currentSize;
	int allSize;
};
struct binarycode {
	int ISBN;
	Books *ptr;
	void operator=(const binarycode &src);
};
void binarycode::operator=(const binarycode &src) {
	ISBN = src.ISBN;
	ptr = src.ptr;
}
Books::Books() {};
Books::Books(int nISBN, char *nname, char *nauthor, int ncurrentSize, int nallSize) {
	ISBN = nISBN;
	strcpy_s(name, nname);
	strcpy_s(author, nauthor);
	currentSize = ncurrentSize;
	allSize = nallSize;
}
Books::~Books() {
}

#endif // !BOOKS_H
