/*
 *    Copyright (C) 2016 by YOUR NAME HERE
 *
 *    This file is part of RoboComp
 *
 *    RoboComp is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    RoboComp is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with RoboComp.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "specificworker.h"

/**
* \brief Default constructor
*/
SpecificWorker::SpecificWorker(MapPrx& mprx) : GenericWorker(mprx)
{
	innerModel = new InnerModel("/home/robocomp/robocomp/files/innermodel/hexapod.xml");
	proxies[0] = legcontroller1_proxy;
	proxies[1] = legcontroller2_proxy;
	proxies[2] = legcontroller3_proxy;
	proxies[3] = legcontroller4_proxy;
	proxies[4] = legcontroller5_proxy;
	proxies[5] = legcontroller6_proxy;
	numPata=0;
	X=0;
	Z=0;
	for(int i=0;i<6;i++){
		posIniciales[i] = proxies[i]->getStateLeg();	
		legCoord[i] = QVec::vec3(posIniciales[i].x,posIniciales[i].y,posIniciales[i].z);
	}
   	qDebug()<<"POSICIONES INICIALES: "<<posIniciales[0].x<<posIniciales[0].y<<posIniciales[0].z;
	
	/* Patas para el mov tripod */
	//Patas 1,4,5
	legsTripodOn[0]=0;	
	legsTripodOn[1]=3;
	legsTripodOn[2]=4;
	//Patas 2,3,6
	legsTripodOff[0]=1;
	legsTripodOff[1]=2;
	legsTripodOff[2]=5;
	
	/* Patas para el mov OneByOne  */
	//Pata 1
	legsOneByOneOn[0]=0;
	//Patas 2,3,4,5,6
	legsOneByOneOff[0]=1;
	legsOneByOneOff[1]=2;
	legsOneByOneOff[2]=3;
	legsOneByOneOff[3]=4;
	legsOneByOneOff[4]=5;
	
	/* Patas para el mov Quadruped */
	//Patas 1,2
	legsQuadrupedOn[0]=0;
	legsQuadrupedOn[1]=1;
	//Patas 3,4,5,6
	legsQuadrupedOff[0]=2;
	legsQuadrupedOff[1]=3;
	legsQuadrupedOff[2]=4;
	legsQuadrupedOff[3]=5;

	/* Vectores de inicio para el Bezier */
	lini=QVec::vec3(0,0,0);
	lfin=QVec::vec3(0,0,0);
	lmed=QVec::vec3(0,70,0);

	try
	{
		motores = jointmotor_proxy->getAllMotorParams();
	}
	catch(const Ice::Exception &ex)
	{
		std::cout << ex << std::endl;
	}	
	
	connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(cambiarEstado(int)));
	connect(resetButton, SIGNAL(clicked()), this, SLOT(resetPos()));

}

/**
* \brief Default destructor
*/
SpecificWorker::~SpecificWorker()
{
	
}

bool SpecificWorker::setParams(RoboCompCommonBehavior::ParameterList params)
{
	
	string name = PROGRAM_NAME;
	base=QString::fromStdString(params[name+".base"].value);
	timer.start(Period);

	return true;
}

void SpecificWorker::compute()
{
  //updateMotorList();
  movimientos(estado); 
}

/*
void SpecificWorker::updateMotorList()
{
	
	try
	{
		foreach(RoboCompJointMotor::MotorParams m, this->motores)
		{
			MotorState ms=jointmotor_proxy->getMotorState(m.name);
			innerModel->updateJointValue(QString::fromStdString(m.name),ms.pos);
		}
	}
	catch(const Ice::Exception &ex)
	{
		  std::cout << ex << std::endl;
	}	
	
}
*/
void SpecificWorker::actualizarPos(){
	for(int i=0;i<6;i++)
		posiciones[i] = proxies[i]->getStateLeg();	   
}

int SpecificWorker::cambiarEstado(int valor){
	estado=valor;
	return estado;
}

void SpecificWorker::movimientos(int estado)
{
	switch(estado)
	{
		case 1:
			if(One_by_One()){
			  actualizarPos();	
			  X = 0;
			  Z = 30;
			  lini=QVec::vec3(-X,0,-Z);
			  lfin=QVec::vec3(X,0,Z);
			}
			break;
		case 2:
			if(Tripod()){
			  actualizarPos();
			  X = 0;
			  Z = 30;
			  lini=QVec::vec3(-X,0,-Z);
			  lfin=QVec::vec3(X,0,Z);
			}
			break;
		case 3:
			if(Quadruped()){
			  actualizarPos();
			  X = 0;
			  Z = 30;
			  lini=QVec::vec3(-X,0,-Z);
			  lfin=QVec::vec3(X,0,Z);
			}
			break;
		case 4:
			if(Free()){
			  actualizarPos();
			}
			break;
	}
}


bool SpecificWorker::Tripod()
{
	static float i=0;
	bool ismoving=false;	
	if(lini!=QVec::vec3(0, 0, 0)&&lfin!=QVec::vec3(0,0,0))
	{
		//Comprueba que alguna pata se mueva
		for(int k=0;k<6;k++)
			if(proxies[k]->getStateLeg().ismoving){
				ismoving=true;
				break;
			}
		if(!ismoving)
		{
		//patas por arco
			for(int s=0;s<3;s++)
			{
				RoboCompLegController::StateLeg st=posiciones[legsTripodOn[s]];
				QVec posini = QVec::vec3(st.x,legCoord[legsTripodOn[s]].y(),st.z);
				QVec ini = posini,fin = legCoord[legsTripodOn[s]]+lfin,med=legCoord[legsTripodOn[s]];
				QVec tmp = bezier3(ini,QVec::vec3(med.x(),0,med.z()),fin,i);
				RoboCompLegController::PoseLeg p;
				p.x=tmp.x();
				p.y=tmp.y();
				p.z=tmp.z();
				p.ref=base.toStdString();
				p.vel=6;
				proxies[legsTripodOn[s]]->setIKLeg(p,false);
				
			}
		// patas por tierra
			for(int s=0;s<3;s++)
			{
				RoboCompLegController::StateLeg st=posiciones[legsTripodOff[s]];
				QVec posini =QVec::vec3(st.x,legCoord[legsTripodOff[s]].y(),st.z);
				
				QVec ini = posini,fin = legCoord[legsTripodOff[s]]+lini;
				QVec tmp=bezier2(ini,fin,i);
				RoboCompLegController::PoseLeg p;
				p.x=tmp.x();
				p.y=tmp.y();
				p.z=tmp.z();
				p.ref=base.toStdString();
				p.vel=6;
				proxies[legsTripodOff[s]]->setIKLeg(p,false);
				
			}
			i+=.1;
			if (i>1)
			{
				int aux[]={legsTripodOn[0],legsTripodOn[1],legsTripodOn[2]};
				legsTripodOn[0]=legsTripodOff[0];
				legsTripodOn[1]=legsTripodOff[1];
				legsTripodOn[2]=legsTripodOff[2];
				legsTripodOff[0]=aux[0];
				legsTripodOff[1]=aux[1];
				legsTripodOff[2]=aux[2];
				i=0;
				return true;
			}
		}
		return false;
	}
	return true;
}


bool SpecificWorker::One_by_One()
{
	static float i=0;
	bool ismoving=false;	
	if(lini!=QVec::vec3(0, 0, 0)&&lfin!=QVec::vec3(0,0,0))
	{
		//Comprueba que alguna pata se mueva
		for(int k=0;k<6;k++)
			if(proxies[k]->getStateLeg().ismoving){
				ismoving=true;
				break;
			}
		if(!ismoving)
		{
			//pata por arco
			RoboCompLegController::StateLeg st=posiciones[legsOneByOneOn[0]];
			QVec posini = QVec::vec3(st.x,legCoord[legsOneByOneOn[0]].y(),st.z);
			QVec ini = posini,fin = legCoord[legsOneByOneOn[0]]+lfin,med=legCoord[legsOneByOneOn[0]];
			QVec tmp = bezier3(ini,QVec::vec3(med.x(),0,med.z()),fin,i);
			RoboCompLegController::PoseLeg p;
			p.x=tmp.x();
			p.y=tmp.y();
			p.z=tmp.z();
			p.ref=base.toStdString();
			p.vel=6;
			proxies[legsOneByOneOn[0]]->setIKLeg(p,false);
				
			// patas por tierra
			for(int s=0; s<5; s++)
			{
				RoboCompLegController::StateLeg st=posiciones[legsOneByOneOff[s]];
				QVec posini =QVec::vec3(st.x,legCoord[legsOneByOneOff[s]].y(),st.z);
				lini = QVec::vec3(-X,0,-Z/6);
				QVec ini = posini,fin = legCoord[legsOneByOneOff[s]]+lini;
				QVec tmp=bezier2(ini,fin,i);
				RoboCompLegController::PoseLeg p;
				p.x=tmp.x();
				p.y=tmp.y();
				p.z=tmp.z();
				p.ref=base.toStdString();
				p.vel=6;
				proxies[legsOneByOneOff[s]]->setIKLeg(p,false);
			}	
			i+=.1;
			if (i>1)
			{
			int aux[]={legsOneByOneOn[0]};
			legsOneByOneOn[0]=legsOneByOneOff[0];
			//La cola avanza
			legsOneByOneOff[0]=legsOneByOneOff[1];
			legsOneByOneOff[1]=legsOneByOneOff[2];
			legsOneByOneOff[2]=legsOneByOneOff[3];
			legsOneByOneOff[3]=legsOneByOneOff[4];
			legsOneByOneOff[4]=aux[0];
			i=0;
			return true;
			}
		}
		return false;
	}
	return true;
}

bool SpecificWorker::Quadruped()
{
	static float i=0;
	bool ismoving=false;	
	if(lini!=QVec::vec3(0, 0, 0)&&lfin!=QVec::vec3(0,0,0))
	{
		//Comprueba que alguna pata se mueva
		for(int k=0;k<6;k++)
			if(proxies[k]->getStateLeg().ismoving){
				ismoving=true;
				break;
			}
		if(!ismoving)
		{
		//patas por arco
			for(int s=0;s<2;s++)
			{
				RoboCompLegController::StateLeg st=posiciones[legsQuadrupedOn[s]];
				QVec posini = QVec::vec3(st.x,legCoord[legsQuadrupedOn[s]].y(),st.z);
				QVec ini = posini,fin = legCoord[legsQuadrupedOn[s]]+lfin,med=legCoord[legsQuadrupedOn[s]];
				QVec tmp = bezier3(ini,QVec::vec3(med.x(),0,med.z()),fin,i);
				RoboCompLegController::PoseLeg p;
				p.x=tmp.x();
				p.y=tmp.y();
				p.z=tmp.z();
				p.ref=base.toStdString();
				p.vel=6;
				proxies[legsQuadrupedOn[s]]->setIKLeg(p,false);
			}
		// patas por tierra
			for(int s=0;s<4;s++)
			{
				RoboCompLegController::StateLeg st=posiciones[legsQuadrupedOff[s]];
				QVec posini =QVec::vec3(st.x,legCoord[legsQuadrupedOff[s]].y(),st.z);
				lini = QVec::vec3(-X,0,-Z/3);
				QVec ini = posini,fin = legCoord[legsQuadrupedOff[s]]+lini;
				QVec tmp=bezier2(ini,fin,i);
				RoboCompLegController::PoseLeg p;
				p.x=tmp.x();
				p.y=tmp.y();
				p.z=tmp.z();
				p.ref=base.toStdString();
				p.vel=6;
				proxies[legsQuadrupedOff[s]]->setIKLeg(p,false);
				
			}
			i+=.1;
			if (i>1)
			{
				int aux[]={legsQuadrupedOn[0],legsQuadrupedOn[1]};
				legsQuadrupedOn[0]=legsQuadrupedOff[0];
				legsQuadrupedOn[1]=legsQuadrupedOff[1];
				legsQuadrupedOff[0]=legsQuadrupedOff[2];
				legsQuadrupedOff[1]=legsQuadrupedOff[3];
				legsQuadrupedOff[2]=aux[0];
				legsQuadrupedOff[3]=aux[1];
				i=0;
				return true;
			}
		}
		return false;
	}
	return true;
}

bool SpecificWorker::Free()
{

}


QVec SpecificWorker::bezier2(QVec p0, QVec p1, float t)
{
	QVec diff = p1 - p0;
	return p0 + (diff * t);
}

QVec SpecificWorker::bezier3(QVec p0, QVec p1, QVec p2, float t)
{
	QVec a=bezier2(p0,p1,t);
	QVec b=bezier2(p1,p2,t);
	return bezier2(a,b,t);
}

/*
void SpecificWorker::subir()
{
	 try
  {
	  float valor=0.7;
	  RoboCompLegController::AnglesLeg angles = {0.0,valor,0.0,2.0};	
	  proxies[numPata]->setFKLeg(angles);
      while(true){
		  actualizarPos();
		  qDebug()<<posiciones[numPata].poshombro;
		if(numPata%2 == 0)
			valor=-valor;
		if(valor==posiciones[numPata].poshombro){
			estado=1;
			break;
	  }
	  /*RoboCompLegController::StateLeg state = legcontroller_proxy->getStateLeg();
	  string ref = state.ref;
	  float incrementoY=10.0;
	  pose.x=posiciones[0].x;
	  pose.z=posiciones[0].z;
	  pose.y=posiciones[0].y+incrementoY;
	  pose.vel=1.0;
	  pose.ref=posiciones[0].ref;
	  if(legcontroller1_proxy->setIKLeg(pose,false)==false)
		 qDebug()<<"No puede llegar";
	  qDebug()<<"POSICIONES AL SUBIR: "<<pose.x<<pose.y<<pose.z;
  }
  }
  catch(const Ice::Exception &e)
  {
	std::cout << "Error!!!!!!!!" << e << std::endl;
  }
	
}
/* ROTANDO
void SpecificWorker::bajar()
{
	 try
  {
	  float valor1=0.7;
	   float valor2=0.7;
	  RoboCompLegController::AnglesLeg angles = {valor1,-valor2,0.0,2.0};	
	  proxies[numPata]->setFKLeg(angles);
      while(true){
		actualizarPos();
		//qDebug()<<posiciones[numPata].poshombro<<posiciones[numPata].posclavicula;
		
		if(numPata%2 != 0)
			valor1=-valor1;
		if(valor1==posiciones[numPata].poshombro && valor2==posiciones[numPata].posclavicula){
			numPata++;
			if(numPata==6){
				numPata=0;
				estado=2;
			}else{
				estado=0;
			}
			break;
	  }
	  
  }
  }
  catch(const Ice::Exception &e)
  {
	std::cout << "Error!!!!!!!!" << e << std::endl;
  }
	
}
*/

/* DE LADO
void SpecificWorker::bajar()
{
	 try
  {
	  float y=-0.7;
	  float z=0.7;
	  if(numPata%2 != 0){
		  z=-z;
	  }
	  RoboCompLegController::AnglesLeg angles = {0.0,y,z,2.0};	
	  proxys[numPata]->setFKLeg(angles);
      while(true){
		actualizarPos();
		
		if(numPata%2 != 0){
			y=-y;
			z=-z;
		}
		qDebug()<<"Z: "<<z<<" Poshombro: "<<posiciones[numPata].poshombro<<" Y: "<<-y<< "PosCodo: "<<posiciones[numPata].poscodo;

		if(z==posiciones[numPata].poshombro && y==posiciones[numPata].poscodo){
			numPata++;
			if(numPata==6){
				numPata=0;
				estado=2;
			}else{
				estado=0;
			}
			break;
	  }
	  
  }
  }
  catch(const Ice::Exception &e)
  {
	std::cout << "Error!!!!!!!!" << e << std::endl;
  }
	
}
/*DE LADO PRIMERO LAS DE LA DERECHA
void SpecificWorker::bajar()
{
	 try
  {
	  float y=-0.7;
	  float z=0.7;
	  if(numPata%2 != 0){
		  z=-z;
	  }
	  RoboCompLegController::AnglesLeg angles = {0.0,y,z,2.0};	
	  proxys[numPata]->setFKLeg(angles);
      while(true){
		actualizarPos();
		if(numPata%2 != 0){
			y=-y;
			z=-z;
		}
		qDebug()<<"Z: "<<z<<" Poshombro: "<<posiciones[numPata].poshombro<<" Y: "<<-y<< "PosCodo: "<<posiciones[numPata].poscodo;

		if(z==posiciones[numPata].poshombro && y==posiciones[numPata].poscodo){
			numPata=numPata+2;
			if(numPata==6){
				numPata=1;
				estado=0;
			}else if(numPata>6){
				numPata=0;
				estado=0;
			}
			if(numPata==7){
				numPata=0;
				estado=2;
			}else{
			    estado=0;
			}
			break;
	  }
	  
  }
  }
  catch(const Ice::Exception &e)
  {
	std::cout << "Error!!!!!!!!" << e << std::endl;
  }
	
}

void SpecificWorker::remar()
{
		 try
  {
	  float y=-0.7;
	  float z=0.7;
	  if(numPata%2 != 0){
		  z=-z;
	  }
	  RoboCompLegController::AnglesLeg angles = {0.0,y,z,2.0};	
	  proxys[numPata]->setFKLeg(angles);
      while(true){
		actualizarPos();
		if(numPata%2 != 0){
			y=-y;
			z=-z;
		}
		qDebug()<<"Z: "<<z<<" Poshombro: "<<posiciones[numPata].poshombro<<" Y: "<<-y<< "PosCodo: "<<posiciones[numPata].poscodo;

		if(z==posiciones[numPata].poshombro && y==posiciones[numPata].poscodo){
			numPata=numPata+2;
			if(numPata==6){
				numPata=1;
				estado=0;
			}else if(numPata>6){
				numPata=0;
				estado=0;
			}
			estado=0;
			break;
	  }
	  
  }
  }
  catch(const Ice::Exception &e)
  {
	std::cout << "Error!!!!!!!!" << e << std::endl;
  }
	
}

*/

double SpecificWorker::mapear(double x, double in_min, double in_max, double out_min, double out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


void SpecificWorker::resetPos()
{
	/*pose.x=posIniciales[0].x;
	pose.y=posIniciales[0].y;
	pose.z=posIniciales[0].z;
	pose.vel=1.0;
	pose.ref=posIniciales[0].ref;
	legcontroller1_proxy->setIKLeg(pose,false);*/
	
	
	RoboCompLegController::AnglesLeg angles;
	angles.q1=0;
	angles.q2=0.3;
	angles.q3=-0.6;
	angles.vel=1;
	for(int i=0;i<6;i++)
		proxies[i]->setFKLeg(angles, false);
	
	qDebug()<<pose.x<<pose.y<<pose.z;

}
/*
void SpecificWorker::fromSliderZ(int z)
{
	//moveLegZ(z);
	
}
*/



