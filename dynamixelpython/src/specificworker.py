#
# Copyright (C) 2016 by YOUR NAME HERE
#
#    This file is part of RoboComp
#
#    RoboComp is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    RoboComp is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with RoboComp.  If not, see <http://www.gnu.org/licenses/>.
#

import sys, os, Ice, traceback, time
from pydynamixel import dynamixel


from PySide import *
from genericworker import *

ROBOCOMP = ''
try:
	ROBOCOMP = os.environ['ROBOCOMP']
except:
	print '$ROBOCOMP environment variable not set, using the default value /opt/robocomp'
	ROBOCOMP = '/opt/robocomp'
if len(ROBOCOMP)<1:
	print 'genericworker.py: ROBOCOMP environment variable not set! Exiting.'
	sys.exit()


preStr = "-I"+ROBOCOMP+"/interfaces/ --all "+ROBOCOMP+"/interfaces/"
Ice.loadSlice(preStr+"JointMotor.ice")
from RoboCompJointMotor import *


from jointmotorI import *

class SpecificWorker(GenericWorker):
	
  
	def __init__(self, proxy_map):
		super(SpecificWorker, self).__init__(proxy_map)
		params = ()
		self.setParams(params)
		self.timer.timeout.connect(self.compute)
		self.Period = 2000
		self.timer.start(self.Period)
		self.motores = ()

	def setParams(self, params):
		i=0
		with open("etc/config","r") as f:
			for linea in f.readlines():
				if "NumMotors" in linea:
				    separacion = linea.split("=")
				    motores = separacion[1]
				if "Params_" in linea:
				    separacion = linea.split("=")
				    
				    #Buscando ID
				    parametros = separacion[1].split(",")
				    Name = parametros[0]
				    BusId = parametros[1]
				    InvertedSign = parametros[2]
				    MinPos = parametros[3]
				    MaxPos = parametros[4]
				    zero = parametros[5]
				    maxVel = parametros[6]
				    stepsRev = parametros[7]
				    MaxDegrees = parametros[8].replace("\n","")

				    print parametros
				    print MaxDegrees
				    
			  
			  
				#print "hola"
				#separacion = linea.split("=")
				#separacion2 = separacion[0].split(".")
				#if(separacion2[0]=="Dynamixel"):
					#if(separacion2[1]=="NumMotors"):
						#nmotores= separacion[1]
					#aux = separacion2[1].split("_")
					#if(separacion2[1]=="Params_"+i)
						#i++
						
				#if linea.contains('Dynamixel.NumMotors'):
					
					#numerodemotores = separacion[1]
					#print numerodemotores
		
		#try:
		#	par = params["InnerModelPath"]
		#	innermodel_path=par.value
		#	innermodel = InnerModel(innermodel_path)
		#except:
		#	traceback.print_exc()
		#	print "Error reading config params"
		return True

	@QtCore.Slot()
	def compute(self):
		print 'SpecificWorker.compute...'
		print self.motores
		#try:
		#	self.differentialrobot_proxy.setSpeedBase(100, 0)
		#except Ice.Exception, e:
		#	traceback.print_exc()
		#	print e
		return True


	#
	# getAllMotorParams
	#
	def getAllMotorParams(self):
		ret = MotorParamsList()
		#
		# YOUR CODE HERE
		#
		return ret


	#
	# getAllMotorState
	#
	def getAllMotorState(self):
		#
		# YOUR CODE HERE
		#
		mstateMap = MotorStateMap()
		return mstateMap


	#
	# getMotorParams
	#
	def getMotorParams(self, motor):
		ret = MotorParams()
		#
		# YOUR CODE HERE
		#
		return ret


	# TODO GETMOTORSTATE
	# getMotorState
	#
	def getMotorState(self, motor):
		ret = MotorState()
		#
		# YOUR CODE HERE
		#
		return ret


	#
	# setSyncVelocity
	#
	def setSyncVelocity(self, listGoals):
		#
		# YOUR CODE HERE
		#
		pass


	#
	# setZeroPos
	#
	def setZeroPos(self, name):
		#
		# YOUR CODE HERE
		#
		pass


	#
	# getBusParams
	#
	def getBusParams(self):
		ret = BusParams()
		#
		# YOUR CODE HERE
		#
		return ret


	#
	# setSyncZeroPos
	#
	def setSyncZeroPos(self):
		#
		# YOUR CODE HERE
		#
		pass


	# TODO SETSYNCPOSITION
	# setSyncPosition
	#
	def setSyncPosition(self, listGoals):
		#
		# YOUR CODE HERE
		#
		pass


	#
	# getMotorStateMap
	#
	def getMotorStateMap(self, mList):
		ret = MotorStateMap()
		#
		# YOUR CODE HERE
		#
		return ret


	#
	# setPosition
	#
	def setPosition(self, goal):
		#
		# YOUR CODE HERE
		#
		pass


	#
	# setVelocity
	#
	def setVelocity(self, goal):
		#
		# YOUR CODE HERE
		#
		pass





