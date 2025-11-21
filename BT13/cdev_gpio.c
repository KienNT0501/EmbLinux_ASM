#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/string.h>
#define GPIO_BASE_RPI3 0x3F200000
#define GPIO_SIZE      0xB4

#define GPFSEL2_OFFSET 0x08
#define GPSET0_OFFSET  0x1C
#define GPCLR0_OFFSET  0x28

static void __iomem *gpio_base = NULL;

/* Device struct */
static dev_t dev_num;
static struct class *cls;
static struct cdev c_dev;

static void gpio27_set_output(void)
{
    uint32_t val;
    volatile uint32_t *gpfsel2 = gpio_base + GPFSEL2_OFFSET;

    val = readl(gpfsel2);
    val &= ~(0x7 << 21);
    val |=  (0x1 << 21);
    writel(val, gpfsel2);
}

static void gpio27_on(void)
{
    volatile uint32_t *gpset0 = gpio_base + GPSET0_OFFSET;
    writel(1 << 27, gpset0);
}

static void gpio27_off(void)
{
    volatile uint32_t *gpclr0 = gpio_base + GPCLR0_OFFSET;
    writel(1 << 27, gpclr0);
}

/* Character device ops */
static ssize_t m_write(struct file *f, const char __user *buf, size_t len, loff_t *off)
{
    char kbuf[10];

    if (len > 9) len = 9;

    if (copy_from_user(kbuf, buf, len))
        return -EFAULT;

    kbuf[len] = '\0';

    if (strcmp(kbuf, "on") == 0) {
        gpio27_on();
        printk("GPIO27 -> HIGH\n");
    }
    else if (strcmp(kbuf, "off") == 0) {
        gpio27_off();
        printk("GPIO27 -> LOW\n");
    }
    else {
        printk("Unknown command: %s\n", kbuf);
    }

    return len;
}

static int m_open(struct inode *inode, struct file *file) {
    return 0;
}

static int m_release(struct inode *inode, struct file *file) {
    return 0;
}

static struct file_operations fops = {
    .owner   = THIS_MODULE,
    .open    = m_open,
    .write   = m_write,
    .release = m_release,
};

/* Module init */
static int __init mod_init(void)
{
    printk("Loading GPIO27 module...\n");

    gpio_base = ioremap(GPIO_BASE_RPI3, GPIO_SIZE);
    if (!gpio_base) return -ENOMEM;

    gpio27_set_output();
    gpio27_off();

    alloc_chrdev_region(&dev_num, 0, 1, "mgpio");
    cls = class_create(THIS_MODULE,"mgpio_class");
    device_create(cls, NULL, dev_num, NULL, "mgpio");

    cdev_init(&c_dev, &fops);
    cdev_add(&c_dev, dev_num, 1);

    printk("Module loaded: /dev/mgpio ready\n");
    return 0;
}

/* Module exit */
static void __exit mod_exit(void)
{
    gpio27_off();

    cdev_del(&c_dev);
    device_destroy(cls, dev_num);
    class_destroy(cls);
    unregister_chrdev_region(dev_num, 1);
    iounmap(gpio_base);

    printk("GPIO27 module unloaded\n");
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("kiennt167");
MODULE_DESCRIPTION("GPIO27 Control Driver");
MODULE_VERSION("1.0");
