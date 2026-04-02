#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct node{
    int id,height;
    char name[30],dept[30];
    float cgpa;
    struct node *left,*right;
};

/* ---------- Utility ---------- */
int height(struct node *n){ return n? n->height:0; }
int max(int a,int b){ return a>b?a:b; }

struct node* create(int id,char n[],char d[],float c){
    struct node* x=(struct node*)malloc(sizeof(struct node));
    x->id=id;
    strcpy(x->name,n);
    strcpy(x->dept,d);
    x->cgpa=c;
    x->height=1;
    x->left=x->right=NULL;
    return x;
}

int balance(struct node* n){
    return n? height(n->left)-height(n->right):0;
}

/* ---------- Rotations ---------- */
struct node* rightRotate(struct node* y){
    struct node* x=y->left;
    struct node* t2=x->right;

    x->right=y;
    y->left=t2;

    y->height=max(height(y->left),height(y->right))+1;
    x->height=max(height(x->left),height(x->right))+1;

    return x;
}

struct node* leftRotate(struct node* x){
    struct node* y=x->right;
    struct node* t2=y->left;

    y->left=x;
    x->right=t2;

    x->height=max(height(x->left),height(x->right))+1;
    y->height=max(height(y->left),height(y->right))+1;

    return y;
}

/* ---------- Insert ---------- */
struct node* insert(struct node* root,int id,char n[],char d[],float c){

    if(root==NULL) return create(id,n,d,c);

    if(id<root->id)
        root->left=insert(root->left,id,n,d,c);
    else if(id>root->id)
        root->right=insert(root->right,id,n,d,c);
    else return root;

    root->height=1+max(height(root->left),height(root->right));
    int b=balance(root);

    if(b>1 && id<root->left->id) return rightRotate(root);
    if(b<-1 && id>root->right->id) return leftRotate(root);

    if(b>1 && id>root->left->id){
        root->left=leftRotate(root->left);
        return rightRotate(root);
    }

    if(b<-1 && id<root->right->id){
        root->right=rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

/* ---------- Min ---------- */
struct node* minNode(struct node* root){
    while(root->left) root=root->left;
    return root;
}

/* ---------- Delete ---------- */
struct node* deleteNode(struct node* root,int id){

    if(root==NULL) return root;

    if(id<root->id)
        root->left=deleteNode(root->left,id);
    else if(id>root->id)
        root->right=deleteNode(root->right,id);
    else{
        if(root->left==NULL || root->right==NULL){
            struct node* temp=root->left?root->left:root->right;

            if(temp==NULL){
                free(root);
                return NULL;
            }else{
                *root=*temp;
                free(temp);
            }
        }else{
            struct node* temp=minNode(root->right);

            root->id=temp->id;
            strcpy(root->name,temp->name);
            strcpy(root->dept,temp->dept);
            root->cgpa=temp->cgpa;

            root->right=deleteNode(root->right,temp->id);
        }
    }

    root->height=1+max(height(root->left),height(root->right));
    int b=balance(root);

    if(b>1 && balance(root->left)>=0) return rightRotate(root);
    if(b>1 && balance(root->left)<0){
        root->left=leftRotate(root->left);
        return rightRotate(root);
    }

    if(b<-1 && balance(root->right)<=0) return leftRotate(root);
    if(b<-1 && balance(root->right)>0){
        root->right=rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

/* ---------- Search ---------- */
struct node* search(struct node* root,int id,int *c){
    if(root==NULL) return NULL;
    (*c)++;

    if(id==root->id) return root;

    if(id<root->id)
        return search(root->left,id,c);
    else
        return search(root->right,id,c);
}

/* ---------- Traversals ---------- */
void inorder(struct node* root){
    if(root){
        inorder(root->left);
        printf("%d %s %.2f\n",root->id,root->name,root->cgpa);
        inorder(root->right);
    }
}

void preorder(struct node* root){
    if(root){
        printf("%d ",root->id);
        preorder(root->left);
        preorder(root->right);
    }
}

void postorder(struct node* root){
    if(root){
        postorder(root->left);
        postorder(root->right);
        printf("%d ",root->id);
    }
}

/* ---------- Predecessor ---------- */
struct node* predecessor(struct node* root){
    root=root->left;
    while(root && root->right) root=root->right;
    return root;
}

/* ---------- Successor ---------- */
struct node* successor(struct node* root){
    root=root->right;
    while(root && root->left) root=root->left;
    return root;
}

/* ---------- Range ---------- */
void range(struct node* root,int L,int R){
    if(root==NULL) return;

    if(root->id>L)
        range(root->left,L,R);

    if(root->id>=L && root->id<=R)
        printf("%d %s\n",root->id,root->name);

    if(root->id<R)
        range(root->right,L,R);
}

/* ---------- Main ---------- */
int main(){

    struct node* root=NULL;
    int ch,id,L,R,c;
    char n[30],d[30];
    float cg;

    while(1){
        printf("\n1.Insert 2.Search 3.Delete 4.Inorder 5.Pre 6.Post 7.Range 8.Exit\n");
        scanf("%d",&ch);

        switch(ch){

            case 1:
                printf("Enter id name dept cgpa: ");
                scanf("%d %s %s %f",&id,n,d,&cg);
                root=insert(root,id,n,d,cg);
                break;

            case 2:
                c=0;
                printf("Enter id: ");
                scanf("%d",&id);
                {
                    struct node* s=search(root,id,&c);
                    if(s)
                        printf("Found: %s %.2f Comparisons:%d\n",s->name,s->cgpa,c);
                    else
                        printf("Not Found\n");
                }
                break;

            case 3:
                printf("Enter id: ");
                scanf("%d",&id);
                root=deleteNode(root,id);
                break;

            case 4: inorder(root); break;
            case 5: preorder(root); break;
            case 6: postorder(root); break;

            case 7:
                printf("Enter range: ");
                scanf("%d %d",&L,&R);
                range(root,L,R);
                break;

            case 8: exit(0);
        }
    }
}
