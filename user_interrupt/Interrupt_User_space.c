#include <linux/module.h>
#include <linux/kernel.h>    /* printk() */
#include <linux/moduleparam.h>
#include <asm/uaccess.h>
#include <asm/pgtable.h>
#include <linux/fs.h>
#include <linux/gfp.h>
#include <linux/cdev.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
 
DECLARE_WAIT_QUEUE_HEAD(hq);
 
static int x=0;
 
static irqreturn_t *irq_handle(int irq,void * dev_id,struct pt_regs *regs)
{
    x=1;
    wake_up(&hq);
    printk(KERN_DEBUG "Interrupt\n");
    return 0;
}
 
static ssize_t
acme_read(struct file *file, char __user *buf,size_t count,loff_t *ppos)
{
    wait_event(hq,x);
    x=0;
    return 0;
}
 
static struct file_operations acme_fops =
{
    .owner = THIS_MODULE,
    .read = acme_read,
};
 
static struct cdev *acme_cdev;
 
static int
hello_init (void)
{
    request_irq(12, &irq_handle, IRQF_SHARED, "mydev", &hello_init);
    if(register_chrdev_region(MKDEV(230,0),1,"bubble"))
    {
         printk (KERN_INFO "alloc chrdev error.\n");
         return -1;
    }
 
    acme_cdev=cdev_alloc();
    if(!acme_cdev)
    {
        printk (KERN_INFO "cdev alloc error.\n");
         return -1;
    }
    acme_cdev->ops = &acme_fops;
    acme_cdev->owner = THIS_MODULE;
 
    if(cdev_add(acme_cdev,MKDEV(230,0),1))
    {
        printk (KERN_INFO "cdev add error.\n");
         return -1;
    }
 
  return 0;
 
}
 
static void
hello_cleanup (void)
{
 
  printk (KERN_INFO "hello unloaded succefully.\n");
  free_irq(12, &hello_init);
 
}
 
module_init (hello_init);
module_exit (hello_cleanup);
MODULE_LICENSE("GPL");
