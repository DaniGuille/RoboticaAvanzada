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
  
  try
  {
	motores = jointmotor_proxy->getAllMotorParams();
  }
  catch(const Ice::Exception &ex)
  {
	std::cout << ex << std::endl;
  }	
  
  connect(horizontalScrollBar, SIGNAL(valueChanged(int)), spinBox, SLOT(setValue(int)));
  connect(spinBox, SIGNAL(valueChanged(int)), this, SLOT(fromSliderZ(int)));	
  connect(spinBox, SIGNAL(valueChanged(int)), horizontalScrollBar, SLOT(setValue(int)));	
  
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

void SpecificWorker::moveLegZ(float val)
{
	 try
  {
		//RoboCompLegController::AnglesLeg angles = {1.0,1.0,1.0,1.0};	
		//legcontroller_proxy->setFKLeg(angles);
	  RoboCompLegController::StateLeg state = legcontroller_proxy->getStateLeg();
	  qDebug()<<state.x<<state.y<<state.z;
      //RoboCompLegController::PoseLeg pose = {state.x, state.y, (float)horizontalScrollBar->value(), 1.0};
	  //legcontroller_proxy->setIKLeg(pose);
	  
  }
  catch(const Ice::Exception &e)
  {
	std::cout << "Error!!!!!!!!" << e << std::endl;
  }
	
}


void SpecificWorker::fromSliderZ(int z)
{
	moveLegZ(z);
	
}




