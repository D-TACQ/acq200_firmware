#Makefile for GCC
#
#Tom St Denis

# Dropbear takes flags from the toplevel makefile
CFLAGS += -I./

RANLIB=ranlib
#CFLAGS  +=  -I./ -Wall -W -Wshadow -Wsign-compare

#for speed 
#CFLAGS += -O3 -funroll-loops

#for size 
#CFLAGS += -Os

#x86 optimizations [should be valid for any GCC install though]
#CFLAGS  += -fomit-frame-pointer

VERSION=0.30

default: libtommath.a

#default files to install
LIBNAME=libtommath.a
HEADERS=tommath.h

#LIBPATH-The directory for libtomcrypt to be installed to.
#INCPATH-The directory to install the header files for libtommath.
#DATAPATH-The directory to install the pdf docs.
DESTDIR=
LIBPATH=/usr/lib
INCPATH=/usr/include
DATAPATH=/usr/share/doc/libtommath/pdf

OBJECTS=bncore.o bn_mp_init.o bn_mp_clear.o bn_mp_exch.o bn_mp_grow.o bn_mp_shrink.o \
bn_mp_clamp.o bn_mp_zero.o  bn_mp_set.o bn_mp_set_int.o bn_mp_init_size.o bn_mp_copy.o \
bn_mp_init_copy.o bn_mp_abs.o bn_mp_neg.o bn_mp_cmp_mag.o bn_mp_cmp.o bn_mp_cmp_d.o \
bn_mp_rshd.o bn_mp_lshd.o bn_mp_mod_2d.o bn_mp_div_2d.o bn_mp_mul_2d.o bn_mp_div_2.o \
bn_mp_mul_2.o bn_s_mp_add.o bn_s_mp_sub.o bn_fast_s_mp_mul_digs.o bn_s_mp_mul_digs.o \
bn_fast_s_mp_mul_high_digs.o bn_s_mp_mul_high_digs.o bn_fast_s_mp_sqr.o bn_s_mp_sqr.o \
bn_mp_add.o bn_mp_sub.o bn_mp_karatsuba_mul.o bn_mp_mul.o bn_mp_karatsuba_sqr.o \
bn_mp_sqr.o bn_mp_div.o bn_mp_mod.o bn_mp_add_d.o bn_mp_sub_d.o bn_mp_mul_d.o \
bn_mp_div_d.o bn_mp_mod_d.o bn_mp_expt_d.o bn_mp_addmod.o bn_mp_submod.o \
bn_mp_mulmod.o bn_mp_sqrmod.o bn_mp_gcd.o bn_mp_lcm.o bn_fast_mp_invmod.o bn_mp_invmod.o \
bn_mp_reduce.o bn_mp_montgomery_setup.o bn_fast_mp_montgomery_reduce.o bn_mp_montgomery_reduce.o \
bn_mp_exptmod_fast.o bn_mp_exptmod.o bn_mp_2expt.o bn_mp_n_root.o bn_mp_jacobi.o bn_reverse.o \
bn_mp_count_bits.o bn_mp_read_unsigned_bin.o bn_mp_read_signed_bin.o bn_mp_to_unsigned_bin.o \
bn_mp_to_signed_bin.o bn_mp_unsigned_bin_size.o bn_mp_signed_bin_size.o  \
bn_mp_xor.o bn_mp_and.o bn_mp_or.o bn_mp_rand.o bn_mp_montgomery_calc_normalization.o \
bn_mp_prime_is_divisible.o bn_prime_tab.o bn_mp_prime_fermat.o bn_mp_prime_miller_rabin.o \
bn_mp_prime_is_prime.o bn_mp_prime_next_prime.o bn_mp_dr_reduce.o \
bn_mp_dr_is_modulus.o bn_mp_dr_setup.o bn_mp_reduce_setup.o \
bn_mp_toom_mul.o bn_mp_toom_sqr.o bn_mp_div_3.o bn_s_mp_exptmod.o \
bn_mp_reduce_2k.o bn_mp_reduce_is_2k.o bn_mp_reduce_2k_setup.o \
bn_mp_radix_smap.o bn_mp_read_radix.o bn_mp_toradix.o bn_mp_radix_size.o \
bn_mp_fread.o bn_mp_fwrite.o bn_mp_cnt_lsb.o bn_error.o \
bn_mp_init_multi.o bn_mp_clear_multi.o bn_prime_sizes_tab.o bn_mp_exteuclid.o bn_mp_toradix_n.o \
bn_mp_prime_random_ex.o bn_mp_get_int.o bn_mp_sqrt.o bn_mp_is_square.o bn_mp_init_set.o \
bn_mp_init_set_int.o

libtommath.a:  $(OBJECTS)
	$(AR) $(ARFLAGS) libtommath.a $(OBJECTS)
	$(RANLIB) libtommath.a

install: libtommath.a
	install -d -g root -o root $(DESTDIR)$(LIBPATH)
	install -d -g root -o root $(DESTDIR)$(INCPATH)
	install -g root -o root $(LIBNAME) $(DESTDIR)$(LIBPATH)
	install -g root -o root $(HEADERS) $(DESTDIR)$(INCPATH)

test: libtommath.a demo/demo.o
	$(CC) demo/demo.o libtommath.a -o test
	
mtest: test	
	cd mtest ; $(CC) $(CFLAGS) mtest.c -o mtest -s
        
timing: libtommath.a
	$(CC) $(CFLAGS) -DTIMER demo/demo.c libtommath.a -o ltmtest -s

# makes the LTM book DVI file, requires tetex, perl and makeindex [part of tetex I think]
docdvi: tommath.src
	cd pics ; make 
	echo "hello" > tommath.ind
	perl booker.pl
	latex tommath > /dev/null
	latex tommath > /dev/null
	makeindex tommath
	latex tommath > /dev/null

# poster, makes the single page PDF poster
poster: poster.tex
	pdflatex poster
	rm -f poster.aux poster.log 

# makes the LTM book PDF file, requires tetex, cleans up the LaTeX temp files
docs:   docdvi
	dvipdf tommath
	rm -f tommath.log tommath.aux tommath.dvi tommath.idx tommath.toc tommath.lof tommath.ind tommath.ilg
	cd pics ; make clean
	
#LTM user manual
mandvi: bn.tex
	echo "hello" > bn.ind
	latex bn > /dev/null
	latex bn > /dev/null
	makeindex bn
	latex bn > /dev/null

#LTM user manual [pdf]
manual:	mandvi
	pdflatex bn >/dev/null
	rm -f bn.aux bn.dvi bn.log bn.idx bn.lof bn.out bn.toc
	
clean:
	rm -f *.bat *.pdf *.o *.a *.obj *.lib *.exe *.dll etclib/*.o demo/demo.o test ltmtest mpitest mtest/mtest mtest/mtest.exe \
        *.idx *.toc *.log *.aux *.dvi *.lof *.ind *.ilg *.ps *.log *.s mpi.c 
	cd etc ; make clean
	cd pics ; make clean

zipup: clean manual poster docs
	perl gen.pl ; mv mpi.c pre_gen/ ; \
	cd .. ; rm -rf ltm* libtommath-$(VERSION) ; mkdir libtommath-$(VERSION) ; \
	cp -R ./libtommath/* ./libtommath-$(VERSION)/ ; \
	tar -c libtommath-$(VERSION)/* | bzip2 -9vvc > ltm-$(VERSION).tar.bz2 ; \
	zip -9 -r ltm-$(VERSION).zip libtommath-$(VERSION)/*
