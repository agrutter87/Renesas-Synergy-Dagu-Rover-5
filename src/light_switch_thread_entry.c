#include "light_switch_thread.h"
#include "light_switch.h"

/* Light Switch Thread entry function */
void light_switch_thread_entry(void)
{
    g_light_switch_irq.p_api->open(g_light_switch_irq.p_ctrl, g_light_switch_irq.p_cfg);

    /* TODO: add your own code here */
    while (1)
    {
        tx_semaphore_get(&g_light_switch_semaphore, TX_WAIT_FOREVER);
        light_switch_state_update();
    }
}

void light_switch_callback(external_irq_callback_args_t * p_args)
{
    SSP_PARAMETER_NOT_USED(p_args);
    tx_semaphore_put(&g_light_switch_semaphore);
}
