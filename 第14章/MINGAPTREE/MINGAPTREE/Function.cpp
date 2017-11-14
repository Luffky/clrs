//
//  Function.cpp
//  MINGAPTREE
//
//  Created by 伏开宇 on 2017/3/21.
//  Copyright © 2017年 伏开宇. All rights reserved.
//
#include "Function.hpp"
GapTreeNode* GapTree::nil = new GapTreeNode(-1 ,1); //静态成员nil
GapTree::GapTree(){
    root = nil;
    n = 0;
    bh = 0;
}
GapTree::GapTree(GapTreeNode *x){
    n = 0;
    bh = 1;
    root = x;
    x->color = 1;
    x->p = nil;
    x->left = nil;
    x->right = nil;
    //nil->left = root;
    //nil->right = root;
}
int GapTree::Mingap(){
    return root->mingap;
}
void GapTree::SetRoot(GapTreeNode *x){
    root = x;
    bh = 1;
    x->color = 1;
    x->p = nil;
    x->left = nil;
    x->right = nil;
    //nil->left = root;
    //nil->right = root;
}
void GapTree::InorderTreeWalk(GapTreeNode *x){
    if(x != nil){
        InorderTreeWalk(x->left);
        cout<<x->key<<" ";
        InorderTreeWalk(x->right);
    }
}
GapTreeNode* GapTree::GapTreeMinimum(GapTreeNode *x){
    GapTreeNode *tree = x;
    while(tree->left != nil){
        tree = tree->left;
    }
    return tree;
}
GapTreeNode* GapTree::GapTreeSuccessor(GapTreeNode *x){
    GapTreeNode *tree = x->right;
    if(tree != nil){
        return GapTreeMinimum(tree);
    }
    GapTreeNode * parent = x->p;
    while(parent->left != x and parent != nil){
        x = parent;
        parent = parent->p;
    }
    return parent;
}
void GapTree::LeftRotate(GapTreeNode *x){
    if(x->right != nil){
        GapTreeNode *temp = x->right;
        x->right = temp->left;
        if(temp->left != nil)
            temp->left->p = x;
        temp->left = x;
        temp->p = x->p;
        if(x->p == nil){
            root = temp;
        }
        else if(x->p->left == x)
            x->p->left = temp;
        else
            x->p->right = temp;
        
        x->p = temp;
        //维护旋转结点的额外域
        x->mingap = min(x->left->mingap, min(x->right->mingap, min(x->key - x->left->max, x->right->min - x->key)));
        x->min = min(x->left->min, x->key);
        x->max = max(x->right->max, x->key);
        temp->mingap = min(temp->left->mingap, min(temp->right->mingap, min(temp->key - temp->left->max, temp->right->min - temp->key)));
        temp->min = min(temp->left->min, temp->key);
        temp->max = max(temp->right->max, temp->key);
        
    }
    else{
        cout<<"右子树为空，左旋失败"<<endl;
        exit(0);
    }
}
void GapTree::RightRotate(GapTreeNode *x){
    if(x->left != nil){
        GapTreeNode *temp = x->left;
        x->left = temp->right;
        if(temp->right != nil)
            temp->right->p = x;
        temp->right = x;
        temp->p = x->p;
        if(x->p == nil){
            root = temp;
        }
        else if(x->p->left == x)
            x->p->left = temp;
        else
            x->p->right = temp;
        
        x->p = temp;
        //维护旋转结点的额外域
        x->mingap = min(x->left->mingap, min(x->right->mingap, min(x->key - x->left->max, x->right->min - x->key)));
        x->min = min(x->left->min, x->key);
        x->max = max(x->right->max, x->key);
        temp->mingap = min(temp->left->mingap, min(temp->right->mingap, min(temp->key - temp->left->max, temp->right->min - temp->key)));
        temp->min = min(temp->left->min, temp->key);
        temp->max = max(temp->right->max, temp->key);
        
    }
    else{
        cout<<"左子树为空，右旋失败"<<endl;
        exit(0);
    }
}
void GapTree::GapTreeInsert(GapTreeNode *z){
    n++;
    GapTreeNode *y = nil;
    GapTreeNode *x = root;
    while(x != nil){
        y = x;
        if(x->key < z->key){
            x = x->right;
        }
        else{
            x = x->left;
        }
    }
    z->p = y;
    if(y == nil)
        SetRoot(z);
    else{
        if(y->key < z->key)
            y->right = z;
        else
            y->left = z;
        z->left = nil;
        z->right = nil;
        z->color = 0;
        //自底向上维护额外域
        while(y != nil){
            int mi = min(y->key - y->left->max, y->right->min - y->key);
            mi = min(mi, y->right->mingap);
            y->mingap = min(y->left->mingap, mi);
            y->min = min(y->left->min, y->key);
            y->max = max(y->right->max, y->key);
            y = y->p;
        }
        GapInsertFixup(z);
    }
}
void GapTree::GapInsertFixup(GapTreeNode *x){
    while(x->p->color == 0){//如果x的父亲是红色，则迭代
        if(x->p == x->p->p->left){ //如果x的父亲是左孩子
            GapTreeNode *y = x->p->p->right; //x的叔叔
            if(y->color == 0){  //x的叔叔如果是红色
                y->color = 1;
                x->p->color = 1;  //将x的父亲和叔叔都置为黑色
                x = x->p->p;
                x->color = 0; //x的爷爷置为红色，以其爷爷为当前节点再次迭代
            }
            else{//x的叔叔如果是黑色
                if(x == x->p->left){ //如果x是左孩子
                    x = x->p; //当前节点变为x的父亲
                    RightRotate(x); //右旋
                    x->p->color = 1;//父亲置为黑色
                    x->p->p->color = 0;//爷爷置为红色
                    RightRotate(x->p->p);//右旋爷爷
                }
                else{//如果x是右孩子
                    x = x->p; //当前节点变为x的父亲
                    LeftRotate(x); //左旋
                    x->p->color = 1;//父亲置为黑色
                    x->p->p->color = 0;//爷爷置为红色
                    RightRotate(x->p->p);//右旋爷爷
                }
            }
        }
        else{ //x的父亲是右孩子
            GapTreeNode *y = x->p->p->left; //x的叔叔
            if(y->color == 0){ //同上
                y->color = 1;
                x->p->color = 1;  //将x的父亲和叔叔都置为黑色
                x = x->p->p;
                x->color = 0; //x的爷爷置为红色，以其爷爷为当前节点再次迭代
            }
            else{
                if(x == x->p->left){ //如果x是左孩子
                    x = x->p; //当前节点变为x的父亲
                    RightRotate(x); //右旋
                    x->p->color = 1;//父亲置为黑色
                    x->p->p->color = 0;//爷爷置为红色
                    LeftRotate(x->p->p);//左旋爷爷
                }
                else{//如果x是右孩子
                    x = x->p; //当前节点变为x的父亲
                    LeftRotate(x); //左旋
                    x->p->color = 1;//父亲置为黑色
                    x->p->p->color = 0;//爷爷置为红色
                    LeftRotate(x->p->p);//左旋爷爷
                }
            }
            
        }
    }
    if(root->color == 0){
        root->color = 1;
        bh++;
    }
}
void GapTree::GapTreeDelete(GapTreeNode *z){
    GapTreeNode *y = nil;
    GapTreeNode *x = nil;
    int flag = 0;//flag为1表示不进行回复红黑性质
    if(z->left == nil or z->right == nil){
        y = z;
        GapTreeNode *temp = z->p;
        if(y->left != nil)
            x = y->left;
        else
            x = y->right;
        x->p = temp;
        if(temp == nil){
            root = x;
        }
        else if(temp->left == y)
            temp->left = x;
        else
            temp->right = x;
        //自底向上维护额外域
        while(temp != nil){
            if(temp->left == nil and temp->right == nil){
                temp->mingap = INT32_MAX;
                temp->min = temp->key;
                temp->max = temp->key;
            }
            else{
                temp->mingap = min(temp->left->mingap, min(temp->right->mingap, min(temp->key - temp->left->max, temp->right->min - temp->key)));
                temp->min = min(temp->left->min, temp->key);
                temp->max = max(temp->right->max, temp->key);
            }
            temp = temp->p;
        }
    }
    else {
        y = GapTreeSuccessor(z);
        z->Copy(y);
        GapTreeDelete(y);
        flag = 1;
    }
    if(flag == 0 and y->color == 1)
        GapDeleteFixup(x);
    
}
void GapTree::GapDeleteFixup(GapTreeNode *x){
    while(x != root and x->color == 1){
        if(x->p->left == x){ //如果x是左子树
            GapTreeNode *w = x->p->right;  //设置x的兄弟
            //第一阶段
            if(w->color == 0){ //如果x的兄弟是红色
                w->color = 1;
                x->p->color = 0;
                LeftRotate(x->p); //左旋父亲
                w = x->p->right; //设置新的兄弟，此兄弟必为黑色，所以根据兄弟的孩子着色情况进入第二或者第三或者第四阶段
            }
            //第二阶段
            if(w->left->color == 1 and w->right->color == 1){//如果兄弟的两个孩子都是黑色
                w->color = 0;//置兄弟为红色，且不违反性质4,消除掉了x的两重黑色
                x = x->p;//开始下一循环，若从第一阶段进入此阶段，则因为父亲必为红色所以跳出循环，直接在最后置父亲为黑色，程序结束，否则x则上升，以x父亲为当前节点进入下一个循环。
                if(x == root and x->color == 1)
                    bh--;
            }
            else{
                //第三阶段
                if(w->right->color == 1){ //如果兄弟右孩子是黑色
                    w->color = 0;
                    w->left->color = 1; //兄弟左孩子置为黑色
                    RightRotate(w); //右旋兄弟
                    w = x->p->right; //设置新的兄弟，此兄弟必为黑色,且此兄弟的右孩子必为红色，进入第四阶段
                }
                //第四阶段 //如果兄弟右孩子是红色则跳过上一阶段，直接到达该阶段
                w->color = x->p->color; //置兄弟为父亲同色
                x->p->color = 1;
                w->right->color = 1;
                LeftRotate(x->p); //左旋父亲
                x = root; //跳出循环
            }
        }
        else{//如果x是右子树
            GapTreeNode *w = x->p->left; //设置x的兄弟
            //第一阶段
            if(w == 0){ //如果x的兄弟是红色
                w->color = 1;
                x->p->color = 0;
                RightRotate(x->p);//右旋父亲
                w = x->p->left; //设置新的兄弟，此兄弟必为黑色，所以进入下一阶段
            }
            //第二阶段
            if(w->left->color == 1 and w->right->color == 1){//如果兄弟的两个孩子都是黑色
                w->color = 0;//置兄弟为红色，且不违反性质4
                x = x->p;//开始下一循环，若从第一阶段进入此阶段，则因为父亲必为红色所以跳出循环，直接在最后置父亲为黑色，程序结束，否则x则上升，以x父亲为当前节点进入下一个循环。
                if(x == root and x->color == 1)
                    bh--;
            }
            else{
                //第三阶段
                if(w->left->color == 1){ //如果兄弟左孩子是黑色
                    w->color = 0;
                    w->right->color = 1; //兄弟右孩子置为黑色
                    LeftRotate(w); //右旋兄弟
                    w = x->p->left; //设置新的兄弟，此兄弟必为黑色，进入第四阶段
                }
                //第四阶段
                w->color = x->p->color;
                x->p->color = 1;
                w->left->color = 1;
                RightRotate(x->p);
                x = root;
            }
        }
    }
    x->color = 1;
}
