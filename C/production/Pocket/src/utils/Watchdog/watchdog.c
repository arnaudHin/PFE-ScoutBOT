/**
 * @file
 *
 * @author team FORMATO, ESEO
 *
 * @section License
 *
 * The MIT License
 *
 * Copyright (c) 2016, Jonathan ILIAS-PILLET (ESEO)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include "watchdog.h"
#include <malloc.h>
#include <time.h>
#include <signal.h>

struct Watchdog_t
{
    timer_t timerId;             /**< POSIX Timer*/
    uint32_t myDelay;            /**< configured delay */
    WatchdogCallback myCallback; /**< function to be called at delay expiration */
};

/**
 * Calls the watchdog callback when the delay of the timer expires
 *
 * @param handlerParam must be the watchdog reference
 */
static void mainHandler(union sigval info)
{
    Watchdog *theWatchdog = info.sival_ptr;

    theWatchdog->myCallback(theWatchdog->timerId); // Obligation de passer un argument à l'appel
                                                   // de la fonction => autant passer le timer id pour une évolution future de abortComputePi
}

Watchdog *Watchdog_construct(uint32_t thisDelay, WatchdogCallback callback)
{
    Watchdog *watchdog;
    struct sigevent event;

    // allocates and initializes the watchdog's attributes
    watchdog = (Watchdog *)malloc(sizeof(Watchdog));

    watchdog->myDelay = thisDelay;
    watchdog->myCallback = callback;

    /* Set up alarm */
    event.sigev_notify = SIGEV_THREAD;      /* Notification method */
    event.sigev_value.sival_ptr = watchdog; /* Data passed with notification */
    event.sigev_notify_function = &mainHandler;
    event.sigev_notify_attributes = 0;

    timer_create(CLOCK_REALTIME, &event, &(watchdog->timerId));

    return watchdog;
}

void Watchdog_start(Watchdog *this)
{
    struct itimerspec spec;

    /* Period of the timer */
    spec.it_interval.tv_sec = 0; // The callback function is executed once every <n> seconde
    spec.it_interval.tv_nsec = 0;

    /* the timer is reloaded from the value specified in it_interval */
    spec.it_value.tv_sec = this->myDelay / 1000000;
    spec.it_value.tv_nsec = (this->myDelay % 1000000) * 1000;

    timer_settime(this->timerId, 0, &spec, NULL);
}

void Watchdog_cancel(Watchdog *this)
{
    if (this != NULL)
    {
        struct itimerspec spec;

    /**
     * If the it_value member of value is zero, the timer shall be disarmed.
     * extrait de la man page => mettre le timer à zero revient donc à le désarmer
    */

        /* Period of the timer */
        spec.it_interval.tv_sec = 0;
        spec.it_interval.tv_nsec = 0;

        /* the timer is reloaded from the value specified in it_interval */
        spec.it_value.tv_sec = 0;
        spec.it_value.tv_nsec = 0;

        timer_settime(this->timerId, 0, &spec, NULL);
    }
}

void Watchdog_destroy(Watchdog *this)
{
    Watchdog_cancel(this); // Fermeture complete de la structure si celle si évolue dans le futur

    timer_delete(this->timerId);

    // then we can free memory
    free(this);
}
