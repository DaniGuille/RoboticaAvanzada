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
#include <qt4/Qt/qlocale.h>
#define tbezier 0.2
/**
* \brief Default constructor
*/
SpecificWorker::SpecificWorker(MapPrx& mprx) : GenericWorker(mprx)
{
	syn=0;
	IK=false;
	modovalue=0;
	vel=2;
	proxies[0]=legcontroller1_proxy;
	proxies[1]=legcontroller2_proxy;
	proxies[2]=legcontroller3_proxy;
	proxies[3]=legcontroller4_proxy;
	proxies[4]=legcontroller5_proxy;
	proxies[5]=legcontroller6_proxy;
	
	lini=QVec::vec3(0,0,0);
	lfin=QVec::vec3(0,0,0);
	lmed=QVec::vec3(0,70,0);
	l1[0]=0;
	l1[1]=3;
	l1[2]=4;
	l2[0]=1;
	l2[1]=2;
	l2[2]=5;
	x=0;
	y=0;
	z=0;
	X=0;
	Y=0;
	Z=0;
	X_pre=0;
	Y_pre=0;
	Z_pre=0;
	angles.q1=0;
	angles.q2=0;
	angles.q3=0;
	//Innerviewer
	osgView = new OsgView(this->frame);
	show();
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
	
	
	innerViewer = new InnerModelViewer(inner, "root", osgView->getRootGroup(), false);

	
	for(int i=1;i<=6;i++)
	{
		legs<<QString::fromStdString(params[name+".nameleg"+to_string(i)].value);
	}
	qDebug()<<"-----------------------legs----------------";
	qDebug()<<base;
	qDebug()<<legs;
	qDebug()<<"-----------------------legs----------------";
	
	for(int i=0;i<6;i++){
		statelegs[i] = proxies[i]->getStateLeg();
		legsp[i]=QVec::vec3(statelegs[i].x,statelegs[i].y,statelegs[i].z);
	}
	for(auto p:legsp)
		qDebug()<<p;
	
	timer.start(10);
	
	return true;
}

void SpecificWorker::compute()
{	
	syn+=3;
	switch(modovalue)
	{
		case -1:
			uphexapod();
			break;
		case 0:
			modo->setText("FK");
			if(X_pre!=X || Y_pre!=Y || Z_pre!=Z)
			{
				fkLegs();
				X_pre=X;
				Y_pre=Y;
				Z_pre=Z;
				qDebug()<<"entro";
			}
			break;
		case 1:
			modo->setText("IK Leg");
			if(X_pre!=X || Y_pre!=Y || Z_pre!=Z)
			{
				ikLegs();
			
				X_pre=X;
				Y_pre=Y;
				Z_pre=Z;
			}
			break;
		case 2:
			modo->setText("IK Body");
			if(X_pre!=X || Y_pre!=Y || Z_pre!=Z)
			{
				ikBody();
				X_pre=X;
				Y_pre=Y;
				Z_pre=Z;
			}
			break;
		case 3:
			modo->setText("Caminar3x3");
// 			if(X_pre!=X || Z_pre!=Z)
// 			{
				if(caminar3x3())
				{
					lini=QVec::vec3(-X,0,-Z);
					lfin=QVec::vec3(X,0,Z);
				}
// 				X_pre=X;
// 				Y_pre=Y;
// 				Z_pre=Z;
// 			}
			break;
		case 4:
			modo->setText("Rotar");
// 			if(Y_pre!=Y)
// 			{
				if(rotar())
					lrot=QVec::vec3(0,0,Y);
// 				X_pre=X;
// 				Y_pre=Y;
// 				Z_pre=Z;
// 			}
			break;
	}
	for(int i=0;i<6;i++)
		statelegs[i] = proxies[i]->getStateLeg();
	for(int i=0;i<6;i++)
	{
		RoboCompLegController::StateLeg s=statelegs[i];
		inner->updateJointValue(QString::fromStdString(s.q1.name),s.q1.pos);
		inner->updateJointValue(QString::fromStdString(s.q2.name),s.q2.pos);
		inner->updateJointValue(QString::fromStdString(s.q3.name),s.q3.pos);
	}
	innerViewer->update();
	osgView->autoResize();
	osgView->frame();
}

void SpecificWorker::caminarDespacio()
{
	static float i=0;
	static int j=0;
	static bool mcuerpo=false;
	if(!mcuerpo)
	{
		if(!statelegs[j].ismoving&&i<1)
		{
			QVec ini = legsp[j]+lini,fin = legsp[j]+lfin;
			QVec tmp=bezier3(ini,legsp[j]+lmed,fin,i);
			RoboCompLegController::PoseLeg p;
			p.x=tmp.x();
			p.y=tmp.y();
			p.z=tmp.z();
			p.ref=base.toStdString();
			p.vel=6;
			proxies[j]->setIKLeg(p,false);
			i+=.1;
// 			qDebug()<<i;
		}
		else
		{
			j++;
			i=0;

			if(j>=6)
			{
				j=0;
				mcuerpo=true;
			}
		}
	}
	else
	{
		for(int k=0;k<6;k++)
		{
			QVec ini = legsp[k]+lfin,fin = legsp[k]+lini;
			QVec tmp=bezier2(ini,fin,i);
			RoboCompLegController::PoseLeg p;
			p.x=tmp.x();
			p.y=tmp.y();
			p.z=tmp.z();
			p.ref=base.toStdString();
			p.vel=6;
			proxies[k]->setIKLeg(p,false);
		}
		i+=.1;
// 		qDebug()<<i;
		if (i>1)
		{
			i=0;
			mcuerpo=false;
		}
	}
	
}

bool SpecificWorker::caminar3x3()
{
	static float i=0;
	bool ismoving=false;
	if(i==0)
		for(int k=0;k<6;k++)
			if(proxies[k]->getStateLeg().ismoving)
			{
				ismoving=true;
				break;
			}
	if(!ismoving)	
	{
		if(lini!=QVec::vec3(0, 0, 0)&&lfin!=QVec::vec3(0,0,0))
		{
			//patas por arco
			for(int s=0;s<3;s++)
			{
				RoboCompLegController::StateLeg st=statelegs[l1[s]];
				QVec posini =QVec::vec3(st.x,legsp[l1[s]].y(),st.z);
				QVec ini = posini,fin = legsp[l1[s]]+lfin,med=legsp[l1[s]];
				QVec tmp=bezier3(ini,QVec::vec3(med.x(),0,med.z()),fin,i);
				RoboCompLegController::PoseLeg p;
				p.x=tmp.x();
				p.y=tmp.y();
				p.z=tmp.z();
				p.ref=base.toStdString();
				p.vel=6;
				proxies[l1[s]]->setIKLeg(p,false);
				
			}
		// patas por tierra
			for(int s=0;s<3;s++)
			{
				RoboCompLegController::StateLeg st=statelegs[l2[s]];
				QVec posini =QVec::vec3(st.x,legsp[l2[s]].y(),st.z);
				QVec ini = posini,fin = legsp[l2[s]]+lini;
				QVec tmp=bezier2(ini,fin,i);
				RoboCompLegController::PoseLeg p;
				p.x=tmp.x();
				p.y=tmp.y();
				p.z=tmp.z();
				p.ref=base.toStdString();
				p.vel=6;
				proxies[l2[s]]->setIKLeg(p,false);
				
			}
			i+=tbezier;
			if (i>1)
			{
				int aux[]={l1[0],l1[1],l1[2]};
				l1[0]=l2[0];
				l1[1]=l2[1];
				l1[2]=l2[2];
				l2[0]=aux[0];
				l2[1]=aux[1];
				l2[2]=aux[2];
				i=0;
				return true;
			}
			return false;
		}
	}
	return true;
}

bool SpecificWorker::rotar()
{
	static bool estado=true;
	if(lrot!=QVec::vec3(0, 0, 0))
	{
		static float i=0;
		bool ismoving=false;
		for(int k=0;k<6;k++)
			if(proxies[k]->getStateLeg().ismoving)
			{
				ismoving=true;
				break;
			}
		if(!ismoving)
		{
			QVec ini,fin;
			for(int j=0;j<6;j++)
			{
				RoboCompLegController::StateLeg st=statelegs[j];
				QVec posini =QVec::vec3(st.x,legsp[j].y(),st.z);
				if((j==2 || j==3))
				{
					ini = posini;
					if(estado)
						fin = legsp[j]+lrot;
					else
						fin = legsp[j]-lrot;
				}
				else
				{
					ini = posini;
					if(estado)
						fin = legsp[j]-lrot;
					else
						fin = legsp[j]+lrot;
				}
				QVec tmp;
				if(estado)
					if(j==0||j==3||j==4)
						tmp=bezier3(ini,bezier2(ini,fin,0.5)+lmed,fin,i);
					else
						tmp = bezier2(ini,fin,i);
				else
					if(j==0||j==3||j==4)
						tmp = bezier2(ini,fin,i);
					else
						tmp=bezier3(ini,bezier2(ini,fin,0.5)+lmed,fin,i);
				
				RoboCompLegController::PoseLeg p;
				p.x   = tmp.x();
				p.y   = tmp.y();
				p.z   = tmp.z();
				p.ref = base.toStdString();
				p.vel = 6;
				proxies[j]->setIKLeg(p,false);
			}
			i+=tbezier;
			if (i>1)
			{
				i=0;
				estado=!estado;
				return true;
			}
		}
		return false;
	}
	else
		return true;
}

void SpecificWorker::sendData(const TData& data)
{
	ButtonParams b=data.buttons.at(0);
	if(b.clicked)
	{
		if(modovalue++==5)
			modovalue=0;
	}
	b=data.buttons.at(1);
	if(b.clicked)
	{	
// 		RoboCompLegController::AnglesLeg a;
// 		a.q1=0;
// 		a.q2=0.3;
// 		a.q3=0.6;
// 		a.vel=6;
// 		for (int i=0;i<6;i++){
// 			proxies[i]->setFKLeg(a,false);
// 		}
		if(modovalue!=-1)
			modoaux=modovalue;
// 		sleep(2);
		modovalue=-1;
	}
	b=data.buttons.at(2);
	if(b.clicked)
	{
		for(int i=0;i<6;i++)
		{
			statelegs[i] = proxies[i]->getStateLeg();
			legsp[i]=QVec::vec3(statelegs[i].x,statelegs[i].y,statelegs[i].z);
		}
	}
	else
	{
		for(auto m:data.axes)
		{
			if(m.name=="x")
			{
				angles.q3=(m.value/35537);
				x=m.value/300;
				X_pre=X;
				X=mapear(m.value,-65537,65537, -39,39);
			}
			if(m.name=="y")
			{
				angles.q2=(m.value/35537);
				y=m.value/300;
				Y_pre=Y;
				Y=mapear(m.value,-65537,65537, 47,-47);
			}
			if(m.name=="z")
			{
				angles.q1=(m.value/35537);
				z=m.value/300;
				Z_pre=Z;
				Z=mapear(m.value,65537,-65537, -47,47);
			}
			if(m.name=="vel")
				vel=mapear(-m.value,-65537,65537, 0.0,6.0);
		}
	}
}

void SpecificWorker::ikLegs()
{
	RoboCompLegController::PoseLeg pos[6];
	bool simufallida=true;
	for(int i=0;i<6;i++)
	{
		pos[i].ref=base.toStdString();
		pos[i].vel=vel;
		pos[i].x=legsp[i].x()+x;
		pos[i].y=legsp[i].y()+y;
		pos[i].z=legsp[i].z()+z;
		
	}
	for(int i=0;i<6;i++)
	{	
		RoboCompLegController::PoseLeg p= pos[i];
		if(!proxies[i]->setIKLeg(pos[i],true))
		{
			simufallida=false;
			break;
		}
	}
	if(simufallida)
		for(int i=0;i<6;i++)
			proxies[i]->setIKLeg(pos[i],false);
}

void SpecificWorker::fkLegs()
{
	angles.vel=vel;
	for(int i=0;i<6;i++)
		proxies[i]->setFKLeg(angles, false);
}

void SpecificWorker::ikBody()
{
	RoboCompLegController::PoseBody pb[6];
	bool simufallida =false;
	for(int i=0;i<6;i++)
	{
		pb[i].vel = vel;
		pb[i].ref = base.toStdString();
		pb[i].rx = angles.q1/1.5;
		pb[i].ry = angles.q2/1.5;
		pb[i].rz = angles.q3/1.5;
		pb[i].px=legsp[i].x();
		pb[i].py=legsp[i].y();
		pb[i].pz=legsp[i].z();
		pb[i].x = 0;
		pb[i].y = 0;
		pb[i].z = 0;
	}
	
	for(int i=0;i<6;i++)
	{
		if(!proxies[i]->setIKBody(pb[i],true))
		{
			simufallida=true;
			break;
		}
	}
	if(!simufallida)
		for(int i=0;i<6;i++)
			proxies[i]->setIKBody(pb[i],false);
}

/* uphexapod
 * bool ismov=false;
	static bool first=true,state=true;
	static float i;
	for(int s=0;s<6;s++)
		if(proxies[s]->getStateLeg().ismoving)
		{
			ismov=true;
			break;
		}
	if(!ismov){
		if(state)
		{
			if(first)
			{
				for(int i=0;i<6;i++)
					statelegs[i] = proxies[i]->getStateLeg();
				first=false;
			}
			for(int s=0;s<6;s++)
			{
				RoboCompLegController::StateLeg st=statelegs[s];
				QVec fin = QVec::vec3(legsp[s].x(),20,legsp[s].z()),ini = QVec::vec3(st.x,st.y,st.z);
				QVec tmp=bezier2(ini,fin,i);
				RoboCompLegController::PoseLeg p;
				p.x=tmp.x();
				p.y=tmp.y();
				p.z=tmp.z();
				p.ref=base.toStdString();
				p.vel=6;
				proxies[s]->setIKLeg(p,false);
			}
// 			qDebug()<<i;
			i+=tbezier;
		}
		else
		{
			if(first)
			{
				for(int i=0;i<6;i++)
					statelegs[i] = proxies[i]->getStateLeg();
				first=false;
			}
			for(int s=0;s<6;s++)
			{
				RoboCompLegController::StateLeg st=statelegs[s];
				QVec fin =legsp[s],ini = QVec::vec3(st.x,st.y,st.z);
				QVec tmp=bezier2(ini,fin,i);
				RoboCompLegController::PoseLeg p;
				p.x=tmp.x();
				p.y=tmp.y();
				p.z=tmp.z();
				p.ref=base.toStdString();
				p.vel=6;
				proxies[s]->setIKLeg(p,false);
			}
// 			qDebug()<<i;
			i+=tbezier;
		}
	}
	if(i>1)
	{
		if(!state){
			state=true;
			modovalue=modoaux;
		}
		else
			state=false;
		i=0;
		first=true;
	}
 * 
 * 
 * 
 */



void SpecificWorker::uphexapod()
{
	static bool state=true;
	static float i;
	static QVec ini[6],fin[6];
	RoboCompLegController::PoseLeg p;
	p.ref=base.toStdString();
	p.vel=6;
	if(state)
	{
		for(int s=0;s<6;s++)
		{
			if(i==0)
			{
				RoboCompLegController::StateLeg st=statelegs[s];
				fin[s] = QVec::vec3(legsp[s].x(),20,legsp[s].z());
				ini[s] = QVec::vec3(st.x,st.y,st.z);
			}
			QVec tmp=bezier2(ini[s],fin[s],i);
			p.x=tmp.x();
			p.y=tmp.y();
			p.z=tmp.z();
			proxies[s]->setIKLeg(p,false);
		}
		qDebug()<<i;
		i+=0.1;
	}
	else
	{
		for(int s=0;s<6;s++)
		{
			if(i==0)
			{
				RoboCompLegController::StateLeg st=statelegs[s];
				fin[s] =legsp[s];
				ini[s] = QVec::vec3(st.x,st.y,st.z);
			}
			QVec tmp=bezier2(ini[s],fin[s],i);
			p.x=tmp.x();
			p.y=tmp.y();
			p.z=tmp.z();
			proxies[s]->setIKLeg(p,false);
		}
		qDebug()<<i;
		i+=0.1;
	}
	if(i>1)
	{
		if(!state)
		{
			state=true;
			modovalue=modoaux;
		}
		else
			state=false;
		i=0;
	}
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

double SpecificWorker::mapear(double x, double in_min, double in_max, double out_min, double out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
