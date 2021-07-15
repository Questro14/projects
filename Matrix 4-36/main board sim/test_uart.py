import uart_drive as uart
import uart_commands as uart_cmd
import matrix_126 as mt1
import matrix_436 as mt2
import time

obj = uart.UARTServer("COM6")
matrix = mt1.MatrixStatus()

input_, output_ = 1, 1
board, switch, ch = matrix.auto_to_manual(input_, output_)
board, switch, ch = 1, 1, 1

i = 0
er = 0
#t1 = time.time()
#while i < 100000:
    #i+=1
s_byte1, s_byte2 = uart_cmd.get_send_bytes(board, switch, ch)
obj.uart_wr(s_byte1, s_byte2)

r_byte1, r_byte2 = obj.uart_rd()
    #if s_byte1!= r_byte1 or s_byte2 != r_byte2:
        #er += 1
        #if uart_cmd.check_status(s_byte1, s_byte2, r_byte1, r_byte2):
            #er-=1

#t2 = time.time()   
#print("time = ", t2 - t1)
if not uart_cmd.check_status(s_byte1, s_byte2, r_byte1, r_byte2):
    print('status error or recv error')
if not uart_cmd.check_error(r_byte1):
    print('internal error')
#print ("error = " , er)
