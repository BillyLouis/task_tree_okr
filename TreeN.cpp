#include <iostream>
#include<iomanip>
#include<string>
#include <stdlib.h>
#include <vector>
#include <functional>

#include "TreeN.h"

using namespace std;
Tree::Tree()
{
    root = new TreeNode;
}

bool Tree::isEmpty( ) const{
            return root == 0;
          }

TreeItr Tree::createChild( TreeItr p ){
    // Create a new node and attach it as the last child of p.current.
    TreeNode* newNode = new TreeNode;
    if (!p.current->leftChild) {
        p.current->leftChild = newNode;
    } else {
        TreeNode* sibling = p.current->leftChild;
        while (sibling->rightSibling) {
            sibling = sibling->rightSibling;
        }
        sibling->rightSibling = newNode;
    }
    return TreeItr(newNode);
}

TreeItr Tree::TreeRoot( ) const{
            return TreeItr(root);}

TreeItr Tree::setupTree(Tree & T){
    TreeItr current, i, j, k, s;
    Task tmp;
    current = T.TreeRoot();
    tmp.element = " install product"; tmp.duration = 4;
    tmp.critical = 1;
    current.changeInfo(tmp);

    i = T.createChild(current);
    tmp.element = "design I/O"; tmp.duration = 1;
    tmp.critical = 0;
    i.changeInfo(tmp);

    i = T.createChild(current);
    tmp.element = "design protocol"; tmp.duration = 4;
    tmp.critical = 1;
    i.changeInfo(tmp);

    i = T.createChild(current);
    tmp.element = "write I/O"; tmp.duration = 7;
    i.changeInfo(tmp);

    i = T.createChild(current);
    tmp.element = "test user interface"; tmp.duration = 11;
    i.changeInfo(tmp);

    j = T.createChild(current);
    tmp.element = "order platform"; tmp.duration = 2;
    tmp.critical = 0;
    j.changeInfo(tmp);

    j = T.createChild(current);
    tmp.element = "set up platform"; tmp.duration = 3;
    tmp.critical = 0;
    j.changeInfo(tmp);

    k = T.createChild(current);
    tmp.element = "get Dept report"; tmp.duration = 1; tmp.critical = 0;
    k.changeInfo(tmp);

    k = T.createChild(current);
    tmp.element = "get CIO input"; tmp.duration = 2; tmp.critical = 0;
    k.changeInfo(tmp);

    k = T.createChild(current);
    tmp.element = "get CEO report"; tmp.duration = 3; tmp.critical = 0;
    k.changeInfo(tmp);

    tmp.critical = 0;
    s = T.createChild(current);
    tmp.element = "design driver"; tmp.duration = 4;
    tmp.critical = 1;
    s.changeInfo(tmp);

    tmp.critical = 0;
    s = T.createChild(current);
    tmp.element = "design utilities"; tmp.duration = 2;
    tmp.critical = 0;
    s.changeInfo(tmp);

    tmp.critical = 0;
    s = T.createChild(current);
    tmp.element = "design engine"; tmp.duration = 4;
    tmp.critical = 1;
    s.changeInfo(tmp);

    tmp.critical = 0;
    s = T.createChild(current);
    tmp.element = "write main"; tmp.duration = 7;
    tmp.critical = 1;
    s.changeInfo(tmp);

    tmp.critical = 0;
    s = T.createChild(current);
    tmp.element = "install lib"; tmp.duration = 1;
    tmp.critical = 0;
    s.changeInfo(tmp);

    tmp.critical = 0;
    s = T.createChild(current);
    tmp.element = "install sort"; tmp.duration = 1;
    s.changeInfo(tmp);

    tmp.critical = 0;
    s = T.createChild(current);
    tmp.element = "test main"; tmp.duration = 11;
    tmp.critical = 1;
    s.changeInfo(tmp);

    tmp.critical = 0;
    s = T.createChild(current);
    tmp.element = "install product"; tmp.duration = 15;
    tmp.critical = 1;
    s.changeInfo(tmp);

    return current;
}


void Tree::preorderPrint(TreeItr p) {
    if (p.isPast()) return;
    Task t = p.getInfo();
    cout << t.element <<  setw(30) <<" (" << t.duration << " days)";
    cout <<setw(30) << (t.critical ? " [Critical]" : " [Not Critical]") << setw(30);
    // print status if set
    switch (t.status) {
        case Task::Status::Passed: cout << " [Passed]"; break;
        case Task::Status::Failed: cout << " [Failed]"; break;
        default: break;
    }
    cout << endl;

    if (p.hasChild()) {
        TreeItr child = TreeItr(p.current->leftChild);
        while (!child.isPast()) {
            preorderPrint(child);
            child.advanceSibling();
        }
    }
}

TreeItr Tree::addTask(TreeItr parent, Task newTask) {
    TreeItr child = createChild(parent);
    child.changeInfo(newTask);
    return child;
}

void Tree::updateStatusByElapsedDays(int daysPassed) {
    std::function<void(TreeItr)> dfs = [&](TreeItr node) {
        if (node.isPast()) return;
        Task t = node.getInfo();

        // Determine status: if completionTime is set (>0) treat it as the deadline,
        // otherwise use duration as the needed days to finish.
        if (t.completionTime > 0) {
            t.status = (daysPassed <= t.completionTime) ? Task::Status::Passed : Task::Status::Failed;
        } else {
            t.status = (daysPassed >= t.duration) ? Task::Status::Passed : Task::Status::Failed;
        }

        node.changeInfo(t);

        if (node.hasChild()) {
            TreeItr child = TreeItr(node.current->leftChild);
            while (!child.isPast()) {
                dfs(child);
                child.advanceSibling();
            }
        }
    };

    dfs(TreeItr(root));
}

double Tree::calculateOKRPercentage() const {
    int total = 0;
    int passed = 0;

    std::function<void(TreeItr)> dfs = [&](TreeItr node) {
        if (node.isPast()) return;
        Task t = node.getInfo();
        // count this task
        total++;
        if (t.status == Task::Status::Passed) passed++;

        if (node.hasChild()) {
            TreeItr child = TreeItr(node.current->leftChild);
            while (!child.isPast()) {
                dfs(child);
                child.advanceSibling();
            }
        }
    };

    dfs(TreeItr(root));

    if (total == 0) return 0.0;
    return (static_cast<double>(passed) * 100.0) / static_cast<double>(total);
}

bool Tree::deleteTask(TreeItr target) {
    if (target.isPast() || target.current == root) return false;

    TreeNode* parent = nullptr;

    std::function<void(TreeNode*, TreeNode*&)> findParent = [&](TreeNode* node, TreeNode*& foundParent) {
        if (!node) return;
        TreeNode* c = node->leftChild;
        while (c) {
            if (c == target.current) {
                foundParent = node;
                return;
            }
            findParent(c, foundParent);
            c = c->rightSibling;
        }
    };

    findParent(root, parent);
    if (!parent) return false;

    TreeNode* prev = nullptr;
    TreeNode* curr = parent->leftChild;

    while (curr && curr != target.current) {
        prev = curr;
        curr = curr->rightSibling;
    }

    if (!curr) return false;

    if (prev) {
        prev->rightSibling = curr->rightSibling;
    } else {
        parent->leftChild = curr->rightSibling;
    }

    delete curr;
    return true;
}

void Tree::printDiagram(TreeItr p, int depth) {
    if (p.isPast()) return;

    cout << "  ";
    for (int i = 0; i < depth; ++i) cout << "  ";
    Task t = p.getInfo();
    cout << "- " << t.element <<setw(30)<< " (" << t.duration << " days)";
    if (t.critical) cout <<setw(30)<< " [Critical]" << setw(30);
    if (!t.critical) cout <<setw(30)<< " [Not Critical]" << setw(30);

    switch (t.status) {
        case Task::Status::Passed: cout <<" [Passed]"; break;
        case Task::Status::Failed: cout <<" [Failed]"; break;
        default: break;
    }
    cout << endl;

    if (p.hasChild()) {
        TreeItr child = TreeItr(p.current->leftChild);
        while (!child.isPast()) {
            printDiagram(child, depth + 1);
            child.advanceSibling();
        }
    }
}
