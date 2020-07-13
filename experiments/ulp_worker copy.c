#ifdef _ULPCC_
#include <ulp_c.h>
#define debounce_limit 5
unsigned _switch = 0;
unsigned _touch = 0;
unsigned _encoder_state = 0;
unsigned _prev_encoder_state = 255;

#define debounce(bit_in, true_state, debouncer) \
    if ((bit_in & 0x1) == (true_state & 0x1))   \
    {                                           \
        debouncer += 1;                         \
        if (debouncer > debounce_limit)         \
        {                                       \
            debouncer = debounce_limit;         \
        }                                       \
    }                                           \
    else                                        \
    {                                           \
        if (debouncer > 0)                      \
        {                                       \
            debouncer--;                        \
        }                                       \
    }

void entry()
{
    unsigned clk = (READ_RTC_REG(RTC_GPIO_IN_REG, RTC_GPIO_IN_NEXT_S + 16, 1) & 0x01);
    unsigned dt = (READ_RTC_REG(RTC_GPIO_IN_REG, RTC_GPIO_IN_NEXT_S + 14, 1) & 0x01);
    debounce(READ_RTC_REG(RTC_GPIO_IN_REG, RTC_GPIO_IN_NEXT_S + 17, 1), 0x00, _switch);
    debounce(READ_RTC_REG(RTC_GPIO_IN_REG, RTC_GPIO_IN_NEXT_S + 7, 1), 0x00, _touch);
    _encoder_state = dt << 1 | clk;
    if ((_prev_encoder_state & 0xff) == 0xff)
    {
        _prev_encoder_state = _encoder_state;
    }
    else if (_prev_encoder_state != _encoder_state)
    {
        wake();
        halt();
    }

    if (_switch == debounce_limit || _touch == debounce_limit)
    {
        wake();
        halt();
    }
    halt();
}

#endif
