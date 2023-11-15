clear;
close all;
clc;

%% PRIMEIRA PARTE - IDENTIFICAÇÃO DO MODELO
load('barrassmodel.mat')
load('state_space_model.mat')
%---------------------------------------------------%
tfinal=200;
f = 0.02;
fs = 100;
Ts= 1/fs;
sys = ss(A,B,C,D,0.01);

%% SEGUNDA PARTE - IDENTIFICAÇÃO DO MODELO
% CONTROLADOR

% R = 0  -> Pouca importância à energia (controlador nervoso)
% R = infinito -> Muita importância à energia (controlador lento)

% Botões de ajuste do LQR: matrizes Q e R
Q = C'*C; % Fixo, não se altera
R = 50; % Botão de ajuste do custo da energia

% Gerar o ganho do controlador
[K,S,p] = dlqr(A, B, Q, R); 
%Regulador de realimentação de estado linear quadrático (LQ) para sistema de espaço de estadO discreto

% OBSERVADOR

% Re = 0 -> Saída fiável (observador nervoso/ráido)
% Re = infinito -> Saída ruidosa (observador lento)

% Botões de ajuste do LQE: matrizes Qe e Re
qe = 100;
Re = 1;
Be = eye(6:6);
Qe = qe*eye(6:6);

% Gerar o ganho do observador
[Ke,~,~,pe] = dlqe(A, Be, C, Qe, Re);
%Aplicação do filtro de Kalman segundo o modelo de espaço de estados
%e os dados de covariância de ruído de medição.

% LQG
N = inv([A-eye(size(A)), B; C,0])*[zeros(size(A,1),1);1];
Nx = N(1:end-1,:);
Nu = N(end,:);
Nbar = Nu+K*Nx;
%Calculo do ganho para a entrada externa na malha fechada

figure(1);
T_lqr = ss(A,B,K,0,Ts);
bode(T_lqr)
%Representação no dimínio da frequência do modelo do espaço de estados do
%sistema linear quadrático regulado (LQR)

C_lqr = ss(A-B*K,B*Nbar,C,0,Ts);
figure(2);
bode(C_lqr)
%Representação no dimínio da frequência do modelo do espaço de estados do
%sistema de malha fechada, obtido através da combinação das duas entradas
%de feedback com a entrada externa (pré-filtro)

figure(3);
[NC_lqr,DC_lqr] = tfdata(C_lqr,'v');
subplot(1,2,1)
zplane(NC_lqr,DC_lqr)
ax = axis;
subplot(1,2,2)
zplane([],p)
axis(ax)
%representação dos zeros e polos para o circuito em malha fechada

figure(4);
[NG,DG] = tfdata(sys,'v');
[NG,DG] = eqtflength(NG,DG);
SRL = tf(conv(NG,fliplr(NG)),conv(DG,fliplr(DG)));
p_srl = rlocus(SRL,1/R);
subplot(1,2,2)
zplane([],p_srl)
title(['SRL poles for \rho = 1/R = ' num2str(1/R)])
ax = axis;
subplot(1,2,1)
rlocus(SRL)
hold on;
zplane([],[]);
hold off
axis(2*ax)
%representação dos root-locus

PHIE = A-Ke*C*A;
GAMMAE = B-Ke*C*B;
%Providenciou uma arquitetura de algo proximo de um modelo de espaço de estados 

figure(5);
subplot(1,2,1)
zplane([],eig(PHIE))
subplot(1,2,2)
zplane([],pe)
%Representação no dimínio da frequência do modelo do espaço de estados do
%sistema LQE que serve para confirmar a consistência dos polos em malha
%fechada

K2_lqg = ss(PHIE-GAMMAE*K,Ke,K,0,Ts);
[NK2_lqg,DK_lqg] = tfdata(K2_lqg,'v');
%NK_lqg = wshift('1D',NK2_lqg,1);
K_lqg = tf(NK2_lqg,DK_lqg,Ts);
T_lqg = series(sys,K_lqg);
C_lqg = feedback(sys,K_lqg)*Nbar; %(pré-filtro)

figure(6);
T2_lqg = ss([A zeros(size(A)); Ke*C*A PHIE-GAMMAE*K],[B; Ke*C*B],[zeros(size(K)) K],0,Ts);
clf
bode(T_lqg)
legend('T (LQG)')
%representação do sistema LQG no diminio da frequência

figure(7);
C2_lqg = ss([A -B*K; Ke*C*A PHIE-GAMMAE*K-Ke*C*B*K],[B; Ke*C*B+GAMMAE]*Nbar,[C zeros(size(C))],0,Ts);
bode(C_lqr,C2_lqg)
legend('C (LQR)','C (LQG)')
%Comparação no diminio da frquencia do sistema LQG com o sistema LQR 

figure(8);
[NC2_lqg,DC2_lqg] = tfdata(C2_lqg,'v');
subplot(1,2,1)
zplane(NC2_lqg,DC2_lqg)
ax = axis;
subplot(1,2,2)
zplane([],[p; pe])
axis(ax)
%Representação do gráfico de polos e zero para o sistema LQG de forma a
%verificar o principio de separação para a localização dos polos

figure(9);
Ke1 = reshape(Ke,[1,6]);
T_lqe = ss(A,B,Ke1,0,Ts);
bode(T_lqe)
%representação do sistema LQE no diminio da frequência

figure(10);
C_lqe = ss(A-B*Ke1,B*Nbar,C,0,Ts);
bode(C_lqe)
%representação do sistema LQE no diminio da frequência com o pre-filtro