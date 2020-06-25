%clear all,clc
[filename, pathname] = uigetfile('*.Hdwt', 'open file');
if isequal(filename, 0) | isequal(pathname, 0)   
    disp('file input canceled.');  
  else
    f1=fopen(fullfile(pathname, filename),'r');
end;
fichier2=fread(f1,'ubit8');
sgnQ=fichier2(end);
sgn=mod(sgnQ,8);
Qr=sgnQ-sgn;
DIFLcdR=fichier2(end-1);
DIFLrlR=fichier2(end-2);
smo=fichier2(end-3);
 bb=0;
 fichier2=fichier2';
 for jj=7:-1:1
     if(smo>=2^(jj-1))
         smo=smo-2^(jj-1);
         A=fichier2(1:end-4-bb);
         B=[0 0];
         C=fichier2(end-3-bb:end);
        fichier2=cat(2,A,B,C);
     end;
     bb=bb+4;
 end; 
 Lfich=length(fichier2);
i=1;
k=Lfich-31;
h=zeros(1,7);
while k<Lfich-3
   for j=0:3
  h(i)=(fichier2(k+j))*(256^j)+h(i);
    end;
  k=k+4;
  i=i+1;
end;
LTR=h(1);
LXfreqR=h(2);
DWTmaxR=h(3)*10^(-4);
DWTminR=h(4)*10^(-4);
if sgn==1
  DWTminR=-DWTminR;  
end;
lR=h(5);
LXrle1R=h(6);
LcompR=h(7);
if(DIFLrlR>=25)&(DIFLrlR<50)
    N=0;
DIFLrlR=DIFLrlR-25;
RR(2)=fichier2(end-32);
RR(1)=fichier2(end-33);
elseif(DIFLrlR>=50)&(DIFLrlR<75)
    N=1;
DIFLrlR=DIFLrlR-50;
RR(3)=fichier2(end-32);
RR(2)=fichier2(end-33);
RR(1)=fichier2(end-34);
elseif(DIFLrlR>=75)&(DIFLrlR<100)
    N=2;
DIFLrlR=DIFLrlR-75;
RR(4)=fichier2(end-32);
RR(3)=fichier2(end-33);
RR(2)=fichier2(end-34);
RR(1)=fichier2(end-35);
elseif(DIFLrlR>=100)&(DIFLrlR<125)
    N=3;
DIFLrlR=DIFLrlR-100;
RR(5)=fichier2(end-32);
RR(4)=fichier2(end-33);
RR(3)=fichier2(end-34);
RR(2)=fichier2(end-35);
RR(1)=fichier2(end-36);
elseif(DIFLrlR>=125)&(DIFLrlR<150)
    N=4;
DIFLrlR=DIFLrlR-125;
RR(6)=fichier2(end-32);
RR(5)=fichier2(end-33);
RR(4)=fichier2(end-34);
RR(3)=fichier2(end-35);
RR(2)=fichier2(end-36);
RR(1)=fichier2(end-37);
elseif(DIFLrlR>=150)&(DIFLrlR<175)
    N=5;
DIFLrlR=DIFLrlR-150;
RR(7)=fichier2(end-32);
RR(6)=fichier2(end-33);
RR(5)=fichier2(end-34);
RR(4)=fichier2(end-35);
RR(3)=fichier2(end-36);
RR(2)=fichier2(end-37);
RR(1)=fichier2(end-38);
elseif(DIFLrlR>=175)&(DIFLrlR<200)
    N=6;
DIFLrlR=DIFLrlR-175;
RR(8)=fichier2(end-32);
RR(7)=fichier2(end-33);
RR(6)=fichier2(end-34);
RR(5)=fichier2(end-35);
RR(4)=fichier2(end-36);
RR(3)=fichier2(end-37);
RR(2)=fichier2(end-38);
RR(1)=fichier2(end-39);
elseif(DIFLrlR>=200)
    N=7;
DIFLrlR=DIFLrlR-200;
RR(9)=fichier2(end-32);
RR(8)=fichier2(end-33);
RR(7)=fichier2(end-34);
RR(6)=fichier2(end-35);
RR(5)=fichier2(end-36);
RR(4)=fichier2(end-37);
RR(3)=fichier2(end-38);
RR(2)=fichier2(end-39);
RR(1)=fichier2(end-40);
end;
d32=0;
if RR(1)>=100
    d32=1;
    RR(1)=RR(1)-100;
end;
WAV=RR(2);
if(WAV==1)
    wav='haar';
elseif(WAV==2)
    wav='db4';
elseif(WAV==3)
    wav='sym3';
elseif(WAV==4)
    wav='coif1';
elseif(WAV==5)
    wav='bior3.1';
elseif(WAV==6)
    wav='rbio3.1'; 
elseif(WAV==7)
    wav='dmey';
else
    disp('Unknown wavelet name.');
end; 
RR(2)=0;
kk(1)=RR(1);
for i=2:N+2
    kk(i)=0;
    for j=1:i
kk(i)=RR(j)+kk(i);
    end;
end;
SS=[kk;kk];
SR=SS';
%--------------------------------------------------------------------------
                %%% Reconstructive %%%
%--------------------------------------------------------------------------
Tcod8R=fichier2(1:LTR);
LcdR=LTR*8-DIFLcdR;
k=8;
for j=1:LTR
    for i=0:7
        cdR(k-i)=fix(Tcod8R(j)/(2^(7-i)));
        if Tcod8R(j)>=2^(7-i)
            Tcod8R(j)=Tcod8R(j)-2^(7-i);
        end;
    end;
        k=k+8;
end;
for i=1:LcdR
    codeR(i)=cdR(i);
end; 
%--------------------------------------------------------------------------
                          %%% Reconstructive Xfreq %%%
%--------------------------------------------------------------------------
AR=fichier2(LTR+1:LTR+lR);
BR=fichier2(LTR+lR+1:LTR+lR/2+LXfreqR);
PR=fichier2(LTR+lR/2+LXfreqR+1:LTR+lR+LXfreqR);
if length(AR)==0
    XfR=BR;
else
ARR=Iabais(AR);
XfR=zeros(1,LXfreqR);
XfR(PR)=ARR;
k=1;
for i=1:LXfreqR
    if XfR(i)==0
        XfR(i)=BR(k);
        k=k+1;
    end;
end;
end;
somf=sum(XfR);
%--------------------------------------------------------------------------
                          %%%% Décode Huffman %%%%
%--------------------------------------------------------------------------
XfreqR=XfR./somf;
[dict,avglen] = huffmandict([0:2^Qr-1],XfreqR);
XrleRR = huffmandeco(codeR,dict);
Xrle1R=XrleRR(1:LXrle1R);
if d32==1
    temp=Iabais(Xrle1R);
Xrle1R=temp;
end;
Trl8R=XrleRR(LXrle1R+1:length(XrleRR)-LcompR);
compR=XrleRR(length(XrleRR)-LcompR+1:length(XrleRR));
LTrl8=length(Trl8R);
LrlR=LTrl8*8-DIFLrlR;
k=8;
for j=1:LTrl8
    for i=0:7
        rlR(k-i)=fix(Trl8R(j)/(2^(7-i)));
        if Trl8R(j)>=2^(7-i)
            Trl8R(j)=Trl8R(j)-2^(7-i);
        end;
    end;
        k=k+8;
end;
k=1;
for i=1:LrlR
       if rlR(i)==1
           XrleR(i)=Xrle1R(k);
           k=k+1;
       else
          XrleR(i)=1; 
       end;
   end;
   XrleeR(2:2:2*length(XrleR))=XrleR;
   XrleeR(1:2:2*length(XrleR))=compR;
%--------------------------------------------------------------------------
                            %%% Décodage RLE %%%% 
%--------------------------------------------------------------------------
CQr=irle(XrleeR);
%--------------------------------------------------------------------------
                       %%%%  Déquantification  %%%%
%--------------------------------------------------------------------------
ICQ=round(((DWTmaxR-DWTminR)/(-1+2^Qr))*CQr+DWTminR);
%--------------------------------------------------------------------------
           %%% Application to DWT Inverse  %%%
%--------------------------------------------------------------------------
XR = waverec2(ICQ,SR,wav);%Transformation IDWT .

XR=uint8(XR);
% rgbImage = ind2rgb(XR, jet(256));
% figure,imshow(XR),title('my img');
imwrite(XR,'new_ima.bmp','bmp');
figure,imshow(XR),title('Image Reconstructed');
%--------------------------------------------------------------------------
                            %%% MSE, PSNR %%%
%--------------------------------------------------------------------------
% [filename, pathname] = uigetfile('*.bmp', 'open image');% only image Bitmap
% if isequal(filename, 0) | isequal(pathname, 0)   
%     disp('Image input canceled.');  
%    X = [];    map = []; 
% else
%     [X,MAP]=imread(fullfile(pathname, filename));%Lecture d'image.
% end;
[M N]=size(X);
XR=double(XR);
XE=zeros(size(X));
XE=double(XE);
E=0;
for i=1:M
    for j=1:N
      XE(i,j)=X(i,j)-XR(i,j);
      E=E+((XE(i,j))^2);
          end
end
MSE=E/(M*N)
PSNR=10*log10((2^Qr-1)^2/MSE)%Peak signal to noise ratio PSNR
% imwrite(XR,'Image Error.bmp','bmp');
% figure,imshow(XE,[]),title('Image Error');

