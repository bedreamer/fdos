/*
 *	kernel.h
 *	bedreamer@163.com
 */
#   ifndef _KERNEL_INCLUDE_
#   define _KERNEL_INCLUDE_

#   define _user_	/* flag as user address space.  */
#   define _core_	/* flag as kernel address space.*/

#   define _user_in_
#   define _user_out_
#   define _user_inout_

#   define _core_in_
#   define _core_out_
#   define _core_inout_

#   define _in_
#   define _out_
#   define _inout_

#   define _u_i_ _user_in_
#   define _u_o_ _user_out_
#   define _c_i_ _core_in_
#   define _c_o_ _core_out_
#   define _c_io_ _core_inout_

#    define _ui_ _u_i_
#    define _ui _ui_
#    define _uo_ _u_o_
#    define _uo _uo_
#    define _ci_ _c_i_
#    define _ci _ci_
#    define _co_ _c_o_
#    define _co _co_
#    define _cio _c_io_

#    include <kernel/config.h>
extern void start_kernel(char *cmdline);
struct pcistr{
    char *p;
    char *pt;
    char *name;
    char *vendor;
    char *serial;
};

#   if (2==_VERSION_)
#       define _NAMEA_ "DEMON"
#   elif (1==_VERSION_)
#       define _NAMEA_ "DEBUG"
#   elif (0==_VERSION_)
#       define _NAMEA_ "RELEASE"
#   else
#   error "need vesion."
#   endif /*VERSION head*/

#   define VERSION "FDOS-"_NAMEA_", "__DATE__", "__TIME__", GNU/FDOS"

#   endif /*_KERNEL_INCLUDE_*/
