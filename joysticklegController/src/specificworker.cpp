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
	IK=false;
	modovalue=0;
	vel=2;
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
	base=QString::fromStdString(params[name+".floor"].value);
	base=QString::fromStdString(params[name+".base"].value);
	inner = new InnerModel(params[name+".InnerModel"].value);
	
	for(int i=1;i<=6;i++)
	{
		legs<<QString::fromStdString(params[name+".nameleg"+to_string(i)].value);
	}
	qDebug()<<"-----------------------legs----------------";
	qDebug()<<base;
	qDebug()<<legs;
	qDebug()<<"-----------------------legs----------------";
	
// 	leg1=inner->transform(base,legs.at(0));
// 	leg2=inner->transform(base,legs.at(1));
// 	leg3=inner->transform(base,legs.at(2));
// 	leg4=inner->transform(base,legs.at(3));
// 	leg5=inner->transform(base,legs.at(4));
// 	leg6=inner->transform(base,legs.at(5));
// 	qDebug()<<leg1;
	
	RoboCompLegController::StateLeg s;
	s = legcontroller1_proxy->getStateLeg();
	leg1=QVec::vec3(s.x,s.y,s.z);
	s = legcontroller2_proxy->getStateLeg();
	leg2=QVec::vec3(s.x,s.y,s.z);
	s = legcontroller3_proxy->getStateLeg();
	leg3=QVec::vec3(s.x,s.y,s.z);
	s = legcontroller4_proxy->getStateLeg();
	leg4=QVec::vec3(s.x,s.y,s.z);
	s = legcontroller5_proxy->getStateLeg();
	leg5=QVec::vec3(s.x,s.y,s.z);
	s = legcontroller6_proxy->getStateLeg();
	leg6=QVec::vec3(s.x,s.y,s.z);
	
	qDebug()<<leg1;
	
	timer.start(Period);
	timer.start(Period);

	return true;
}

void SpecificWorker::compute()
{
	switch(modovalue)
	{
		case 0:
			modo->setText("FK");
			break;
		case 1:
			modo->setText("IK Leg");
			break;
		case 2:
			modo->setText("IK Body");
			break;
	}
		
// 		leg1=inner->transform(base,legs.at(0));
// 		leg2=inner->transform(base,legs.at(1));
// 		leg3=inner->transform(base,legs.at(2));
// 		leg4=inner->transform(base,legs.at(3));
// 		leg5=inner->transform(base,legs.at(4));
// 		leg6=inner->transform(base,legs.at(5));
	
// 	try
// 	{
// 		camera_proxy->getYImage(0,img, cState, bState);
// 		memcpy(image_gray.data, &img[0], m_width*m_height*sizeof(uchar));
// 		searchTags(image_gray);
// 	}
// 	catch(const Ice::Exception &e)
// 	{
// 		std::cout << "Error reading from Camera" << e << std::endl;
// 	}
}


void SpecificWorker::sendData(const TData& data)
{
// 	QVec angles=QVec::zeros(3);
	RoboCompLegController::AnglesLeg angles;
	RoboCompLegController::PoseLeg pos1, pos2, pos3, pos4, pos5, pos6;
	pos1.vel=2;
	pos1.ref=base.toStdString();
	pos2.vel=2;
	pos2.ref=base.toStdString();
	pos3.vel=2;
	pos3.ref=base.toStdString();
	pos4.vel=2;
	pos4.ref=base.toStdString();
	pos5.vel=2;
	pos5.ref=base.toStdString();
	pos6.vel=2;
	pos6.ref=base.toStdString();
	angles.q1=0;
	angles.q2=0;
	angles.q3=0;
	float x=0,y=0,z=0;
	ButtonParams b=data.buttons.at(0);
	if(b.clicked)
	{
		modovalue+=1;
		if(modovalue==3)
			modovalue=0;
	}
	for(auto m:data.axes)
	{
		if(m.name=="x")
		{
			angles.q3=(m.value/65537);
			x=m.value/300;
		}
		if(m.name=="y")
		{
			angles.q2=(m.value/65537);
			y=m.value/300;
		}
		if(m.name=="z")
		{
			angles.q1=(m.value/65537);
			z=m.value/300;
		}
		if(m.name=="vel")
		{
			vel=mapear(-m.value,-65537,65537, 0.0,3.0);
		}
	}
	switch(modovalue)
	{
		case 0:
			angles.vel=vel;
			legcontroller1_proxy->setFKLeg(angles);
			legcontroller2_proxy->setFKLeg(angles);
			legcontroller3_proxy->setFKLeg(angles);
			legcontroller4_proxy->setFKLeg(angles);
			legcontroller5_proxy->setFKLeg(angles);
			legcontroller6_proxy->setFKLeg(angles);
			break;
		case 1:
			pos1.vel=vel;
			pos1.x=leg1.x()+x;
			pos1.y=leg1.y()+y;
			pos1.z=leg1.z()+z;
			
			pos2.vel=vel;
			pos2.x=leg2.x()+x;
			pos2.y=leg2.y()+y;
			pos2.z=leg2.z()+z;
			
			pos3.vel=vel;
			pos3.x=leg3.x()+x;
			pos3.y=leg3.y()+y;
			pos3.z=leg3.z()+z;
			
			pos4.vel=vel;
			pos4.x=leg4.x()+x;
			pos4.y=leg4.y()+y;
			pos4.z=leg4.z()+z;
			
			pos5.vel=vel;
			pos5.x=leg5.x()+x;
			pos5.y=leg5.y()+y;
			pos5.z=leg5.z()+z;
			
			pos6.vel=vel;
			pos6.x=leg6.x()+x;
			pos6.y=leg6.y()+y;
			pos6.z=leg6.z()+z;
			if(legcontroller1_proxy->setIKLeg(pos1,true))
				if(legcontroller2_proxy->setIKLeg(pos2,true))
					if(legcontroller3_proxy->setIKLeg(pos3,true))
						if(legcontroller4_proxy->setIKLeg(pos4,true))
							if(legcontroller5_proxy->setIKLeg(pos5,true))
								if(legcontroller6_proxy->setIKLeg(pos6,true))
								{
									legcontroller1_proxy->setIKLeg(pos1,false);
									legcontroller2_proxy->setIKLeg(pos2,false);
									legcontroller3_proxy->setIKLeg(pos3,false);
									legcontroller4_proxy->setIKLeg(pos4,false);
									legcontroller5_proxy->setIKLeg(pos5,false);
									legcontroller6_proxy->setIKLeg(pos6,false);
								}
			break;
		case 2:
			RoboCompLegController::PoseBody pb;
			pb.vel=vel;
			pb.rx=angles.q1;
			pb.ry=angles.q2;
			pb.rz=angles.q3;
			if(legcontroller1_proxy->setIKBody(pb,true))
				if(legcontroller2_proxy->setIKBody(pb,true))
					if(legcontroller3_proxy->setIKBody(pb,true))
						if(legcontroller4_proxy->setIKBody(pb,true))
							if (legcontroller5_proxy->setIKBody(pb,true))
								if(legcontroller6_proxy->setIKBody(pb,true))
								{
									legcontroller1_proxy->setIKBody(pb,false);
									legcontroller2_proxy->setIKBody(pb,false);
									legcontroller3_proxy->setIKBody(pb,false);
									legcontroller4_proxy->setIKBody(pb,false);
									legcontroller5_proxy->setIKBody(pb,false);
									legcontroller6_proxy->setIKBody(pb,false);
								}
			break;
	}
}

double SpecificWorker::mapear(double x, double in_min, double in_max, double out_min, double out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}





