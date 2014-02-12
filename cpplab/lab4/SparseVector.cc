#include "SparseVector.hh"
#include <cassert>
#include <iostream>

using namespace std;

void SparseVector::copy(const SparseVector &sv)
{
    size = sv.size;
    start = 0;
    node *svcurr = sv.start;
    node *curr = 0;
    node *prev = 0;
    while (svcurr != 0) {
        curr = new node(svcurr->value, svcurr->index);

        if (prev == 0) { // first node
            start = curr;
        } else {
            prev->next = curr;
        }

        prev = curr;
        svcurr = svcurr->next;
    }
    assert(checkListOrder());
}

void SparseVector::cleanup()
{
    size = 0;
    node *curr = start;
    node *next = 0;
    while (curr != 0) {
        next = curr->next;
        delete curr;
        curr = next;
    }
    start = 0;
}

SparseVector::SparseVector(const SparseVector &sv)
{
    copy(sv);
}

SparseVector::~SparseVector()
{
    cleanup();
}

int SparseVector::getSize() const
{
    return size;
}

int SparseVector::getElem(int col) const
{
    assert(checkListOrder());
    assert(col >= 0 && col <= size);
    node *curr = start;
    while (curr != 0 && curr->index <= col) {
        if (col == curr->index) {
            return curr->value;
        }
        curr = curr->next;
    }
    // not found
    return 0;
}

void SparseVector::setNoZeroElem(int col, int value)
{
    assert(checkListOrder());
    assert(col >= 0 && col <= size);
    node *prev = 0;
    node *curr = start;
    while (curr != 0 && curr->index <= col) {
        if (col == curr->index) {
            curr->value = value;
            return;
        }
        prev = curr;
        curr = curr->next;
    }
    // not found in the list, insert it
    curr = new node(value, col);
    if (prev == 0) { 
        // at the head of the list
        curr->next = start;
        start = curr;
    } else {
        curr->next = prev->next;
        prev->next = curr;
    }
    assert(checkListOrder());
}

void SparseVector::removeElem(int col)
{
    assert(checkListOrder());
    assert(col >= 0 && col <= size);
    node *curr = start;
    node *prev = 0;
    while (curr != 0 && curr->index <= col) {
        if (col == curr->index) {
            if (prev == 0) { 
                // at the head of the list
                start = curr->next;
            } else {
                prev->next = curr->next;
            }
            delete curr;
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

void SparseVector::setElem(int col, int value)
{
    assert(checkListOrder());
    assert(col >= 0 && col <= size);
    if (value == 0) {
        removeElem(col);
    } else {
        setNoZeroElem(col, value);
    }
}

void SparseVector::addElem(int col, int value)
{
    assert(checkListOrder());
    if (value == 0) { 
        // add nothing
        return;
    }
    node *curr = start;
    node *prev = 0;
    while (curr != 0 && curr->index <= col) {
        if (curr->index == col) {
            curr->value += value;
            if (curr->value == 0) {
                // we have to delete it
                if (prev == 0) {
                    // at the head of the list
                    start = curr->next;
                } else {
                    prev->next = curr->next;
                }
                delete curr;
                return;
            }
        }
        prev = curr;
        curr = curr->next;
    }
    
    // not found, we insert it to the list
    curr = new node(value, col);
    if (prev == 0) {
        // at the head of the list
        curr->next = start;
        start = curr;
    } else {
        curr->next = prev->next;
        prev->next = curr;
    }
    assert(checkListOrder());
}

void SparseVector::addList(const SparseVector &sv, int sign /* = 1 */)
{
    node *curr = start;
    node *prev = 0;
    node *svcurr = sv.start;

    while (curr != 0 && svcurr != 0) {
        if (curr->index == svcurr->index) {
            // same index, add them, after that move forword one step together 
            curr->value += svcurr->value * sign; 
            if (curr->value == 0) {
                // delete zero node first
                if (prev == 0) {
                    // at the head of the list
                    start = curr->next;
                    delete curr;
                    // start has changed, we should point to it again
                    // prev doesn't have to change
                    curr = start;
                } else {
                    prev->next = curr->next;
                    delete curr;
                    // curr point to the node after that node deleted
                    // prev doesn't have to change
                    curr = prev->next;
                }
            } else {
                // normally move forword on step
                prev = curr;
                curr = curr->next;
            }
            // sv move forword on step
            svcurr = svcurr->next;
        } else if (curr->index < svcurr->index) {
            // this index is less than sv index
            // move forword this list one step
            prev = curr;
            curr = curr->next;
        } else {
            // this index is greater than sv index
            // so insert sv current node in this list
            curr = new node(svcurr->value * sign, svcurr->index);
            // prev should point to the insert node
            if (prev == 0) {
                // at the head of the list
                curr->next = start;
                start = curr;
                prev = start;
            } else {
                curr->next = prev->next;
                prev->next = curr;
                prev = prev->next;
            }
            curr = prev->next;
            // sv move forword one step
            svcurr = svcurr->next;
        }
    }
    while (svcurr != 0) { 
        // at the end of the this list
        // insert the rest nodes of the sv list to this list
        curr = new node(svcurr->value * sign, svcurr->index);
        if (prev == 0) {
            // this list is empty
            start = curr;
        } else {
            // append the tail
            prev->next = curr;
        }
        prev = curr;
        svcurr = svcurr->next;
    }
}


SparseVector & SparseVector::operator=(const SparseVector &sv)
{
    // make sure they are not the same instance
    if (this != &sv) { 
        cleanup();
        copy(sv);
    }
    return *this;
}

SparseVector & SparseVector::operator+=(const SparseVector &sv)
{
    /*
     *node *p = sv.start;
     *while (p != 0) {
     *    addElem(p->index, p->value);
     *    p = p->next;
     *}
     */
    addList(sv);
    return *this;
}

SparseVector & SparseVector::operator-=(const SparseVector &sv)
{
    /*
     *node *p = sv.start;
     *while (p != 0) {
     *    addElem(p->index, -(p->value));
     *    p = p->next;
     *}
     */
    addList(sv, -1);
    return *this;
}

const SparseVector SparseVector::operator+(const SparseVector &sv) const
{
    SparseVector result(*this);
    result += sv;
    return result;
}

const SparseVector SparseVector::operator-(const SparseVector &sv) const
{
    SparseVector result(*this);
    result -= sv;
    return result;
}

bool SparseVector::operator==(const SparseVector &sv) const
{
    if (size != sv.size) {
        return false;
    }
    node *curr = start;
    node *svcurr = sv.start;
    while (curr != 0 && svcurr != 0) {
        // the content of the nodes are not the same
        if (curr->index != svcurr->index || curr->value != svcurr->value) {
            return false;
        }
        curr = curr->next;
        svcurr = svcurr->next;
    }
    if (curr == 0 && svcurr == 0) {
        // have checked all the nodes in the two list
        return true;
    } else {
        return false;
    }
}

bool SparseVector::operator!=(const SparseVector &sv) const
{
    return !(*this == sv);
}

// monitor function
bool SparseVector::checkListOrder() const
{
    node *p = start;
    while (p != 0 && p->next != 0) {
        if (p->index > p->next->index) {
            cerr << "Warning: node " << p->index
                 << " is out of order, its value:  "
                 << p->value << endl;
            return false;
        }
        p = p->next;
    }
    return true;
}
