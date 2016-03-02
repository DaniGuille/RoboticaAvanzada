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
   for(int i=0;i<6;i++){
      posiciones[i] = proxys[i]->getStateLeg();	   
   }
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
  connect(legButton1, SIGNAL(clicked()), this, SLOT(moveLegZ()));
  connect(legButton2, SIGNAL(clicked()), this, SLOT(moveLeg()));

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

void SpecificWorker::moveLegZ()
{
	 try
  {
		//RoboCompLegController::AnglesLeg angles = {1.0,1.0,1.0,1.0};	
		//legcontroller_proxy->setFKLeg(angles);
		//RoboCompLegController::StateLeg state = legcontroller_proxy->getStateLeg();
		//string ref = state.ref;
	  float incrementoX=60.0;
	  float incrementoY=15.0;

	  pose.x=posiciones[1].x;
	  pose.z=posiciones[1].z;
	  pose.y=posiciones[1].y+incrementoY;
	  pose.vel=1.0;
	  pose.ref=posiciones[1].ref;
	  legcontroller1_proxy->setIKLeg(pose);
	  qDebug()<<pose.x<<pose.y<<pose.z;
  }
  catch(const Ice::Exception &e)
  {
	std::cout << "Error!!!!!!!!" << e << std::endl;
  }
	
}

void SpecificWorker::moveLeg()
{
	  float incrementoX=60.0;
	  float incrementoY=30.0;

	  pose.x=posiciones[1].x+incrementoX;
	  pose.z=posiciones[1].z;
	  pose.y=posiciones[1].y-incrementoY;
	  pose.vel=1.0;
	  pose.ref=posiciones[1].ref;
	  legcontroller1_proxy->setIKLeg(pose);
	  qDebug()<<pose.x<<pose.y<<pose.z;


}
void SpecificWorker::fromSliderZ(int z)
{
	//moveLegZ(z);
	
}




