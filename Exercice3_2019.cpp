#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <iomanip>
#include "ConfigFile.tpp" // Fichier .tpp car inclut un template

using namespace std;

class Exercice3{

private:
  double t,dt,tFin;
  int nsteps;
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
      double epot = m*g*y+0.5*(computeL()-l0)*(computeL()-l0)-q*(Ex*x+Ey*y); // TODO: A completer
      double Pnc = -nu*(vx*vx+vy*vy);  // TODO: A completer
      *outputFile << t << " " << x << " " << y << " " << vx << " " << vy << " "
                  << ekin << " " << epot << " " << Pnc << endl;
      last=1;
    }else{
      last++;
    }
  };
    
  double theta(double x,double y) {
      double L = computeL();
      if (L > 0.00000001) {
          double theta = acos(x/L);
          if (abs(sin(theta)-y/L) < 0.00001)   {
              cout << "first way";
              return theta+M_PI/2;
          }
          else if (abs(-sin(theta)-y/L) < 0.00001)  {
              cout << "second way";
              return -theta+M_PI/2;
          }
      }
      else  {
          cout << "third way";
          return 0.0;
      }
  }
    
  double computeL(){ // longueur du pendule
   return sqrt(x*x + y*y);
  }
  
  double ax(double t,double x1,double x2)   { // replace default arguments in ax
      return ax(t,x1,x2,vx,vy);
  }
      
  double ax(double t, double x1, double x2, double v1, double v2)   { // TODO: calculer l'acceleration selon x
    vector<double> F_C(Couple(t));
      return -omega02*x1  + omega02*l0*sin(theta(x1,x2))+q/m*Ex*cos(w*t) + F_C[0]/m -gamma*v1;
  }
    
  double ay(double t, double x1, double x2)   { // replace default arguments in ay
        return ay(t,x1,x2,vx,vy);
  }
      
  double ay(double t, double x1, double x2, double v1, double v2) { // TODO: calculer l'acceleration selon y
    vector<double> F_C(Couple(t));
    return -omega02*x2 - omega02*l0*cos(theta(x1,x2)) + q/m*Ey*cos(w*t) - g + F_C[1]/m -gamma*v2;
  }
    
    vector<double> Couple(double t)   {
        vector<double> F;
        double f(0.0);
        if (computeL() > 0.00001)   {
            f = c/computeL()*sin(w*t);
        }
        vector<double> e = e_theta();
        F.push_back(f*e[0]);
        F.push_back(f*e[1]);
        return F;
    }
    
    vector<double> e_theta()  {
        vector<double> e;
        e.push_back(cos(theta(x,y)));
        e.push_back(sin(theta(x,y)));
        return e;
    }
  
  void step()   { // TODO: programmer un pas du schema de Verlet
	double tx(x),ty(y),tvx(vx),tvy(vy);
    x+=dt*tvx+0.5*dt*dt*ax(t,tx,ty);
    y+=dt*tvy+0.5*dt*dt*ay(t,tx,ty);
    double t2vx(tvx), t2vy(tvy);
    t2vx+=0.5*dt*ax(t,tx,ty,tvx,tvy);
    t2vy+=0.5*dt*ay(t,tx,ty,tvx,tvy);
    vx+=0.5*dt*(ax(t,tx,ty,t2vx,t2vy)+ax(t+dt,x,y,t2vx,t2vy));
    vy+=0.5*dt*(ay(t,tx,ty,t2vx,t2vy)+ay(t+dt,x,y,t2vx,t2vy));
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
    nsteps   = configFile.get<int>("nsteps");
    if  (nsteps > 0)    {
        dt = tFin/nsteps;
    }
    else    {
        dt       = configFile.get<double>("dt");
    }
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
