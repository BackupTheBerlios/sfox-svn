#!/usr/bin/perl -w
use strict;

my @exts;

#exts.list contains all needed extensions

sub doDeclaration {
    my $s = shift;
    if(!m%^//|^$%) {
	chomp($s);
	my $tmp=$s;
	$s =~ tr/a-z/A-Z/;
	return "extern PFN".$s."PROC $tmp;\n";
    }
    return $_;
}

sub doCode {
    my $s = shift;
    if(!m%^//|^$%) {
	chomp($s);
	my $tmp=$s;
	$s =~ tr/a-z/A-Z/;
	return "\t$tmp = (PFN".$s."PROC)SDL_GL_GetProcAddress(\"".$tmp."\");\n";
    }
    return "\t".$_;
}

open(EXTS, "exts.list");
@exts = <EXTS>;
close(EXTS);

my @externs = map { doDeclaration($_) } @exts;
my @code = map { doCode($_) } @exts;

open(OUTH, ">glext.h");
open(TMPLH, "template.h");
while(defined(my $l = <TMPLH>)) {
    if($l =~ m%////HERE$%) {
	print OUTH @externs;
    } else {
	print OUTH $l;
    }
}
close(TMPLH);
close(OUTH);

#Remove extern for cpp file
map { s/^extern // } @externs;

open(OUTC, ">glext.cpp");
open(TMPLC, "template.cpp");
while(defined(my $l = <TMPLC>)) {
    if($l =~ m%////HERE$%) {
	print OUTC @externs;
    } elsif($l =~ m%////CODE$%) {
	print OUTC @code;
    } else {
	print OUTC $l;
    }
}
close(TMPLC);
close(OUTC);
