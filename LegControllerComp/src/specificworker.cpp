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
#include <qt4/Qt/qdebug.h>

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
	}
	catch(std::exception e)
	{
		qFatal("Error reading config params");
	}
	try{
		moverangles(QVec::vec3(0.,0.35,-0.7),1);
	}
	catch(std::exception e)
	{
	    qDebug()<<"no he llegado";
	}
	for(auto name:motores)
	{
		motorsparams[name.toStdString()]=jointmotor2_proxy->getMotorParams(name.toStdString());
// 		statemap.insert(std::pair<string,RoboCompJointMotor::MotorState>(name.toStdString()));
	}
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
			statemap[m.toStdString()]=jointmotor2_proxy->getMotorState(m.toStdString());//robot
			inner->updateJointValue(m,statemap[m.toStdString()].pos);
		}
	}
	catch(const Ice::Exception &ex)
	{
		  std::cout << ex << std::endl;
	}	
}

bool SpecificWorker::setListIKLeg(const ListPoseLeg &ps, const bool &simu)
{
	bool exito=true;
	for(auto p:ps)
	{
		while(getStateLeg().ismoving){}
		exito=setIKLeg(p,simu);
		if (!exito)
			break;
	}
	return exito;
}

StateLeg SpecificWorker::getStateLeg()
{
	StateLeg s;
	s.ismoving=false;
	QVec aux=QVec::zeros(3);
	int i=0;
	MotorState ms;
	foreach(QString m, motores)
	{
		try
		{
			ms=jointmotor2_proxy->getMotorState(m.toStdString());
			if(ms.isMoving)
				s.ismoving=true;
			aux(i)=ms.pos;
		}
		catch(const Ice::Exception &ex)
		{
			std::cout << ex << std::endl;
			RoboCompLegController::HardwareFailedException e;
			e.what="Hardware Failed" +m.toStdString();
			//send e
		}
		i++;
	}
	s.q1=aux(0);
	s.q2=aux(1);
	s.q3=aux(2);
	aux=inner->transform(base,foot);
	s.x=aux.x();
	s.y=aux.y();
	s.z=aux.z();
	s.ref=base.toStdString();
	return s;
}

bool SpecificWorker::setIKLeg(const PoseLeg &p, const bool &simu)
{
	bool exito;
	try
	{
		QVec posfoot=inner->transform(motores.at(0),QVec::vec3(p.x,p.y,p.z),QString::fromStdString(p.ref));
		qDebug()<<p.x<<p.y<<p.z;
		QVec angles=movFoottoPoint(posfoot, exito);
		if(!simu&&exito)
			moverangles(angles, p.vel);
		if(!exito)
		{
			RoboCompLegController::ImpossiblePositionException e;
			e.what="Impossible Position";
			//send e
		}
		return exito;
	}
	catch(const Ice::Exception &ex)
	{
		std::cout << ex << std::endl;
		RoboCompLegController::HardwareFailedException e;
		e.what="Hardware Failed";
		//send e
		return exito;
	}
}

bool SpecificWorker::setIKBody(const PoseBody &p, const bool &simu)
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
	return setIKLeg(pl,simu);
}

bool SpecificWorker::setFKLeg(const AnglesLeg &al, const bool &simu)
{	
	bool exito=false;
	try
	{
		QVec angles=QVec::vec3(al.q1,al.q2,al.q3);
		double max=M_PI/2, min=- M_PI/2;
		if((min<al.q1&&al.q1<max)&&(min<al.q2&&al.q2<max)&&(min<al.q3&&al.q3<max))
			exito=true;
		if(exito&&!simu)
			moverangles(angles, al.vel);
		return exito;
	}
	catch(const Ice::Exception &ex)
	{
		std::cout << ex << std::endl;
		RoboCompLegController::HardwareFailedException e;
		e.what="Hardware Failed";
		//send e
		return exito;
	}
}

QVec SpecificWorker::movFoottoPoint(QVec p, bool &exito)
{
	QVec angles=QVec::zeros(3);

	double q1, q2, q3,x=p.x(), y=p.y(), z=p.z(),
		r=abs(sqrt(pow(x,2)+pow(z,2))-coxa),
		cosq3=(pow(r,2)+pow(y,2)-pow(tibia,2)-pow(femur,2))/(2*tibia*femur);
	if(cosq3>1)
		cosq3=1;
	else if(cosq3<-1)
		cosq3=-1;
	double senq3=-sqrt(1-pow(cosq3,2));
	if(senq3>1)
		senq3=1;
	else if(senq3<-1)
		senq3=-1;
	double L=sqrt(pow(y,2)+pow(r,2));
	if(L<tibia+femur /*&&( x>0 || z>0)*/){
		q1=atan2(x,z);
		q3=atan2(senq3,cosq3);
		q2=atan2(y,r)-atan2((tibia*senq3),(femur+(tibia*cosq3)));
// 		if(q1<motorsparams[motores.at(0).toStdString()].maxPos && q1>motorsparams[motores.at(0).toStdString()].minPos &&
// 		   q2<motorsparams[motores.at(1).toStdString()].maxPos && q2>motorsparams[motores.at(2).toStdString()].minPos &&
// 		   q3<motorsparams[motores.at(2).toStdString()].maxPos && q3>motorsparams[motores.at(2).toStdString()].minPos)
// 		{
		q2 += 0.22113;
		q3 += 0.578305;
		double max=M_PI/2+0.15, min=- M_PI/2-0.15;
		if((min<q1&&q1<max)&&(min<q2&&q2<max)&&(min<q3&&q3<max)){
			exito=true;
		}
		else
		{
			qDebug()<<"Imposible llegar: angulos sobrepasados.";
			exito=false;
		}
	}
	else
	{
		qDebug()<<"Imposible llegar: punto demasiado lejano.";
		exito=false;
	}
	
	angles(0)=q1+statemap[motores.at(0).toStdString()].pos/*+motorsparams[motores.at(0).toStdString()].offset*/;
	angles(1)=q2/*+motorsparams[motores.at(1).toStdString()].offset*/;
	angles(2)=q3/*+motorsparams[motores.at(2).toStdString()].offset*/;
	return angles;
}

void SpecificWorker::moverangles(QVec angles,double vel)
{
	if(!isnan(angles(0))&&!isnan(angles(1))&&!isnan(angles(2)))
	{
		RoboCompJointMotor::MotorGoalPositionList mg;
		RoboCompJointMotor::MotorGoalPosition p;
		RoboCompJointMotor::MotorGoalVelocityList mv;
		RoboCompJointMotor::MotorGoalVelocity v;
		double 	q1=angles(0),
				q2=angles(1) *signleg,
				q3=angles(2) *signleg;
		qDebug()<<"Leg: "<<foot<<"q1 = "<<q1<<"  q2 = "<<q2<<"  q3 = "<<q3;
		MotorState m=jointmotor2_proxy->getMotorState(motores.at(0).toStdString());
		v.name = p.name = motores.at(0).toStdString();
		v.velocity = p.maxSpeed=fabs(q1-m.pos)*vel;
		p.position=q1;
		mg.push_back(p);
		mv.push_back(v);
		
		m=jointmotor2_proxy->getMotorState(motores.at(1).toStdString());
		v.name = p.name=motores.at(1).toStdString();
		v.velocity = p.maxSpeed=fabs(q2-m.pos)*vel;
		p.position=q2;
		mg.push_back(p);
		mv.push_back(v);
		
		m=jointmotor2_proxy->getMotorState(motores.at(2).toStdString());
		v.name = p.name=motores.at(2).toStdString();
		v.velocity = p.maxSpeed=fabs(q3-m.pos)*vel;
		p.position=q3;
		mg.push_back(p);
		mv.push_back(v);
		
		jointmotor2_proxy->setSyncPosition(mg);
	}
	else
		qDebug()<< "Posicion no alcanzada";

}



