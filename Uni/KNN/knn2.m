function vektor = matvek(m,n,matrix)
vektor = zeros(m*n,1);
ja=1; je=n;
for i=1:m
    vektor(ja:je) = matrix(i,:);
    ja = ja + n; je = je + n;
end

arg =zeros(9,2);
arg(:,1) = b1; arg(:,2) = b2;

erg = [1 0 ; 0 1];

typen = zeros(9,2);
typen(:,1) = 0; 
typen(:,2) = 1;


net = newff(typen,[5 2],{'logsig','logsig'});

net.trainParam.epochs = 50;
net = train(net,arg,erg);

y = sim(net,arg); disp(y)