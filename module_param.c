
#include<linux/kernel.h>  
#include<linux/init.h>
#include<linux/module.h>
#include<linux/moduleparam.h>
 
int myValue, arr_myValue[4];
char *myName;
int cb_myValue = 0;



module_param(myValue, int, S_IRUSR|S_IWUSR);                      //integer value
module_param(myName, charp, S_IRUSR|S_IWUSR);                     //String
module_param_array(arr_myValue, int, NULL, S_IRUSR|S_IWUSR);      //Array of integers
 
/*----------------------Module_param_cb()--------------------------------*/
int notify_param(const char *val, const struct kernel_param *kp)
{
        int res = param_set_int(val, kp); // Use helper for write variable
        if(res==0) {
                printk(KERN_INFO "Call back function called...\n");
                printk(KERN_INFO "New value of cb_myValue = %d\n", cb_myValue);
                return 0;
        }
        return -1;
}
 
const struct kernel_param_ops my_param_ops = 
{
        .set = &notify_param, // Use our setter ...
        .get = &param_get_int, // .. and standard getter
};
 
module_param_cb(cb_myValue, &my_param_ops, &cb_myValue, S_IRUGO|S_IWUSR );
/*-------------------------------------------------------------------------*/

/*
** Module init function
*/
static int __init hello_world_init(void)
{
        int i;
        printk(KERN_INFO "myValue = %d  \n", myValue);
        printk(KERN_INFO "cb_myValue = %d  \n", cb_myValue);
        printk(KERN_INFO "myName = %s \n", myName);
        for (i = 0; i < (sizeof arr_myValue / sizeof (int)); i++) {
                printk(KERN_INFO "Arr_value[%d] = %d\n", i, arr_myValue[i]);
        }

        printk(KERN_INFO "Kernel Module Inserted Successfully...\n");
    return 0;
}

/*
** Module Exit function
*/
static void __exit hello_world_exit(void)
{
    printk(KERN_INFO "Kernel Module Removed Successfully...\n");
}
 
module_init(hello_world_init);
module_exit(hello_world_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("johny");
MODULE_DESCRIPTION("A simple module param driver");
MODULE_VERSION("1.0");
