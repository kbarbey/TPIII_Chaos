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

repertoire = ''; % Chemin d'acces au code compile (NB: enlever le ./ sous Windows)
executable = 'Exercice3_2019.exe'; % Nom de l'executable (NB: ajouter .exe sous Windows)
input = 'configuration_nom.in'; % Nom du fichier d'entree de base

nsimul = 11; % Nombre de simulations a faire entre w-Deltaw et w+Deltaw
Deltaw=0.01; omega0=1.0 % A MODIFIER...
w =  linspace(omega0-Deltaw,omega0+Deltaw,nsimul);  % frequence %round(logspace(2,4,nsimul)); % Nombre d'iterations entier de 10^2 a 10^4
tfin = 1000.0; % TODO: Verifier que la valeur de tfin est la meme que dans le fichier input
dt = tfin ./ nsteps;

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