#include <linux/slab.h>
#include "identity.h"


int identity_create(struct identities *idents, char *name, int id) 
{
        struct identity *new = NULL;

        if (!idents || !name) {
		pr_info("NULL param");
		return 1;
	}

	new = kmalloc(sizeof(*new), GFP_KERNEL);
        if (NULL == new) {
                pr_info("Failed allocating identity\n");
                return 1;
        }

        strncpy(new->name, name, MAX_NAME_LEN);
        new->id = id;
        new->busy = false;
        INIT_LIST_HEAD(&new->list);

        mutex_lock(&idents->list_lock);
	list_add(&new->list, &idents->list);
	mutex_unlock(&idents->list_lock);

        pr_info("Added %s with id %d\n", new->name, id);

        return 0;
}


struct identity * __identity_find(struct identities *idents, int id, bool lock) 
{
        struct identity *curr = NULL;
        
        if (!idents) {
                pr_info("NULL param");
                return NULL;
        }

	if (lock) {
		mutex_lock(&idents->list_lock);
	}
	
	list_for_each_entry(curr, &idents->list, list) {
                if (id == curr->id) {
                        goto find_exit;
                }
        }

find_exit:
	if (lock) {
		mutex_unlock(&idents->list_lock);
	}
        return curr;
}

struct identity * identity_find(struct identities *idents, int id) 
{
	return __identity_find(idents, id, true);
} 

void identity_destroy(struct identities *idents, int id) 
{
        struct identity *found = NULL;
	
	if (!idents) {
                pr_info("NULL param");
        }

	mutex_lock(&idents->list_lock);
	
	found = __identity_find(idents, id, false);
        if (found) {
                list_del(&found->list);
                kfree(found);
        }
	mutex_unlock(&idents->list_lock);
}

struct identity * identity_get(struct identities *idents) {
	struct identity *found = NULL;
	
        if (!idents) {
                pr_info("NULL param");
                return NULL;
        }

	mutex_lock(&idents->list_lock);
	
	if (list_empty(&idents->list)) {
		goto get_exit;
	}
	found = list_last_entry(&idents->list, struct identity, list);
	list_del(&found->list);
get_exit:
	mutex_unlock(&idents->list_lock);
	return found;
}

