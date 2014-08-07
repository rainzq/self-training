#include <iostream>

using namespace std;

template<typename T>
struct list {
    T data;
    list *next;
    list(T data_, list *next_ = NULL): data(data_), next(next_){}
    ~list() {
        list *tmp = NULL;
        list *p = next;
        while (p) {
            tmp = p->next;
            delete p; 
            p = tmp;
        } 
    }
};

template<typename T>
void print(list<T> *alist)
{
    while(alist) {
        cout << alist->data << " ";
        alist = alist->next;
    }
    cout << endl;
}

void quick_sort(list<int> **head, list<int> *tail);
list<int> *partition(list<int> **head, list<int> *tail);

int main() 
{
    list<int> *h = NULL;
    int data;
    while (cin >> data) {
        h = new list<int>(data, h);
    };
    print(h);
    quick_sort(&h, NULL);
    print(h);
}

void quick_sort(list<int> **head, list<int> *tail)
{
    if (*head == tail) {return ;}
    if ((*head)->next == tail) {return ;}
    list<int> *part = partition(head, tail);
    cout << "part: " << part->data << endl;
    print(*head);
    if (tail) cout << "tail: " << tail->data << endl;
    quick_sort(head, part);
    quick_sort(&(part->next), tail);
}

list<int> *partition(list<int> **head, list<int> *tail)
{
    if (*head == NULL) {return NULL;}
    if (*head == tail) {return NULL;};
    list<int> *small = *head;
    list<int> *pivot = *head;
    list<int> *curr = (*head)->next;
    list<int> *prev = *head;
    while (curr != tail) {
        if (curr->data < pivot->data) {
            prev->next = curr->next;
            curr->next = small;
            small = curr;
            curr = prev->next;
        } else {
            prev = curr;
            curr = curr->next;
        }
    }
    *head = small;
    return pivot;
}
