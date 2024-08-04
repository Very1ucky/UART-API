import pytest
import serial
import os

uart1_com = "COM9"
uart1_baud = 9600
uart2_com = "COM11"
uart2_baud = 115200

rxPacketSize = 128

def test_write_rx_packet_size_bytes_one_message():
    ser1 = serial.Serial(port=uart1_com, baudrate=uart1_baud, timeout = 1)
    ser2 = serial.Serial(port=uart2_com, baudrate=uart2_baud, timeout = 1)

    tx_message = os.urandom(rxPacketSize)
    ser1.write(tx_message)
    rx_message = ser2.read(rxPacketSize)

    assert tx_message == rx_message

def test_write_rx_packet_size_bytes_several_messages():
    ser1 = serial.Serial(port=uart1_com, baudrate=uart1_baud, timeout = 1)
    ser2 = serial.Serial(port=uart2_com, baudrate=uart2_baud, timeout = 1)

    tx_message1 = os.urandom(int(rxPacketSize/2))
    tx_message2 = os.urandom(rxPacketSize - int(rxPacketSize/2))
    ser1.write(tx_message1)
    ser1.write(tx_message2)
    rx_message = ser2.read(rxPacketSize)

    assert tx_message1+tx_message2 == rx_message

def test_write_less_than_rx_packet_size_bytes():
    ser1 = serial.Serial(port=uart1_com, baudrate=uart1_baud, timeout = 1)
    ser2 = serial.Serial(port=uart2_com, baudrate=uart2_baud, timeout = 1)

    tx_message = os.urandom(int(rxPacketSize/2))
    ser1.write(tx_message)
    rx_message = ser2.read(rxPacketSize)

    assert len(rx_message) == 0