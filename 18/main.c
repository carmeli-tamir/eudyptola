#include <linux/module.h>	
#include <linux/kernel.h>	
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/kthread.h>
#include <linux/wait.h>
#include <linux/printk.h>
#include <linux/delay.h>
#include <linux/freezer.h>
#include <linux/slab.h>

#include "identity.h"

MODULE_AUTHOR("BitsCruncher");
MODULE_DESCRIPTION("Eudyptola18");
MODULE_LICENSE("GPL");

static DEFINE_ITENTITIES(identities);
static atomic_t id_counter = ATOMIC_INIT(0);
static const char id[] = "BitsCruncher";
static wait_queue_head_t wee_wait;

ssize_t hello_write (struct file *f, const char __user *c, size_t s, loff_t *l)
{
     	char local_buf[MAX_NAME_LEN] = {0}; 
     
	if (s >= MAX_NAME_LEN) { 
		s = MAX_NAME_LEN - 1;  
	}
     	if (copy_from_user(local_buf, c, s)) {
        	return -EFAULT;
     	}
     	if (identity_create(&identities, local_buf, atomic_inc_return(&id_counter))) {
		return -EINVAL;
	}
	
	wake_up(&wee_wait);
		
	return s;
}

int hello_open (struct inode *i, struct file *f)
{
	return 0;
}

int hello_release (struct inode *i, struct file *f)
{
	return 0;
}

static const struct file_operations hello_fops = {
    .owner			= THIS_MODULE,
    .write			= hello_write,
    .open			= hello_open,
    .release			= hello_release,
};


static struct miscdevice
hello_misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name  = "eudyptula",
	.fops  = &hello_fops,
        .mode  = 0222,
};

static struct task_struct *eu_thread = NULL;

static int wait_to_die(void *data)
{
	struct identity *iden = NULL;
	do {
		if (iden) {
			msleep_interruptible(5 * 1000);
			pr_info("kthread got id %d with name %s\n", iden->id, iden->name);
			kfree(iden);
		}
		wait_event(wee_wait, 
			kthread_should_stop() || (iden = identity_get(&identities)));
	}
	while(!kthread_should_stop());
	return 0;
}

static void clean_list(void)
{
	struct identity *iden = NULL;
	while ((iden = identity_get(&identities))) {
		kfree(iden);
	}
}

static int __init eu17_init(void)
{
	int ret = 0;
	init_waitqueue_head(&wee_wait);
        eu_thread = kthread_run(wait_to_die, NULL, "eudyptula");
	if (IS_ERR(eu_thread)) {
		pr_err("Failed creating eudyptola thread\n");
		return -ENOMEM;
	}
	ret = misc_register(&hello_misc); 
        return ret;
}

static void __exit eu17_exit(void)
{
	misc_deregister(&hello_misc);
	clean_list();
	kthread_stop(eu_thread); 
}

module_init(eu17_init);
module_exit(eu17_exit);
