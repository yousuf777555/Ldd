#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/slab.h>

#define MAJORNO 300
#define MINORNO  0
#define CHAR_DEV_NAME "my_cdev"
#define MAX_LENGTH 4000
#define SUCCESS 0

static char char_device_buf[MAX_LENGTH];
struct cdev *my_cdev;
dev_t mydev;
int count=1,inuse=0;


static int char_dev_open(struct inode *inode,
			    struct file  *file)
{
	if(inuse)
	{
		printk(KERN_INFO "\nDevice busy %s\n",CHAR_DEV_NAME);
		return -EBUSY;
	}	
	inuse=1;
	printk(KERN_INFO "Open operation invoked \n");
	return SUCCESS;
		
}

static int char_dev_release(struct inode *inode, struct file *file)
{
	inuse=0;
	return SUCCESS;
}

static ssize_t char_dev_write(struct file *file, const char *buf, size_t lbuf, loff_t *ppos)
{
       int nbytes = lbuf - raw_copy_from_user (char_device_buf + *ppos, buf, lbuf);
       *ppos += nbytes;
       printk (KERN_INFO "\n Rec'vd data from app %s , nbytes=%d\n",char_device_buf,nbytes);

       return nbytes;

}

static struct file_operations char_dev_fops = {
	.owner = THIS_MODULE,
	.write = char_dev_write,
	.open = char_dev_open,
	.release = char_dev_release
};

static int __init char_dev_init(void)
{
	int ret;
	mydev = MKDEV(MAJORNO,MINORNO);                             // step 1 to create major and minor no.==mydev
	register_chrdev_region(mydev,count,CHAR_DEV_NAME);          // step 2:create one name for character driverv  and 
								    // register that name to mydev

	my_cdev= cdev_alloc();  // allow allocate cdev instance, 
	cdev_init(my_cdev,&char_dev_fops);                           // step 3 initilizing file operation  to cdev instance 

	ret=cdev_add(my_cdev,mydev,count);//step 4 step 2 + step 3 ;  register with VFS layer , count = how many minor no. in use .
	
	if( ret < 0 ) {
		printk("Error registering device driver\n");
		return ret;
	}
	printk(KERN_INFO"\nDevice Registered %s\n",CHAR_DEV_NAME); 
	
	memset(char_device_buf,'\0',MAX_LENGTH);
	return 0;
}

static void __exit char_dev_exit(void)
{
	
	 cdev_del(my_cdev);
	 unregister_chrdev_region(mydev,1);
	 printk(KERN_INFO "\n Driver unregistered \n");
}

module_init(char_dev_init);
module_exit(char_dev_exit);

MODULE_AUTHOR("Johny");
MODULE_DESCRIPTION("Static Character Device Driver");
MODULE_LICENSE("GPL");
/* End of code */
