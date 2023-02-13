#!/usr/bin/env perl

use warnings;
use strict;

# Compute coefficients for a 4th order polynomial that will return the desired
# character when passed one of four characters, and 0 otherwise.
sub poly {
    my ($z0, $z1, $z2, $m3, $l) = @_;
    my ($c0, $c1, $c2, $c3, $c4);

    my $z3 = $m3 - 1;
    $m3 -= ord('A');
    $z0 -= ord('A');
    $z1 -= ord('A');
    $z2 -= ord('A');
    $z3 -= ord('A');
    $l -= ord('a');

    print "$z0 $z1 $z2 $z3 $l\n";

    $c0 = $z0 * $z1 * $z2 * $z3;
    $c1 = - ($z0 * $z1 * $z2 + $z0 * $z1 * $z3 + $z0 * $z2 * $z3
            + $z1 * $z2 * $z3);
    $c2 = $z0 * $z1 + $z0 * $z2 + $z0 * $z3 + $z1 * $z2 + $z1 * $z3 + $z2 * $z3;
    $c3 = - ($z0 + $z1 + $z2 + $z3);
    $c4 = 1;

    my $numer = $c0 + $m3*($c1 + $m3*($c2 + $m3*($c3 + $m3*$c4)));

    $c0 *= $l / $numer;
    $c1 *= $l / $numer;
    $c2 *= $l / $numer;
    $c3 *= $l / $numer;
    $c4 *= $l / $numer;

    return ( $c0, $c1, $c2, $c3, $c4 );
}

sub sum_poly {
    my ($a, $c, $g, $t) = @_;
    my @sum = ( 0, 0, 0, 0, 0 );

    for my $i (0..4) {
        $sum[$i] = $a->[$i] + $c->[$i] + $g->[$i] + $t->[$i];
    }
    return @sum;
}

sub print_poly {
  printf "    alpha[int(%.12g\n".
         "             + dc*(%.12g\n".
         "             + dc*(%.12g\n".
         "             + dc*(%.12g\n".
         "             + dc*%.12g))))],\n", @_;
}

my @a0 = poly(ord('C'), ord('G'), ord('T'), ord('A'), ord('d'));
my @a1 = poly(ord('C'), ord('G'), ord('T'), ord('A'), ord('e'));
my @a2 = poly(ord('C'), ord('G'), ord('T'), ord('A'), ord('n'));
my @a3 = poly(ord('C'), ord('G'), ord('T'), ord('A'), ord('z') + 1);

my @c0 = poly(ord('A'), ord('G'), ord('T'), ord('C'), ord('y'));
my @c1 = poly(ord('A'), ord('G'), ord('T'), ord('C'), ord('t'));
my @c2 = poly(ord('A'), ord('G'), ord('T'), ord('C'), ord('o'));
my @c3 = poly(ord('A'), ord('G'), ord('T'), ord('C'), ord('s'));

my @g0 = poly(ord('A'), ord('C'), ord('T'), ord('G'), ord('u'));
my @g1 = poly(ord('A'), ord('C'), ord('T'), ord('G'), ord('a'));
my @g2 = poly(ord('A'), ord('C'), ord('T'), ord('G'), ord('n'));
my @g3 = poly(ord('A'), ord('C'), ord('T'), ord('G'), ord('z') + 1);

my @t0 = poly(ord('A'), ord('C'), ord('G'), ord('T'), ord('h'));
my @t1 = poly(ord('A'), ord('C'), ord('G'), ord('T'), ord('y'));
my @t2 = poly(ord('A'), ord('C'), ord('G'), ord('T'), ord('m'));
my @t3 = poly(ord('A'), ord('C'), ord('G'), ord('T'), ord('z') + 1);


my @l0 = sum_poly(\@a0, \@c0, \@g0, \@t0);
my @l1 = sum_poly(\@a1, \@c1, \@g1, \@t1);
my @l2 = sum_poly(\@a2, \@c2, \@g2, \@t2);
my @l3 = sum_poly(\@a3, \@c3, \@g3, \@t3);

# Round up.
$l0[0] += 0.5;
$l1[0] += 0.5;
$l2[0] += 0.5;
$l3[0] += 0.5;

#printf "%.10g %.10g %.10g %.10g %.10g\n", @l0;
#printf "%.10g %.10g %.10g %.10g %.10g\n", @l1;
#printf "%.10g %.10g %.10g %.10g %.10g\n", @l2;
#printf "%.10g %.10g %.10g %.10g %.10g\n", @l3;

print_poly(@l0);
print_poly(@l1);
print_poly(@l2);
print_poly(@l3);
