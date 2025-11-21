#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
// Define Author, Description and Version
#define DRIVER_AUTHOR "kiennt167"
#define DRIVER_DESC "Hello World Kernel Module"
#define DRIVER_VERS "1.0"

// File Operations 
static int m_open(struct inode *inode, struct file *file);
static int m_release(struct inode *inode, struct file *file);
static ssize_t m_read(struct file *filp, char __user *user_buf, size_t size, loff_t *offset);
static ssize_t m_write(struct file *filp, const char __user *user_buf, size_t size, loff_t *offset);
// Define struct of Device Number
struct m_dev {
    dev_t dev_num;
    struct class *m_class;
    struct cdev m_cdev;
};

static struct m_dev m_dev;

// Function Pointers
static struct file_operations fops = {
    .owner   = THIS_MODULE,
    .read    = m_read,
    .write   = m_write,
    .open    = m_open,
    .release = m_release,
};
// Function Definitions
static int m_open(struct inode *inode, struct file *file){
    printk("Called system call open()...\n");
    return 0;
}
static int m_release(struct inode *inode, struct file *file){
    printk("Called system call release()...\n");
    return 0;
}
static ssize_t m_read(struct file *filp, char __user *user_buf, size_t size, loff_t *offset){
    char *data = "Hello from mydevice!\n";
    int datalen = strlen(data);
    if(*offset >= datalen) return 0;
    if(size > datalen - *offset) size = datalen - *offset;
    if(copy_to_user(user_buf, data + *offset, size)) return -EFAULT;
    *offset += size;
    return size;
}
static ssize_t m_write(struct file *filp, const char __user *user_buf, size_t size, loff_t *offset){
     printk("mydevice: received %zu bytes\n", size);
    char kbuf[256];
    if (size > 255)
        size = 255;
    if (copy_from_user(kbuf, user_buf, size))
        return -EFAULT;
    kbuf[size] = '\0';
    printk("mydevice write: %s\n", kbuf);
    return size;
}
/* Constructor */
static int __init chdev_init(void){
    // 1. Allocate a device number for device
    if(alloc_chrdev_region(&m_dev.dev_num, 0, 1, "dev_num") < 0){
        pr_err("Failed to allocate device number\n");
        return -1;
    }
    // 2. Create device class
    m_dev.m_class = class_create("m_class");
    if (IS_ERR(m_dev.m_class)) {
        pr_err("Failed to create Device Class\n");
        goto rm_dev_num;
    }

    // 3. Create Device file
    if(device_create(m_dev.m_class, NULL, m_dev.dev_num, NULL, "kien_m_device") == NULL){
        pr_err("Failed to create Device File\n");
        goto rm_dev_class;
    }
    // 4. Create Character Device Driver
    cdev_init(&m_dev.m_cdev, &fops);
    // 5. Adding Character Device into system
    if((cdev_add(&m_dev.m_cdev, m_dev.dev_num, 1))<0){
        pr_err("Failed to add the device to system\n");
        goto rm_dev_file;
    }
    printk("Call chdev_init()\n");
    return 0;

rm_dev_class:
    class_destroy(m_dev.m_class);
rm_dev_num:
    unregister_chrdev_region(m_dev.dev_num,1);
rm_dev_file:
    device_destroy(m_dev.m_class, m_dev.dev_num);
    return -1;
}
/* Destructor */
static void __exit chdev_exit(void){
    cdev_del(&m_dev.m_cdev);
    device_destroy(m_dev.m_class, m_dev.dev_num);
    class_destroy(m_dev.m_class);
    unregister_chrdev_region(m_dev.dev_num,1);
    printk("Call chdev_exit()\n");
}

module_init(chdev_init);
module_exit(chdev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_VERSION(DRIVER_VERS);

