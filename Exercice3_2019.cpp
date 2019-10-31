#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>
#include "ConfigFile.tpp" // Fichier .tpp car inclut un template

using namespace std;

class Exercice3{

private:
  double t,dt,tFin;
  double x,y,vx,vy;
  double m,g,k,l0,q; 
  double w,nu,c;  // frequence d'excitation; amortissement; couple (facultatif)
  double Ex, Ey;  // champ electrique oscillatoire
  int sampling;   // output tous les sampling pas de temps
  int last;
  ofstream *outputFile;

  void printOut(bool force){
    if((!force && last>=sampling) || (force && last!=1)){
      double ekin = 0.; // TODO: A completer
      double epot = 0.; // TODO: A completer
      double Pnc = 0.;  // TODO: A completer
      *outputFile << t << " " << x << " " << y << " " << vx << " " << vy << " "
                  << ekin << " " << epot << " " << Pnc << endl;
      last=1;
    }else{
      last++;
    }
  };
    

  double computeL(double t){ // longueur du pendule
   return sqrt(x*x + y*y);
  }
    
  double ax(double t){ // TODO: calculer l'acceleration selon x
    return 0.0;
  }
  
  double ay(double t){ // TODO: calculer l'acceleration selon y
    return 0.0;
  }
  
  void step(){ // TODO: programmer un pas du schema de Verlet

    x+=0.0;
    y+=0.0;
    vx+=0.0;
    vy+=0.0;
  }
  
  
public:
  
  Exercice3(int argc, char* argv[]){
    
    string inputPath("configuration.in"); // Fichier d'input par defaut
    if(argc>1) // Fichier d'input specifie par l'utilisateur ("./Exercice3 config_perso.in")
      inputPath = argv[1];

    ConfigFile configFile(inputPath); // Les parametres sont lus et stockes dans une "map" de strings.

    for(int i(2); i<argc; ++i) // Input complementaires ("./Onde config_perso.in input_scan=[valeur]")
      configFile.process(argv[i]);
  
    tFin     = configFile.get<double>("tFin");
    dt       = configFile.get<double>("dt");
    x        = configFile.get<double>("x0");
    y        = configFile.get<double>("y0");
    vx       = configFile.get<double>("vx0");
    vy       = configFile.get<double>("vy0");
    m        = configFile.get<double>("m");
    q        = configFile.get<double>("q");    
    g        = configFile.get<double>("g");
    k        = configFile.get<double>("k");
    l0       = configFile.get<double>("l0");
    Ex       = configFile.get<double>("Ex");
    Ey       = configFile.get<double>("Ey");
    w        = configFile.get<double>("w");
    nu       = configFile.get<double>("nu");
    c        = configFile.get<double>("c");
    sampling = configFile.get<int>("sampling");
    
    // Ouverture du fichier de sortie
    outputFile = new ofstream(configFile.get<string>("output").c_str());
    outputFile->precision(15); 
  };
  
  ~Exercice3(){
    outputFile->close();
    delete outputFile;
  };
  
  void run(){
    last = 0;
    t = 0;
    printOut(true);
    while( t<(tFin-0.5*dt) ) {
      step();
      t += dt;
      printOut(false);
    }
    printOut(true);
  };

};

 
int main(int argc, char* argv[]) 
{
  Exercice3 engine(argc, argv);
  engine.run();
  return 0;
}
