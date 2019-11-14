% Ce script Matlab automatise la production de resultats
% lorsqu'on doit faire une serie de simulations en
% variant un des parametres d'entree.
% 
% Il utilise les arguments du programme (voir ConfigFile.h)
% pour remplacer la valeur d'un parametre du fichier d'input
% par la valeur scannee.
%

%% Parametres %%
%%%%%%%%%%%%%%%%

repertoire = './'; % Chemin d'acces au code compile (NB: enlever le ./ sous Windows)
executable = 'Exercice3'; % Nom de l'executable (NB: ajouter .exe sous Windows)
input = 'configuration.in'; % Nom du fichier d'entree de base

nsimul = 20; % Nombre de simulations a faire entre w-Deltaw et w+Deltaw
w1 = 1.860521018838127;
w2 = 1.498282769900674;
Deltaw = 0.2;
omega = w2;
w =  linspace(omega-Deltaw,omega+Deltaw,nsimul);  % frequence %round(logspace(2,4,nsimul)); % Nombre d'iterations entier de 10^2 a 10^4
tfin = 1000; % TODO: Verifier que la valeur de tfin est la meme que dans le fichier input
nsteps = round(logspace(2,5,nsimul));
dt = tfin ./ nsteps;

%% Parametres physiques pour le calcul analytique :
%{
x0       = 0.0;
y0       = -4.37+1.e-6;
vx0      = 0.0;
vy0      = 0.0;
m        = 1.5;
q        = 1.e-4;
g        = 9.81;
k        = 4.5;
l0       = 1.1;
Ex       = 0.0;
Ey       = 0.0;
w        = 0.0;
nu       = 0.0;
c        = 0.0;
w2       =sqrt((k*g)/(g*m+k*l0))
%}

paramstr = 'w'; % Nom du parametre a scanner
param = w; % Valeurs du parametre a scanner

%% Simulations %%
%%%%%%%%%%%%%%%%%

output = cell(1, nsimul); % Tableau de cellules contenant le nom des fichiers de sortie
for i = 1:nsimul
    output{i} = [paramstr, '=', num2str(param(i)), '.out'];
    % Execution du programme en lui envoyant la valeur a scanner en argument
    cmd = sprintf('%s%s %s %s=%.15g output=%s', repertoire, executable, input, paramstr, param(i), output{i})
    disp(cmd)
    system(cmd);
end

%% Analyse %%
%%%%%%%%%%%%%

error = zeros(1,nsimul);
x_th = x0;
y_th = y0;
for i = 1:nsimul % Parcours des resultats de toutes les simulations
    data = load(output{i}); % Chargement du fichier de sortie de la i-ieme simulation
    y = data(end,3);
    error(i)=abs(y-y_th);
end

%{
fs=16;lw=2;
figure
loglog(dt,error,'k+-','linewidth',lw)
set(gcf,'Color','w')
set(gca,'fontsize',fs)
xlabel('dt')
ylabel('\Delta y(t_{fin})')
grid on


p = polyfit(log(dt), log(error), 1);
tau = p(1);
kappa = exp(p(2));
figure % plot
loglog(dt, kappa*dt.^tau, 'r', dt, error, 'k+')
legend(sprintf('y = %.3f * x^{%.2f}', kappa, tau))
legend(sprintf('y = %.3f * x^{%.2f}', kappa, tau))
xlabel('\Deltat [s]')
ylabel('Maximum de l''erreur sur x(t_{fin}) [m]')
grid on
%}

error = zeros(1,nsimul);
for i = 1:nsimul % Parcours des resultats de toutes les simulations
    data = load(output{i}); % Chargement du fichier de sortie de la i-ieme simulation
    ekin = data(:,6);
    epot = data(:,7);
    maxE(i)=max(ekin+epot)-(ekin(1)+epot(1));
end

fs=16;lw=2;
figure
plot(w,maxE,'k+-','linewidth',lw)
set(gca,'fontsize',fs)
xlabel('\omega')
ylabel('Max(E)-E(0)')
grid on


