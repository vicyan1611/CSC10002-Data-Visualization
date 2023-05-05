#include <SLinkedList.hpp>

Aircraft*& SLinkedListNode::getData()
{
	return data;
}

SLinkedListNode* SLinkedList::getNode(int id)
{
	if (pHead == nullptr) return nullptr;
	SLinkedListNode* cur = pHead;
	int cnt = 1;
	while (cnt < id) {
		cnt++;
		cur = cur->next;
	}
	return cur;
}

int SLinkedList::getSize()
{
	if (pHead == nullptr) return 0;
	SLinkedListNode* cur = pHead;
	int cnt = 1;
	while (cur->next != nullptr) {
		cnt++;
		cur = cur->next;
	}
	return cnt;
}

void SLinkedList::addNode(Aircraft* data)
{
	SLinkedListNode* newNode(new SLinkedListNode);
	newNode->data = data;
	newNode->next = nullptr;
	if (pHead == nullptr) {
		pHead = newNode;
		return;
	}
	SLinkedListNode* cur = pHead;
	while (cur->next != nullptr) {
		cur = cur->next;
	}
	cur->next = newNode;
}

void SLinkedList::reset()
{
	SLinkedListNode* cur = pHead;
	while (cur != nullptr) {
		SLinkedListNode* tmp = cur;
		cur = cur->next;
		delete tmp;
	}
	pHead = nullptr;
}