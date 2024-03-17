n=10000000;
mu1 = zeros(1,n)+1.25; % 1-by-n array of zeros
exp = exprnd(mu1);

fileID = fopen('exp.in','w');
fprintf(fileID,'%f\n',exp);
fclose(fileID);

histogram(exp);