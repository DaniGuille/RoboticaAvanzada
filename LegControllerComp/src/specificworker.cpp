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
#include </home/ivan/robocomp/components/g1/GoToPoint/src/specificworker.h>
#include <qt4/Qt/qvarlengtharray.h>

/**
* \brief Default constructor
*/
SpecificWorker::SpecificWorker(MapPrx& mprx) : GenericWorker(mprx)
{
	
}

/**
* \brief Default destructor
*/
SpecificWorker::~SpecificWorker()
{
	
}

bool SpecificWorker::setParams(RoboCompCommonBehavior::ParameterList params)
{
	try
	{
			string s=params["InnerModel"].value;
			qDebug()<<"    InnerModel ="<<QString::fromStdString(s);
			inner = new InnerModel(s);
			motores<<QString::fromStdString(params["m1"].value)<<QString::fromStdString(params["m2"].value)<<QString::fromStdString(params["m3"].value);
			foot=QString::fromStdString(params["foot"].value);
			signleg=atof(params["signleg"].value.c_str());
			QVec aux=inner->transform(motores.at(1),motores.at(0));
			coxa=aux.norm2();
			
			aux=inner->transform(motores.at(2),motores.at(1));
			femur=aux.norm2();
			
			aux=inner->transform(foot,motores.at(2));
			tibia=aux.norm2();
			
			qDebug()<<"-----------------------------";
			qDebug()<<"    InnerModel ="<<QString::fromStdString(s);
			qDebug()<<"    coxa   = "<<coxa;
			qDebug()<<"    femur  = "<<femur;
			qDebug()<<"    tibia  = "<<tibia;
			qDebug()<<"    signleg = "<<signleg;
			qDebug()<<"    foot = "<<foot;
			qDebug()<<"    m1 = "<<motores.at(0);
			qDebug()<<"    m2 = "<<motores.at(1);
			qDebug()<<"    m2 = "<<motores.at(2);
			qDebug()<<"-----------------------------";
		}
		catch(std::exception e)
		{
			qFatal("Error reading config params");
		}
			
	timer.start(Period);

	return true;
}

void SpecificWorker::compute()
{
	try{
		foreach(QString m, motores)
		{
			MotorState ms=jointmotor_proxy->getMotorState(m.toStdString());
			inner->updateJointValue(m,ms.pos);
		}
	}
	catch(const Ice::Exception &ex)
	{
		  std::cout << ex << std::endl;
	}	
}


StateLeg SpecificWorker::getStateLeg()
{
	StateLeg s;
	QVec aux=QVec::zeros(3);
	int i=0;
	MotorState ms;
	foreach(QString m, motores)
	{
		try
		{
			ms=jointmotor_proxy->getMotorState(m.toStdString());
			if(ms.isMoving)
				s.ismoving=true;
			aux(i)=ms.pos;
		}
		catch(const Ice::Exception &ex)
		{
			std::cout << ex << std::endl;
		}
		i++;
	}
	s.posclavicula=aux(0);
	s.poshombro=aux(1);
	s.poscodo=aux(2);
	aux=inner->transform("base",foot);
	s.x=aux.x();
	s.y=aux.y();
	s.z=aux.z();
	s.ref="base";
	return s;
}

void SpecificWorker::setIKLeg(const PoseLeg &p)
{
	try
	{
	QVec posfoot=inner->transform(motores.at(0),QVec::vec3(p.x,p.y,p.z),QString::fromStdString(p.ref));
	QVec angles=movFoottoPoint(posfoot);
	moverangles(angles, p.vel);
	}
	catch(const Ice::Exception &ex)
	{
		std::cout << ex << std::endl;
	}
}

void SpecificWorker::setFKLeg(const AnglesLeg &al)
{	
	try
	{
	QVec angles=QVec::vec3(al.q1,al.q2,al.q3);
	moverangles(angles, al.vel);
	}
	catch(const Ice::Exception &ex)
	{
		std::cout << ex << std::endl;
	}
}




QVec SpecificWorker::movFoottoPoint(QVec p)
{
	QVec angles=QVec::zeros(3);
	
	float q1=atan(p.x()/p.z());
	
	float x2=p.x()*p.x(),y2=p.y()*p.y(),z2=p.z()*p.z(),Femur2=femur*femur,Tibia2=tibia*tibia,
		r=sqrt(x2+z2)-coxa,
		cosq3=((r*r)+y2-Tibia2-Femur2)/(2*tibia*femur),
		senq3=-sqrt(1-(cosq3*cosq3));
// 	qDebug()<<"cos (q3) = "<<r*r<<"+"<<y2<<"-"<<Tibia2<<"-"<<Femur2<<")/(2*"<<tibia<<"*"<<femur<<")"<<" = "<<cosq3;
// 	qDebug()<<"sen (q3) = "<<senq3;
// 	
	float q3=atan(senq3/cosq3);
	
	
	float q2=atan(p.y()/r)-atan((tibia*senq3)/(femur+(tibia*cosq3)));
	
	
	angles(0)=q1;
	angles(1)=(q2+0.22113)*signleg;
	angles(2)=(q3+0.578305)*signleg;
	return angles;
}


void SpecificWorker::moverangles(QVec angles,float vel)
{
	RoboCompJointMotor::MotorGoalPositionList mg;
	RoboCompJointMotor::MotorGoalPosition p;
	float 	q1=angles(0),
			q2=angles(1),
			q3=angles(2);
	qDebug()<<"q1 = "<<q1<<"  q2 = "<<q2<<"  q3 = "<<q3;
	MotorState m=jointmotor_proxy->getMotorState(motores.at(0).toStdString());
	
	p.name=motores.at(0).toStdString();
	p.maxSpeed=fabs(q1-m.pos);
	p.position=q1;
	mg.push_back(p);
	
	jointmotor_proxy->getMotorState(motores.at(1).toStdString());
	p.name=motores.at(1).toStdString();
	p.maxSpeed=fabs(q2-m.pos);
	p.position=q2;
	mg.push_back(p);
	
	jointmotor_proxy->getMotorState(motores.at(2).toStdString());
	p.name=motores.at(2).toStdString();
	p.maxSpeed=fabs(q3-m.pos);
	p.position=q3;
	mg.push_back(p);
	jointmotor_proxy->setSyncPosition(mg);
}





