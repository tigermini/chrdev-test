#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>

#define CHRDEV_DEBUG
#ifdef CHRDEV_DEBUG
#define debug(fmt, args...) printk(KERN_DEBUG fmt, ##args)
#else
#define debug(fmt, args...)
#endif

#define CHRDEV_NAME "chrdev_tiger"
#define CHR_MAJOR 0

/* 定义全局主设备号，方便释放主设备号 */
static int major;
/* 定义全局class结构体，方便释放class */
static struct class *chrdev_class;

static const struct file_operations chrdev_fops = {
    .owner = THIS_MODULE,
    .read = NULL,
    .write = NULL,
    .unlocked_ioctl = NULL,
    .open = NULL,
    .release = NULL
};

int hello_init(void)
{
    int ret;

    debug("[%s]hello init\n", __FUNCTION__);

    /* 对于整体来说，这一步相当于初始化-init */
    major = register_chrdev(CHR_MAJOR, CHRDEV_NAME, &chrdev_fops);

    debug("[%s]major = %d\n", __FUNCTION__, major);

    if (major == 0) {
        return -1;
    }

    /* 在system下创建节点，可以使用ls /sys/module来查看，
     * 可以参考内核drivers/usb/core/file.c驱动 
     */
    ret = 0;
    chrdev_class = class_create(THIS_MODULE, CHRDEV_NAME);
    if (IS_ERR(chrdev_class)) {
        ret = PTR_ERR(chrdev_class);
        printk(KERN_ERR "chrdev class failed\n");
        goto class_err;
    }

    return 0;

class_err:
    unregister_chrdev(major, CHRDEV_NAME);
    return ret;
}

void hello_exit(void)
{
    printk("hello exit\n");

    class_destroy(chrdev_class);

    unregister_chrdev(major, CHRDEV_NAME);
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");

