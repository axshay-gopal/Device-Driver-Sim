#include <linux/init.h>

#include <linux/module.h>

MODULE_LICENSE("GPL");

MODULE_AUTHOR("Your Name");

MODULE_DESCRIPTION("My First Device Driver");

static int __init mydevice_init(void)

{

printk(KERN_INFO "mydevice: Driver loaded!\n");

return 0;

}

static void __exit mydevice_exit(void)

{

printk(KERN_INFO "mydevice: Driver unloaded!\n");

}

module_init(mydevice_init);

module_exit(mydevice_exit);
