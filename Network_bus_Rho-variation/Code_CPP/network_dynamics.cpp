#include "network_dynamics.h"

/**********************************************************************************************/

void imprimir_matriz(int *y, int N, int M)
{
  std::ofstream fout("matrix.csv");
  for(int i=0; i<N; i++){
    for(int j=0; j<M; j++){
      fout << y[i*M + j] << '\t';			
    }
    fout << std::endl;
  }
  fout.close();
}

/**********************************************************************************************/

void Gillespie_buses(bus &the_bus, std::vector<agents> &aux, int &exposed, int i, double t,
										 double ran_1, double ran_2, double ran_3, int T)
{
  //Inicio el tiempo
  double aux_t=0;
  
  //Creo los números aleatorios que me dirán el 'dt', el evento que pasará y el agente a escoger que realizará el evento.
  Crandom ran1((int)(ran_1*MY_MAX)), ran2((int)(ran_2*MY_MAX)), ran3((int)(ran_3*MY_MAX));
  
  //Creo las variables a utilizar del algoritmo.
  double prob, a1, A, dt;	
  
  //Variable del agente que realizará el evento.
  int the_agent;
  
  //Abro el archivo para imprimir los datos
  std::ofstream fout, fout1;	
  
  //Creo el iterador para encontrar el agente.
  std::vector<agents>::iterator it;
  
  while(aux_t<T){
    //Defino la probabilidad con la cual se realizará el evento.
    prob = ran2.r();
    
    //Calculo el 'dt' en el que se realizará el evento.
    a1=beta*the_bus.Ns.size()*the_bus.Ni.size();
    A=a1;
    dt = -(1.0/A)*log(ran1.r());
    aux_t += dt;
    if(aux_t > T){break;}
    
    //Hago que el agente pase a ser expuesto y deje de ser susceptible si hay algún susceptible.
    if(the_bus.Ns.size() > 0){			
      
      for(int i=0; i<aux.size(); i++){
	if((it = std::find(the_bus.Ns.begin(),the_bus.Ns.end(),aux[i])) != the_bus.Ns.end()){
	  //Hallo el item del agente.
	  the_agent = std::distance(the_bus.Ns.begin(),it);
	  //Le digo al agente que pasa a ser expuesto.
	  the_bus.Ns[the_agent].susceptible = false;
	  the_bus.Ns[the_agent].exposed = true;			
	  //Paso el agente al vector de expuestos.
	  the_bus.Ne.push_back(the_bus.Ns[the_agent]);
	  //Elimino al agente del vector de susceptibles.
	  the_bus.Ns.erase( the_bus.Ns.begin() + the_agent);
	  //Le digo al agente que pasa a ser expuesto.
	  aux[i].susceptible = false;
	  aux[i].exposed = true;
	  //Sumo uno al número de expuestos nuevos.
	  exposed++;
	  /*
	  //Imprimo únicamente los expuestos
	  fout.open("Datos/exposed.csv", std::ofstream::app);
	  fout << t + aux_t << '\t' << exposed << std::endl;
	  fout.close();
	  */
	  //Termino el ciclo
	  break;
	}				
      }
    }
    
    /*
    //Imprimo los datos
    fout.open("Datos/bus_" + std::to_string(i) + ".csv", std::ofstream::app);
    fout << t + aux_t << '\t' << the_bus.Ns.size() << '\t' << the_bus.Ne.size() << '\t' << the_bus.Ni.size() << std::endl;
    fout.close();
    fout1.open("Datos/bus.csv", std::ofstream::app);		
    fout1 << t + aux_t << '\t' << the_bus.Ns.size() << '\t' << the_bus.Ne.size() << '\t' << the_bus.Ni.size() << std::endl;
    fout1.close();
    */
  }
}

/**********************************************************************************************/

void Gillespie_estaciones(station &the_station, std::vector<agents> &aux, int &exposed, int i, double t, int id_NP, int count_NP,
													double ran_1, double ran_2, double ran_3, int T)
{
  //Inicio el tiempo
  double aux_t=0;
  
  //Creo los números aleatorios que me dirán el 'dt', el evento que pasará y el agente a escoger que realizará el evento.
  Crandom ran1((int)(ran_1*MY_MAX)), ran2((int)(ran_2*MY_MAX)), ran3((int)(ran_3*MY_MAX));
  
  //Creo las variables a utilizar del algoritmo.
  double prob, a1, a2, a3, a4, a5, A, dt, S, E, I, N;
  
  //Variable del agente que realizará el evento.
  int the_agent, out_agent, agent_mat;	
  
  //Abro el archivo para imprimir los datos
  std::ofstream fout;	
  
  //Creo el iterador 
  std::vector<agents>::iterator it;
  
  while(aux_t<T){
    //Defino la probabilidad con la cual se realizará el evento.
    prob = ran2.r();
    
    //Miro cuántos susceptibles, expuestos e infectados hay.
    S = the_station.Ns.size();
    E = the_station.Ne.size();
    I = the_station.Ni.size();
    N = the_station.N();

    if(N==0){break;}
    
    //Calculo el 'dt' en el que se realizará el evento.
    a1 = beta*S*I; //Un susceptible se expone.
    a2 = Sa; //Un susceptible entra a la estación.
    a3 = Sa; //Una persona sale de la estación.
    /*
    a2 = Sa*(the_station.Nmax-N); //Un susceptible entra a la estación.
    a3 = Sa*S; //Un susceptible sale de la estación.
    a4 = Sa*I*0; //El infectado sale de la estación.
    a5 = Sa*E; //Un expuesto sale de la estación.
    */
    A = a1 + a2 + a3;;
    dt = -(1.0/A)*log(ran1.r());
    aux_t += dt;
    
    if(aux_t > T){break;}
    
    //Se escoge el evento.
    if(prob < a1/A && S > 0){//Un susceptible se expone.
      for(int i=0; i<aux.size(); i++){
	if((it = std::find(the_station.Ns.begin(),the_station.Ns.end(),aux[i])) != the_station.Ns.end()){
	  //Hallo el item del agente.
	  the_agent = std::distance(the_station.Ns.begin(),it);
	  //Le digo al agente que pasa a ser expuesto.
	  the_station.Ns[the_agent].susceptible = false;
	  the_station.Ns[the_agent].exposed = true;			
	  //Paso el agente al vector de expuestos.
	  the_station.Ne.push_back(the_station.Ns[the_agent]);
	  //Elimino al agente del vector de susceptibles.
	  the_station.Ns.erase( the_station.Ns.begin() + the_agent);
	  //Le digo al agente que pasa a ser expuesto.
	  aux[i].susceptible = false;
	  aux[i].exposed = true;
	  //Sumo uno al número de expuestos infectados.
	  exposed++;
	  /*
	  //Imprimo únicamente los expuestos
	  fout.open("Datos/exposed.csv", std::ofstream::app);
	  fout << t + aux_t << '\t' << exposed << std::endl;
	  fout.close();
	  */
	  //Termino el ciclo
	  break;
	}			
      }			
    }
    else if(prob < (a1+a2)/A){//Ingresa un susceptible.
      //Se crea el agente.
      agents new_agent;
      //Se le indica en dónde está.
      new_agent.location = i;
      //Se le da un número identificador.
      new_agent.number = id_NP;			
      //Se agraga el id al vector de susceptibles de la estación.
      the_station.Ns.push_back(new_agent);
      //Le sumo 1 a la variable que me identifica los agentes y también a la que me dice cuántos agentes hay.
      id_NP++;
      count_NP++;
      //Si el número aleatorio es menor a 0.05, entonces el nuevo susceptible va a estar conectado al infectado.
      if(ran3.r() < prob_net){
	//Lo agrego al vector de agentes conectados al infectado
	aux.push_back(new_agent);
      }				
    }
    else if(S+E > 0){
      //Escogo al azar el agente que sale.
      the_agent = (int)(ran3.r()*(S+E));

      //Reviso si es un susceptible y lo saco.
      if(the_agent < S){
	//Reviso si el agente está conectado con el infectado.
	if((it = std::find(aux.begin(), aux.end(), the_station.Ns[the_agent])) != aux.end()){
	  //Lo elimino del vector auxiliar.
	  aux.erase(it);
	}
	//Quito el agente de la estación.
	the_station.Ns.erase( the_station.Ns.begin() + the_agent);
      }
      else{
	the_agent -= S;
	//Reviso si el agente está conectado con el infectado.
	if((it = std::find(aux.begin(), aux.end(), the_station.Ne[the_agent])) != aux.end()){
	  //Lo elimino del vector auxiliar.
	  aux.erase(it);
	}
	//Quito el agente de la estación.
	the_station.Ne.erase( the_station.Ne.begin() + the_agent );	
      }

      //Le resto 1 al contador de agentes.
      count_NP--;
    }


    /*
      if(prob < (a1+a2+a3)/A){
      //Escojo al azar el susceptible que sale.
      the_agent = (int)(ran3.r()*S);
      //Reviso si el agente está conectado con el infectado.
      if((it = std::find(aux.begin(), aux.end(), the_station.Ns[the_agent])) != aux.end()){
	//Lo elimino del vector auxiliar.
	aux.erase(it);
      }
      //Quito el agente de la estación.
      the_station.Ns.erase( the_station.Ns.begin() + the_agent);
      //Le resto 1 al contador de agentes.
      count_NP--;
    }	
    else if(prob < (a1+a2+a3+a4)/A){//Se va el infectado.				
      //Quito el agente de la estación.
      the_station.Ni.erase( the_station.Ni.begin() + 0);
      //Borro el vector auxiliar y la matriz.
      aux.clear();
      //std::cout << "El infectado se fue" << std::endl;
      //Le resto 1 al contador de agentes.
      count_NP--;
    }
    else{//Se va un expuesto.
      //Escojo al azar el susceptible que sale.
      the_agent = (int)(ran3.r()*E);
      //Reviso si el agente está conectado con el infectado.
      if((it = std::find(aux.begin(), aux.end(), the_station.Ne[the_agent])) != aux.end()){
	//Lo elimino del vector auxiliar.
	aux.erase(it);
      }
      //Quito el agente de la estación.
      the_station.Ne.erase( the_station.Ne.begin() + the_agent );
      //Le resto 1 al contador de agentes.
      count_NP--;			
    }
    */
    /*
    //Imprimo los datos
    fout.open("Datos/station_" + std::to_string(i) + ".csv", std::ofstream::app);
    fout << t + aux_t << '\t' << the_station.Ns.size() << '\t' << the_station.Ne.size() << '\t' << the_station.Ni.size() << std::endl;
    fout.close();
    */
  }	
}

/**********************************************************************************************/
