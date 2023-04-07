import os
import serial
import time

class Atmel(object):
    def __init__(self, port='/dev/ttyACM0', inputs:list=[], outputs:list=[]) -> None:
        self.port = port
        self.delayTime = 0.3
        self.baudRate = 115200
        self.inputs = inputs
        self.outputs = outputs
        self._startCom()

    def _startCom(self):
        while True:
            try:
                self.serial = serial.Serial(self.port, self.baudRate)
                time.sleep(3)

                if len(self.inputs) > 0:
                    self._sendInfoBase('i' + ''.join([self._addZeros(c, 2) for c in self.inputs]))

                if len(self.outputs) > 0:
                    self._sendInfoBase('o' + ''.join([self._addZeros(c, 2) for c in self.outputs]))
                break
            except:
                pass
            time.sleep(self.delayTime)

    def _sendInfoBase(self, cad:str):
        self.serial.write((cad + '\n').encode())

    def _addZeros(self, cad, length:int):
        cad = str(cad)
        return '0'*(length-len(cad))+cad

    def _sendInfo(self, cad:str):
        while True:
            try:
                self._sendInfoBase(cad)
                break
            except:
                self._startCom()
            
    def _sendReadInfo(self, command:str):
        while True:
            try:
                self._sendInfo(command)
                msg = self.serial.readline()
                break
            except:
                self._startCom()
        return msg

    def digitalWrite(self, pins:list, states:list):
        assert len(pins) == len(states)
        self._sendInfo('dw' + ''.join(f'{self._addZeros(c, 2)}{int(states[i])}' for i, c in enumerate(pins)))

    def digitalRead(self, pins:list):
        pins = 'dr' + ''.join([self._addZeros(c, 2) for c in pins])
        return [int(c) for c in self._sendReadInfo(pins)]

    def analogRead(self, pins:list):
        pins = 'ar' + ''.join([self._addZeros(c, 2) for c in pins])
        return [int(c) for c in self._sendReadInfo(pins).decode("utf-8").strip().split(',')]

