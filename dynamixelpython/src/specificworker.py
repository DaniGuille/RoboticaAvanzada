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
import numpy as np
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
	
	motorParams = []
	motorStateMap = {}
	serial_port = '/dev/ttyUSB1'

	
	def __init__(self, proxy_map):
		super(SpecificWorker, self).__init__(proxy_map)
		params = ()
		self.setParams(params)
		self.timer.timeout.connect(self.compute)
		self.Period = 1000
		self.timer.start(self.Period)

	def setParams(self, params):
		i=0
		with open("etc/config","r") as f:
			for linea in f.readlines():
				#if "NumMotors" in linea:
				    #separacion = linea.split("=")
				    #motorParams = separacion[1]
				if "Params_" in linea:
				    separacion = linea.split("=")
				    
				    #Buscando ID
				    parametros = separacion[1].split(",")
				    name = parametros[0]
				    param = MotorParams()
				    param.name = name
				    param.busId = int (parametros[1])
				    param.invertedSign = parametros[2]
				    param.minPos = float(parametros[3])
				    param.maxPos = float(parametros[4])
				    param.zero = parametros[5]
				    param.maxVel = parametros[6]
				    param.stepsRev = parametros[7]
				    param.maxDegrees = float(parametros[8].replace("\n",""))
				    
				    self.motorParams.append(param)
		
		self.bus = dynamixel.get_serial_for_url(self.serial_port)
		
		return True

	@QtCore.Slot()
	def compute(self):
		#print 'SpecificWorker.compute...'
		
		for m in self.motorParams:
			state = MotorState()
			state.pos = float(dynamixel.get_position(self.bus, m.busId, num_error_attempts=10))
			state.pos=(state.pos) * (2.618 + 2.618) / 1023
			
			state.isMoving = dynamixel.get_is_moving(self.bus, m.busId, verbose=True, num_error_attempts=10)
			#print state.isMoving
			##print state.pos
			self.motorStateMap[m.name] = state
	#
	# getAllMotorParams
	#
	def getAllMotorParams(self):
		return self.motorParams


	#
	# getAllMotorState
	#
	def getAllMotorState(self):
		return self.motorStateMap


	#
	# getMotorParams
	#
	def getMotorParams(self, motor):
		 
		pos=[x for x in self.motorParams if x.name == motor]
		return pos[0]

	# TODO GETMOTORSTATE
	# getMotorState
	#
	def getMotorState(self, motor):
		if motor in self.motorStateMap: 
			return self.motorStateMap[motor]
		else:
			e = UnknownMotorException()
			e.what = "Error " + motor + "does not exist"
			raise e

	#
	# setSyncVelocity
	#
	def setSyncVelocity(self, listGoals):
		pass


	#
	# setZeroPos
	#
	def setZeroPos(self, name):
		pass


	#
	# getBusParams
	#
	def getBusParams(self):
		ret = BusParams()
		return ret


	#
	# setSyncZeroPos
	#
	def setSyncZeroPos(self):

		pass


	# TODO SETSYNCPOSITION
	# setSyncPosition
	#
	def setSyncPosition(self, listGoals):
		for goal in listGoals:
			m = [x for x in self.motorParams if x.name == goal.name]
			
			if len(m)>0:
				
					position=(goal.position + 2.618) * (1023 - 0) / (2.618 + 2.618)
					pos=np.ushort(position)
					vel=np.ushort(goal.maxSpeed)
				
					dynamixel.set_velocity(self.bus,(m[0].busId),vel)
					dynamixel.set_position(self.bus, (m[0].busId),pos)
					dynamixel.send_action_packet(self.bus)
				

	#
	# getMotorStateMap
	#
	def getMotorStateMap(self, mList):
		ret = MotorStateMap()
		return ret


	#
	# setPosition
	#
	def setPosition(self, goal):
		m = [x for x in self.motorParams if x.name == goal.name]
		if len(m)>0:
			position=(goal.position + 2.618) * (1023 - 0) / (2.618 + 2.618)
			pos=np.ushort(position)
			dynamixel.set_position(self.bus, (m[0].busId),pos)
			dynamixel.send_action_packet(self.bus)
	

	#
	# setVelocity
	#
	def setVelocity(self, goal):
		m = [x for x in self.motorParams if x.name == goal.name]
		if len(m)>0:
			
			pos=np.ushort(goal.velocity)
			dynamixel.set_velocity(self.bus, (m[0].busId),pos)
			dynamixel.send_action_packet(self.bus)
		





