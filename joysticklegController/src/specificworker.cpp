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

/**
* \brief Default constructor
*/
SpecificWorker::SpecificWorker(MapPrx& mprx) : GenericWorker(mprx)
{
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
	lmed=QVec::vec3(0,50,0);
	l1[0]=0;
	l1[1]=3;
	l1[2]=4;
	l2[0]=1;
	l2[1]=2;
	l2[2]=5;
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
	
	for(int i=0;i<6;i++){
		statelegs[i] = proxies[i]->getStateLeg();
		legsp[i]=QVec::vec3(statelegs[i].x,statelegs[i].y,statelegs[i].z);
	}
	timer.start(Period);
	timer.start(Period);
	return true;
}

void SpecificWorker::compute()
{
	for(int i=0;i<6;i++)
		statelegs[i] = proxies[i]->getStateLeg();
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
		case 3:
			modo->setText("Caminar3x3");
			if(caminar3x3())
			{
				lini=QVec::vec3(-X,0,-Z);
				lfin=QVec::vec3(X,0,Z);
			}
			break;
	}
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
			QVec tmp=bezier(ini,legsp[j]+lmed,fin,i);
			RoboCompLegController::PoseLeg p;
			p.x=tmp.x();
			p.y=tmp.y();
			p.z=tmp.z();
			p.ref=base.toStdString();
			p.vel=6;
			proxies[j]->setIKLeg(p,false);
			i+=.1;
			qDebug()<<i;
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
			QVec tmp=bezier(ini,legsp[k],fin,i);
			RoboCompLegController::PoseLeg p;
			p.x=tmp.x();
			p.y=tmp.y();
			p.z=tmp.z();
			p.ref=base.toStdString();
			p.vel=6;
			proxies[k]->setIKLeg(p,false);
		}
		i+=.1;
		qDebug()<<i;
		if (i>=1)
		{
			i=0;
			mcuerpo=false;
		}
	}
	
}
bool SpecificWorker::caminar3x3()
{
	if(lini!=QVec::vec3(0, 0, 0)&&lfin!=QVec::vec3(0,0,0))
	{
		static float i=0;
		bool ismoving=false;;
		for(int k=0;k<6;k++)
			if(proxies[k]->getStateLeg().ismoving){
				ismoving=true;
				break;
			}
		if(!ismoving)
		{
		//patas por arco
			for(int s=0;s<3;s++)
			{
				QVec ini = legsp[l1[s]]+lini,fin = legsp[l1[s]]+lfin;
				QVec tmp=bezier(ini,legsp[l1[s]]+lmed,fin,i);
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
				QVec ini = legsp[l2[s]]+lfin,fin = legsp[l2[s]]+lini;
				QVec tmp=bezier(ini,legsp[l2[s]],fin,i);
				RoboCompLegController::PoseLeg p;
				p.x=tmp.x();
				p.y=tmp.y();
				p.z=tmp.z();
				p.ref=base.toStdString();
				p.vel=6;
				proxies[l2[s]]->setIKLeg(p,false);
				
			}
			i+=.1;
			if (i>=1)
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
		}
		return false;
	}
	else
		return true;
	
}

void SpecificWorker::sendData(const TData& data)
{
// 	QVec angles=QVec::zeros(3);
	bool simufallida=true,ismov=false;;
	RoboCompLegController::AnglesLeg angles;
	RoboCompLegController::PoseLeg pos[6];
	for (int i=0;i<6;i++)
	{
		pos[i].vel=2;
		pos[i].ref=base.toStdString();
	}
	angles.q1=0;
	angles.q2=0;
	angles.q3=0;
	float x=0,y=0,z=0;
	ButtonParams b=data.buttons.at(0);
	if(b.clicked)
	{
		modovalue+=1;
		if(modovalue==4)
			modovalue=0;
	}
	b=data.buttons.at(1);
	if(b.clicked)
	{
		for(float i=.0;i<1;i+=.01)
		{
			for(int s=0;s<6;s++)
				if(proxies[s]->getStateLeg().ismoving)
				{
					ismov=true;
					break;
				}
			if(!ismov)
				for(int s=0;s<6;s++)
				{
					QVec fin = QVec::vec3(legsp[s].x(), 0, legsp[s].z()),ini = legsp[s];
					QVec tmp=bezier(ini,ini,fin,i);
					RoboCompLegController::PoseLeg p;
					p.x=tmp.x();
					p.y=tmp.y();
					p.z=tmp.z();
					p.ref=base.toStdString();
					p.vel=6;
					proxies[s]->setIKLeg(p,false);
				}
			else
				i-=.01;
			qDebug()<<i;
		}
		usleep(1000);
		for(float i=.0;i<1;i+=.01)
		{
			for(int s=0;s<6;s++)
				if(proxies[s]->getStateLeg().ismoving)
				{
					ismov=true;
					break;
				}
			if(!ismov)
				for(int s=0;s<6;s++)
				{
					QVec ini = QVec::vec3(legsp[s].x(), 0, legsp[s].z()),fin = legsp[s];
					QVec tmp=bezier(ini,ini,fin,i);
					RoboCompLegController::PoseLeg p;
					p.x=tmp.x();
					p.y=tmp.y();
					p.z=tmp.z();
					p.ref=base.toStdString();
					p.vel=6;
					proxies[s]->setIKLeg(p,false);
				}
			else
				i-=.01;
			qDebug()<<i;
		}
	}
	for(auto m:data.axes)
	{
		if(m.name=="x")
		{
			angles.q3=(m.value/65537);
			x=m.value/300;
			X=mapear(m.value,-65537,65537, -39,39);
		}
		if(m.name=="y")
		{
			angles.q2=(m.value/65537);
			y=m.value/300;
// 			Y=mapear(m.value,-65537,65537, -30,30);
		}
		if(m.name=="z")
		{
			angles.q1=(m.value/65537);
			z=m.value/300;
			Z=mapear(m.value,65537,-65537, -47,47);
		}
		if(m.name=="vel")
			vel=mapear(-m.value,-65537,65537, 0.0,6.0);
	}
	switch(modovalue)
	{
		case 0:
			angles.vel=vel;
			for(int i=0;i<6;i++)
				proxies[i]->setFKLeg(angles, false);
			break;
		case 1:
			for(int i=0;i<6;i++)
			{
				pos[i].vel=vel;
				pos[i].x=legsp[i].x()+x;
				pos[i].y=legsp[i].y()+y;
				pos[i].z=legsp[i].z()+z;
			}
			for(int i=0;i<6;i++)
			{
				if(!proxies[i]->setIKLeg(pos[i],true))
				{
					simufallida=false;
					break;
				}
			}
			if(simufallida)
				for(int i=0;i<6;i++)
					proxies[i]->setIKLeg(pos[i],false);
			break;
		case 2:
			RoboCompLegController::PoseBody pb;
			pb.vel=vel;
			pb.rx=angles.q1;
			pb.ry=angles.q2;
			pb.rz=angles.q3;
			
			for(int i=0;i<6;i++)
			{
				if(!proxies[i]->setIKBody(pb,true))
				{
					simufallida=false;
					break;
				}
			}
			
			if(simufallida)
				for(int i=0;i<6;i++)
					proxies[i]->setIKBody(pb,false);
			break;
		case 3:
			break;
	}
}

void SpecificWorker::colocar_patas()
{
	RoboCompLegController::AnglesLeg angles;
	angles.q1=0;
	angles.q2=0.3;
	angles.q3=-0.6;
	angles.vel=1;
	for(int i=0;i<6;i++)
		proxies[1]->setFKLeg(angles, false);
}

float SpecificWorker::getPoint( int n1 , int n2 , float perc )
{
    float diff = n2 - n1;

    return n1 + ( diff * perc );
}  

QVec SpecificWorker::bezier(QVec p0, QVec p1, QVec p2, float t)
{
	float xa = getPoint( p0.x() , p1.x() , t );
	float ya = getPoint( p0.y() , p1.y() , t );
	float za = getPoint( p0.z() , p1.z() , t );
	float xb = getPoint( p1.x() , p2.x() , t );
	float yb = getPoint( p1.y() , p2.y() , t );
	float zb = getPoint( p1.z() , p2.z() , t );
	// The Black Dot
	float x = getPoint( xa , xb , t );
	float y = getPoint( ya , yb , t );
	float z = getPoint( za , zb , t );
	return QVec::vec3(x,y,z);
}
QVec SpecificWorker::bezier(QVec p0, QVec p2, float t)
{
	float x = getPoint( p0.x() , p2.x() , t );
	float y = getPoint( p0.y() , p2.y() , t );
	float z = getPoint( p0.z() , p2.z() , t );
	return QVec::vec3(x,y,z);
}
double SpecificWorker::mapear(double x, double in_min, double in_max, double out_min, double out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}





