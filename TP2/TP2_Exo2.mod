set matieres :={'F1','F2','F3','C1','C2','A1','A2'};
set compo:={'C','Cu','Mn'};

param prix {j in matieres};
param compoMin {i in compo};
param compoMax {i in compo};
param stock {j in matieres};
param a {j in matieres, i in compo};

var x{i in compo} >=0;

minimize z : sum{j in matieres} prix[j]* x[j];
stock_limit{j in matieres}: x[j] <= stock[j];

Rmin{i in compo} sum{j in matieres} a[i,j] * x[j] >= compoMin[i];
Rmax{i in compo}sum{j in matieres} a[i,j] * x[j] <= compoMax[i];
solve;

for{j in matieres}
{
printf 'valeur de matiere %s utilise est %d \n',j,x[j];
}
printf'z = %f\n',z;
data;
param prix:=
		'F1'	1.2
		'F2'	1.5
		'F3'	0.9
		'C1'	1.3
		'C2'	1.2
		'A1'	1.2
		'A2'	1;

param compoMin:=
			'C' 	2
			'Cu'	0.4
			'Mn'	1.2;

param compoMax :=
			'C' 	3
			'Cu'	0.6
			'Mn'	1.65;

param stock :=
		'F1'	4000
		'F2'	3000
		'F3'	6000
		'C1'	5000
		'C2'	2000
		'A1'	3000
		'A2'	2500;

param a 'C'	'Cu'	'Mn' :=
	'F1'2.5	0	1.3
	'F2'3	0	0.8
	'F3'0	0.3	0
	'C1'0	90	0
	'C2'0	96	4
	'A1'0	0.4	1.2
	'A2'0	0.6	0;


