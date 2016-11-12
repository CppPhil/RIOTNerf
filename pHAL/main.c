#include <stdio.h>
#include <stdlib.h>

#include "shell.h"
#include "periph/gpio.h"
#include "xtimer.h"

#include "pHAL.h"

#define RN_UNUSED(arg)  ((void) (arg))

static void cb(void *arg)
{
    printf("INT: external interrupt from pin %i\n", (int) arg);
}

static int init_pin(int argc, char **argv, gpio_mode_t mode)
{
    int po, pi;

    if (argc < 3) {
	printf("usage: %s <port> <pin>\n", argv[0]);
	return 1;
    }

    po = atoi(argv[1]);
    pi = atoi(argv[2]);

    if (gpio_init(GPIO_PIN(po, pi), mode) < 0) {
	printf("Error to initialize GPIO_PIN(%i, %02i)\n", po, pi);
	return 1;
    }

    return 0;
}

static int init_out(int argc, char **argv)
{
    return init_pin(argc, argv, GPIO_OUT);
}

static int init_in(int argc, char **argv)
{
    return init_pin(argc, argv, GPIO_IN);
}

static int init_in_pu(int argc, char **argv)
{
    return init_pin(argc, argv, GPIO_IN_PU);
}

static int init_in_pd(int argc, char **argv)
{
    return init_pin(argc, argv, GPIO_IN_PD);
}

static int init_od(int argc, char **argv)
{
    return init_pin(argc, argv, GPIO_OD);
}

static int init_od_pu(int argc, char **argv)
{
    return init_pin(argc, argv, GPIO_OD_PU);
}

static int init_int(int argc, char **argv)
{
    int po, pi;
    gpio_mode_t mode = GPIO_IN;
    gpio_flank_t flank;
    int fl;

    if (argc < 4) {
	printf("usage: %s <port> <pin> <flank> [pull_config]\n", argv[0]);
	puts("\tflank:\n"
	     "\t0: falling\n"
	     "\t1: rising\n"
	     "\t2: both\n"
	     "\tpull_config:\n"
	     "\t0: no pull resistor (default)\n"
	     "\t1: pull up\n" "\t2: pull down");
	return 1;
    }

    po = atoi(argv[1]);
    pi = atoi(argv[2]);

    fl = atoi(argv[3]);
    switch (fl) {
    case 0:
	flank = GPIO_FALLING;
	break;
    case 1:
	flank = GPIO_RISING;
	break;
    case 2:
	flank = GPIO_BOTH;
	break;
    default:
	puts("error: invalid value for active flank");
	return 1;
    }

    if (argc >= 5) {
	int pr = atoi(argv[4]);
	switch (pr) {
	case 0:
	    mode = GPIO_IN;
	    break;
	case 1:
	    mode = GPIO_IN_PU;
	    break;
	case 2:
	    mode = GPIO_IN_PD;
	    break;
	default:
	    puts("error: invalid pull resistor option");
	    return 1;
	}
    }

    if (gpio_init_int(GPIO_PIN(po, pi), mode, flank, cb, (void *) pi) < 0) {
	printf("error: init_int of GPIO_PIN(%i, %i) failed\n", po, pi);
	return 1;
    }
    printf("GPIO_PIN(%i, %i) successfully initialized as ext int\n", po,
	   pi);

    return 0;
}

static int read(int argc, char **argv)
{
    int port, pin;

    if (argc < 3) {
	printf("usage: %s <port> <pin>\n", argv[0]);
	return 1;
    }

    port = atoi(argv[1]);
    pin = atoi(argv[2]);

    if (gpio_read(GPIO_PIN(port, pin))) {
	printf("GPIO_PIN(%i.%02i) is HIGH\n", port, pin);
    } else {
	printf("GPIO_PIN(%i.%02i) is LOW\n", port, pin);
    }

    return 0;
}

static int set(int argc, char **argv)
{
    if (argc < 3) {
	printf("usage: %s <port> <pin>\n", argv[0]);
	return 1;
    }

    gpio_set(GPIO_PIN(atoi(argv[1]), atoi(argv[2])));

    return 0;
}

static int clear(int argc, char **argv)
{
    if (argc < 3) {
	printf("usage: %s <port> <pin>\n", argv[0]);
	return 1;
    }

    gpio_clear(GPIO_PIN(atoi(argv[1]), atoi(argv[2])));

    return 0;
}

static int toggle(int argc, char **argv)
{
    if (argc < 3) {
	printf("usage: %s <port> <pin>\n", argv[0]);
	return 1;
    }

    gpio_toggle(GPIO_PIN(atoi(argv[1]), atoi(argv[2])));

    return 0;
}

static int init_hal(int argc, char **argv)
{
    RN_UNUSED(argc);
    RN_UNUSED(argv);
    pHAL_init();
    return 0;
}

static int movehto(int argc, char **argv)
{
    if (argc < 2) {
	printf("usage: %s <angle (integer) -90...90 >\n", argv[0]);
	return 1;
    }

    int temp = atoi(argv[1]);
    printf("in move_x, temp: %d\n", temp);
    set_h(temp);

    return 0;
}

static int movevto(int argc, char **argv)
{
    if (argc < 2) {
	printf("usage: %s <angle (integer) -90...90 >\n", argv[0]);
	return 1;
    }

    int temp = atoi(argv[1]);
    printf("in move_y, temp: %d\n", temp);
    set_v(temp);
    return 0;
}

static int left(int argc, char **argv)
{
    RN_UNUSED(argc);
    RN_UNUSED(argv);
    step_l();
    return 0;
}

static int right(int argc, char **argv)
{
    RN_UNUSED(argc);
    RN_UNUSED(argv);
    step_r();
    return 0;
}

static int up(int argc, char **argv)
{
    RN_UNUSED(argc);
    RN_UNUSED(argv);
    step_u();
    return 0;
}

static int down(int argc, char **argv)
{
    RN_UNUSED(argc);
    RN_UNUSED(argv);
    step_d();
    return 0;

}

static int cntrh(int argc, char **argv)
{
    RN_UNUSED(argc);
    RN_UNUSED(argv);
    allign_h_cntr();
    return 0;
}

static int cntrv(int argc, char **argv)
{
    RN_UNUSED(argc);
    RN_UNUSED(argv);
    allign_v_cntr();
    return 0;
}

static int cntr(int argc, char **argv)
{
    RN_UNUSED(argc);
    RN_UNUSED(argv);
    allign_cntr();
    return 0;
}

static int demo(int argc, char **argv)
{
    RN_UNUSED(argc);
    RN_UNUSED(argv);
    allign_cntr();
    xtimer_sleep(1);
    set_h(-90);
    xtimer_sleep(1);
    set_h(90);
    xtimer_sleep(1);
    allign_h_cntr();
    xtimer_sleep(1);
    set_v(-90);
    xtimer_sleep(1);
    set_v(90);
    xtimer_sleep(1);
    allign_v_cntr();
    xtimer_sleep(1);
    return 0;
}

static const shell_command_t shell_commands[] = {
    {"init_out", "init as output (push-pull mode)", init_out},
    {"init_in", "init as input w/o pull resistor", init_in},
    {"init_in_pu", "init as input with pull-up", init_in_pu},
    {"init_in_pd", "init as input with pull-down", init_in_pd},
    {"init_od", "init as output (open-drain without pull resistor)",
     init_od},
    {"init_od_pu", "init as output (open-drain with pull-up)", init_od_pu},
    {"init_int", "init as external INT w/o pull resistor", init_int},
    {"read", "read pin status", read},
    {"set", "set pin to HIGH", set},
    {"clear", "set pin to LOW", clear},
    {"toggle", "toggle pin", toggle},
    {"init_hal", "init the pHAL", init_hal},
    {"seth", "move h to n degree", movehto},
    {"setv", "move v to n degree", movevto},
    {"left", "step left", left},
    {"right", "step right", right},
    {"up", "step up", up},
    {"down", "step down", down},
    {"centerh", "allign h at cntr", cntrh},
    {"centerv", "allign v to cntr", cntrv},
    {"center", "allign h and v to cntr", cntr},
    {"demo", "demo", demo},
    {NULL, NULL, NULL}
};

int main(void)
{
    puts("GPIO peripheral driver test\n");
    puts("In this test, pins are specified by integer port and pin numbers.\n" "So if your platform has a pin PA01, it will be port=0 and pin=1,\n" "PC14 would be port=2 and pin=14 etc.\n\n" "NOTE: make sure the values you use exist on your platform! The\n" "      behavior for not existing ports/pins is not defined!");

    /* start the shell */
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}