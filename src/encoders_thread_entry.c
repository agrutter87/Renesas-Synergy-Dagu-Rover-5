#include "encoders_thread.h"
#include "encoders.h"

static bool g_encoders_interrupt = false;
encoder_memory_t g_counter_values;

/* Encoders Thread entry function */
void encoders_thread_entry(void)
{

    g_encoders_timer.p_api->open (g_encoders_timer.p_ctrl, g_encoders_timer.p_cfg);
    g_encoders_timer.p_api->start(g_encoders_timer.p_ctrl);

    /* TODO: add your own code here */
    while (1)
    {
        if (g_encoders_interrupt)
        {
            g_encoders_interrupt = false;
            encoders_update (&g_counter_values);
        }
        else
        {
            encoders_timer_mode_update();
        }

        /* Wait for notification */
        tx_semaphore_get (&g_encoders_semaphore, TX_WAIT_FOREVER);
    }
}

void encoders_timer_callback(timer_callback_args_t * p_args)
{
    SSP_PARAMETER_NOT_USED(p_args);

    g_counter_values.left_counter_value  = R_GPTB4->GTCNT;
    g_counter_values.right_counter_value = R_GPTB5->GTCNT;

    //TODO: Consider clearing the counter each time so they don't overflow
    R_GPTB4->GTCNT = 0;
    R_GPTB5->GTCNT = 0;

    g_encoders_interrupt = true;

    tx_semaphore_put (&g_encoders_semaphore);
}
