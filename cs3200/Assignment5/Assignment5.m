clear
clc
a=imread('Tiger.jpeg');
[m,n,d]=size(a);
kmax=floor((m*n)/(m+n+1));
da=double(a);
U=zeros(m,m);S=zeros(m,n);V=zeros(n,n);e=zeros(kmax,d);cr=zeros(kmax,1);rmse=zeros(kmax,d);
for i=1:d
    [U(:,:,i),S(:,:,i),V(:,:,i)]=svd(da(:,:,i));
end
for k=1:kmax
    ca=zeros(m,n,d);
    cr(k)=m*n/(k*(m+n+1));
    for i=1:d
        cai=zeros(m,n,d);
        [ca(:,:,i),cai(:,:,i)]=deal(U(:,1:k,i)*S(1:k,1:k,i)*V(:,1:k,i)');
        e(k,i)=S(k+1,k+1,i)/S(1,1,i);
        rmse(k,i)=sqrt(sum(sum(((da(:,:,i)-ca(:,:,i)).^2)))/(m*n));
        imwrite(uint8(cai), sprintf('%dk%d.jpg',k,i));
    end
    imwrite(uint8(ca), sprintf('%dk.jpg', k));
end
figure
p=plot(1:kmax,e,'MarkerEdgeColor','r','MarkerEdgeColor','g');
set(p,{'color'},{'red';'green';'blue'})
xlabel('Approximation Rank k');
ylabel('Relative 2-Norm');
xlim([1 kmax])
legend('Red','Green','Blue')
grid on
figure
p=plot(1:kmax,rmse,'MarkerEdgeColor','r','MarkerEdgeColor','g');
set(p,{'color'},{'red';'green';'blue'})
xlabel('Approximation Rank k');
ylabel('RMS Erorr');
xlim([1 kmax])
legend('Red','Green','Blue')
grid on
figure
plot(1:kmax,cr);
xlabel('Approximation Rank k');
ylabel('Compression Ratio');
xlim([1 kmax])
grid on

% %% Clear Everthing For A Fresh Start
% clear;
% clc;
% 
% %reading and converting the image
% inImage=imread('Tiger.jpeg');
% inImage=rgb2gray(inImage);
% inImageD=double(inImage);
% 
% % decomposing the image using singular value decomposition
% [U,S,V]=svd(inImageD);
% 
% % Using different number of singular values (diagonal of S) to compress and
% % reconstruct the image
% dispEr = [];
% numSVals = [];
% for N=5:25:300
%     % store the singular values in a temporary var
%     C = S;
% 
%     % discard the diagonal values not required for compression
%     C(N+1:end,:)=0;
%     C(:,N+1:end)=0;
% 
%     % Construct an Image using the selected singular values
%     D=U*C*V';
% 
% 
%     % display and compute error
%     figure;
%     buffer = sprintf('Image output using %d singular values', N)
%     imshow(uint8(D));
%     title(buffer);
%     error=sum(sum((inImageD-D).^2));
% 
%     % store vals for display
%     dispEr = [dispEr; error];
%     numSVals = [numSVals; N];
% end
% 
% % dislay the error graph
% figure; 
% title('Error in compression');
% plot(numSVals, dispEr);
% grid on
% xlabel('Number of Singular Values used');
% ylabel('Error between compress and original image');