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
		pos_foot =inner->transform(floor,foot);
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
		qDebug()<<"    posfoot = "<<pos_foot; 
		qDebug()<<"-----------------------------";
		moverangles(QVec::vec3(0.,0.3,-0.6),1);
// 		moverangles(QVec::vec3(0.,0.22113,0.578305),2);
	}
	catch(std::exception e)
	{
		qFatal("Error reading config params");
	}
	for(auto name:motores)
		motorsparams[name.toStdString()]=jointmotor_proxy->getMotorParams(name.toStdString());
	for(auto name:motores)
		qDebug()<<motorsparams[name.toStdString()].offset;
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

void SpecificWorker::setListIKLeg(const ListPoseLeg &ps)
{
	for(auto p:ps)
	{
		while(getStateLeg().ismoving){}
		setIKLeg(p);
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
	StateLeg s=getStateLeg();
	double  q1=s.posclavicula,
			q2=s.poshombro,
			q3=s.posclavicula;
// 	double x=p.x(), y=p.y(), z=p.z();
// 	
// 	double q1=atan(x/z);
// 	
// 	double r=sqrt(pow(x,2)+pow(z,2))-coxa,
// 		cosq3=(pow(r,2)+pow(y,2)-pow(tibia,2)-pow(femur,2))/(2*tibia*femur),	//comprobar con el 2 negativo(-2*tibia*femur)
// 		senq3=-sqrt(1-pow(cosq3,2));											//comprobar que sea positivo
// 	double q3=atan(senq3/cosq3);												//comprobar que cosq3 no sea 0
// 	double q2=atan(y/r)-atan((tibia*senq3)/(femur+(tibia*cosq3)));				//comprobar que r y (femur+(tibia*cosq3) no sea 0
// 	qDebug()<<sqrt(pow(x,2)+pow(z,2))<<"<="<<cos(q2)*femur+coxa;
// 	if(sqrt(pow(x,2)+pow(z,2))<=cos(q2)*femur+coxa+29.679){
// 		q3=-q3;
// 		q2=atan(y/r)-atan((tibia*sin(q3))/(femur+(tibia*cos(q3))));
// 	}
	
//-------------------------------Prueba---------------------------------------------
	double x=p.x(), y=p.y(), z=p.z(),
		r=abs(sqrt(pow(x,2)+pow(z,2))-coxa),
		cosq3=(pow(r,2)+pow(y,2)-pow(tibia,2)-pow(femur,2))/(2*tibia*femur),
		senq3=-sqrt(1-pow(cosq3,2));
	if(r<tibia+femur && r>fabs(tibia-femur)){
		if(z!=0)
			q1=atan2(x,z);
		else
			q1=0;
		if(cosq3!=0)
			q3=atan2(senq3,cosq3);
		else
			q3=0;
		if((femur+(tibia*cosq3))!=0)
			q2=atan2(y,r)-atan2((tibia*senq3),(femur+(tibia*cosq3)));
		else
			q2=0;
	}
	else
		qDebug()<<"Posicion imposible";
	
//----------------------------------------------------------------------------
	/*
	KDL::Chain chain;
															 //a        alfa          d      theta
	chain.addSegment(KDL::Segment(KDL::Joint(KDL::Joint::RotY),KDL::Frame(KDL::Vector(0.0,0.0,1))));
	chain.addSegment(KDL::Segment(KDL::Joint(KDL::Joint::RotX),KDL::Frame(KDL::Vector(0.0,0.0,1))));
	chain.addSegment(KDL::Segment(KDL::Joint(KDL::Joint::RotX),KDL::Frame(KDL::Vector(0.0,0.0,1))));

	KDL::ChainFkSolverPos_recursive fksolver(chain);//objeto para calcular la cinemática directa del robot
	KDL::ChainIkSolverVel_pinv iksolver1v(chain);//objeto para calcular la cinemática inversa
	KDL::ChainIkSolverPos_NR iksolver1(chain,fksolver,iksolver1v,100,1e-6);//como máximo 100 iteraciones, parar si el error es menor que 1e-6
	
	KDL::JntArray q(chain.getNrOfJoints());
	KDL::JntArray q_init(chain.getNrOfJoints());	
	
	KDL::Frame F_dest;
	F_dest.p= KDL::Vector(p.x(),p.y(),p.z());
	F_dest.M= KDL::Rotation(1,0,0,
					   0,1,0,
					   0,0,1);
	
	int ret = iksolver1.CartToJnt(q_init,F_dest,q);*/
	
	// 1.43311   q3 =  -1.15668
	
	angles(0)=q1/*+motorsparams[motores.at(0).toStdString()].offset*/;
	angles(1)=q2+0.22113/*+motorsparams[motores.at(1).toStdString()].offset*/;
	angles(2)=q3+0.578305/*+motorsparams[motores.at(2).toStdString()].offset*/;
	return angles;
}


void SpecificWorker::moverangles(QVec angles,double vel)
{
	if(!isnan(angles(0))&&!isnan(angles(1))&&!isnan(angles(2)))
	{
		RoboCompJointMotor::MotorGoalPositionList mg;
		RoboCompJointMotor::MotorGoalPosition p;
		double 	q1=angles(0),
				q2=angles(1) *signleg,
				q3=angles(2) *signleg;
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
	else
		qDebug()<< "Posicion no alcanzada";

}



