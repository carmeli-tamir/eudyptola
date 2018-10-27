#include <linux/mutex.h>
#include <linux/list.h>

#define MAX_NAME_LEN 20

struct identity {
    char name[MAX_NAME_LEN];
    int id;
    bool busy;
    struct list_head list; /* kernel's list structure */
};

struct identities {
	struct list_head list;
	struct mutex list_lock;
};

#define DEFINE_ITENTITIES(ident) \
	struct identities ident = { \
		.list = LIST_HEAD_INIT(ident.list), \
		.list_lock = __MUTEX_INITIALIZER(ident.list_lock) \
		}

/**
 * identity_create - add a identity to the given list.
 * @idents: A struct identities.
 * @name: The name field of the new struct identity.
 * @id: The id field of the new struct identity.
 * 
 * @returns: 0 if succeeded.
 * @note: The new identity's busy field is set to false.
 */
int identity_create(struct identities *idents, char *name, int id);

/**
 * identity_find - find and return an identity with the given id.
 * @idents: A struct identities.
 * @id: The i identity to find.
 * 
 * @returns: A pointer to the found identity,
 * or NULL if no such id exists in list.
 */
struct identity * identity_find(struct identities *idents, int id);

/**
 * identity_destroy -  remove from the list an identity with the given id.
 * @idents: A struct identities.
 * @id: The id of identity to remove.
 * 
 * @note: If the requested identity doesn't exist, nothing happens.
 */
void identity_destroy(struct identities *idents, int id);

/**
 * identity_get - removes and returns the next identity on the list.
 * @idents: A struct identities.
 *
 * @note: The caller must release the memory returned.
 * @returns: A pointer to the next identity,
 * or NULL if the list is empty.
 */
struct identity * identity_get(struct identities *idents);
