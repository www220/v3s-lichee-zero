#ifndef __IO_H__
#define __IO_H__


#define uswap_16(x) \
	((((x) & 0xff00) >> 8) | \
	 (((x) & 0x00ff) << 8))
#define uswap_32(x) \
	((((x) & 0xff000000) >> 24) | \
	 (((x) & 0x00ff0000) >>  8) | \
	 (((x) & 0x0000ff00) <<  8) | \
	 (((x) & 0x000000ff) << 24))


# define cpu_to_le16(x)		uswap_16(x)
# define cpu_to_le32(x)		uswap_32(x)
# define le16_to_cpu(x)		uswap_16(x)
# define le32_to_cpu(x)		uswap_32(x)
# define cpu_to_be16(x)		(x)
# define cpu_to_be32(x)		(x)
# define be16_to_cpu(x)		(x)
# define be32_to_cpu(x)		(x)



#define ISB	asm volatile ("" : : : "memory")
#define DSB	CP15DSB
#define DMB	asm volatile ("" : : : "memory")

#define isb()	ISB
#define dsb()	DSB
#define dmb()	DMB

#define mb()		dsb()
#define __iormb()	dmb()
#define __iowmb()	dmb()

#define out_arch(type,endian,a,v)	__raw_write##type(cpu_to_##endian(v),a)
#define in_arch(type,endian,a)		endian##_to_cpu(__raw_read##type(a))

#define out_le64(a,v)	out_arch(q,le64,a,v)
#define out_le32(a,v)	out_arch(l,le32,a,v)
#define out_le16(a,v)	out_arch(w,le16,a,v)

#define in_le64(a)	in_arch(q,le64,a)
#define in_le32(a)	in_arch(l,le32,a)
#define in_le16(a)	in_arch(w,le16,a)

#define __arch_getb(a)			(*(volatile unsigned char *)(a))
#define __arch_getw(a)			(*(volatile unsigned short *)(a))
#define __arch_getl(a)			(*(volatile unsigned int *)(a))
#define __arch_getq(a)			(*(volatile unsigned long long *)(a))

#define __arch_putb(v,a)		(*(volatile unsigned char *)(a) = (v))
#define __arch_putw(v,a)		(*(volatile unsigned short *)(a) = (v))
#define __arch_putl(v,a)		(*(volatile unsigned int *)(a) = (v))
#define __arch_putq(v,a)		(*(volatile unsigned long long *)(a) = (v))

#define __raw_writeb(v,a)	__arch_putb(v,a)
#define __raw_writew(v,a)	__arch_putw(v,a)
#define __raw_writel(v,a)	__arch_putl(v,a)
#define __raw_writeq(v,a)	__arch_putq(v,a)

#define __raw_readb(a)		__arch_getb(a)
#define __raw_readw(a)		__arch_getw(a)
#define __raw_readl(a)		__arch_getl(a)
#define __raw_readq(a)		__arch_getq(a)


#define writeb(v,c)	({ unsigned char  __v = v; __iowmb(); __arch_putb(__v,c); __v; })
#define writew(v,c)	({ unsigned short __v = v; __iowmb(); __arch_putw(__v,c); __v; })
#define writel(v,c)	({ unsigned int __v = v; __iowmb(); __arch_putl(__v,c); __v; })
#define writeq(v,c)	({ unsigned long long __v = v; __iowmb(); __arch_putq(__v,c); __v; })

#define readb(c)	({ unsigned char  __v = __arch_getb(c); __iormb(); __v; })
#define readw(c)	({ unsigned short __v = __arch_getw(c); __iormb(); __v; })
#define readl(c)	({ unsigned int __v = __arch_getl(c); __iormb(); __v; })
#define readq(c)	({ unsigned long long __v = __arch_getq(c); __iormb(); __v; })


#define clrbits(type, addr, clear) \
	out_##type((addr), in_##type(addr) & ~(clear))

#define setbits(type, addr, set) \
	out_##type((addr), in_##type(addr) | (set))

#define clrsetbits(type, addr, clear, set) \
	out_##type((addr), (in_##type(addr) & ~(clear)) | (set))

#define clrbits_be32(addr, clear) clrbits(be32, addr, clear)
#define setbits_be32(addr, set) setbits(be32, addr, set)
#define clrsetbits_be32(addr, clear, set) clrsetbits(be32, addr, clear, set)

#define clrbits_le32(addr, clear)  ((*(volatile unsigned int *)(addr)) &= ~((unsigned int)clear))
#define setbits_le32(addr, set)    ((*(volatile unsigned int *)(addr)) |= set)

#define clrsetbits_le32(addr, clear, set) ({clrbits_le32(addr, clear);setbits_le32(addr, set);})

#define clrbits_be16(addr, clear) clrbits(be16, addr, clear)
#define setbits_be16(addr, set) setbits(be16, addr, set)
#define clrsetbits_be16(addr, clear, set) clrsetbits(be16, addr, clear, set)

#define clrbits_le16(addr, clear) clrbits(le16, addr, clear)
#define setbits_le16(addr, set) setbits(le16, addr, set)
#define clrsetbits_le16(addr, clear, set) clrsetbits(le16, addr, clear, set)

#define clrbits_8(addr, clear) clrbits(8, addr, clear)
#define setbits_8(addr, set) setbits(8, addr, set)
#define clrsetbits_8(addr, clear, set) clrsetbits(8, addr, clear, set)





#endif