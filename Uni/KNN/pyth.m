load lernxor.mat
net = newff([0 1;0 1],[3 1],{'logsig','logsig'},'trainlm');
y=sim(net,arg);
net.biasConnect(1) = 1;
%net.biasConnect(2) = 1;
net.trainParam.epochs = 50;
net = train(net,arg,erg);
y=sim(net,arg);
x=linspace(0,1,51); 
y=x;
[x,y] = meshgrid(x,y);

z = zeros(51,51);	% Initialisierung
for i=1:51
    for j=1:51
        z(i,j) = sim(net,[x(i,j);y(i,j)]);
    end
end

mesh(x,y,z)