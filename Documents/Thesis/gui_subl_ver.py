#!/usr/bin/python
#-*- coding: utf-8 -*-
from Tkinter import *
import tkMessageBox
import time
from random import randint
#from temp_read import * 
#import RPi.GPIO as GPIO

LARGE_FONT= ("Verdana", 12)
# temperatura=0
# time1=0
# temperatura2=0
# time2=0
# brewing_time=0
#GPIO.setmode(GPIO.BCM)
#GPIO.setup(23,GPIO.OUT)
#class Sensor():
#	def __init__(self,sensor_addr):
#		self.sensor_addr=sensor_addr
#	def temp_sensor(self, sensor_addr):
#		temp_measured=read_temp(sensor_addr)
#		return temp_measured
class InputVerification():
	def correctInput(self):
		tkMessageBox.showerror(
				"Wrong input",
				"Please check input!")
	def checkInput(self, inputVal):
		if inputVal.isdigit():
			inputVal=int(float(inputVal))
			if inputVal > 100:
				self.correctInput()
			else:
				return True
		else:
			self.correctInput()


class Counter():
	seconds=0
	minutes=0
	hours=0
	def __init__(self, start_time):
		self.start_time=start_time
		#self.total_time=time1+time2
		self.temp_set=temperatura
		#self.time_set=time1
	def count(self, start_time):
		self.refreshed_time=time.time()
		self.seconds=round(float(self.refreshed_time-self.start_time))
		if self.seconds==5:
			self.start_time=time.time()
			self.seconds=0
			self.minutes=self.minutes+1
			if self.minutes==60:
				self.minutes=0
				self.hours=self.hours+1

class Start():
	def __init__(self, master, *args, **kwargs):
		#__init__(self, *args, **kwargs)
		self.container = Frame(master, width=600,height=600)
		self.container.pack(side="top", fill="both", expand = True)
		self.container.grid_rowconfigure(0, weight=1)
		self.container.grid_columnconfigure(0, weight=1)
		self.history = []
		self.show_frame(StartPage)
	def show_frame(self, cont):		
		frame=cont(self.container, self)
		frame.grid(row=0, column=0, sticky="nsew")
		frame.tkraise()

class StartPage(Frame):
	def __init__(self, parent, controller):
		Frame.__init__(self,parent)
		self.controller=controller
		label = Label(self, text="Start Page", font=LARGE_FONT)
		label.pack(pady=10,padx=10)
		button = Button(self, text="Press to start mashing")
		button.bind("<Return>",lambda event: controller.show_frame(PageOne))
		button.pack()
		button.focus_set()
		button2 = Button(self, text="Press to start brewing")
		button2.bind("<Return>",lambda event: controller.show_frame(PageTwo))
		button2.pack()
		button3=Button(self, text="Press to start the pump")
		button3.bind("<Return>", self.startPump)
		button3.pack()
	def startPump(self, event):
		print("pump on")
		result=tkMessageBox.askyesno(
			"Pumping",
			"Stop the pump?")
		if result:
			print("pump stopped")
 
class PageOne(Frame):
	preheated=False
	def __init__(self, parent, controller):
		self.controller=controller
		Frame.__init__(self, parent)
		label = Label(self, text="Mashing", font=LARGE_FONT)
		#label.pack(pady=10,padx=10)
		label.pack()
		label2=Label(self, text="First rest temperature")
		label2.pack()
		self.temp_var = StringVar()
		temp1= Entry(self, textvariable=self.temp_var)
		temp1.pack()
		temp1.focus_force()
		temp1.selection_range(0, END)			
		temp1.bind("<Return>", self.firstTemp)
		
		label3=Label(self, text="First rest time")
		label3.pack()
		self.time_var = StringVar()
		time1= Entry(self, textvariable=self.time_var)
		time1.pack()
		time1.selection_range(0, END)			
		time1.bind("<Return>", self.firstTime)
		
		label4=Label(self, text="Second rest temperature")
		label4.pack()
		self.temp_var2 = StringVar()
		temp2= Entry(self, textvariable=self.temp_var2)
		temp2.pack()
		temp2.selection_range(0, END)			
		temp2.bind("<Return>", self.secondTemp)
		
		label5=Label(self, text="Second rest time")
		label5.pack()
		self.time_var2 = StringVar()
		time2= Entry(self, textvariable=self.time_var2)
		time2.pack()
		time2.selection_range(0, END)			
		time2.bind("<Return>", self.secondTime)

		self.label6=Label(self, text="You can't proceed to mashing yet, preheat first", fg="red")
		self.label6.pack()

		self.button1 = Button(self, text="Start preheating")
		self.button1.bind("<Return>", self.preheatDone)#lambda event: controller.show_frame(Mashing))
		self.button1.pack()
		
		button2 = Button(self, text="Back to Home")
		button2.bind("<Return>",lambda event: controller.show_frame(StartPage))
		button2.pack()
		self.verification=InputVerification()
	def preheatInfo(self,event):
		if self.preheated==False:
			self.preheatOfHLT()
			tkMessageBox.showinfo(
        	    "Preheating",
        	    "Preheating in progress, please wait until proper information is displayed")
		else:
			tkMessageBox.showinfo(
				"Preheating",
				"The water is already preheated!")
	def preheatDone(self,event):
		result=tkMessageBox.askyesno(
            "Preheating",
            "Preheating done, make sure the valves are in appropriate position\n Start the pump now?")
		if result:
			print result
			self.pumpOff()
	def pumpOff(self):
		result=tkMessageBox.askyesno(
			"Pumping",
			"Stop the pump?")
		if result:
			self.button1.configure(text="Start mashing")
			self.button1.bind("<Return>", lambda event: self.controller.show_frame(Mashing))
			#pump relay on
			print("pump stopped")
			self.preheated=True
			self.label6.configure(text="You may now proceed, remember to set valves to appropriate position", fg="green", font=LARGE_FONT)
	def preheatOfHLT(self):
		#HLT_Sensor=Sensor("addressss")
		#measured=HLT_Sensor.temp_sensor("addressss")
		pre_HLT_temp=temperatura+5
		print pre_HLT_temp
		self.pumpOff()
		#if True:
		#	self.preheatOfBK()
		#relays on, controller, if measured==pre_temp {start preheatOfBK, break PreheatHLT)
	def preheatOfBK(self):
		#BK_Sensor=Sensor("adresss_BK")
		#measured_BK=BK_Sensor.temp_sensor("adresss_BK")
		pre_BK_temp=temperatura+2
		print pre_BK_temp
		#relays on, controller, if measured_BK==pre_BK_temp{prompt to set valves BK->MLT, breakself}
		#if True:
		#	self.preheatDone()


	def firstTemp(self, event):
		global temperatura
		temp_var = self.temp_var.get()
		if self.verification.checkInput(temp_var):
			temperatura=int(float(temp_var))
			self.temp_var.set(temperatura)
			event.widget.tk_focusNext().focus()
			return temperatura
	def secondTemp(self,event):
		global temperatura2
		temp_var2 = self.temp_var2.get()
		#self.temp_var2.set(temp_var2)
		if self.verification.checkInput(temp_var2):
			temperatura2=int(float(temp_var2))
			event.widget.tk_focusNext().focus()
			return temperatura2		
	def firstTime(self, event):
   		global time1
		time_var=self.time_var.get()
   		if self.verification.checkInput(time_var):
			time1=int(float(time_var))
			event.widget.tk_focusNext().focus()
			return time1
	def secondTime(self, event):
		global time2
		time_var2=self.time_var2.get()
		if self.verification.checkInput(time_var2):
			time2=int(float(time_var2))
			event.widget.tk_focusNext().focus() 

class Mashing(Frame):
	break_var=True	
	def __init__(self, parent, controller):
		self.controller=controller
		self.time_set=time1
		self.total_time=time1+time2
		self.StartTime=time.time()
		Frame.__init__(self, parent)
		self.temp=StringVar()
		self.state=Label(self, text="Mashing in progess...", font=LARGE_FONT)
		self.state.grid(row=0, column=1)
		set_temp_info=Label(self, text="Temperature and time of first rest: %d°C for %d minutes" %(temperatura,time1), font=LARGE_FONT)
		set_temp_info.grid(row=1, column=1)
		set_temp_info2=Label(self, text="Temperature and time of second rest: %d°C for %d minutes" %(temperatura2,time2), font=LARGE_FONT)
		set_temp_info2.grid(row=2, column=1)
		temp_info=Label(self, text="Current temperature of the mash tun:", font=LARGE_FONT)
		temp_info.grid(row=4, column=1)
		self.text = Label(self,text=self.temp, font=LARGE_FONT)
		self.text.grid(row=4, column=2)		
		time_info=Label(self, text="Time of mashing:", font=LARGE_FONT)
		time_info.grid(row=5, column=1)
		self.timer = Label(self,text='%d:%d:%d' %(0,0,0), font=LARGE_FONT)
		self.timer.grid(row=5, column=2)		
		button = Button(self, text="Stop and back to home")
		button.bind("<Return>", self.stop)		
		button.focus_force()
		button.grid(row=6, column=1)
		self.time_control=Counter(self.StartTime)
		self.control()
	def control(self):
		if self.break_var:
			self.time_control.count(self.StartTime)
			#Mash_sensor=Sensor("28-0215021f66ff")	
			self.timer.configure(text='%d:%d:%d' %(self.time_control.hours, self.time_control.minutes,self.time_control.seconds))
			temp_meas=randint(0,100)#Mash_sensor.temp_sensor("28-0215021f66ff")
			self.text.configure(text=temp_meas)       			#use random 
			error = self.time_control.temp_set-temp_meas
			print('temp set %d' %self.time_control.temp_set)
			print('total_time %d' %self.total_time)
			#if error < 10:
			#	GPIO.output(23,GPIO.HIGH)
			#else:  
			#	GPIO.output(23,GPIO.LOW)
			self._timer=self.after(1000,self.control)
			if (self.time_control.minutes==self.time_set)&(self.time_set!=self.total_time):
				self.time_control.temp_set=temperatura2
			elif self.time_control.minutes==self.total_time:
				self.break_var=False
				self.state.configure(text="Done! :)", fg="green")
				print self.break_var
	def stop(self, event):
		result=tkMessageBox.askyesno(
			"Exit",
			"Are you sure you want to exit?")
		if result:
			self.break_var=False
			self.controller.show_frame(StartPage)
		
class PageTwo(Frame):
	def __init__(self, parent, controller):
		Frame.__init__(self, parent)
		label = Label(self, text="Brewing", font=LARGE_FONT)
		label.pack(pady=10,padx=10)
		label2= Label(self, text="Set the time of brewing:")
		label2.pack()
		self.brew_var=StringVar()
		brew_time= Entry(self, textvariable=self.brew_var)
		brew_time.pack()
		brew_time.focus_force()
		brew_time.selection_range(0, END)
		brew_time.bind("<Return>", self.brewingTime)

		button1 = Button(self, text="Start brewing")
		button1.bind("<Return>", lambda event: controller.show_frame(Brewing))
		button1.pack()
		button2 = Button(self, text="Back to Home")
		button2.bind("<Return>", lambda event: controller.show_frame(StartPage))		
		button2.pack()
		self.verification=InputVerification()
	def brewingTime(self, event):
		global brewing_time
		brew_var=self.brew_var.get()
		if self.verification.checkInput(brew_var):
			brewing_time=int(float(brew_var))
			event.widget.tk_focusNext().focus()
			print brewing_time 

class Brewing(Frame):
	break_var=True
	def __init__(self, parent, controller):
		self.controller=controller
		Frame.__init__(self, parent)
		self.StartTime=time.time()
		self.time_set=brewing_time
		self.label = Label(self, text="Brewing in progress...", font=LARGE_FONT)
		self.label.pack(pady=10,padx=10)
		brew_time_info=Label(self, text="Time of brewing: %d minutes" % brewing_time, font=LARGE_FONT)
		brew_time_info.pack()
		self.timer=Label(self, text="%d:%d:%d" %(0,0,0))
		self.timer.pack()
		exit = Button(self, text="Stop and back to Home")
		exit.bind("<Return>", self.stop)		
		exit.pack()
		exit.focus_force()
		self.brew_time_control=Counter(self.StartTime)
		self.control()
	def control(self):
		if self.break_var:
			self.brew_time_control.count(self.StartTime)
			self.timer.configure(text='%d:%d:%d' %(self.brew_time_control.hours, self.brew_time_control.minutes,self.brew_time_control.seconds))
			self._timer=self.after(1000,self.control)
			if self.brew_time_control.minutes==self.time_set:
				self.break_var=False
				self.label.configure(text="Done! :)", fg="green")
				print self.break_var
	def stop(self,event):
		result=tkMessageBox.askyesno(
			"Exit",
			"Are you sure you want to exit?")
		if result:
			self.break_var=False
			self.controller.show_frame(StartPage)

root=Tk()
root.geometry('600x300+750+100')
#root.attributes('-fullscreen', True)    set to fullscreen
app=Start(root)
root.title("BreWizard")
root.mainloop()


