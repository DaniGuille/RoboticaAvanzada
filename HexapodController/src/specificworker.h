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


public slots:
	void compute();
	int  cambiarEstado(int valor);
	void movimientos(int estado);
	bool One_by_One();
	bool Tripod();
	bool Quadruped();
	bool Free();
	QVec bezier2(QVec p0, QVec p1, float t);
	QVec bezier3(QVec p0, QVec p1, QVec p2, float t);
	
	//void avanzar();	
	//void subir();
	//void bajar();
	//void remar();
	//void fromSliderZ(int);
	void resetPos();

private:
  RoboCompLegController::PoseLeg pose;
  InnerModel *innerModel;
  RoboCompJointMotor::MotorParamsList motores;
  //void updateMotorList();
  void actualizarPos();
  LegControllerPrx proxies[6];
  RoboCompLegController::StateLeg posiciones[6];
  RoboCompLegController::StateLeg posIniciales[6];
  int estado;
  int numPata;
  int X,Z;
  
  int legsTripodOn[3], legsTripodOff[3], legsOneByOneOn[1], legsOneByOneOff[5], legsQuadrupedOn[2], legsQuadrupedOff[4] ;
  QVec lini,lfin,lmed,lrot;
  QVec legCoord[6];
  QString base;
  
  double mapear(double x, double in_min, double in_max, double out_min, double out_max);


  
};

#endif

