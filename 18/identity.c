#include <linux/slab.h>

#include "identity.h"

int identity_create(struct list_head *list, char *name, int id) {
        struct identity *new = NULL;

        new = kmalloc(sizeof(*new), GFP_KERNEL);
        if (NULL == new) {
                pr_info("Failed allocating identity\n");
                return 1;
        }

        strncpy(new->name, name, MAX_NAME_LEN);
        new->id = id;
        new->busy = false;
        INIT_LIST_HEAD(&new->list);

        list_add(&new->list, list);

        pr_info("Added %s with id %d\n", new->name, id);

        return 0;
}

struct identity * identity_find(struct list_head *list, int id) {
        struct identity *curr = NULL;
        list_for_each_entry(curr, list, list) {
                if (id == curr->id) {
                        return curr;
                }
        }
        return NULL;
}


void identity_destroy(struct list_head *list, int id) {
        struct identity *found = identity_find(list, id);
        if (found) {
                list_del(&found->list);
                kfree(found);
        }
}

