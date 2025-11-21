#include <linux/module.h>

#define DRIVER_AUTHOR "kiennt167"
#define DRIVER_DESC "Hello World Kernel Module"
#define DRIVER_VERS "1.0"

static int __init lkm_init(void){
    printk("Hello, Kernel World!\n");
    return 0;
}

static void __exit lkm_exit(void){
    printk("Goodbye, Kernel World!\n");
}

module_init(lkm_init);
module_exit(lkm_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_VERSION(DRIVER_VERS);