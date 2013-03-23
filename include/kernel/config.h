/*
 * config.h
 * Copyright (C) 2012, 2013 Lijie
 * <cuplision@163.com>
 * Sun 20 Jan 2013 17:26:35 
 */
#   ifndef _CONFIG_H_
#   define _CONFIG_H_

/* early root-image address*/
#   define FDOS_INIT_START

/* kernel code start at address 0xC0000000 */
#   define FDOS_CODE_START  0xC0000000
/* kerel alloc operate start at address 0xC1000000 */
#   define FDOS_ALLOC_START 0xC1000000

/* system config path. */
#   define FDOS_CONFIG_PATH "/etc/fdos"
/* supported device table. */
#   define FDOS_SUPPORT_DEV FDOS_CONFIG_PATH"/devs_id"
/* modules path. */
#   define FDOS_MODULE_PATH "/usr/lib/ko"
/* pre-load modules table. */
#   define FDOS_PRELOAD  FDOS_MODULE_PATH"/preload_ko"

#   ifdef _ARCH_x86_
#       include <system.h>
#   else
#       error "Must config platform first!"
#   endif

#   endif /*_CONFIG_H_*/
