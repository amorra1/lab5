#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct bst_node *bst;

struct bst_node {
    int value;
    bst left, right;
};

void bst_print_elements(bst tree) {
    if (tree != NULL) {
        bst_print_elements(tree->left);
        printf("%d, ", tree->value);
        bst_print_elements(tree->right);
    }
}

void bst_print(bst tree) {
    printf("{");
    bst_print_elements(tree);
    printf("}\n");
}


bool bst_insert(bst *tree, int value) {
    // TODO: Task 1: insert 'value' into 'tree'.
       if (*tree == NULL) { //if there is no tree
        *tree = malloc(sizeof(struct bst_node)); //create a new tree
        (*tree)->value = value;
        (*tree)->left = NULL;
        (*tree)->right = NULL;
        return true;
    } else if (value < (*tree)->value) //if value is less than the value of the tree
        return bst_insert(&(*tree)->left, value); //insert left
    else if (value > (*tree)->value) //if value is greater than the value of the tree
        return bst_insert(&(*tree)->right, value); //insert right
    else
        return false;
}

bool bst_remove(bst *tree, int value) {
    // TODO: Task 2: remove 'value' from 'tree'.
    if (*tree == NULL) //if there is no tree
        return false;
    else if (value < (*tree)->value) //if value is less than the value of the tree
        return bst_remove(&(*tree)->left, value); //remove left 
    else if (value > (*tree)->value) //if value is greater than the value of the tree
        return bst_remove(&(*tree)->right, value); //remove right
    else if ((*tree)->left == NULL && (*tree)->right == NULL) { //if there is no left and right tree
        free(*tree); //free the tree
        *tree = NULL; //set the tree to NULL
        return true;
    } else if ((*tree)->left == NULL) { //if there is no left tree
        bst tmp = *tree; //create a new tree
        *tree = (*tree)->right; //set the tree to the right
        free(tmp); //free the tree
        return true;
    } else if ((*tree)->right == NULL) { //if there is no right tree
        bst tmp = *tree; //create a new tree
        *tree = (*tree)->left; //set the tree to the left
        free(tmp);  //free the tree
        return true;
    } else { //if there is a left and right tree
        bst *tmp = &(*tree)->right; //create a new tree
        while ((*tmp)->left != NULL) //while the left tree is not NULL
            tmp = &(*tmp)->left; //set the tree to the left
        (*tree)->value = (*tmp)->value; 
        return bst_remove(tmp, (*tmp)->value); //remove the tree
     }
    return false;
}

//copy helper function
void bst_copy_tree(bst copy_tree, bst* result) {
    if (copy_tree != NULL) //if there is a tree
    {
    bst_insert(result, copy_tree->value); //insert the tree
    bst_copy_tree(copy_tree->left, result); //copy the left tree
    bst_copy_tree(copy_tree->right, result); //copy the right tree
    }
}

bst bst_union(bst tree1, bst tree2) {
    // TODO: Task 3: compute the union of 'tree1' and 'tree2'.
    if (tree1 == NULL) { //if tree1 is NULL, return tree2
        return tree2;
    }
    if (tree2 == NULL) { //if tree2 is NULL, return tree1
        return tree1;
    }

    bst result = NULL; //create a new tree
    bst_insert(&result, tree1->value); //insert the tree

    bst_copy_tree(tree1->left, &result); //copy the left tree
    bst_copy_tree(tree1->right, &result); //copy the right tree

    bst_copy_tree(tree2, &result); //copy tree2

    return result;
}


bst bst_intersection(bst tree1, bst tree2) {
    // TODO: Task 3: compute the intersection of 'tree1' and 'tree2'.
    bst result = NULL; //create a new tree
    struct bst_node **stack1 = (struct bst_node **)malloc(sizeof(struct bst_node*)); //create a stack1
    struct bst_node **stack2 = (struct bst_node **)malloc(sizeof(struct bst_node*)); //create a stack2
    int top1 = -1, top2 = -1;

    while(1){ //while true
        while(tree1 !=NULL){ //while tree1 is not NULL
            stack1[++top1] = tree1; //push tree1 into stack1
            tree1 = tree1->left; //set tree1 to the left
        }
        while(tree2 !=NULL){ //while tree2 is not NULL
            stack2[++top2] = tree2; //push tree2 into stack2
            tree2 = tree2->left; //set tree2 to the left
        }
        if(top1 == -1 || top2 == -1){ //if top1 or top2 is -1
            break;
        }
        if(stack1[top1]->value == stack2[top2]->value){ //if the value of stack1 is equal to the value of stack2
            bst_insert(&result, stack1[top1]->value); //insert the value of stack1 into result
            tree1 = stack1[top1]->right; //set tree1 to the right
            tree2 = stack2[top2]->right; //set tree2 to the right
            top1--;
            top2--;
        }
        else if(stack1[top1]->value < stack2[top2]->value){ //if the value of stack1 is less than the value of stack2
            tree1 = stack1[top1]->right; //set tree1 to the right
            top1--;
        }
        else if(stack1[top1]->value > stack2[top2]->value){ //if the value of stack1 is greater than the value of stack2
            tree2 = stack2[top2]->right; //set tree2 to the right
            top2--;
        }
    }
    free(stack1); //free the stacks
    free(stack2); 
    
    return result;
}

int main() {
    // Initialize two trees.
    bst tree1 = NULL, tree2 = NULL;

    assert(bst_insert(&tree1, 1));
    assert(bst_insert(&tree1, 9));
    assert(bst_insert(&tree1, 2));
    assert(bst_insert(&tree1, 7));
    assert(bst_insert(&tree1, 0));
    assert(bst_insert(&tree1, 3));
    assert(bst_insert(&tree1, 13));
    assert(bst_insert(&tree1, 5));
    assert(!bst_insert(&tree1, 3));

    // Should print: {0, 1, 2, 3, 5, 7, 9, 13, }
    bst_print(tree1);

    assert(bst_insert(&tree2, 12));
    assert(bst_insert(&tree2, 13));
    assert(bst_insert(&tree2, 1));
    assert(bst_insert(&tree2, 0));
    assert(bst_insert(&tree2, 19));
    assert(bst_insert(&tree2, 7));
    assert(!bst_insert(&tree2, 13));
    assert(bst_insert(&tree2, 5));

    // Should print: {0, 1, 5, 7, 12, 13, 19, }
    bst_print(tree2);

    // Should print: {0, 1, 2, 3, 5, 7, 9, 12, 13, 19, }
    bst union_1_2 = bst_union(tree1, tree2);
    bst_print(union_1_2);

    // Should print: {0, 1, 5, 7, 13, }
    bst intersection_1_2 = bst_intersection(tree1, tree2);
    bst_print(intersection_1_2);

    return 0;
}
