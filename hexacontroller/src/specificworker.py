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

from PySide import *
from genericworker import *
import thread

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



class SpecificWorker(GenericWorker):
	def __init__(self, proxy_map):
		super(SpecificWorker, self).__init__(proxy_map)
		self.timer.timeout.connect(self.compute)
		self.Period = 2000
		self.timer.start(self.Period)
		self.ui.pushAvanzar.clicked.connect(self.avanzar)
		
	def setParams(self, params):
		return True
	
	@QtCore.Slot()
	def compute(self):
		
		#state = self.jointmotor_proxy.getMotorState("arm1motor1")
		#print state
		##if self.ui.pushAvanzar.isChecked() is True:
			#self.avanzar()	
		return True

	def avanzar(self):
		thread.start_new_thread( self.raiseLeg, () )
		
	def raiseLeg(self):
		try:
			print "raising leg"
			mg = MotorGoalPosition()
			mg.name = "arm2motor1"
			mg.position = 0
			
			mg1 = MotorGoalPosition()
			mg1.name = "arm2motor2"
			mg1.position = 0.75
			
			mg2 = MotorGoalPosition()
			mg2.name = "arm2motor3"
			mg2.position = 0.75
			
			
			
			self.jointmotor_proxy.setPosition( mg ) 
			self.jointmotor_proxy.setPosition( mg1 ) 
			self.jointmotor_proxy.setPosition( mg2 ) 
			
			state = self.jointmotor_proxy.getMotorState(mg.name)
			while state.isMoving is True:
				time.sleep(0.1) 
				state = self.jointmotor_proxy.getMotorState(mg.name)
			print "acabe", state
		except:
			print "mal"
		


