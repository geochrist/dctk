#!/usr/bin/perl

use liberty;

#init

$x = 11;
$z = 34;
$str1 = "some strrrrrrrrrrrr";
$str2 = "another strrrrrrrrrrrr";

$pi = liberty::si2drPIInit(\$x);

print "PIInit returns $x\n";

liberty::si2drReadLibertyFile("../inverter_blatantcopy1.lib", \$x);

$y = liberty::si2drPIGetErrorText($x, \$z);

print "Result of reading lib was:  '$y'\n";

$groups = liberty::si2drPIGetGroups(\$x);

while( !liberty::si2drObjectIsNull($ngroup = liberty::si2drIterNextGroup($groups,\$x), \$z) )
{
	$gt = liberty::si2drGroupGetGroupType($ngroup,\$z);
	$names = liberty::si2drGroupGetNames($ngroup, \$z);
	$nam1 = liberty::si2drIterNextName($names,\$z);
	liberty::si2drIterQuit($names, \$x);
	print "group in PI: type: $gt; name: $nam1\n";

	$groups2 = liberty::si2drGroupGetGroups($ngroup,\$x);
	while( !liberty::si2drObjectIsNull($ngroup2 = liberty::si2drIterNextGroup($groups2,\$x), \$z) )
	{
		if ( $x == $liberty::SI2DR_NO_ERROR )
		{print "Hey-- no error!!\n";}
		$gt = liberty::si2drGroupGetGroupType($ngroup2,\$z);
		$names = liberty::si2drGroupGetNames($ngroup2, \$z);
		$nam2 = liberty::si2drIterNextName($names,\$z);
		liberty::si2drIterQuit($names, \$x);
		print "        $gt; name: $nam2\n";
	}
	liberty::si2drIterQuit($groups2, \$x);

	liberty::si2drWriteLibertyFile("../inverter_blatantcopy55.lib", $ngroup, \$x);
}
liberty::si2drIterQuit($groups, \$x);

$pi = liberty::si2drPIQuit(\$x);

$pi = liberty::si2drPIInit(\$x);
print "PIInit returns $x\n";

$group1 = liberty::si2drPICreateGroup("lib1", "library", \$x);

$y = liberty::si2drPIGetErrorText($x, \$z);
print "Result of lib creation was:  '$y'\n";

$x = $liberty::SI2DR_COMPLEX;

print "complex is $x\n";

$attr1 = liberty::si2drGroupCreateAttr($group1, "attr1", $liberty::SI2DR_COMPLEX, \$x);
$y = liberty::si2drPIGetErrorText($x, \$z);
print "Result of attr creation was:  '$y'\n";

liberty::si2drComplexAttrAddInt32Value($attr1, 45, \$x);
liberty::si2drComplexAttrAddStringValue($attr1, "Hello, World", \$x);
liberty::si2drComplexAttrAddBooleanValue($attr1, 1, \$x);
liberty::si2drComplexAttrAddFloat64Value($attr1, 98.6, \$x);

$expr1 = liberty::si2drCreateDoubleValExpr(28.79,\$x);
$expr2 = liberty::si2drCreateDoubleValExpr(34.46,\$x);
$expr3 = liberty::si2drCreateBinaryOpExpr($expr1, $liberty::SI2DR_EXPR_OP_ADD, $expr2, \$x);

$str5 = liberty::si2drExprToString($expr1,\$x);
print "expr1 is $str5\n";

$str5 = liberty::si2drExprToString($expr2,\$x);
print "expr2 is $str5\n";

$str5 = liberty::si2drExprToString($expr3,\$x);
print "expr3 is $str5\n";

liberty::si2drComplexAttrAddExprValue($attr1, $expr3, \$x);

$vt = 0;
$i1 = 0;
$f1 = 2.2;
$str6 = "Hello there";
$b1 = 0;

$values = liberty::si2drComplexAttrGetValues($attr1, \$x);
while( 1 )
{
	$cplex = liberty::si2drIterNextComplex($values, \$x);
	$vt = liberty::si2drComplexValGetValueType($cplex, \$x);

	if($vt == $liberty::SI2DR_INT32)
	{
		$i1 = liberty::si2drComplexValGetInt32Value($cplex, \$x);
		print "Integer: $i1\n";
	}
	elsif($vt == $liberty::SI2DR_FLOAT64)
	{
		$f1 = liberty::si2drComplexValGetFloat64Value($cplex, \$x);
		print "Double: $f1\n";
	}
	elsif($vt == $liberty::SI2DR_BOOLEAN)
	{
		$b1 = liberty::si2drComplexValGetBooleanValue($cplex, \$x);
		print "Boolean: $b1\n";
	}
	elsif($vt == $liberty::SI2DR_STRING)
	{
		$str6 = liberty::si2drComplexValGetStringValue($cplex, \$x);
		print "String: $str6\n";
	}
	elsif($vt == $liberty::SI2DR_EXPR)
	{
		$expr1 = liberty::si2drComplexValGetExprValue($cplex, \$x);
		$str5 = liberty::si2drExprToString($expr1,\$x);
		print "Expr: $str5\n";
	}
	else{ last; }
}
liberty::si2drIterQuit($values, \$x);
$pi = liberty::si2drPIQuit(\$x);
print "Done!\n";
