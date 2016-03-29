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

/**
       \brief
       @author authorname
*/







#ifndef SPECIFICWORKER_H
#define SPECIFICWORKER_H

#include <genericworker.h>
#include <innermodel/innermodel.h>

class SpecificWorker : public GenericWorker
{
Q_OBJECT
public:
	SpecificWorker(MapPrx& mprx);	
	~SpecificWorker();
	bool setParams(RoboCompCommonBehavior::ParameterList params);

	void sendData(const TData &data);
	

public slots:
	void compute(); 	

private:
	InnerModel *inner;
	QString base;
	QStringList legs;
// 	QVec leg1,leg2,leg3,leg4,leg5,leg6;
	int X, Y, Z;
	QVec legsp[6];
	QVec lini,lfin,lmed,lrot;
	float vel;
	int modovalue,modoaux;
	bool IK;
	LegControllerPrx proxies[6];
	int l1[3],l2[3];
	RoboCompLegController::StateLeg statelegs[6];
	
	void caminarDespacio();
	bool caminar3x3();
	bool rotar();
	void cubo();
	void colocar_patas();
	void uphexapod();
	QVec bezier3(QVec p0, QVec p1, QVec p2, float t);
	QVec bezier2(QVec p0, QVec p2, float t);
	float getPoint( int n1 , int n2 , float perc );
	double mapear(double x, double in_min, double in_max, double out_min, double out_max);
	
};

#endif

