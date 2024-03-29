system('g++ -o Exercice3 Exercice3_2019.cpp')
system('./Exercice3 configuration.in')

output = load('Verlet.out');

t = output(:,1);
x = output(:,2);
y = output(:,3);
vx = output(:,4);
vy = output(:,5);
ekin = output(:,6);
epot = output(:,7);
Pnc = output(:,8);

clear output

fs=18; lw=1;
figure
set(gcf,'Color','w')
plot(x,y,'-', 'linewidth',lw)
set(gca,'fontsize',fs)
%axis equal
xlabel('x [m]')
ylabel('y [m]')
%% Position en fonction du temps
figure
set(gcf,'Color','w')
x0 = 1e-6;
w2 = 1.498282769900674;
y0 = -4.369999;
yeq = -4.37;
w1 = 1.860521018838127;
X = x0*cos(w2*t); % Analytique
Y = (y0-yeq)*cos(w1*t) + yeq;
plot(t,x,'linewidth',lw) % peut rajouter y(t) ou enlever (pour petites oscill par ex)
set(gca,'fontsize',fs)
xlabel('t [s]')
ylabel('x [m]')
legend('x','(y_0-y_{eq})cos(\omega_1t)+y_{eq}')
%% Vitesses
figure
set(gcf,'Color','w')
plot(t,vx,t,vy, 'linewidth',lw)
set(gca,'fontsize',fs)
xlabel('t [s]')
ylabel('v_x,v_y [m/s]')
legend('vx','vy')

figure
set(gcf,'Color','w')
plot(t,ekin+epot -ekin(1)-epot(1), 'linewidth',lw)
set(gca,'fontsize',fs)
xlabel('t [s]')
ylabel('\Delta E [J]')



% figure % spectre de Fourier 
% tfin=t(end);
% vxk=fft(vx);
% [ns,ii]=size(vxk); dtfourier=tfin/ns; 
% fn=2*pi*[0:1/(ns*dtfourier):(ns-1)/(ns*dtfourier)]; % DFT frequencies [s^{-1}]
% hk=plot(fn,abs(vxk),'b-','linewidth',lw);
% set(gca,'fontsize',fs, 'yscale','log');
% xlabel('\omega [s^{-1}]')
% ylabel('|fft(v_x)|')

% Lyapounov plots: run 2 simulations then Analyse first one, then:
% xsave=x; ysave=y; vxsave=vx; vysave=vy;
% figure
% plot(t,sqrt((x-xsave).^2+(y-ysave).^2+((vx-vxsave).^2+(vy-vysave).^2)/0.8660254037844386^2))
% set(gca,'fontsize',fs)
% set(gca,'yscale','log')
% xlabel('t [s]')
% ylabel('d [m]')

% -----------Poincare plots: assumes N steps per period and sampling=N
%{
istart=100; % do not plot until istart periods
figure
plot(x(istart:end),vx(istart:end),'.',y(istart:end),vy(istart:end),'.')
axis equal
xlabel('x,y [m]')
ylabel('v_x, v_y [m/s]')

figure
plot(x(istart:end),y(istart:end),'.')
axis equal
xlabel('x [m]')
ylabel('y [m]')

figure
plot(vx(istart:end),vy(istart:end),'.')
axis equal
xlabel('v_x [m/s]')
ylabel('v_y [m/s]')
%}




