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

#include "identity.h"

MODULE_AUTHOR("BitsCruncher");
MODULE_DESCRIPTION("Eudyptola18");
MODULE_LICENSE("GPL");

static LIST_HEAD(identities);

static const char id[] = "BitsCruncher";

ssize_t hello_write (struct file *f, const char __user *c, size_t s, loff_t *l)
{
     	char local_buf[sizeof(id)] = {0}; 
     
	if (s != sizeof(id) - 1){ 
		return -EINVAL;   
	}
     	if (copy_from_user(local_buf, c, s)) {
        	return -EFAULT;
     	}
     	if (strncmp(local_buf, id, s) == 0) {
		return s;
     	}
 
	return -EINVAL;
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

static wait_queue_head_t wee_wait;
static struct task_struct *eu_thread = NULL;

static int wait_to_die(void *data)
{
	wait_event(wee_wait, kthread_should_stop());
	return 0;
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
	kthread_stop(eu_thread); 
}

module_init(eu17_init);
module_exit(eu17_exit);
