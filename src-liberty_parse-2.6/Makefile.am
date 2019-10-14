## Process this file with automake to produce Makefile.in

bin_PROGRAMS = liberty_parse
noinst_PROGRAMS = syntax_parse
liberty_parse_SOURCES = main.c PI.c libhash.c libhash.h libstrtab.c libstrtab.h liberty_parser.y token.l mymalloc.c mymalloc.h syntax_decls.c group_lookup.c attr_lookup.c syntax_checks.c liberty_front_lex.c
liberty_parse_CPPFLAGS = ## -DDMALLOC
syntax_parse_SOURCES = syntform.c synttok.l syntax_parser.y
liberty_parse_LDADD = -lm ## -ldmalloc
syntax_parse_LDADD = ## -ldmalloc

liberty_parse_DEPENDENCIES = syntax_decls.$(OBJEXT) liberty_structs.h group_enum.h attr_enum.h

lib_LTLIBRARIES = libsi2dr_liberty.la
libsi2dr_liberty_la_SOURCES = PI.c si2dr_liberty.h syntax_checks.c liberty_parser.y libhash.c libstrtab.c mymalloc.c token.l syntax_decls.c group_lookup.c attr_lookup.c liberty_front_lex.c

##noinst_LIBRARIES = libsi2dr_liberty.a
##libsi2dr_liberty_a_SOURCES = PI.c si2dr_liberty.h syntax_checks.c liberty_parser.y libhash.c libstrtab.c mymalloc.c token.l syntax_decls.c group_lookup.c attr_lookup.c liberty_front_lex.c

AM_YFLAGS = -p $*_ -vd

AM_CFLAGS = -DYYERROR_VERBOSE -g 
syntax_parse : syntform.$(OBJEXT) syntax_parser.$(OBJEXT) synttok.$(OBJEXT) mymalloc.$(OBJEXT) libhash.$(OBJEXT)
	$(CC) -o syntax_parse syntform.$(OBJEXT) syntax_parser.$(OBJEXT) synttok.$(OBJEXT) mymalloc.$(OBJEXT) libhash.$(OBJEXT)

## syntform.o : syntform.c


##syntax_decls.$(OBJEXT) : syntax_decls.c
##	$(CC) -c -O2 syntax_decls.c

group_lookup.c : group_lookup
	gperf -L ANSI-C -t -H group_hash_func -N lookup_group_name -C -I -k \* -D group_lookup > group_lookup.c

attr_lookup.c : attr_lookup
	gperf -L ANSI-C -t -H attr_hash_func -N lookup_attr_name -C -I -k \* -D attr_lookup > attr_lookup.c

syntax_decls.c syntax_decls.h group_lookup attr_lookup group_enum.h attr_enum.h : syntax_parse desc/syntax.cmos.desc desc/syntax.fpga.desc
	./syntax_parse

PI.o : group_enum.h attr_enum.h
PI.lo : PI.c liberty_structs.h group_enum.h attr_enum.h

syntax_checks.$(OBJEXT) : group_enum.h attr_enum.h

check :
	(cd test; ./runtests)

perl/liberty.pm perl/liberty_wrap.c : perl/liberty.i
	(cd perl; swig -perl liberty.i)

P2=`perl -v | grep perl\, | sed 's@This is perl, v\([^ ]\+\) built for \(.\+\)@/usr/lib/perl5/\1/\2/CORE@'`
perl/liberty.so : perl/liberty_wrap.c .libs/PI.o .libs/syntax_checks.o .libs/liberty_parser.o .libs/libhash.o .libs/libstrtab.o .libs/mymalloc.o .libs/token.o .libs/syntax_decls.o .libs/group_lookup.o .libs/attr_lookup.o .libs/liberty_front_lex.o
	(cd perl; gcc -c -fpic liberty_wrap.c -I${P2} -I.. -D_GNU_SOURCE)
	(cd perl; gcc -shared liberty_wrap.o ../.libs/PI.o ../.libs/syntax_checks.o ../.libs/liberty_parser.o ../.libs/libhash.o ../.libs/libstrtab.o ../.libs/mymalloc.o ../.libs/token.o ../.libs/syntax_decls.o ../.libs/group_lookup.o ../.libs/attr_lookup.o ../.libs/liberty_front_lex.o -o liberty.so)
