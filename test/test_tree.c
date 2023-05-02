#include "tree.h"
#include "array_list.h"
#include "test_tree.h"
#include <assert.h>
#include <stdio.h>

int *create_int_malloc(int v){
    int *malloced_int = malloc(sizeof(int));
    *malloced_int = v;
    return malloced_int;
}

void test_create(void){
    printf("\t%s\t", __func__);
    int *a = create_int_malloc(50);
    node_t* root = tree_create(a, free);
    assert(array_list_length(node_get_children(root)) == 0);
    assert(node_get_parent(root) == NULL);
    assert(node_get_value(root) == a);
    tree_free(root);
    printf("OK\n");
}

void test_add_child(void){
    printf("\t%s\t", __func__);

    int *a = create_int_malloc(50);
    node_t* root = tree_create(a, free);
    int *b = create_int_malloc(200);
    node_t* root_child_one = node_add_child(root, b);
    int *c = create_int_malloc(10);
    node_t* root_child_one_child_one = node_add_child(root_child_one, c);
    int *d = create_int_malloc(3);
    node_t* root_child_one_child_two = node_add_child(root_child_one, d);

    assert(array_list_length(node_get_children(root)) == 1);
    assert(node_get_parent(root_child_one) == root);
    assert(node_get_value(root_child_one) == b);
    assert(array_list_length(node_get_children(root_child_one)) == 2);
    assert(node_get_parent(root_child_one_child_one) == root_child_one);
    assert(node_get_parent(root_child_one_child_two) == root_child_one);

    tree_free(root);
    printf("OK\n");

}

int vs_cmp_int_int(void *integer, void *integer2){
    return *(int*)integer - *(int*)integer2;
}

void test_remove_child(void){
    printf("\t%s\t", __func__);

    int *a = create_int_malloc(50);
    node_t* root = tree_create(a, free);
    int *b = create_int_malloc(200);
    node_t* root_child_one = node_add_child(root, b);
    int *c = create_int_malloc(10);
    node_add_child(root_child_one, c);
    int *d = create_int_malloc(3);
    node_t* root_child_one_child_two = node_add_child(root_child_one, d);
    node_t *cur_node = root_child_one_child_two;
    for(int i = 0; i<5; i++){
        cur_node = node_add_child(cur_node, create_int_malloc(i));
    }
    assert(array_list_length(node_get_children(root_child_one_child_two)) == 1);

    node_t* child = array_list_get(node_get_children(root_child_one_child_two), 0);
    node_t* to_remove = array_list_get(node_get_children(child), 0);
    int check = 1;
    node_t* check_remove = node_remove_child(child, &check, vs_cmp_int_int);
    assert(check_remove == to_remove);
    assert(array_list_length(node_get_children(child)) == 0);

    tree_free(to_remove);
    tree_free(root);
    printf("OK\n");

}

void test_has_parent(void){
    printf("\t%s\t", __func__);
    int *integer_list[10];
    for(int i = 0; i<10; i++){
        integer_list[i] = create_int_malloc(i);
    }
    node_t* zero = tree_create(integer_list[0], free);
    node_t* one = node_add_child(zero, integer_list[1]);
    node_t* seven = node_add_child(one, integer_list[7]);
    node_add_child(seven, integer_list[9]);
    node_t* two = node_add_child(zero, integer_list[2]);
    node_t* three = node_add_child(two, integer_list[3]);
    node_add_child(three, integer_list[8]);
    node_t* four = node_add_child(two, integer_list[4]);
    node_t* five = node_add_child(four, integer_list[5]);
    node_t* six = node_add_child(five, integer_list[6]);

    assert(!node_has_parent(six, integer_list[7], vs_cmp_int_int));
    assert(node_has_parent(six, integer_list[0], vs_cmp_int_int));
    assert(node_has_parent(three, integer_list[2], vs_cmp_int_int));

    tree_free(zero);
    printf("OK\n");

}

void test_trees(void){
	printf("Running test file \"%s\"\n", __FILE__);
    test_create();
    test_add_child();
    test_remove_child();
    test_has_parent();
}