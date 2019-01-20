# Etch-A-Sketch

Etch-A-Sketch is a poplar children's toy in which children use two circular knobs to draw a picture on the toy screen. The digital version involved four sensors, where each one represented either left, right, up or down and the screen was displayed on a monitor. 

The project uses 4 QRD1114 Reflective Object sensors connected to an Arduino UNO. The input from the sensors was passed from the Arduino to an Altera DE1-SoC board through GPIO Pins. The digital toy was shown on a monitor through VGA for the player to draw using the sensors. The project was coded in Arduino scripts, and Verilog. 

The repository contains: 
1. Verilog File (rpoject.v) 
  The logic of the VGA output was coded in Verilog on Quartus. This file contains mainy modules, including the toplevel
  module.
2. Arduino File (four_sensors.ino)
  This controld the logic of the sensors circuit. If any of the proximity sensors received a signal, they would send an analog 
  input to the Arduino which was then produced as a digital output to the FPGA board through GPIO pins.
3. mif File (etchasketch.mif)
  This is the background sprite which the user draws on. It ressembles the toy board. The mif file was created using Paint.
  
The project was coded by Nurin Fazil and Idilo Abdalla. November 2018.
