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
   proxys[0] = legcontroller1_proxy;
   proxys[1] = legcontroller2_proxy;
   proxys[2] = legcontroller3_proxy;
   proxys[3] = legcontroller4_proxy;
   proxys[4] = legcontroller5_proxy;
   proxys[5] = legcontroller6_proxy;
   estado=0;
   numPata=0;
   for(int i=0;i<6;i++){
      posIniciales[i] = proxys[i]->getStateLeg();	   
   }
   	  qDebug()<<"POSICIONES INICIALES: "<<posIniciales[0].x<<posIniciales[0].y<<posIniciales[0].z;

  try
  {
	motores = jointmotor_proxy->getAllMotorParams();
  }
  catch(const Ice::Exception &ex)
  {
	std::cout << ex << std::endl;
  }	
  
  //connect(horizontalScrollBar, SIGNAL(valueChanged(int)), spinBox, SLOT(setValue(int)));
  //connect(spinBox, SIGNAL(valueChanged(int)), this, SLOT(fromSliderZ(int)));	
  //connect(spinBox, SIGNAL(valueChanged(int)), horizontalScrollBar, SLOT(setValue(int)));	
  connect(legButton1, SIGNAL(clicked()), this, SLOT(subir()));
  connect(legButton2, SIGNAL(clicked()), this, SLOT(avanzar()));
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
	
	timer.start(Period);

	return true;
}

void SpecificWorker::compute()
{
  updateMotorList();
  if(legButton2->isChecked()){
	avanzar();  
  }
  
}


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

void SpecificWorker::actualizarPos(){
	   for(int i=0;i<6;i++){
      posiciones[i] = proxys[i]->getStateLeg();	   
   }
}

void SpecificWorker::avanzar()
{
	
	switch(estado)
	{
		//Sube
		case 0:
			subir();
			break;
		//Avanza y baja
		case 1:
			bajar();
			break;
		case 2:
			legButton2->setCheckable(false);
			//remar();
			break;
	}
	
	
	
	/*
	  float incrementoX=20.0;
	  float incrementoY=150.0;

	  pose.x=posiciones[0].x+incrementoX;
	  pose.z=posiciones[0].z;
	  pose.y=posiciones[0].y;
	  pose.vel=1.0;
	  pose.ref=posiciones[0].ref;
	  if(legcontroller1_proxy->setIKLeg(pose,false)==false)
		 qDebug()<<"No puede llegar";
	  qDebug()<<"POSICIONES AL AVANZAR:"<<pose.x<<pose.y<<pose.z;

*/
}

void SpecificWorker::subir()
{
	 try
  {
	  float valor=0.7;
	  RoboCompLegController::AnglesLeg angles = {0.0,valor,0.0,0.5};	
	  proxys[numPata]->setFKLeg(angles);
      while(true){
		  actualizarPos();
		  qDebug()<<posiciones[numPata].poshombro;
		if(-valor==posiciones[numPata].poshombro){
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
	  qDebug()<<"POSICIONES AL SUBIR: "<<pose.x<<pose.y<<pose.z;*/
  }
  }
  catch(const Ice::Exception &e)
  {
	std::cout << "Error!!!!!!!!" << e << std::endl;
  }
	
}

void SpecificWorker::bajar()
{
	 try
  {
	  float valor=0.7;
	  RoboCompLegController::AnglesLeg angles = {valor,-valor,0.0,0.5};	
	  proxys[numPata]->setFKLeg(angles);
      while(true){
		actualizarPos();
		qDebug()<<posiciones[numPata].poshombro<<posiciones[numPata].posclavicula;
		if(valor==posiciones[numPata].poshombro && valor==posiciones[numPata].posclavicula){
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


void SpecificWorker::resetPos()
{
	pose.x=posIniciales[0].x;
	pose.y=posIniciales[0].y;
	pose.z=posIniciales[0].z;
	pose.vel=1.0;
	pose.ref=posIniciales[0].ref;
	legcontroller1_proxy->setIKLeg(pose,false);
	qDebug()<<pose.x<<pose.y<<pose.z;

}

void SpecificWorker::fromSliderZ(int z)
{
	//moveLegZ(z);
	
}




