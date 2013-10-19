#!/usr/bin/python
#
"""
Holiday class implementation for the Secret API for Holiday by Moorescloud

Homepage and documentation: http://dev.moorescloud.com/

Copyright (c) 2013, Mark Pesce.
License: MIT (see LICENSE for details)
"""

__author__ = 'Mark Pesce'
__version__ = '1.0b4'
__license__ = 'MIT'

import sys, array, socket

class HolidaySecretAPI:

	NUM_GLOBES = 50

	# Storage for all 50 globe values
	# 
	globes = [ [ 0x00, 0x00, 0x00], [ 0x00, 0x00, 0x00], [ 0x00, 0x00, 0x00], [ 0x00, 0x00, 0x00], [ 0x00, 0x00, 0x00],
	[ 0x00, 0x00, 0x00], [ 0x00, 0x00, 0x00], [ 0x00, 0x00, 0x00], [ 0x00, 0x00, 0x00], [ 0x00, 0x00, 0x00],
	[ 0x00, 0x00, 0x00], [ 0x00, 0x00, 0x00], [ 0x00, 0x00, 0x00], [ 0x00, 0x00, 0x00], [ 0x00, 0x00, 0x00],
	[ 0x00, 0x00, 0x00], [ 0x00, 0x00, 0x00], [ 0x00, 0x00, 0x00], [ 0x00, 0x00, 0x00], [ 0x00, 0x00, 0x00],
	[ 0x00, 0x00, 0x00], [ 0x00, 0x00, 0x00], [ 0x00, 0x00, 0x00], [ 0x00, 0x00, 0x00], [ 0x00, 0x00, 0x00],
	[ 0x00, 0x00, 0x00], [ 0x00, 0x00, 0x00], [ 0x00, 0x00, 0x00], [ 0x00, 0x00, 0x00], [ 0x00, 0x00, 0x00],
	[ 0x00, 0x00, 0x00], [ 0x00, 0x00, 0x00], [ 0x00, 0x00, 0x00], [ 0x00, 0x00, 0x00], [ 0x00, 0x00, 0x00],
	[ 0x00, 0x00, 0x00], [ 0x00, 0x00, 0x00], [ 0x00, 0x00, 0x00], [ 0x00, 0x00, 0x00], [ 0x00, 0x00, 0x00],
	[ 0x00, 0x00, 0x00], [ 0x00, 0x00, 0x00], [ 0x00, 0x00, 0x00], [ 0x00, 0x00, 0x00], [ 0x00, 0x00, 0x00],
	[ 0x00, 0x00, 0x00], [ 0x00, 0x00, 0x00], [ 0x00, 0x00, 0x00], [ 0x00, 0x00, 0x00], [ 0x00, 0x00, 0x00] ]

	def __init__(self, addr=''):
		"""If remote, you better supply a valid address.  
		We'll throw an exception when you don't do this."""
		self.addr = addr    # IP address we're chatting with.
		self.port = 9988	# Secret API port
		self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # UDP

	def setglobe(self, globenum, r, g, b):
		"""Set a globe"""
		if (globenum < 0) or (globenum >= self.NUM_GLOBES):
			return
		self.globes[globenum][0] = r
		self.globes[globenum][1] = g
		self.globes[globenum][2] = b

	def fill(self, r, g, b):
		"""Sets the whole string to a particular colour"""
		for e in self.globes:
			e[0] = int(r)
			e[1] = int(g)
			e[2] = int(b)

	def getglobe(self, globenum):
		"""Return a tuple representing a globe's RGB color value"""
		if (globenum < 0) or (globenum >= self.NUM_GLOBES):
			return False
		return (self.globes[globenum][0], self.globes[globenum][1], self.globes[globenum][2])

	def chase(self, direction="True"):
		"""Rotate all of the globes around - up if TRUE, down if FALSE"""
		return

	def rotate(self, newr, newg, newb, direction="True", ):
		"""Rotate all of the globes up if TRUE, down if FALSE
		   Set the new start of the string to the color values"""
		return

	def render(self):
		"""The render routine sends out a UDP packet using the SecretAPI"""
		# Create the 160-byte array of data
		packet = array.array('B', [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0])  # initialize basic packet, ignore first 10 bytes
		for g in self.globes:
			packet.append(g[0])
			packet.append(g[1])
			packet.append(g[2])

		# Send the packet to the Holiday
		self.sock.sendto(packet, (self.addr, self.port))
		return

# Just some basic testerating from the command linery
#
if __name__ == '__main__':
	if len(sys.argv) > 1:
		hol = HolidaySecretAPI(addr=sys.argv[1])
	else:
		sys.exit(1)
	import random, time
	while True:
		r = random.randint(0, 255)
		g = random.randint(0, 255)
		b = random.randint(0, 255)
		hol.fill(r, g, b)
		hol.render()
		time.sleep(.1)

