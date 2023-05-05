#pragma once

#include <Aircraft.hpp>

struct SLinkedListNode {
	Aircraft* data;
	SLinkedListNode* next;
	Aircraft* &getData();
};

struct SlinkedList {
	SLinkedListNode *pHead = nullptr;
	SLinkedListNode *getNode(int id);
	int getSize();
	void addNode(Aircraft* data);
};