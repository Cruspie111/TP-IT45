set produits := {'P1','P2'};
set composants := {'a','b','c'};

param prix {i in produits};
param stock {j in composants};
param a {i in produits, j in composants};
var x{i in produits} >=0;

maximize z : sum{i in produits} prix[i]*x[i];
 
s.t. R{j in composants} : sum{i in produits} a[i,j]*x[i]<=stock[j];
solve;

for {i in produits} {
    printf 'nombre de %s=%d\n', i, x[i] ;
}
printf 'z=%f\n', z ;

data;

param prix :=
			'P1'	3
			'P2'	4;

param stock:=
			'a'	180
			'b'	120
			'c'	150;


param a 'P1'  'P2' :=
	'a'  2     3
	'b'  2     1
	'c'  1     3;

end;
