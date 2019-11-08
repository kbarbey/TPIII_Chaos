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
  double omega02=0.0; // pulsation
  double gamma=0.0; //coefficient d'amortissement sur masse
  int sampling;   // output tous les sampling pas de temps
  int last;
  ofstream *outputFile;

  void printOut(bool force){
    if((!force && last>=sampling) || (force && last!=1)){
      double ekin = 0.5*(vx*vx+vy*vy); // TODO: A completer
      double epot = m*g*y+0.5*(computeL(t)-l0)*(computeL(t)-l0)-q*(Ex*x+Ey*y); // TODO: A completer
      double Pnc = -nu*(vx*vx+vy*vy);  // TODO: A completer
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
    
  double ax(double t, double x){ // TODO: calculer l'acceleration selon x
    return -omega02*x  + omega02*l0*sin(atan(x/(-y)))+q/m*Ex*cos(w*t); //-gamma*vx
  }
  
  double ay(double t, double y){ // TODO: calculer l'acceleration selon y
    return -omega02*y - omega02*l0*cos(atan(x/(-y)))+q/m*Ey*cos(w*t) - g; //-gamma*vy
  }
  
  
  void step(){ // TODO: programmer un pas du schema de Verlet
	  
	double tx(x),ty(y),tvx(vx),tvy(vy);
    x+=dt*tvx+(dt*dt)*0.5*(ax(t,tx)-gamma*tvx);
    y+=dt*tvy+(dt*dt)*0.5*(ay(t,ty)-gamma*tvy);
    double t2vx(tvx), t2vy(tvy);
    t2vx+=0.5*dt*(ax(t,tx)-gamma*tvx);
    t2vy+=0.5*dt*(ay(t,ty)-gamma*tvy);
    vx+=0.5*dt*(ax(t,tx)+ax(t+dt,x))+dt*(-gamma*t2vx);
    vy+=0.5*dt*(ay(t,ty)+ay(t+dt,y))+dt*(-gamma*t2vy);
    t+=dt;
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
    omega02=k/m;
    gamma=nu/m;
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
