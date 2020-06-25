clear all; clc
filename=imread('color.jpeg');
imwrite(filename,'image.bmp') ;
[filename, pathname] = uigetfile('*.jpeg', 'open image');% only image Bitmap

if isequal(filename, 0) || isequal(pathname, 0)   
    disp('Image input canceled.');  
   X = [];    map = []; 
else
    [X,MAP]=imread(fullfile(pathname, filename));%Lecture d'image.
end;
[MX,NX]=size(X);
lsz=length(size(X));
if lsz==3
X=rgb2gray(X);
end
%--------------------------------------------------------------------------
                    %%% wavelett compression
%--------------------------------------------------------------------------
%wav='bior4.4';
disp('Input wavelet name: chose the wavelet name by writing the number of classification from 1..7');
fprintf('1--Haar: haar\n2--Daubechies: db4\n3--Symlets: sym3\n4--Coiflets: coif1\n5--Biorthogonal: bior3.1\n6--Reverse Biorthogonal: rbio3.1\n7--Discrete Meyer: dmey\n');
WAV=input('Number=');
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
N=input('Input Number of level(Level of decomposition)\n N=');
[C,S] = wavedec2(X,N,wav); %%% decompose une image en ondelette jusqu'au niveau N
Lc=length(C);
D=S(:,1);
F=D';
%R=zeros(length(S));
R(1)=F(1);
for i=2:N+2
    R(i)=F(i)-F(i-1);
end;    
R(2)=WAV;
%--------------------------------------------------------------------------
                              %%% Threshold %%%
%--------------------------------------------------------------------------
disp('seuil input .')
TH=input('TH=');%Threshold value
NBz=0; % Number of zeroes in DWT coefficients
for i=1:Lc
        if abs(C(i))<TH
            C(i)=0;
            NBz=NBz+1;
        end;
end;
pcdz=100*NBz/Lc; %Pourcentage de coeifficients dct nuls
disp(['Pourcentage des coefficients qui seuillée = ' num2str(pcdz)])
%--------------------------------------------------------------------------
                       %%%%  Quantification  %%%%
%--------------------------------------------------------------------------
%disp('nombre de bits pour coder les éléments de matrice')
%Q=input('Q=');
Q=8;% Number of bits to coding Matrix élements 
DWTmin=min(C);
DWTmax=max(C);
CQ=round((-1+2^Q)*(C-DWTmin)/(DWTmax-DWTmin));
%ICQ=round(((DWTmax-DWTmin)/(-1+2^Q))*CQ+DWTmin);
%DifQ=CQ-ICQ
%--------------------------------------------------------------------------
                       %%% Run Length Encoding %%%% 
%-------------------------------------------------------------------------- 
Xrlee=rle(CQ);
%Grle=(length(C)-length(Xrlee))*100/length(C);%Gain de compression RLE
comp=Xrlee(1:2:length(Xrlee));
Lcomp=length(comp);
compteur=Xrlee(2:2:length(Xrlee));
 %[fff,ccc]=hist(compteur,256);
 %plot(ccc,fff);
p1=find(compteur>1);
if max(compteur)>=256
Xrle11=compteur(p1);
LXrle11=length(Xrle11);
k=1;
   for i=1:LXrle11 
   temp=abais(Xrle11(i));
       for j=1:2
   A(k)=temp(j);
   k=k+1;
       end
  end;
  Xrle1=A;R(1)=R(1)+100;
    else
   Xrle1=compteur(p1);
end
      LXrle1=length(Xrle1);
   Xrle2=zeros(1,length(compteur));
   for i=1:length(compteur)
       if compteur(i)>1
           Xrle2(i)=1;
       end;
   end;
   L=length(Xrle2);
Lrl=round(L/8);
if mod(L,8)<4
   if mod(L,8)~=0
      Lrl=Lrl+1;
   end
end 
Lrln=(Lrl)*8 ;
DIFLrl=(Lrln)-L ;
rln=zeros(1,Lrln);
k=1;
for i=1:L
    rln(k)=Xrle2(i);
    k=k+1;
end
Trl8=zeros(1,Lrl);
k=1;
for i=1:Lrl 
   for j=0:7
  Trl8(i)=(rln(k+j))*(2^j)+Trl8(i);
    end;
  k=k+8; 
end;
Xrle=[Xrle1 Trl8 comp];
  %--------------------------------------------------------------------------
                          %%%% code Huffman  %%%%
%-------------------------------------------------------------------------- 
                         
[Xi,Xf]=proba(Xrle);% fonction proba sert à calculer les fréquences des coefficients.
somf=sum(Xf); %la somme des frequences
Xfreq=Xf./somf;
[dict,avglen] = huffmandict([0:2^Q-1],Xfreq);
code = huffmanenco(Xrle,dict);


L=length(code);
Lcd=round(L/8);
if mod(L,8)<4
   if mod(L,8)~=0
      Lcd=Lcd+1;
   end
end 
Lcdn=(Lcd)*8 ;
DIFLcd=(Lcdn)-L ;
codn=zeros(1,Lcdn);
k=1;
for i=1:L
    codn(k)=code(i);
    k=k+1;
end
Tcod8=zeros(1,Lcd);
k=1;
for i=1:Lcd 
   for j=0:7
  Tcod8(i)=(codn(k+j))*(2^j)+Tcod8(i);
    end;
  k=k+8; 
end;
%--------------------------------------------------------------------------
           %%%   compression %%%
%--------------------------------------------------------------------------
LT=length(Tcod8);
LXf=length(Xf);
A=[];P=[];B=[];
k=1;ii=1; jj=1;
for i=1:LXf
   if Xf(i)>255 
   temp=abais(Xf(i));
   P(ii)=i;
     for j=1:2
   A(k)=temp(j);
   k=k+1;
     end
   ii=ii+1;
   else
      B(jj)=Xf(i);
      jj=jj+1;
   end
end;
 l=length(A);
 h01=resize(LT);
 h02=resize(LXf);
 h03=resize(DWTmax*10^4);
 if DWTmin<0 
     signQ=1+Q;
 else
     signQ=Q;
 end;
 h04=resize(DWTmin*10^4);
 h05=resize(l);
 h06=resize(LXrle1);
 h07=resize(Lcomp);
  smo=0;
 if(h01(3)==0)&&(h01(4)==0)
     smo=1;
     h01=[h01(1) h01(2)];
  end;
 if(h02(3)==0)&&(h02(4)==0)
     smo=2+smo;
     h02=[h02(1) h02(2)];
  end;
 if(h03(3)==0)&&(h03(4)==0)
     smo=4+smo;
     h03=[h03(1) h03(2)];
  end;
 if(h04(3)==0)&&(h04(4)==0)
     smo=8+smo;
     h04=[h04(1) h04(2)];
  end;
 if(h05(3)==0)&&(h05(4)==0)
     smo=16+smo;
     h05=[h05(1) h05(2)];
  end;
 if(h06(3)==0)&&(h06(4)==0)
     smo=32+smo;
     h06=[h06(1) h06(2)];
  end;
 if(h07(3)==0)&&(h07(4)==0)
     smo=64+smo;
     h07=[h07(1) h07(2)];
  end;
 if(N==0)
DIFLrl=DIFLrl+25;
elseif(N==1)
DIFLrl=DIFLrl+50;
elseif(N==2)
DIFLrl=DIFLrl+75;
elseif(N==3)
DIFLrl=DIFLrl+100;
elseif(N==4)
DIFLrl=DIFLrl+125;
elseif(N==5)
DIFLrl=DIFLrl+150;
elseif(N==6)
DIFLrl=DIFLrl+175;
elseif(N==7)
DIFLrl=DIFLrl+200;
end;
fichier=[Tcod8 A B P R h01 h02 h03 h04 h05 h06 h07 smo DIFLrl DIFLcd signQ];
 TC=MX*NX/length(fichier)
 GC=100*(1-1/TC)
 f=fopen('fichier.Hdwt','w');
 fwrite(f,fichier,'ubit8');
 fclose(f);
 
