
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "LinkedList.h"

//
// Initialize a linked list
//
void llist_init(LinkedList * list)
{
	list->head = NULL;
}

//
// It prints the elements in the list in the form:
// 4, 6, 2, 3, 8,7
//
void llist_print(LinkedList * list) {
	
	ListNode * e;

	if (list->head == NULL) {
		printf("{EMPTY}\n");
		return;
	}

	printf("{");

	e = list->head;
	while (e != NULL) {
		printf("%d", e->value);
		e = e->next;
		if (e!=NULL) {
			printf(", ");
		}
	}
	printf("}\n");
}

//
// Appends a new node with this value at the beginning of the list
//
void llist_add(LinkedList * list, int value) {
	// Create new node
	ListNode * n = (ListNode *) malloc(sizeof(ListNode));
	n->value = value;
	
	// Add at the beginning of the list
	n->next = list->head;
	list->head = n;
}

//
// Returns true if the value exists in the list.
//
int llist_exists(LinkedList * list, int value) {
    while ((list->head->value != value) && (list->head->next != NULL)) {
        list->head = list->head->next;
    }
    if ((list->head->value != value) && (list->head != NULL))
        return 0;
    if (list->head->value == value)
        return 1;
}

//
// It removes the entry with that value in the list.
//
int llist_remove(LinkedList * list, int value) {
    ListNode * current = list->head;
    ListNode * pre = NULL;

    if (current == NULL) {
        return 0;
    }
    if (current->next == NULL) {
        if(current->value == value) {
            free(current);
            list->head = NULL;
            return 1;
        }
    }
    while (current != NULL) {
        if(current->next == NULL && current->value == value) {
            pre->next = NULL;
            return 1;
        } else if(current->next != NULL && current->value == value) {
            current->value = current->next->value;
            current->next = current->next->next;
            return 1;
        }
        pre = current;
        current = current->next;
    }
    return 0;
}

//
// It stores in *value the value that correspond to the ith entry.
// It returns 1 if success or 0 if there is no ith entry.
//
int llist_get_ith(LinkedList * list, int ith, int * value) {
    LinkedList * cp = (LinkedList *) malloc(sizeof(LinkedList));
    memcpy(cp, list, sizeof(list));
    int i;
    ListNode * current = cp->head;
    for (i = 0; i < ith; i++) {
        if (current == NULL)
            return 0;
        current = current->next;
    }
    *value = current->value;
	return 1;
}

//
// It removes the ith entry from the list.
// It returns 1 if success or 0 if there is no ith entry.
//
int llist_remove_ith(LinkedList * list, int ith) {
	ListNode * current = list->head;
    ListNode * pre = NULL;
    int i;
    if (current == NULL)
        return 0;
    for (i = 0; i < ith; i++) {
        if (current == NULL)
            return 0;
        pre = current;
        current = current->next;
    }
    if (current->next == NULL) {
        pre->next = NULL;
        free(current);
        return 1;
    } else
        pre->next = current->next;
    return 1;
}

//
// It returns the number of elements in the list.
//
int llist_number_elements(LinkedList * list) {
    LinkedList * cp = (LinkedList *) malloc(sizeof(LinkedList));
    memcpy(cp, list, sizeof(list));
    int i = 0;
    while(cp->head != NULL) {
        i++;
        cp->head = cp->head->next;
    }
	return i;
}


//
// It saves the list in a file called file_name. The format of the
// file is as follows:
//
// value1\n
// value2\n
// ...
//
int llist_save(LinkedList * list, char * file_name) {
    FILE* fp = fopen(file_name, "w");
    ListNode * current = list->head;
    if (fp == NULL)
        return 0;
    while (current != NULL) {
        fprintf(fp, "%d\n", current->value);
        current = current->next;
    }
    fclose(fp);
    return 0;
}

//
// It reads the list from the file_name indicated. If the list already has entries, 
// it will clear the entries.
//
int llist_read(LinkedList * list, char * file_name) {
    // ListNode * current = (ListNode *) malloc(sizeof(ListNode));
    FILE* fp = fopen(file_name, "r");
    char re[50] = "";
    int value, i;
    i = 0;
    char c;
    if (!fp)
        return 0;
    while ((c = fgetc(fp)) != EOF) {
        re[i++] = c;
        // printf("-----------%c-------------\n", c);
        if (c == '\n') {
            // printf("-----------%c-------------\n", c);
            value = atoi(re);
            // printf("------------- %d -------------\n", value);
            i = 0;
            //re = "";
            ListNode * current = (ListNode *) malloc(sizeof(ListNode));
            current->value = value;
            // current->next = list->head;
            // list->head->next = list->head;
            current->next = list->head;
            list->head = current;
        }
    }
	return 1;
}


//
// It sorts the list. The parameter ascending determines if the
// order si ascending (1) or descending(0).
//
void llist_sort(LinkedList * list, int ascending) {

    int num = llist_number_elements(list);
    int sortArray[num];
    int i, j, temp, out;
    // implement array
    for (i = 0; i < num; i++) {
        sortArray[i] = 0;
        out = llist_get_ith(list, i, &sortArray[i]);
    }
    // descending 
    if (ascending == 1) {
        for (j = 0; j < num-1; j++) {
            for (i = 0; i < num-1-j; i++) {
                if (sortArray[i] < sortArray[i+1]) {
                    temp = sortArray[i];
                    sortArray[i] = sortArray[i+1];
                    sortArray[i+1] = temp;
                }
            }
        }
    }
    // ascending 
    if (ascending == 0) {
        for (j = 0; j < num-1; j++) {
            for (i = 0; i < num-1-j; i++) {
                if (sortArray[i] > sortArray[i+1]) {
                    temp = sortArray[i];
                    sortArray[i] = sortArray[i+1]; 
                    sortArray[i+1] = temp;
                }
            }
        }
    }

    //
    llist_init(list);
    for (i = 0; i < num; i++) {
        llist_add(list, sortArray[i]);
    }
}

//
// It removes the first entry in the list and puts value in *value.
// It also frees memory allocated for the node
//
int llist_remove_first(LinkedList * list, int * value) {
    ListNode * current = list->head;

    if (current == NULL)
        return 0;
    //printf("------------- %d ------------\n", list->head->value);
    if (current->next == NULL) {
        *value = current->value;
        free(current);
        list->head = NULL;
        return 1;
    }
    if (current != NULL) {
        *value = current->value;
        current->value = current->next->value;
        current->next = current->next->next;
        return 1;
    }
    return 0;
    //int out;
    //printf("------------- %d ------------\n", list->head->value);
    //llist_get_ith(list, 0, value);
    //printf("--------------%d--------------\n", *value);

    //return llist_remove_ith(list, 0);
}

//
// It removes the last entry in the list and puts value in *value/
// It also frees memory allocated for node.
//
int llist_remove_last(LinkedList * list, int *value) {
    ListNode * current = list->head;
    ListNode * pre = NULL;
    if (current == NULL)
        return 0;
    int num = llist_number_elements(list);
    int i;
    if (current == NULL) 
        return 0;
    for (i = 0; i < num-1; i++) {
        if (current == NULL) {
            return 0;
        }
        pre = current;
        current = current->next;
    }
    if (current->next == NULL) {
        pre->next = NULL;
        *value = current->value;
        free(current);
        return 1;
    }

        return 0;
}

//
// Insert a value at the beginning of the list.
// There is no check if the value exists. The entry is added
// at the beginning of the list.
//
void llist_insert_first(LinkedList * list, int value) {
    llist_add(list, value);
}

//
// Insert a value at the end of the list.
// There is no check if the name already exists. The entry is added
// at the end of the list.
//
void llist_insert_last(LinkedList * list, int value) {
    ListNode * n = (ListNode *)malloc(sizeof(ListNode));
    ListNode * current = list->head;
    ListNode * pre = NULL;
    n->value = value;
    if (current == NULL) {
        list->head = n;
        n->next = NULL;
        return;
    }
    int num = llist_number_elements(list);
    int i;
    for (i = 0; i < num-1; i++) {
        pre = current;
        current = current->next;
    }
    if (current->next == NULL) {
        current->next = n;
        n->next = NULL;
    }
}

//
// Clear all elements in the list and free the nodes
//
void llist_clear(LinkedList *list)
{
    llist_init(list);
}
