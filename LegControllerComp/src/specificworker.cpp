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
		string name = PROGRAM_NAME;
		
		base=QString::fromStdString(params[name+".base"].value);
		floor=QString::fromStdString(params[name+".floor"].value);
		string s=params[name+".InnerModel"].value;
		inner = new InnerModel(params[name+".InnerModel"].value);
		
		motores<<QString::fromStdString(params[name+".m1"].value)<<QString::fromStdString(params[name+".m2"].value)<<QString::fromStdString(params[name+".m3"].value);
		foot=QString::fromStdString(params[name+".foot"].value);
		
		signleg=QString::fromStdString(params[name+".singleg"].value.data()).toInt();
		
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
		qDebug()<<"    base = "<<base;
		qDebug()<<"    floor = "<<floor;
		qDebug()<<"    m1 = "<<motores.at(0);
		qDebug()<<"    m2 = "<<motores.at(1);
		qDebug()<<"    m2 = "<<motores.at(2);
		qDebug()<<"-----------------------------";
		pos_foot =inner->transform(floor,foot);
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
	aux=inner->transform(base,foot);
	s.x=aux.x();
	s.y=aux.y();
	s.z=aux.z();
	s.ref=base.toStdString();
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

void SpecificWorker::setIKBody(const PoseBody &p)
{	
	
	//inicio rotar el cuerpo
	inner->updateRotationValues(base, p.rx, p.ry, p.rz,"");
	//fin rotar el cuerpo
	QVec pos=inner->transform(base,pos_foot,floor);
	PoseLeg pl;
	pl.ref=base.toStdString();
	pl.x=pos.x();
	pl.y=pos.y();
	pl.z=pos.z();
	pl.vel=p.vel;
	setIKLeg(pl);
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
	angles(1)=q2+0.22113;
	angles(2)=q3+0.578305;
	return angles;
}


void SpecificWorker::moverangles(QVec angles,float vel)
{
	RoboCompJointMotor::MotorGoalPositionList mg;
	RoboCompJointMotor::MotorGoalPosition p;
	float 	q1=angles(0),
			q2=angles(1)*signleg,
			q3=angles(2)*signleg;
	qDebug()<<"Leg: "<<foot<<"q1 = "<<q1<<"  q2 = "<<q2<<"  q3 = "<<q3;
	MotorState m=jointmotor_proxy->getMotorState(motores.at(0).toStdString());
	
	p.name=motores.at(0).toStdString();
	p.maxSpeed=fabs(q1-m.pos)*vel;
	p.position=q1;
	mg.push_back(p);
	
	jointmotor_proxy->getMotorState(motores.at(1).toStdString());
	p.name=motores.at(1).toStdString();
	p.maxSpeed=fabs(q2-m.pos)*vel;
	p.position=q2;
	mg.push_back(p);
	
	jointmotor_proxy->getMotorState(motores.at(2).toStdString());
	p.name=motores.at(2).toStdString();
	p.maxSpeed=fabs(q3-m.pos)*vel;
	p.position=q3;
	mg.push_back(p);
	jointmotor_proxy->setSyncPosition(mg);
}





