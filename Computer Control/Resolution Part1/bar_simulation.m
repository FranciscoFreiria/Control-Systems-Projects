% Flexible robot link
%--------------------------------------------------------------------
% Loads the true flexible link model
load('barrassmodel.mat')
% Defines experiment parameters
%Ts=0.01; % Sampling interval
%tfinal=200; %
% Simulates the true model to generate data for identification
%if(i==8)
 %d=input_generation(i,tfinal);   
%end
%d=input_generation(8,tfinal);
%sim('barra1');
% Plots the continuous and the discrete time outputs

%gg=plot(ts,ys,'r');
%set(gg,'LineWidth',1.5);
%hold off
% Saves data for identification
%save('iodata1.mat','us','ys','ts')

%figure(2)
%ys=ys*180/pi;
%plot(ts,ys);

%Ts = mean(diff(ts)); % Intervalo de amostragem (s)
%utrend=us;
%ytrend=ys;

%af = 0.8;
%Afilt = [1 -af];
%Bfilt = (1-af)*[1 -1];
% Filtragem seguida de eliminação de tendências
% Filtering and detrending
%yf = filter(Bfilt,Afilt,ytrend);
%u = detrend(utrend);
%yf=detrend(yf);

%z = [yf u];
%na = 2; % AR part
%nb =1; % X part
%nc = 2; % MA part
%nk = 1; % Atraso puro – pure delay
%nn = [na nb nc nk];

%th = armax(z,nn);
%---------------------------------------------------%
tfinal=200;
f = 0.02;
fs = 100;
Ts= 1/fs;
t_test=(0:Ts:tfinal)';
u_test = square(2*pi*f*t_test);
%u_test = idinput(length(t_test),'PRBS', [0 0.1]);
u = u_test;
t = t_test;
onda = zeros(length(t), 2);

for nl = 1:length(t)
   onda(nl,1) = t(nl);
   onda(nl,2) = u(nl); 
end
sim('barra1');
Ts = mean(diff(ts)); % Intervalo de amostragem (s)
utrend=us;
ytrend=ys;

af = 0.8;
Afilt = [1 -af];
Bfilt = (1-af)*[1 -1];
% Filtragem seguida de eliminação de tendências
% Filtering and detrending
yf = filter(Bfilt,Afilt,ytrend);
u = detrend(utrend);
yf=detrend(yf);

z2 = [yf u];

info = zeros(24, 5);
j = 1;
%for na = 2:7
%    for nb = 1:4
%      for nc = 2:6
%            for nk = 1:2
%                if ((nk + nb <= na) && (nc <= na))
%                    nn = [na nb nc nk];
%                    th = armax(z2,nn);
%                  [~, fit] = compare(z2, th); 
%                    if fit > 90
%                        info(j,:) = [na nb nc nk fit];
%                       j =j +1;
 %                  end
 %              end
 %          end
 %      end
 %   end
%end
nn = [5 3 4 2];               
th = armax(z2,nn); % th is a structure in identification toolbox format


[den1,num1] = polydata(th);
yfsim = filter(num1,den1,u); % Equivalent to idsim(u,th);

[~, fit] = compare(z2, th);
[num,den] = eqtflength(num1,conv(den1,[1 -1]));

[A,B,C,D] = tf2ss(num,den);
sys=tf(num,den);
figure(1);
hold on
bode(sys);
pole(sys);
sys = ss(A,B,C,D,0.01);
hold off

figure(2)
impulse(sys);
axis([0 10 -8 2])
grid on;

figure(3);
step(sys)
axis([0 4 -10 2])
grid on;


%Gráfico dos polos e zeros
figure(4)
zplane(num,den);


ypred = dlsim(A,B,C,D,u);
z3 = [ypred,u];
[~, pila] = compare(z3, th);

figure(5)
hold on;
gg=plot(ts, ypred);
set(gg,'LineWidth',1.5);
gg=xlabel('t (s)');
set(gg,'FontSize',14);
gg=ylabel('Graus º');
set(gg,'Fontsize',14);
plot(ts, ytrend);
xlabel('t (s)')
ylabel('rad/s')
xlim([0 200])
legend('modelo','sistema real')

%Gráfico do sinal de entrada
figure(6)
hold on;
gg=plot(ts,us);
set(gg,'LineWidth',1.5);
gg=xlabel('t (s)');
set(gg,'FontSize',14);
gg=ylabel('rad/s');
set(gg,'Fontsize',14);
%axis([0 tfinal -4 8]);
xlim([0 200]);
ylim([-2 2]);
hold off;

%Gráfico do sinal de saída
figure(7)
hold on;
gg=plot(ts, ys);
set(gg,'LineWidth',1.5);
gg=xlabel('t (s)');
set(gg,'FontSize',14);
gg=ylabel('rad/s');
set(gg,'Fontsize',14);
%axis([0 tfinal -4 8]);
xlim([0 200]);
hold off;

save('space-state_model.mat','A','B','C','D','onda')



%Gráfico de comparação da sáida filtrado com a saída do motor
figure(8)
hold on;
gg=plot(ts,yf);
set(gg,'LineWidth',1.5);
gg=xlabel('t (s)');
%set(gg,'FontSize',14);
%gg=ylabel('rad/s');
%set(gg,'Fontsize',14);
%axis([0 tfinal -4 8]);
%xlim([0 200]);
%plot(ts,ys);
%hold off;

%Gráfico de comparação da sáida com a saída do modelo
figure(9)
hold on;
gg=plot(ts, yfsim);
set(gg,'LineWidth',1.5);
gg=xlabel('t (s)');
set(gg,'FontSize',14);
gg=ylabel('y (volt)');
set(gg,'Fontsize',14);
%axis([0 tfinal -4 8]);
plot(ts,ys);
xlim([20 22])
hold off;

%Diagrama de bode da respota em frequencia do filtro
figure(10);
freqz(Bfilt,Afilt,2000,2000)
set(gca, 'XScale', 'log')

%Diagrama de bode
figure(11);
bode(num,den);




%[den1, num1] = polydata(M);
%y2 = idsim(u, M);

%figure(3)
%zplane(num1, den1); 
% us and ys contain i/o data with a sampling interval of Ts
%---------------------------------------------------------------------
% End of file
