use 5.008008;
use ExtUtils::MakeMaker;
# See lib/ExtUtils/MakeMaker.pm for details of how to influence
# the contents of the Makefile that is written.
WriteMakefile(
    NAME              => 'liberty',
    VERSION      => '2.5.1',
    PREREQ_PM         => {}, # e.g., Module::Name => 1.1
	C                => [liberty_wrap.c],
    ($] >= 5.005 ?     ## Add these new keywords supported since 5.005
      (ABSTRACT  => 'A Wrapper for si2dr_liberty', # retrieve abstract from module
       AUTHOR         => 'Steve Murphy <murf@e-tools.com>') : ()),
);
