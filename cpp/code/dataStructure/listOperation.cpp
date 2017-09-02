//
// Created by ferris on 17-9-2.
//  使用C++实现链表等功能

#include <iostream>

using namespace std;

// 节点结构
struct Node{
    int data;
    Node *pNext;
};

// 单向链表类
class LinkList {
public:
    LinkList(){
        // 构造函数
        // 头节点不参与计数
        head = new Node;
        head->data = 0;
        head->pNext = NULL;
    }
    ~LinkList() {delete head;}  // 析构函数
    void CreateLinkList(int n); // 创建链表
    void DeleteLinkList();      // 删除整个链表
    void InsertNode (int position, int data ); // 插入指定位置的结点
    void DeleteNode (int position );           // 删除指定位置结点
    bool IsEmpty(){ return  head->pNext->pNext ==NULL ? 1:0 ;}; //判断是否为空,head一直存在
private:
    Node *head;
};

void LinkList::CreateLinkList(int n) {
    if (n <0) {
        cout << "输入节点个数错误" <<endl;
        exit(EXIT_FAILURE);
    }
    else {
        Node *pnew, *ptemp;
        ptemp = head;
        int i = n;
        while (n-- != 0){
            pnew = new Node;
            cout <<"输入第"<< i -n <<"个节点值";
            cin >> pnew->data;
            pnew->pNext = NULL;
            ptemp->pNext = pnew;
            ptemp = pnew;
        }

    }
}


void LinkList::DeleteLinkList() {
    Node *pdetele = head->pNext, *ptemp;
    while (!pdetele){  //没到2尾
        ptemp = pdetele->pNext; //保存头节点
        delete pdetele;
        pdetele = ptemp;
    }
}

void LinkList::InsertNode(int position, int data) {
    Node *ptemp = head;       // 遍历链表的flag
    Node *pnew = new Node;
    pnew->data = data;
    pnew->pNext = NULL;
    while (position -- >1)
        ptemp = ptemp->pNext;
    pnew->pNext = ptemp->pNext;    // 先将新增结点添加到原链表
    ptemp ->pNext = pnew;          // 原链表添加新结点
}

void LinkList::DeleteNode(int position) {
    Node *ptemp = head, *pdetele;
    while (position-- != 0)
        ptemp = ptemp->pNext;
    pdetele = ptemp->pNext;
    ptemp->pNext = pdetele->pNext; // 直接跳过一个节点
    delete pdetele;          // 记得释放内存
    pdetele = NULL;
}

int main (int args, char* argv[]){
    LinkList list;
    int position = 0, value = 0, n = 0;
    bool flag = false;
    cout <<"输入n的值： n = ";
    cin >> n;
    list.CreateLinkList(n);


    cout << "请输入插入结点的位置和数值：position =  ; value = ";
    cin >> position >> value ;
    list.InsertNode(position,value);

    cout << "请输入删除结点的位置： position = ";
    cin >> position;
    list.DeleteNode(position);

    list.DeleteLinkList();

    cout << list.IsEmpty();


}
