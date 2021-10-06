/* acq200_debug.h */

#ifndef __ACQ200_DEBUG_H__
#define __ACQ200_DEBUG_H__

#define T(fmt, args...) printk(KERN_DEBUG "acq200_core:" fmt, ## args)
#define E(fmt, args...) printk(KERN_NOTICE "acq200_core:" fmt, ## args)

extern int acq200_debug;

#ifndef FN
#define FN __FUNCTION__
#endif

#define KE KERN_ERR
#define KI KERN_INFO
#define KW KERN_WARNING
//#define KD KERN_DEBUG
#define KD KI

#define dbg(lvl,format, arg...)	\
	do {						\
		if(acq200_debug>=lvl)			\
			printk (KD "%s: " format "\n",	\
				FN , ## arg); 		\
	} while(0)

#define err(format, arg...) printk(KE "%s: " format "\n", FN , ## arg)
#define info(format, arg...) printk(KI "%s: " format "\n", FN , ## arg)
#define warn(format, arg...) printk(KW "%s: " format "\n", FN , ## arg)

extern int acq32_leak_detect;

#define LEAK(format, arg...) if ( acq32_leak_detect ) info( format, ## arg)

#ifdef CONFIG_KERNEL_ASSERTS
/* kgdb stuff */
#define assert(p) KERNEL_ASSERT(#p, p)
#else
#define assert(p) do {	\
	if (!(p)) {	\
		printk(KERN_CRIT "BUG at %s:%d assert(%s)\n",	\
		       __FILE__, __LINE__, #p);			\
		BUG();	\
	}		\
} while (0)
#endif


#define MARK printk(KERN_INFO "%s:%d %s MARK\n", \
             __FILE__, __LINE__, __FUNCTION__ )
#endif /* __ACQ200_DEBUG_H__ */
