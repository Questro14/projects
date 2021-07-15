LSB_MASK = 0
MSB_MASK = 0b1 << 7

READ = 0
WRITE = 0b1 << 6
STATUS = 0b1 << 5
ERROR = 0b1 << 4


def channel_mask(channel): return channel & 0b111
def switch_mask(switch): return (switch & 0b1111) << 3
def board_mask(board): return board & 0b1111


def get_send_bytes(pcb, sw, ch):
    lsb_byte = _get_lsb_byte(pcb)
    msb_byte = _get_msb_byte(sw, ch)
    return lsb_byte, msb_byte


def check_status(lsb_send, msb_send, lsb_recv, msb_recv):
    if lsb_recv == 0 and msb_recv == 0:
        return False
    elif (lsb_send | STATUS) == lsb_recv and msb_send == msb_recv:
        return True
    else:
        return False


def check_error(lsb_recv):
    if lsb_recv & ERROR:
        return False
    else:
        return True


def _get_lsb_byte(pcb):
    byte = LSB_MASK
    byte |= WRITE
    byte |= board_mask(pcb)
    return byte


def _get_msb_byte(sw, ch):
    byte = MSB_MASK
    byte |= switch_mask(sw)
    byte |= channel_mask(ch)
    return byte
