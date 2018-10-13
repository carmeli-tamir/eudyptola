#include <linux/module.h>	
#include <linux/kernel.h>	
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/errno.h>
#include <linux/string.h>

MODULE_AUTHOR("BitsCruncher");
MODULE_DESCRIPTION("Eudyptola17");
MODULE_LICENSE("GPL");

const char id[] = "BitsCruncher";

ssize_t hello_read (struct file *f, char __user *c, size_t s, loff_t *l)
{
	size_t m = min(s, sizeof(id) - 1);

	if (*l >= m) {
	    return 0;
	}

	if (copy_to_user(c, id , m)) {
	  return -EFAULT;
 	}
	
	*l += m;
    	
	return m;
}
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
    .release		= hello_release,
    .read 		= hello_read,
};


static struct miscdevice
hello_misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name  = "eudyptula",
	.fops  = &hello_fops,
};

static int __init eu17_init(void)
{
	int ret = 0;

        ret = misc_register(&hello_misc); 
        return ret;
}

static void __exit eu17_exit(void)
{
        misc_deregister(&hello_misc);
}

module_init(eu17_init);
module_exit(eu17_exit);
