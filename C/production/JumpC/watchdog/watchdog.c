/**
 * @file compute_pi.c
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
#include "../ComJumpC/util.h"
#include <malloc.h>
#include <time.h>
#include <signal.h>

struct itimerspec itimerspec;

struct Watchdog_t
{
	/* TODO: POSIX timer *//**< POSIX Timer*/
	uint32_t myDelay; /**< configured delay */
	WatchdogCallback myCallback; /**< function to be called at delay expiration */
	timer_t myTimer;
};

/**
 * Calls the watchdog callback when the delay of the timer expires
 *
 * @param handlerParam must be the watchdog reference
 */
static void mainHandler (union sigval handlerParam)
{
	Watchdog *theWatchdog = handlerParam.sival_ptr;

	theWatchdog->myCallback (theWatchdog);
}

Watchdog *Watchdog_construct (uint32_t thisDelay, WatchdogCallback callback)
{
	Watchdog *watchdog;
	int error_code;

	// allocates and initializes the watchdog's attributes
	watchdog = (Watchdog *)malloc (sizeof(Watchdog));
	STOP_ON_ERROR (watchdog == NULL);
	watchdog->myDelay = thisDelay;
	watchdog->myCallback = callback;

	// sev : structure that specifies how the caller should be notified when the timer expires
	struct sigevent sev;
	sev.sigev_notify = SIGEV_THREAD;
    sev.sigev_value.sival_ptr = watchdog; // function argument
	sev.sigev_notify_function = &mainHandler;
	sev.sigev_notify_attributes = NULL; // if sigev_notify_attributes is not NULL, it should point to a pthread_attr_t structure that defines attributes for the new thread 

	// TODO: creates the POSIX timer
	error_code = timer_create(CLOCK_REALTIME, &sev, &(watchdog->myTimer));
	STOP_ON_ERROR(error_code != 0);

	return watchdog;
}

void Watchdog_start (Watchdog *this)
{
	int error_code;
	// TODO: starts the POSIX timer
	itimerspec.it_value.tv_sec = this->myDelay/1000000; // if new_value->it_value specifies a nonzero value, then timer_settime() arms (starts) the timer

	error_code = timer_settime(this->myTimer, 0, &itimerspec, NULL);
	STOP_ON_ERROR(error_code != 0);
}

void Watchdog_cancel (Watchdog *this)
{
	int error_code;
	// TODO: disarms the POSIX timer
	itimerspec.it_value.tv_sec = 0; // if new_value->it_value specifies a zero value, then the timer is disarmed

	error_code = timer_settime(this->myTimer, 0, &itimerspec, NULL);
	STOP_ON_ERROR(error_code != 0);
}

void Watchdog_destroy (Watchdog *this)
{
	int error_code;

	// TODO: disarms and deletes the POSIX timer
	itimerspec.it_value.tv_sec = 0; // if new_value->it_value specifies a zero value, then the timer is disarmed

	error_code = timer_settime(this->myTimer, 0, &itimerspec, NULL);
	STOP_ON_ERROR(error_code != 0);
	error_code = timer_delete(this->myTimer);
	STOP_ON_ERROR(error_code != 0);

	// then we can free memory
	free (this);
}
 