
/*
 * app_etc.h
 *
 *  Created on: Mar 25, 2022
 *      Author: seu
 */

#ifndef __APP_ETC_H_
#define __APP_ETC_H_

#include "gstr.h"
#include "glog.h"
#include "gtty.h"
#include "gutils.h"

void command_list(const char*str);
//void scadule_traffic(const char*);
void system_reset(const char *s);

int add_proc(const char*pn, void (*f)(const char*), int timer=-1, int act=1);

#endif /* __APP_ETC_H_ */
