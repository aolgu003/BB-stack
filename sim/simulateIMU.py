import pylab
import matplotlib
from imusim.all import *
import csv


t = np.arange(0, 360, .001)
t = t/360

x = np.cos(t)
y = np.sin(t)
z = np.zeros(t.shape)

position = np.array([x,y,z])

print position

yaw = np.arange(0,360, .001)
pitch = np.zeros(yaw.shape)
roll = np.arange(0,360, .001)

quat = np.array([Quaternion.fromEuler((yaw[0],pitch[0],yaw[0]), order='xyz' )])
for i in range((len(yaw)-1)):
  quat = np.concatenate((quat,np.array([Quaternion.fromEuler((yaw[i+1],pitch[i+1],yaw[i+1]), order='xyz' )  ])))

rotationKeyFrame = TimeSeries(t, quat)
positionKeyFrame = TimeSeries(t, position)
newTraj = SampledTrajectory(positionKeyFrame, rotationKeyFrame)
finalSplinedTrajectory = SplinedTrajectory(newTraj)

sim = Simulation()
imu = IdealIMU(sim, finalSplinedTrajectory)
samplingPeriod = 0.01
behaviour = BasicIMUBehaviour(imu, samplingPeriod)
sim.time = finalSplinedTrajectory.startTime
sim.run(finalSplinedTrajectory.endTime)

with open('imuReadings.csv', 'wb') as csvfile:

plot(imu.accelerometer.rawMeasurements.values)
pylab.show()
