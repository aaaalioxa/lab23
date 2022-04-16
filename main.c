#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct TNode {
    struct TNode *parent;
    int value;
    int available_cnt;
    int CountChild;
    struct TNode **children;
} TNode;

int maxchild = 5;
bool is_genesis = true;
TNode* genesis_node;

struct TStackHead{
    struct TStackList{
        struct TStackList *Next;
        TNode *Node;
        int depth;
    } *Head;
};

typedef struct TStackHead TStackHead;
typedef struct TStackList TStackList;

TNode* FindNode(TNode* u, int vertex) {
    if (u->value == vertex)
        return u;
    TNode* vertex_address = NULL;
    for (int i = 0; i < u->CountChild; ++i) {
        vertex_address = FindNode(u->children[i], vertex);
        if (vertex_address != NULL && vertex_address->value == vertex)
            break;
    }
    return vertex_address;
}

void DelNode(TNode* v) {
    int count = 0;
    for (int i = v->CountChild - 1; i >= 0; --i) {
        DelNode(v->children[i]);
    }

    TNode* v_parent = v->parent;
    if (v->parent != NULL)
        count = v_parent->CountChild;
    int del_index = 0;
    for (int i = 0; i < count; ++i) {
        if (v_parent->children[i]->value == v->value) {
            del_index = i;
            free(v);
            break;
        }
    }
    for (int i = del_index + 1; i < count; ++i) {
        v_parent->children[i - 1] = v_parent->children[i];
    }
    if (v->parent != NULL)
        v_parent->CountChild--;
    else
    {
        free(v);
        genesis_node = NULL;
        is_genesis = true;
    }
}

void TPrint(TNode* u, int space_cnt, int deep) {
    printf("%d\n", u->value);
    for (int i = 0; i < u->CountChild; ++i) {
        for (int j = 0; j < space_cnt; ++j) {
            if (j == 0 || j % 5 == 0)
                printf("|");
            else
                printf(" ");
        }
        printf("|\n");
        for (int j = 0; j < space_cnt; ++j) {
            if (j == 0)
                printf("|");
            else
                printf(" ");
        }
        printf("+");
        printf("----");
        TPrint(u->children[i], space_cnt + 5, deep++);
    }
}

void create_node(TNode* u, int v) {
    TNode *nd = malloc(sizeof(TNode));
    nd->parent = u;
    nd->value = v;
    nd->CountChild = 0;
    nd->available_cnt = maxchild;
    nd->children = malloc(sizeof(TNode) * maxchild);
    if(u->CountChild == u->available_cnt - 1) {
        u->children = realloc(u->children, sizeof(TNode) * u->available_cnt * 2);
        u->available_cnt *= 2;
    }
    u->children[u->CountChild] = nd;
    u->CountChild++;
}

int RInput(char *str) {
    char *str1 = str;
    bool f = 1;
    while (*str) {
        if (*str < '0' || *str >'9')
            f = 0;
        str++;
    }
    if (f)
        return atoi(str1);
    else {
        printf("Wrong input, try again\n");
        printf("Enter values/option: ");
        return -1;
    }
}

TNode *create_genesis_node(int v) {
    TNode *nd = malloc(sizeof(TNode));
    nd->parent = NULL;
    nd->value = v;
    nd->CountChild = 0;
    nd->available_cnt = maxchild;
    nd->children = malloc(sizeof(TNode) * maxchild);
    return nd;
}

int PushStack(TStackHead *HeadS, TNode *Node, int depth) {
    if (!HeadS) //проверка ошибки если голова пустая
        return 1;
    TStackList* New;
    if (!(New = (TStackList*)malloc(sizeof(TStackList)))) { //проверка, получилось ли выделить память
        return 1;
    }
    New->Node = Node;//присваиваю значение
    New->depth = depth;
    if (!HeadS->Head) {//если нет первого элемента
        New->Next = NULL;
    }
    else {
        New->Next = HeadS->Head;//если первый есть, то new указывает на него
    }
    HeadS->Head = New;//первый элемент - new, new->next(указывает на тот что раньше был первый)
    return 0;
}

TStackList* PopStack(TStackHead *HeadS) {
    if (!HeadS || !HeadS->Head) {
        return NULL;
    }
    TStackList* Temp = HeadS->Head;//временный элемент, достаем стека первый
    HeadS->Head = HeadS->Head->Next;//второй становится первым
    return Temp;
}

int Detour(TNode* Tree) {
    if (!Tree)
        return -1;
    TStackHead Head;
    Head.Head = NULL;
    if (PushStack(&Head, Tree, 1)) {
        return -1;
    }
    int maxdepth = 0;
    while(true) {
        TStackList* Temp = PopStack(&Head);
        if (!Temp) {
            break;
        }
        if (maxdepth < Temp->depth)
            maxdepth = Temp->depth;
        for (int i = 0; i < Temp->Node->CountChild;i++) {
            if (PushStack(&Head, Temp->Node->children[i],Temp->depth + 1))
                return -1;
        }
        free(Temp);
    }
    return maxdepth;
}

int main(void) {
    int op;
    int u, v;
    char op1[] = "";
    char u1[] = "";
    char v1[] = "";
    printf("1 - Create new rib\n");
    printf("2 - Delete a rib\n");
    printf("3 - Print the tree\n");
    printf("4 - Check the depth\n");
    printf("5 - End a program\n");
    printf("At first, enter the value of the genesis node (one parameter),\nthen add new ribs by entering the value of the existing node, then enter a value of the new node (two parameters).\n");
    while (true) {
        printf("Enter option: ");
        do {
            scanf("%s", op1);
            op = RInput(op1);
        } while (op == -1);
        switch (op) {
            case 1:
                if (is_genesis) {
                    printf("Enter the value of genesis: ");
                    do {
                        scanf("%s", u1);
                        u = RInput(u1);
                    } while (u == -1);
                }
                else {
                    printf("Enter values: ");
                    do {
                        scanf("%s", u1);
                        u = RInput(u1);
                        scanf("%s", v1);
                        v = RInput(v1);
                    } while (u == -1 || v == -1);
                }
                if (u < 0 || v < 0) {
                    printf("Incorrect data\n");
                    continue;
                }
                if (is_genesis) {
                    is_genesis = false;
                    genesis_node = create_genesis_node(u);
                } else {
                    TNode *u_address = FindNode(genesis_node, u);
                    if (u_address == NULL) {
                        printf("There is no such vertex in the tree\n");
                        continue;
                    }
                    TNode *v_address = FindNode(genesis_node, v);
                    if (v_address == NULL) {
                        create_node(u_address, v);

                    } else {
                        printf("There is such vertex in the tree already\n");
                    }
                }
                break;
            case 2:
                printf("Deleting node: ");
                scanf("%d", &u);
                TNode* vert = FindNode(genesis_node, u);
                if (vert == NULL)
                    printf("Incorrect data\n");
                else
                    DelNode(vert);
                break;
            case 3:
                if (genesis_node != NULL) {
                    printf("\n");
                    TPrint(genesis_node, 0, 0);
                    printf("\n");
                }
                else
                    printf("The tree is empty\n");
                break;
            case 4:
                printf("Depth: %d\n", Detour(genesis_node));
                break;
            case 5:
                return 0;
            default:
                printf("No such option\n");
        }
    }
}