# from header import *
import serial


class UARTServer:

    pBUFFER = 2
    pUART_TIMEOUT = 1
    serial_port: serial.Serial = None

    STR_SERIAL_PORT = 'SERIAL_PORT'
    STR_SERIAL_PORT_BAUDRATE = 'SERIAL_PORT_BAUDRATE'
    SERIAL_PORT = '/dev/serial0'
    SERIAL_PORT_BAUDRATE = 2000000

    def __init__(self, port=SERIAL_PORT):
        self.serial_port = serial.Serial(port=port,
                                         baudrate=self.SERIAL_PORT_BAUDRATE,
                                         timeout=self.pUART_TIMEOUT,
                                         # parity=serial.PARITY_NONE,
                                         # stopbits=serial.STOPBITS_TWO,
                                         # bytesize=serial.EIGHTBITS,
                                         )
        print(f"UART connect")

    def __del__(self):
        if self.serial_port:
            self.serial_port.close()
            print(f"UART disconnect")

    def uart_wr(self, byte1, byte2):
        data = ((byte1 & 0xFF) << 8 | (byte2 & 0xFF)).to_bytes(2, byteorder='big')
        self.serial_port.reset_output_buffer()
        self.serial_port.reset_input_buffer()
        self.serial_port.write(data)
        print(f"UART write: 1[{bin(byte1)}] 2[{bin(byte2)}]")

    def uart_rd(self):
        # self.serial_port.timeout = self.pBUFFER_TIMEOUT
        read = self.serial_port.read(size=self.pBUFFER)
        # self.serial_port.timeout = self.pUART_TIMEOUT
        if read:
            print(f"UART read: 1[{bin(read[0])}] 2[{bin(read[1])}]")
            return read[0], read[1]
        else:
            print("UART read: error")
            return 0, 0
