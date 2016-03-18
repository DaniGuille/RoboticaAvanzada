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
#include </home/ivan/robocomp/components/robocomp-robolab/components/apriltagsComp/src/JointMotor.h>

/**
* \brief Default constructor
*/
SpecificWorker::SpecificWorker(MapPrx& mprx) : GenericWorker(mprx)
{
	connect(ci, SIGNAL(clicked()), this, SLOT(moverpataci()));
	connect(cd, SIGNAL(clicked()), this, SLOT(moverpatacd()));
	
	connect(Xpos, SIGNAL(clicked()), this, SLOT(xpos()));
	connect(Xneg, SIGNAL(clicked()), this, SLOT(xneg()));
	connect(Ypos, SIGNAL(clicked()), this, SLOT(ypos()));
	connect(Yneg, SIGNAL(clicked()), this, SLOT(yneg()));
	connect(Zpos, SIGNAL(clicked()), this, SLOT(zpos()));
	connect(Zneg, SIGNAL(clicked()), this, SLOT(zneg()));
	
	connect(Actualizar, SIGNAL(clicked()), this, SLOT(actualizar()));
	
	
	inner = new InnerModel("/home/ivan/robocomp/files/innermodel/hexapod1pata.xml");
	Posini=inner->transform("base","axisA1T");
	Posfin=Posini;
	
	QVec aux=inner->transform("arm1motor2","arm1motor3");
	qDebug()<<aux;
	angle1=atan(aux.y()/aux.z());
	Femur=aux.norm2();
	
	aux=inner->transform("arm1motor3","axisA1T");
	angle2=atan(aux.y()/aux.z());
	Tibia=aux.norm2();
	qDebug()<<aux;
	
	Coxa=52;
	
	qDebug()<<"-----------------------------";
	qDebug()<<"    Coxa   = "<<Coxa;
	qDebug()<<"    Femur  = "<<Femur;
	qDebug()<<"    Tibia  = "<<Tibia;
	qDebug()<<"    angle1 = "<<angle1;
	qDebug()<<"    angle2 = "<<angle2;
	qDebug()<<"-----------------------------";

	
	
// 	Femur=sqrt(14.5*14.5+64.5*64.5);
// 	Tibia=sqrt(123.714*123.714+ 23.6179 *23.6179);
	H=18.5;
// 	Posini=QVec::zeros(3);
// 	RoboCompJointMotor::MotorParamsList mp = jointmotor_proxy->getAllMotorParams();
// 	RoboCompJointMotor::MotorGoalPositionList mg;
// 	for(auto m:mp){
// 		cout<<m.name<<endl;
// 		RoboCompJointMotor::MotorGoalPosition p;
// 		p.name=m.name;
// 		p.maxSpeed=0.1;
// 		p.position=0;
// 		mg.push_back(p);
// // 		mg.insert(p);
// 	}
// 	jointmotor_proxy->setSyncPosition(mg);
	
	m1="arm1motor1";
	m2="arm1motor2";
	m3="arm1motor3";
}

/**
* \brief Default destructor
*/
SpecificWorker::~SpecificWorker()
{
	
}

bool SpecificWorker::setParams(RoboCompCommonBehavior::ParameterList params)
{



	motores<<QString::fromStdString(m1)<<QString::fromStdString(m2)<<QString::fromStdString(m3);
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
// void SpecificWorker::sendData(const TData& data)
// {
// 	QVec angles=QVec::zeros(3);
// 	
// 	for(auto m:data.axes)
// 	{
// 		if(m.name=="x")
// 			angles(0)=(m.value/65537);
// 		if(m.name=="y")
// 			angles(1)=(m.value/65537);
// 		if(m.name=="z")
// 			angles(2)=(m.value/65537);
// 	}
// 	
// 	moverangles(angles);
// }

void SpecificWorker::moverpatacd()
{
	QVec angles=QVec::vec3((float)clavicula->value()/100,(float)hombro->value()/100,(float)codo->value()/100);
	moverangles(angles);
	
	bool moviendose=true;
	while(moviendose){
		moviendose=false;
		MotorState m = jointmotor_proxy->getMotorState(m1);
		if (m.isMoving) moviendose=true;
		else {
			m = jointmotor_proxy->getMotorState(m2);
			if (m.isMoving) moviendose=true;
				else {
					m = jointmotor_proxy->getMotorState(m3);
					if (m.isMoving) moviendose=true;
				}
		}
	}
	Posfin=inner->transform("base","axisA1T");
}
void SpecificWorker::moverpataci()
{	
	moverpataPunto(Posfin);
	
	bool moviendose=true;
	while(moviendose){
		moviendose=false;
		MotorState m = jointmotor_proxy->getMotorState(m1);
		if (m.isMoving) moviendose=true;
		else {
			m = jointmotor_proxy->getMotorState(m2);
			if (m.isMoving) moviendose=true;
				else {
					m = jointmotor_proxy->getMotorState(m3);
					if (m.isMoving) moviendose=true;
				}
		}
	}
	Posfin=inner->transform("base","axisA1T");
}


void SpecificWorker::moverpataPunto(QVec pfin)
{
	QVec angles=QVec::zeros(3);
	QVec p=inner->transform("arm1motor1T",pfin,"base");
	
	float q1=atan(p.x()/p.z());
	
	float x2=p.x()*p.x(),y2=p.y()*p.y(),z2=p.z()*p.z(),Femur2=Femur*Femur,Tibia2=Tibia*Tibia,
		r=sqrt(x2+z2)-Coxa,
		cosq3=((r*r)+y2-Tibia2-Femur2)/(2*Tibia*Femur),
		senq3=-sqrt(1-(cosq3*cosq3));
	qDebug()<<"cos (q3) = "<<r*r<<"+"<<y2<<"-"<<Tibia2<<"-"<<Femur2<<")/(2*"<<Tibia<<"*"<<Femur<<")"<<" = "<<cosq3;
	qDebug()<<"sen (q3) = "<<senq3;
	
	float q3=atan(senq3/cosq3);
	
	
	float q2=atan(p.y()/r)-atan((Tibia*senq3)/(Femur+(Tibia*cosq3)));
	
	
	angles(0)=q1;
	angles(1)=q2+0.22113-0.5;
	angles(2)=q3+0.578305+0.8;
	
	moverangles(angles);
}

void SpecificWorker::moverangles(QVec angles)
{
	RoboCompJointMotor::MotorGoalPositionList mg;
	RoboCompJointMotor::MotorGoalPosition p;
	float 	q1=angles(0),
			q2=angles(1)*-1,
			q3=angles(2)*-1;
	qDebug()<<"q1 = "<<q1<<"  q2 = "<<q2<<"  q3 = "<<q3;
	MotorState m=jointmotor_proxy->getMotorState(m1);
	
	p.name=m1;
	p.maxSpeed=fabs(q1-m.pos);
	p.position=q1;
	mg.push_back(p);
	
	jointmotor_proxy->getMotorState(m2);
	p.name=m2;
	p.maxSpeed=fabs(q2-m.pos);
	p.position=q2;
	mg.push_back(p);
	
	jointmotor_proxy->getMotorState(m3);
	p.name=m3;
	p.maxSpeed=fabs(q3-m.pos);
	p.position=q3;
	mg.push_back(p);
	jointmotor_proxy->setSyncPosition(mg);
}


void SpecificWorker::xpos()
{
	Posfin(0)+=5;
	string m=to_string(Posfin.x())+" "+to_string(Posfin.y())+" "+to_string(Posfin.z());
	pl->setText(QString::fromStdString(m));
}
void SpecificWorker::xneg()
{
	Posfin(0)-=5;
	string m=to_string(Posfin.x())+" "+to_string(Posfin.y())+" "+to_string(Posfin.z());
	pl->setText(QString::fromStdString(m));
}
void SpecificWorker::ypos()
{
	Posfin(1)+=5;
	string m=to_string(Posfin.x())+" "+to_string(Posfin.y())+" "+to_string(Posfin.z());
	pl->setText(QString::fromStdString(m));
}
void SpecificWorker::yneg()
{
	Posfin(1)-=5;
	string m=to_string(Posfin.x())+" "+to_string(Posfin.y())+" "+to_string(Posfin.z());
	pl->setText(QString::fromStdString(m));
}
void SpecificWorker::zpos()
{
	Posfin(2)+=5;
	string m=to_string(Posfin.x())+" "+to_string(Posfin.y())+" "+to_string(Posfin.z());
	pl->setText(QString::fromStdString(m));
}
void SpecificWorker::zneg()
{
	Posfin(2)-=5;
	string m=to_string(Posfin.x())+" "+to_string(Posfin.y())+" "+to_string(Posfin.z());
	pl->setText(QString::fromStdString(m));
}

void SpecificWorker::actualizar()
{
	MotorState m=jointmotor_proxy->getMotorState(m1);
	clavicula->setValue(m.pos);
	jointmotor_proxy->getMotorState(m2);
	hombro->setValue(m.pos);
	jointmotor_proxy->getMotorState(m3);
	codo->setValue(m.pos);
	
	Posfin=inner->transform("base","axisA1T");
	string st=to_string(Posfin.x())+" "+to_string(Posfin.y())+" "+to_string(Posfin.z());
	pl->setText(QString::fromStdString(st));
	
}
