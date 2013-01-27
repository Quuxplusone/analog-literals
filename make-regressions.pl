
use strict;
use warnings;

use IO::CaptureOutput qw/capture_exec/;

my $trust = 0;

sub remove_nth_line($$) {
    my $filename = shift;
    my $lineno = shift;
    die unless ($filename eq 'analog-punct.hpp');
    system('cp analog-punct.hpp.backup analog-punct.hpp');
    system("perl -ni -e 'print unless \$. == $lineno' analog-punct.hpp");
}

my %blank_lines;
system('git checkout -- analog-punct.hpp');
open (FILE, '<', 'analog-punct.hpp') or die "open failed";
my $i = 0;
while (<FILE>) {
  ++$i;
  if (/^$/) { $blank_lines{$i} = 1; }
  elsif (/^ *\/\//) { $blank_lines{$i} = 1; }
}
close(FILE);
print "Found @{[scalar(keys %blank_lines)]} blank lines in analog-punct.hpp.\n";
system('cp analog-punct.hpp analog-punct.hpp.backup');

my %trusted;
if ($trust and open (FILE, '<', 'regressions.h')) {
    while (<FILE>) {
        if (/^    ([a-z]*<.*>) r([0-9]*) = (.*);/) {
            my $type = $1;
            my $lineno = int($2);
            my $expression = $3;
            $trusted{$lineno} = "    $type r$lineno = $expression;\n";
        }
    }
    close(FILE);
}

open(REGRESSIONS, '>', 'auto-regressions.h') or die "open auto-regressions.h failed";
print REGRESSIONS "\n";

for my $i (72 .. 1093, 1100 .. 1107) {
    print "Line $i: ";
    if ($blank_lines{$i}) {
        print "blank\n";
        next;
    } elsif ($trusted{$i}) {
        print "trusted\n";
        print REGRESSIONS $trusted{$i};
        next;
    } else {
        print "considering removing it... ";
    }
    remove_nth_line('analog-punct.hpp', $i);
    my ($stdout, $stderr, $success, $exit_code) = capture_exec('clang++', '-c', '-ferror-limit=1', 'test.cc');
    if ($exit_code == 0) {
        print REGRESSIONS "    /* TODO: find a regression test for line $i */\n";
        print "nothing happened.\n";
    } else {
        my @stderr_lines = split(/^/m, $stderr);
        if ($#stderr_lines < 2) {
            print "not enough lines in stderr from clang++\n";
            die;
        }
        my $line = shift @stderr_lines;
        if ($line =~ /^In file included from/) {
            $line = shift @stderr_lines;
        }
        unless ($line =~ /:[0-9]*:[0-9]*: error:/) {
            print "first line had a bad format:\n$line";
            die;
        }
        $line = shift @stderr_lines;
        if ($line =~ /^    ([a-z]*<.*>) r[0-9]* = (.*);$/) {
            my $type = $1;
            my $initializer = $2;
            print REGRESSIONS "    $type r$i = $initializer;\n";
            print "added a regression test.\n";
        } else {
            print "line had a bad format:\n";
            print "$line";
            die;
        }
    }
}

close(REGRESSIONS);
print "Done. At this point you probably want to\n";
print "cp auto-regressions.h regressions.h\n";
exit 0;
