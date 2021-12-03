/*
 * kernel interface has been changed, so I can't init tasklet_data successfule.
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/interrupt.h>

char tasklet_data[] = "We use a string; but it could be pointer to a structure";
static struct tasklet_struct tasklet_struct_t={
    .data = (unsigned long)tasklet_data,
};


/* Tasklet handler, that just print the datar */
void tasklet_function(struct tasklet_struct *t){
    pr_info("%s\n", (char *)(t->data));
    return;
}

// DECLARE_TASKLET(my_tasklet, tasklet_function, (unsigned long)tasklet_data);
DECLARE_TASKLET(my_tasklet, tasklet_function);

static int __init my_init(void){
    /* Schedule the handler */
    tasklet_schedule(&my_tasklet);
    pr_info("tasklet example\n");
    return 0;
}

void my_exit(void){
    /* stop the tasklet before we exit*/
    tasklet_kill(&my_tasklet);
    pr_info("tasklet example cleanup\n");
    return;
}

module_init(my_init);
module_exit(my_exit);
MODULE_LICENSE("GPL");