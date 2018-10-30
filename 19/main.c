#include <linux/module.h>	
#include <linux/kernel.h>	
#include <linux/init.h>
#include <linux/printk.h>

MODULE_AUTHOR("BitsCruncher");
MODULE_DESCRIPTION("Eudyptola19");
MODULE_LICENSE("GPL");

static const char id[] = "BitsCruncher";

static int __init eu18_init(void)
{
	pr_info("eudy hey\n");
        return 0;
}

static void __exit eu18_exit(void)
{
	pr_info("eudy bye\n");
}

module_init(eu18_init);
module_exit(eu18_exit);
