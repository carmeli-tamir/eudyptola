#include <linux/list.h>

#define MAX_NAME_LEN 20

struct identity {
    char name[MAX_NAME_LEN];
    int id;
    bool busy;
    struct list_head list; /* kernel's list structure */
};

/**
 * identity_create - add a identity to the given list.
 * @list: A list.
 * @name: The name field of the new struct identity.
 * @id: The id field of the new struct identity.
 * 
 * @note: The new identity's busy field is set to false.
 */
int identity_create(struct list_head *list, char *name, int id);

/**
 * identity_find - find and return an identity with the given id.
 * @list: A list.
 * @id: The id of identity to find.
 * 
 * @returns: A pointer to the found identity,
 * or NULL if no such id exists in list.
 */
struct identity * identity_find(struct list_head *list, int id);

/**
 * identity_destroy -  remove from the list an identity with the given id.
 * @list: A list.
 * @id: The id of identity to remove.
 * 
 * @note: If the requested identity doesn't exist, nothing happens.
 */
void identity_destroy(struct list_head *list, int id);
