#include <linux/module.h>     /* Needed by all modules */
#include <linux/kernel.h>     /* Needed for KERN_INFO */
#include <linux/init.h>       /* Needed for the macros */
#include <linux/list.h>
#include <linux/slab.h>
 
MODULE_LICENSE("GPL");
 
MODULE_AUTHOR("1337H4X0r");
 
MODULE_DESCRIPTION("A simple Hello world LKM!");
 
MODULE_VERSION("0.1");

#define MAX_NAME_LEN 20

typedef struct identity {
    char name[MAX_NAME_LEN];
    int id;
    bool busy;
    struct list_head list; /* kernel's list structure */
};

static LIST_HEAD(identities);

int identity_create(char *name, int id) {
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

	list_add(&new->list, &identities);
	
	pr_info("Added %s with id %d\n", new->name, id);

	return 0;
}

struct identity * identity_find(int id) {
	struct identity *curr = NULL;
	list_for_each_entry(curr, &identities, list) {
		if (id == curr->id) {
			return curr;
		}
	}
	return NULL;
}


void identity_destroy(int id) {
        struct identity *found = identity_find(id);
        if (found) {
                list_del(&found->list);
		kfree(found);
        }
}



static int __init hello_start(void)
{
    struct identity *temp = NULL;
    printk(KERN_INFO "Hello world\n");
    if (identity_create("Alice", 1)) {
        goto return_start;
    }
    if (identity_create("Bob", 2)) {
        goto clean_alice;
    }
    if (identity_create("Dave", 3)) {
        goto clean_bob;
    }

    temp = identity_find(3);
    pr_info("id 3 = %s\n", temp->name);

    identity_destroy(3);
clean_bob:
    identity_destroy(2);
clean_alice:
    identity_destroy(1);
return_start:
    return 0;
}
 
static void __exit hello_end(void)
{
    printk(KERN_INFO "Goodbye Mr.\n");
}
 
module_init(hello_start);
module_exit(hello_end);
