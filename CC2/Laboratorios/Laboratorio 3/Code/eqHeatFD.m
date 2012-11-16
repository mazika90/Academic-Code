% Completar de acuerdo al enunciado.
function [result] = eqHeatFD(L, h, k, a)

[X,X_c] = partition(0,L,L/h);
[Y,Y_c] = partition(0,L,L/k);

u_0 = zeros(1,L/h);
u_J = [];
A = zeros(L/h,L/h);
lambda = (a*a*k)/(h*h);
l = 1-(2*lambda);
MAX_ITER = 5000;

% Computar las condiciones de borde y la matriz A %
for i = 1:((L/h)),
	% condiciones de borde
	x_i = X(1,i);
	u_0(i) = (e**(2*x_i))*sin(x_i)*cos(x_i);
	
	% matriz A
	if i == 1
		A(1,1) = l;
		A(1,2) = lambda;
	elseif i== (L/h)
		A(i,i) = l;
		A(i,i-1) = lambda;
	else
		A(i,i) = l;
		A(i,i+1) = lambda;
		A(i,i-1) = lambda;
end

for i=0:MAX_ITER,
	u_j = A*u_j;
	u_j(0) = 0;
	u_j(L/h) = 0;
 
result = A;
end

