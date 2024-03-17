n=10000000;

sz = [1 n];
u = unifrnd(0.5,1.5,sz);


fileID = fopen('uni.in','w');
fprintf(fileID,'%f\n',u);
fclose(fileID);


histogram(u);